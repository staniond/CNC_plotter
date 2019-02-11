CNC_plotter
==========

CNC plotter using an ESP32 and stepper motors extracted from old DVD drives driven by A4988 microstepping drivers.

Libraries utilities and compilers needed
-----------------------

* esp-idf - the official development framework for the ESP32 chip (latest master branch - https://github.com/espressif/esp-idf, 
commit abea9e4c02bb17e86298aec4e299780399e4789f), using cmake build system
* tqdm - A fast, extensible progress bar for Python and CLI - https://tqdm.github.io (for gcode_streamer.py)
* Socat - command line based utility that establishes two bidirectional byte streams and transfers data between them (for creating virtual serial device -> tcp socket)