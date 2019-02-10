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
import sys
import os
import tqdm


def main():
    serial_path = './ttyUSB0'

    if not os.path.isfile(sys.argv[1]):
        print("ERROR: Given gcode file does not exist")
        exit(2)
    if len(sys.argv) == 3:
        if not os.path.isfile(sys.argv[2]):
            print("ERROR: Given serial device does not exist")
            exit(2)
        else:
            serial_path = sys.argv[2]
    if len(sys.argv) > 3:
        print("Arguments - <gcode_path> [serial_device]")
        exit(1)

    with open(sys.argv[1]) as file:
        lines = len(file.readlines())

    # Open grbl serial port
    with serial.Serial(serial_path, 115200) as s:
        with open(sys.argv[1], 'r') as file:  # Open g-code file
            time.sleep(2)  # Wait for grbl to initialize

            print(f"Connected to {serial_path}, sending data from {sys.argv[1]}:")

            s.write(b'E1\n')
            s.readline()

            # Stream g-code to grbl
            with tqdm.tqdm(total=lines) as pbar:
                for line in file:
                    stripped_line = line.strip()  # Strip all EOL characters for consistency
                    s.write(stripped_line.encode() + b'\n')  # Send g-code block to grbl
                    s.readline()  # Wait for grbl response with carriage return
                    pbar.set_description(f"Processing")
                    pbar.update(1)

            s.write(b'E0\n')
            s.readline()
            s.write(b'R\n')
            s.readline()

    # Wait here until grbl is finished to close serial port and file.
    input(" Press <Enter> to exit and disable grbl.")


if __name__ == '__main__':
    main()
