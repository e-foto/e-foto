# e-foto
![GNU GPL3](https://img.shields.io/github/license/e-foto/e-foto?style=plastic)
![Lines of code](https://img.shields.io/tokei/lines/github/e-foto/e-foto?style=plastic)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/e-foto/e-foto?style=plastic)
![GitHub top language](https://img.shields.io/github/languages/top/e-foto/e-foto?style=plastic)
### A free GNU/GPL educational digital photogrammetric workstation
The e-foto Project aims to deliver a comprehensive and accessible digital photogrammetric workstation 
by offering a complete implementation of the software as free and open-source under the GNU General Public License (GPL).

## Digital Photogrammetry
e-foto is equipped with photogrammetric functionalities that empower the creation of topographic tridimensional mapping projects. 
It supports the utilization of aerial photogrammetric images captured by both analog cameras and digital sensors, 
enabling professionals to harness diverse data sources for their mapping endeavors.

## How to use
You can access our website at http://www.efoto.eng.uerj.br/ where you will find comprehensive tutorials, thus ensuring a smooth and efficient learning experience.

## How to install and run from sources (on Linux)
1. Clone this repository
1. > sudo apt update
1. Install packages:
    1. libgdal-dev
    2. build-essential
    3. libfontconfig1
    4. mesa-common-dev
    5. libx11-xcb-dev
    6. libglu1-mesa-dev
    > sudo apt install libgdal-dev build-essential libfontconfig1 mesa-common-dev libx11-xcb-dev libglu1-mesa-dev qt6-base-dev
1. Install qt6 packages:
    1. qt6-base-dev
    2. qt6-base-dev-tools 
    3. cmake
    4. ninja-build  
    > sudo apt install qt6-base-dev qt6-base-dev-tools
    > sudo apt install cmake
    > sudo apt install ninja-build  
1. Compile the source
```bash
mkdir build
cd build
cmake -G Ninja ..
ninja
```
1. Run e-foto
    > ./build/bin/e-foto
    
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
