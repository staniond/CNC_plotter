//
// Created by standa on 2/8/19.
//

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "main.h"
#include "wifi.h"
#include "leds.h"
#include "gcode.h"

#define WIFI_SSID CONFIG_WIFI_SSID  //from menuconfig
#define WIFI_PASS CONFIG_WIFI_PASS
#define TCP_PORT CONFIG_TCP_PORT

#define BUFFER_SIZE 128
#define LINE_SIZE 256

#define CONFIG_EXAMPLE_IPV4

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

const int IPV4_GOT_IP_BIT = BIT0;
const int IPV6_GOT_IP_BIT = BIT1;

static const char *TAG = "WIFI";

static void client_loop(int);

static esp_err_t event_handler(void *ctx, system_event_t *event) {
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            /* enable ipv6 */
            tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_STA);
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, IPV4_GOT_IP_BIT);
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");

            char *ip4 = ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip);
            ESP_LOGI(TAG, "IPv4: %s", ip4);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            /* This is a workaround as ESP32 WiFi libs don't currently auto-reassociate. */
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, IPV4_GOT_IP_BIT);
            xEventGroupClearBits(wifi_event_group, IPV6_GOT_IP_BIT);
            break;
        case SYSTEM_EVENT_AP_STA_GOT_IP6:
            xEventGroupSetBits(wifi_event_group, IPV6_GOT_IP_BIT);
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP6");

            char *ip6 = ip6addr_ntoa(&event->event_info.got_ip6.ip6_info.ip);
            ESP_LOGI(TAG, "IPv6: %s", ip6);
        default:
            break;
    }
    return ESP_OK;
}

void initialise_wifi(void) {
    if(TCP_PORT < 0 || TCP_PORT > UINT16_MAX) {
        led_light(RED);
        for(;;) {
            ESP_LOGI(TAG, "TCP_PORT not in range: %d", TCP_PORT);
            vTaskDelay(10000 / portTICK_PERIOD_MS);
        }
    }

    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config = {
            .sta = {
                    .ssid = WIFI_SSID,
                    .password = WIFI_PASS,
            },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE))
}

void wait_for_ip(void) {
    uint32_t connected_bits = IPV4_GOT_IP_BIT | IPV6_GOT_IP_BIT;

    ESP_LOGI(TAG, "Waiting for AP connection...");
    EventBits_t event_bits;
    for (;;) {
        led_light(YELLOW);
        event_bits = xEventGroupWaitBits(wifi_event_group, connected_bits, false, true, 250 / portTICK_PERIOD_MS);
        if ((event_bits & connected_bits) == connected_bits) {
            break;
        }
        led_light(NONE);
        event_bits = xEventGroupWaitBits(wifi_event_group, connected_bits, false, true, 250 / portTICK_PERIOD_MS);
        if ((event_bits & connected_bits) == connected_bits) {
            break;
        }
    }
    ESP_LOGI(TAG, "Connected to AP");
    led_light(GREEN);
}

void tcp_server_task(void *pvParameters) {
    char addr_str[128];
    int addr_family;
    int ip_protocol;

#ifdef CONFIG_EXAMPLE_IPV4
    struct sockaddr_in destAddr;
    destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(TCP_PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;
    inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);
#else // IPV6
    struct sockaddr_in6 destAddr;
    bzero(&destAddr.sin6_addr.un, sizeof(destAddr.sin6_addr.un));
    destAddr.sin6_family = AF_INET6;
    destAddr.sin6_port = htons(TCP_PORT);
    addr_family = AF_INET6;
    ip_protocol = IPPROTO_IPV6;
    inet6_ntoa_r(destAddr.sin6_addr, addr_str, sizeof(addr_str) - 1);
#endif

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
    }
    ESP_LOGI(TAG, "Socket created");

    int err = bind(listen_sock, (struct sockaddr *) &destAddr, sizeof(destAddr));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
    }
    ESP_LOGI(TAG, "Socket binded");

    for (;;) {
        err = listen(listen_sock, 1);
        if (err != 0) {
            ESP_LOGE(TAG, "Error occured during listen: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_in6 sourceAddr; // Large enough for both IPv4 or IPv6
        uint addrLen = sizeof(sourceAddr);
        int sock = accept(listen_sock, (struct sockaddr *) &sourceAddr, &addrLen);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket accepted");

        client_loop(sock);

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}


static void client_loop(int sock) {
    char rx_buffer[BUFFER_SIZE];
    size_t line_len = 0;
    char *line_buffer = malloc(sizeof(char) * LINE_SIZE);
    for (;;) {
        int len = recv(sock, rx_buffer, BUFFER_SIZE - 1, 0);
        // Error occured during receiving
        if (len < 0) {
            ESP_LOGE(TAG, "recv failed: errno %d", errno);
            break;
        }
        // Connection closed
        else if (len == 0) {
            ESP_LOGI(TAG, "Connection closed");
            break;
        }
        // Data received
        else {
            for (int i = 0; i < len; i++) {
                if (rx_buffer[i] == '\n') {
                    line_buffer[line_len++] = '\0';

                    Command *command = parse_buffer(line_buffer, line_len);
                    xQueueSend(queue, &command, portMAX_DELAY);

                    int err;
                        err = send(sock, "OK\n", 4, 0);
                    if (err < 0) {
                        ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
                        break;
                    }
                    line_len = 0;
                } else {
                    line_buffer[line_len++] = rx_buffer[i];
                    if (line_len > LINE_SIZE - 1) {
                        ESP_LOGE(TAG, "Error - line longer than buffer (%d bytes), buffer cleared", LINE_SIZE);
                        line_len = 0;
                    }
                }
            }
        }
    }
    free(line_buffer);
}
