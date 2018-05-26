# Description
Trace Ray Application, use CPU, could be multithread (set by argument of command line)

# Documentation
See the documentation of code at include/ directory

# Build
* mkdir build && cd build
* cmake .. && make

# Usage
For help
```
./mashgraph3 --help
```
See the sutable option at the next section.

# Options
* --save-to - /path/to/result/image.bmp (required argument)
* --distance-to-camera - Distance from camera to projection screen (default: 50)
* --camera-position-z  - Camera Z position (default: 10)
* --image-width        - Width of result image (default: 512)
* --image-height       - Height of result image (default: 512)
* --threads            - Threads number (default: 1)
* --antialiasing       - Enable antialiasing (default: none)
* --help               - Show this message

# The example of result image
![Alt text](examples/example.png?raw=true "Title")
