# e-foto
![GNU GPL3](https://img.shields.io/github/license/e-foto/e-foto?style=plastic)
![Lines of code](https://img.shields.io/tokei/lines/github/e-foto/e-foto?style=plastic)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/e-foto/e-foto?style=plastic)
![GitHub top language](https://img.shields.io/github/languages/top/e-foto/e-foto?style=plastic)
### A free GNU/GPL educational digital photogrammetric workstation
The goal of Project E-foto is to provide the full implementation of a 
digital photogrammetric workstation as free software under a GNU GPL license.

## Digital Photogrammetry
E-foto Software possess photogrammetric functionalities that allow the development of professional topographic tridimentional mapping projects, 
by using as data sources aerial photogrammetric images obtained by both analog cameras and digital sensors.

## How to use
You can find tutorials with a step by step aproach to teach how to use the program on our website http://www.efoto.eng.uerj.br/

## How to install and run from sources (on Linux)
1. Clone this repository
1. Install packages:
    1. libgdal.dev
    2. build-essential
    3. libfontconfig1
    4. mesa-common-dev
    5. libx11-xcb-dev
    6. libglu1-mesa-dev
    > sudo apt installlibgdal−dev build−essential libfontconfig1 mesa−common−dev libx11−xcb−dev libglu1−mesa−dev
1. Install qt5 packages:
    1. qt5-default
    2. qt5-qmake
    > sudo apt install qt5−default
    > sudo apt install −y qt5−qmake
1. Compile the source
    > qmake e−foto.pro
    > make
1. Run e-foto
    > ./build/bin/e−foto
    
## Contribution Guidelines
* Fork this repository
* Clone your forked repository
* Add your contribution
* Commit and push
* Create a pull request
* Star this repository
* Wait for pull request to merge
* Celebrate your first step into the open source world and contribute more

## License
This project is licensed under the terms of the [GNU Public License](http://www.gnu.org/licenses/gpl.html).
