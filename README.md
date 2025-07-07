# IMP Project 2024/2025

**Author: Denis Fekete, xfeket01@fit.vutbr.cz**

**Github imp_project: https://github.com/denis-fekete/imp_project**

**Github imp_drawing: https://github.com/denis-fekete/imp_bitmap_editor**


## About:
This is project for Mikroprocesorové a vestavěné systémy, project demonstrates serial communication between ESP32 and TFT display ILI9163C, communication between ESP32 and user is implemented through WiFi and simple protocol based on UDP (User Datagram Protocol).

## Installation
Tested using Linux Mint 21.3 Cinnamon.

### Using Platformio
Make sure to have installed Platformio, ideally extension to Visual Studio Code.

Before running project run Makefile command:
```
$ make pio
```

Run project using pio command line or VS extension

### Using ESP IDF
Make sure to have installed ESP IDF and it is in environment.

Run `. export.sh` from installed esp-dif
```
esp-idf$ . ./export.sh

imp_project$ make idfpy
imp_project$ idf.py build
```

might wary from system to system

```
imp_project$ idf.py -p /dev/ttyUSB0 flash
imp_project$ idf.py monitor

```

## Testing with GUI application
Clone, install and run GUI application from [github](https://github.com/denis-fekete/imp_bitmap_editor). 
Git repository is added as a submodule in this repository under `/gui` directory.
Make sure to have installed Qt, tested on version 6.8.1.

Navigate to `/gui` directory:
```
imp_project$ cd gui
```

Create and navigate to build directory:
```
imp_project/gui$ mkdir build
imp_project/gui$ cd build
```
Run cmake, after than run make:
```
imp_project/gui$ cmake ..
imp_project/gui$ make
```
Run compiled binary `imp_drawing`
```
imp_project/gui$ ./imp_drawing
```
