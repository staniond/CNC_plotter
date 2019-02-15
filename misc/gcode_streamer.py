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

import serial
import time
import os
import tqdm
import argparse


def main(args):
    with open(args.gcode_path) as file:
        lines = len(file.readlines())

    with serial.Serial(args.serial_path, 115200) as s:
        with open(args.gcode_path, 'r') as file:  # Open g-code file
            time.sleep(0.25)

            print(f"Connected to {args.serial_path}, streaming data from {args.gcode_path}:")

            s.write(b'E1\n')
            s.readline()

            if args.fans:
                s.write(b"F1\n")
                s.readline()

            with tqdm.tqdm(total=lines) as pbar:
                # pbar.set_description(f"Processing")
                for line in file:
                    stripped_line = line.strip()  # Strip all EOL characters for consistency
                    s.write(stripped_line.encode() + b'\n')  # Send g-code command
                    s.readline()  # response
                    pbar.update(1)

            s.write(b'R\n')  # reset at the end
            s.readline()

    print("Gcode streaming has finished")


def get_arguments():
    parser = argparse.ArgumentParser("gcode_streamer.py", description="Streams gcode files to serial device")

    parser.add_argument("gcode_path", help="Path to gcode file")
    parser.add_argument("-s", "--serial-path", help="Path to serial device", default="./ttyUSB0")
    parser.add_argument("-f", "--fans", action="store_true", help="If you want to turn on fans during the streaming")

    args = parser.parse_args()

    if not os.path.isfile(args.gcode_path) or not os.path.exists(args.serial_path):
        print("Provide a valid path to gcode file and to serial device")
        print()

        parser.print_help()
        exit(1)

    return args


if __name__ == '__main__':
    main(get_arguments())
