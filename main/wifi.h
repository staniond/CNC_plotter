//
// Created by standa on 2/8/19.
//

#ifndef CNC_PLOTTER_WIFI_H
#define CNC_PLOTTER_WIFI_H

void initialise_wifi(void);

void wait_for_ip(void);

void tcp_server_task(void *pvParameters);


#endif //CNC_PLOTTER_WIFI_H
