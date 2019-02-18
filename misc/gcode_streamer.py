#!/usr/bin/env python3
"""\
Simple g-code streaming script for grbl
Provided as an illustration of the basic communication interface
for grbl. When grbl has finished parsing the g-code block, it will
return an 'ok' or 'error' response. When the planner buffer is full,
grbl will not send a response until the planner buffer clears space.
G02/03 arcs are special exceptions, where they inject short line
segments directly into the planner. So there may not be a response
from grbl for the duration of the arc.
---------------------
The MIT License (MIT)
Copyright (c) 2012 Sungeun K. Jeon
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
---------------------
"""

import socket
import time
import os
import tqdm
import argparse

buffer_size = 128


def main(args):
    with open(args.gcode_path) as file:
        lines = len(file.readlines())

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((args.ip_address, args.port))
        with open(args.gcode_path, 'r') as file:  # Open g-code file
    
            try:
                time.sleep(0.25)
                print(f"Connected to {args.ip_address}:{args.port}, streaming data from {args.gcode_path}:")
    
                s.sendall(b'E1\n')
                s.recv(buffer_size)  # wait until the command is processed
    
                if args.fans:
                    s.sendall(b"F1\n")
                    s.recv(buffer_size)
    
                with tqdm.tqdm(total=lines) as pbar:
                    for line in file:
                        stripped_line = line.strip()  # Strip all EOL characters for consistency
                        s.sendall(stripped_line.encode() + b'\n')  # Send g-code command

                        answer = s.recv(buffer_size)
                        pbar.set_description(answer.decode().strip())
                        pbar.update(1)
    
                s.sendall(b'R\n')  # reset at the end
                s.recv(buffer_size)
            except KeyboardInterrupt:
                s.sendall(b'R\n')
                s.recv(buffer_size)

    print("Gcode streaming has finished")


def get_arguments():
    parser = argparse.ArgumentParser("gcode_streamer.py", description="Streams gcode files to serial device")

    parser.add_argument("gcode_path", help="Path to gcode file")
    parser.add_argument("ip_address", help="IP address to connect to")
    parser.add_argument("port", help="Port to connect to", type=int)
    parser.add_argument("-f", "--fans", action="store_true", help="If you want to turn on fans during the streaming")

    args = parser.parse_args()

    if not os.path.isfile(args.gcode_path):
        print("Provide a valid path to a gcode file")
        print()

        parser.print_help()
        exit(1)

    if not args.fans:
        answer = input("Do you want to turn on the fans? yes/no:\n")
        if answer.lower() in ["yes", "y", ""]:
            args.fans = True

    return args


if __name__ == '__main__':
    try:
        main(get_arguments())
    except KeyboardInterrupt:
        pass
