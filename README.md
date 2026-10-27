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
    
## File path handling
All paths exchanged inside the software (project XML, forms, manager interfaces)
are **UTF-8** `std::string` values; `c/infrastructure/PathUtils` is the only
place where they are converted to the operating system representation.

* A `<filePath>` stored in a `.epp` file is relative to the **project file
  directory**, never to the process working directory, so a project can be
  opened from anywhere (including by double clicking / dragging the file).
* When an image lives on another drive or volume than the project, a relative
  path cannot express it, so an absolute path is stored instead.
* Images that were moved together with the project are searched inside the
  project folder when the project is opened, and the stored paths are updated.
* Non ASCII characters (Chinese, accented), spaces, XML special characters,
  and paths longer than `MAX_PATH` are supported. On Windows the extended
  length prefix is applied automatically and `efoto.manifest` declares the
  process as `longPathAware`.
* Saving works the same way: exports, reports and the binary DSM/ortho-image
  writers use `PathUtils::OutputFileStream` / `PathUtils::openFile` instead of
  `std::ofstream` / `fopen`, which interpret the bytes with the local code page
  and silently fail on such names.

The path layer has standalone regression tests that do not need Qt or GDAL:

```bash
cmake -G Ninja -DEFOTO_BUILD_TESTS=ON ..
ninja pathutils_test project_path_test
ctest --output-on-failure
```

## Images and points lists

The `Images` and `Points` pages of the project manager are lists to manage the
registered objects, not launchers: a single click **selects** a row (the whole
row is highlighted and the matching node is selected in the project tree), and
the item form is only opened by a double click (or by pressing Enter, or by
clicking the item in the project tree). The buttons at the bottom apply to the
selected row: `New` adds an item and `Delete` removes it after a confirmation;
the neighbour row stays selected, so a sequence of items can be deleted without
clicking again.

Deleting an image also removes the `<imageIO>`, `<imageEO>` and `<imageSR>`
records that belong to it and the measurements of the points on it. Deleting a
point removes its `<imageCoordinates>` measurements and its `<pointKey>` entries
in the phototriangulation and in the spatial resections. The project therefore
never keeps data pointing at an object that does not exist anymore.

## Translations

The interface is translated with the Qt Linguist tool chain. User visible strings
are wrapped in `tr()` (`QCoreApplication::translate("efoto-core", ...)` for the
messages produced by the `c/` control layer) and the translations live in
`translations/*.ts`.

```bash
# refresh the .ts from the sources (also the CMake target `update-translations`)
lupdate -recursive -locations none c qt -ts translations/e-foto_zh_CN.ts

# edit translations/e-foto_zh_CN.ts with Qt Linguist, then compile it; the
# CMake build does this automatically into build/translations/
lrelease translations/e-foto_zh_CN.ts -qm build/translations/e-foto_zh_CN.qm
```

The `.qm` file is looked up next to the executable (`translations/`), in the Qt
translations directory, or in the `:/i18n` resources. At runtime the language can
be changed from the *Language* menu of the project manager; the choice is stored
with `QSettings`, and `system` follows the operating system locale. To add a new
language, copy the `.ts`, change its `language` attribute, translate it and add
the file to `TRANSLATIONS` in `e-foto.pro` and to the CMake translation block.

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
