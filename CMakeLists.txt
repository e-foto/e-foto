cmake_minimum_required(VERSION 3.16)
project(e-foto VERSION 1.0 LANGUAGES C CXX)

# Config build with commit hash number and date of compilation

############# Your CONFIGS ####################
##set your configs here

# Include the config file
if(WIN32)
    include(${CMAKE_SOURCE_DIR}/config.cmake)
endif()

###############################################
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(CURRENT_DATE "$<DATE:yyyy-MM-dd>")
set(HEADER_FILE "${CMAKE_BINARY_DIR}/revision_info.h")
configure_file(
    "${CMAKE_SOURCE_DIR}/c/infrastructure/revision_info_template.h.in"
    "${HEADER_FILE}"
)
include_directories("${CMAKE_BINARY_DIR}")

# Check system and compiler and set differences
set(PLATFORM_DEFINITIONS "")
if(WIN32)
    message(STATUS "Sistema Operacional: Windows")
    set(THREADS_PREFER_PTHREAD_FLAG OFF)
    find_package(Threads REQUIRED)
        # The gdal_dir may need to be changed depending on user settings.
        set(GDAL_ROOT ${MY_GDAL_ROOT})

        file(MAKE_DIRECTORY "${GDAL_ROOT}/efoto")

        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GDAL_ROOT}/apps/gdal/lib/cmake/gdal"
                "${GDAL_ROOT}/efoto/apps/gdal/lib/cmake/gdal"
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GDAL_ROOT}/apps"
                "${GDAL_ROOT}/efoto/apps/gdal/apps"
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GDAL_ROOT}/bin"
                "${GDAL_ROOT}/efoto/apps/gdal/bin"
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GDAL_ROOT}/etc"
                "${GDAL_ROOT}/efoto/apps/gdal/etc"
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GDAL_ROOT}/include"
                "${GDAL_ROOT}/efoto/apps/gdal/include"
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GDAL_ROOT}/lib"
                "${GDAL_ROOT}/efoto/apps/gdal/lib"
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GDAL_ROOT}/share"
                "${GDAL_ROOT}/efoto/apps/gdal/share"
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GDAL_ROOT}/usr"
                "${GDAL_ROOT}/efoto/apps/gdal/usr"
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${GDAL_ROOT}/var"
                "${GDAL_ROOT}/efoto/apps/gdal/var"
        )
        set(GDAL_DIR "${GDAL_ROOT}/efoto/apps/gdal/lib/cmake/gdal")
    if(MSVC)
        message(STATUS "Compilador: MSVC (Microsoft Visual Studio)")
        add_compile_definitions(HAVE_STDATOMIC=0)
    endif()

    if(MINGW)
        message(STATUS "Compilador: MinGW (GCC no Windows)")
    endif()

    set(GDAL_INCLUDE_DIRS "${GDAL_ROOT}/efoto/apps/gdal/include")
    set(GDAL_LIBRARIES "${GDAL_ROOT}/efoto/apps/gdal/lib/gdal.lib")
    include_directories(${GDAL_INCLUDE_DIRS})
    link_directories("${GDAL_ROOT}/efoto/apps/gdal/lib")
    list(APPEND PLATFORM_DEFINITIONS win)

endif()
if(UNIX)
    message(STATUS "Sistema Operacional: UNIX-based")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Compilador: GCC (GNU Compiler Collection)")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Compilador: Clang (Clang Compiler)")
    else()
        message(STATUS "Compilador desconhecido: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    list(APPEND PLATFORM_DEFINITIONS unix)

endif()

# Configuration based on qmake2cmake app by qt6 
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC "YES")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS qt/interface qt/imageDisplay)

find_package(GDAL CONFIG REQUIRED)
message(STATUS "GDAL_INCLUDE_DIRS: ${GDAL_INCLUDE_DIRS}")
message(STATUS "GDAL_LIBRARIES: ${GDAL_LIBRARIES}")
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui OpenGLWidgets OpenGL Widgets)
add_compile_definitions(QT_DISABLE_DEPRECATED_UP_TO=0x060300)
add_compile_definitions(USE_GDAL)
qt_standard_project_setup()

qt_add_executable(e-foto WIN32 MACOSX_BUNDLE
    c/control/DEMManager.cpp c/control/DEMManager.h
    c/control/EFotoManager.cpp c/control/EFotoManager.h
    c/control/IOManager.cpp c/control/IOManager.h
    c/control/OrthoManager.cpp c/control/OrthoManager.h
    c/control/PTManager.cpp c/control/PTManager.h
    c/control/ProjectManager.cpp c/control/ProjectManager.h
    c/control/ReportManager.cpp c/control/ReportManager.h
    c/control/SPManager.cpp c/control/SPManager.h
    c/control/SRManager.cpp c/control/SRManager.h
    c/imageProcessing/ImageMatching.cpp c/imageProcessing/ImageMatching.h
    c/imageProcessing/Interpolation.cpp c/imageProcessing/Interpolation.h
    c/imageProcessing/LeastSquaresMatching.cpp c/imageProcessing/LeastSquaresMatching.h
    c/imageProcessing/NormalizedCrossCorrelation.cpp c/imageProcessing/NormalizedCrossCorrelation.h
    c/imageProcessing/RadiometricTransformation.cpp c/imageProcessing/RadiometricTransformation.h
    c/infrastructure/AbstractDetectorCoordinate.cpp c/infrastructure/AbstractDetectorCoordinate.h
    c/infrastructure/AbstractImageCoordinate.cpp c/infrastructure/AbstractImageCoordinate.h
    c/infrastructure/CommonDef.h
    c/infrastructure/ConvertionsSystems.cpp c/infrastructure/ConvertionsSystems.h
    c/infrastructure/Coordinate.cpp c/infrastructure/Coordinate.h
    c/infrastructure/Dms.cpp c/infrastructure/Dms.h
    c/infrastructure/EDom.cpp c/infrastructure/EDom.h
    c/infrastructure/EDomValidator.cpp c/infrastructure/EDomValidator.h
    c/infrastructure/EObject.h
    c/infrastructure/ETreeModel.cpp c/infrastructure/ETreeModel.h
    c/infrastructure/GeoSystem.cpp c/infrastructure/GeoSystem.h
    c/infrastructure/Matrix.cpp c/infrastructure/Matrix.h
    c/infrastructure/PositionMatrix.cpp c/infrastructure/PositionMatrix.h
    c/infrastructure/RectSupport.cpp c/infrastructure/RectSupport.h
    c/infrastructure/SparseMatrix.cpp c/infrastructure/SparseMatrix.h
    c/infrastructure/TreeFeatures.cpp c/infrastructure/TreeFeatures.h
    c/interface/DEMUserInterface.cpp c/interface/DEMUserInterface.h
    c/interface/IOUserInterface.cpp c/interface/IOUserInterface.h
    c/interface/OrthoUserInterface.cpp c/interface/OrthoUserInterface.h
    c/interface/PTUserInterface.cpp c/interface/PTUserInterface.h
    c/interface/ProjectUserInterface.cpp c/interface/ProjectUserInterface.h
    c/interface/ReportUserInterface.cpp c/interface/ReportUserInterface.h
    c/interface/SPUserInterface.cpp c/interface/SPUserInterface.h
    c/interface/SRUserInterface.cpp c/interface/SRUserInterface.h
    c/photogrammetry/BundleAdjustment.cpp c/photogrammetry/BundleAdjustment.h
    c/photogrammetry/DemFeatures.cpp c/photogrammetry/DemFeatures.h
    c/photogrammetry/DemGrid.cpp c/photogrammetry/DemGrid.h
    c/photogrammetry/DetectorFiducialMark.cpp c/photogrammetry/DetectorFiducialMark.h
    c/photogrammetry/DetectorSpaceCoordinate.cpp c/photogrammetry/DetectorSpaceCoordinate.h
    c/photogrammetry/Dummies.cpp c/photogrammetry/Dummies.h
    c/photogrammetry/EOQuality.cpp c/photogrammetry/EOQuality.h
    c/photogrammetry/ExteriorOrientation.cpp c/photogrammetry/ExteriorOrientation.h
    c/photogrammetry/Flight.cpp c/photogrammetry/Flight.h
    c/photogrammetry/FrameSensor.cpp c/photogrammetry/FrameSensor.h
    c/photogrammetry/IOQuality.cpp c/photogrammetry/IOQuality.h
    c/photogrammetry/Image.cpp c/photogrammetry/Image.h
    c/photogrammetry/ImageFiducialMark.cpp c/photogrammetry/ImageFiducialMark.h
    c/photogrammetry/ImageSpaceCoordinate.cpp c/photogrammetry/ImageSpaceCoordinate.h
    c/photogrammetry/InteriorOrientation.cpp c/photogrammetry/InteriorOrientation.h
    c/photogrammetry/MatchingPoints.cpp c/photogrammetry/MatchingPoints.h
    c/photogrammetry/MatchingPointsGrid.cpp c/photogrammetry/MatchingPointsGrid.h
    c/photogrammetry/ObjectSpaceCoordinate.cpp c/photogrammetry/ObjectSpaceCoordinate.h
    c/photogrammetry/Orthorectification.cpp c/photogrammetry/Orthorectification.h
    c/photogrammetry/PhotoTri.cpp c/photogrammetry/PhotoTri.h
    c/photogrammetry/PhotoTriReport.cpp c/photogrammetry/PhotoTriReport.h
    c/photogrammetry/Point.cpp c/photogrammetry/Point.h
    c/photogrammetry/Project.cpp c/photogrammetry/Project.h
    c/photogrammetry/ProjectHeader.cpp c/photogrammetry/ProjectHeader.h
    c/photogrammetry/ProjectiveRay.cpp c/photogrammetry/ProjectiveRay.h
    c/photogrammetry/RayTester.cpp c/photogrammetry/RayTester.h
    c/photogrammetry/Sensor.cpp c/photogrammetry/Sensor.h
    c/photogrammetry/SensorWithFiducialMarks.cpp c/photogrammetry/SensorWithFiducialMarks.h
    c/photogrammetry/SensorWithKnowDimensions.cpp c/photogrammetry/SensorWithKnowDimensions.h
    c/photogrammetry/SensorWithKnowParameters.cpp c/photogrammetry/SensorWithKnowParameters.h
    c/photogrammetry/SpatialIntersection.cpp c/photogrammetry/SpatialIntersection.h
    c/photogrammetry/SpatialRessection.cpp c/photogrammetry/SpatialRessection.h
    c/photogrammetry/StereoPair.cpp c/photogrammetry/StereoPair.h
    c/photogrammetry/Terrain.cpp c/photogrammetry/Terrain.h
    c/xml_definitions/XMLAerial.cpp c/xml_definitions/XMLAerial.h
    c/xml_definitions/XMLFlight.cpp c/xml_definitions/XMLFlight.h
    c/xml_definitions/XMLProjectHeader.cpp c/xml_definitions/XMLProjectHeader.h
    c/xml_definitions/XMLSensor.cpp c/xml_definitions/XMLSensor.h
    c/xml_definitions/XMLSensorWithFiducialMarks.cpp c/xml_definitions/XMLSensorWithFiducialMarks.h
    c/xml_definitions/XMLTerrain.cpp c/xml_definitions/XMLTerrain.h
    c/xml_definitions/XmlUpdater.cpp c/xml_definitions/XmlUpdater.h
    qt/formDisplay/AbstractForm.h
    qt/formDisplay/ControlButtons.cpp qt/formDisplay/ControlButtons.h
    qt/formDisplay/FeatEditor.ui
    qt/formDisplay/FeatsEditor.ui
    qt/formDisplay/FlightForm.cpp qt/formDisplay/FlightForm.h
    qt/formDisplay/FlightLayout.ui
    qt/formDisplay/HeaderForm.cpp qt/formDisplay/HeaderForm.h
    qt/formDisplay/HeaderLayout.ui
    qt/formDisplay/ImageForm.cpp qt/formDisplay/ImageForm.h
    qt/formDisplay/ImageLayout.ui
    qt/formDisplay/ImagesForm.cpp qt/formDisplay/ImagesForm.h
    qt/formDisplay/ImagesLayout.ui
    qt/formDisplay/PointForm.cpp qt/formDisplay/PointForm.h
    qt/formDisplay/PointLayout.ui
    qt/formDisplay/PointsForm.cpp qt/formDisplay/PointsForm.h
    qt/formDisplay/PointsLayout.ui
    qt/formDisplay/SensorForm.cpp qt/formDisplay/SensorForm.h
    qt/formDisplay/SensorLayout.ui
    qt/formDisplay/SpEditor.ui
    qt/formDisplay/TerrainForm.cpp qt/formDisplay/TerrainForm.h
    qt/formDisplay/TerrainLayout.ui
    qt/imageDisplay/GLDisplay.cpp qt/imageDisplay/GLDisplay.h
    qt/imageDisplay/GeometryResource.cpp qt/imageDisplay/GeometryResource.h
    qt/imageDisplay/ImageViewers.cpp qt/imageDisplay/ImageViewers.h
    qt/imageDisplay/GDALRasterResource.cpp qt/imageDisplay/GDALRasterResource.h
    qt/imageDisplay/RasterResource.cpp qt/imageDisplay/RasterResource.h
    qt/imageDisplay/SingleDisplay.cpp qt/imageDisplay/SingleDisplay.h
    qt/imageDisplay/SingleScene.cpp qt/imageDisplay/SingleScene.h
    qt/imageDisplay/SingleTools.cpp qt/imageDisplay/SingleTools.h
    qt/imageDisplay/StereoDisplay.cpp qt/imageDisplay/StereoDisplay.h
    qt/imageDisplay/StereoScene.cpp qt/imageDisplay/StereoScene.h
    qt/imageDisplay/StereoTools.cpp qt/imageDisplay/StereoTools.h
    qt/infrastructure/AboutForm.cpp qt/infrastructure/AboutForm.h
    qt/infrastructure/AboutLayout.ui
    qt/infrastructure/DmsEdit.cpp qt/infrastructure/DmsEdit.h
    qt/infrastructure/EPolygon.cpp qt/infrastructure/EPolygon.h
    qt/infrastructure/ETableWidget.cpp qt/infrastructure/ETableWidget.h
    qt/infrastructure/EfotoDoubleSpinBox.cpp qt/infrastructure/EfotoDoubleSpinBox.h
    qt/infrastructure/FlightDirectionForm.cpp qt/infrastructure/FlightDirectionForm.h qt/infrastructure/FlightDirectionForm.ui
    qt/infrastructure/LoadingScreen.cpp qt/infrastructure/LoadingScreen.h qt/infrastructure/LoadingScreen.ui
    qt/infrastructure/ScienceSpinBox.cpp qt/infrastructure/ScienceSpinBox.h
    qt/infrastructure/SigmaForm.cpp qt/infrastructure/SigmaForm.h
    qt/infrastructure/WindowsSelectPage.cpp qt/infrastructure/WindowsSelectPage.h
    qt/interface/DEMUserInterface_Qt.cpp qt/interface/DEMUserInterface_Qt.h
    qt/interface/DemExtraction.ui
    qt/interface/EfotoViewer.ui
    qt/interface/FormProject.ui
    qt/interface/FotoTriForm.ui
    qt/interface/IOForm.ui
    qt/interface/IOUserInterface_Qt.cpp qt/interface/IOUserInterface_Qt.h
    qt/interface/Imageview.ui
    qt/interface/OrthoForm.ui
    qt/interface/OrthoQualityEditor.ui
    qt/interface/OrthoUserInterface_Qt.cpp qt/interface/OrthoUserInterface_Qt.h
    qt/interface/PTUserInterface_Qt.cpp qt/interface/PTUserInterface_Qt.h
    qt/interface/ProgressWindow.ui
    qt/interface/ProgressWindow_Qt.cpp qt/interface/ProgressWindow_Qt.h
    qt/interface/ProjectUserInterface_Qt.cpp qt/interface/ProjectUserInterface_Qt.h
    qt/interface/ReportForm.ui
    qt/interface/ReportUserInterface_Qt.cpp qt/interface/ReportUserInterface_Qt.h
    qt/interface/SPUserInterface_Qt.cpp qt/interface/SPUserInterface_Qt.h
    qt/interface/SRForm.ui
    qt/interface/SRUserInterface_Qt.cpp qt/interface/SRUserInterface_Qt.h
    qt/interface/SeedEditor.ui
    qt/interface/Stereoplotter.ui
    qt/interface/EfotoViewer.ui
    qt/interface/main.cpp
)
target_include_directories(e-foto PRIVATE
    .
    c/control
    c/imageProcessing
    c/infrastructure
    c/interface
    c/photogrammetry
    c/shapelib
    c/xml_definitions
    qt/formDisplay
    qt/imageDisplay
    qt/infrastructure
    qt/interface
)

find_package(GDAL REQUIRED)
if(GDAL_FOUND)
    message(STATUS "GDAL found!")
    include_directories(${GDAL_INCLUDE_DIRS})
    set(GDAL_LIBRARIES GDAL::GDAL)
else()
    message(WARNING "GDAL not found! Using manual configuration.")
    set(GDAL_INCLUDE_DIRS "${GDAL_ROOT}/efoto/apps/gdal/include")
    set(GDAL_LIBRARIES "${GDAL_ROOT}/efoto/apps/gdal/lib/gdal.lib")
    include_directories(${GDAL_INCLUDE_DIRS})
    link_directories("${GDAL_ROOT}/efoto/apps/gdal/lib")
endif()

target_link_libraries(e-foto PRIVATE
    ${GDAL_LIBRARY}
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::Widgets
    Qt::OpenGLWidgets
)
target_compile_options(e-foto PRIVATE
    -Wall
    -std=c++17
)

# Resources:
set_source_files_properties("qt/resource/efotux-about.png"
    PROPERTIES QT_RESOURCE_ALIAS "Efotux"
)
set_source_files_properties("qt/resource/none.png"
    PROPERTIES QT_RESOURCE_ALIAS "None"
)
set_source_files_properties("qt/resource/pattern.png"
    PROPERTIES QT_RESOURCE_ALIAS "Pattern"
)
set(resource_resource_files
    "qt/resource/checked.png"
    "qt/resource/efotux-about.png"
    "qt/resource/exclamation.png"
    "qt/resource/logobrazil.png"
    "qt/resource/logoefoto.png"
    "qt/resource/logouerj.png"
    "qt/resource/none.png"
    "qt/resource/pattern.png"
    "qt/resource/unchecked.png"
)

qt_add_resources(e-foto "resource"
    PREFIX
        "/image"
    BASE
        "qt/resource"
    FILES
        ${resource_resource_files}
)
set_source_files_properties("qt/resource/ARROW32x32.png"
    PROPERTIES QT_RESOURCE_ALIAS "Arrow"
)
set_source_files_properties("qt/resource/T16x16.png"
    PROPERTIES QT_RESOURCE_ALIAS "ActivatedControl"
)
set_source_files_properties("qt/resource/T16x16red.png"
    PROPERTIES QT_RESOURCE_ALIAS "DeactivatedControl"
)
set_source_files_properties("qt/resource/X16x16.png"
    PROPERTIES QT_RESOURCE_ALIAS "Fiducial"
)
set_source_files_properties("qt/resource/X16x16_Amarelo.png"
    PROPERTIES QT_RESOURCE_ALIAS "FotoTriangulacao_Mark"
)
set_source_files_properties("qt/resource/cursor.bmp"
    PROPERTIES QT_RESOURCE_ALIAS "FotoTriangulacao_Cursor"
)
set_source_files_properties("qt/resource/cursorMask.bmp"
    PROPERTIES QT_RESOURCE_ALIAS "FotoTriangulacao"
)
set_source_files_properties("qt/resource/glcursor.png"
    PROPERTIES QT_RESOURCE_ALIAS "FotoTriangulacao_CursorGL"
)
set(resource1_resource_files
    "qt/resource/ARROW32x32.png"
    "qt/resource/T16x16.png"
    "qt/resource/T16x16red.png"
    "qt/resource/X16x16.png"
    "qt/resource/X16x16_Amarelo.png"
    "qt/resource/cursor.bmp"
    "qt/resource/cursorMask.bmp"
    "qt/resource/glcursor.png"
)

qt_add_resources(e-foto "resource1"
    PREFIX
        "/pin"
    BASE
        "qt/resource"
    FILES
        ${resource1_resource_files}
)
set_source_files_properties("qt/resource/AliasingIcon.png"
    PROPERTIES QT_RESOURCE_ALIAS "aliasingIcon"
)
set_source_files_properties("qt/resource/ClosedHandCursor.png"
    PROPERTIES QT_RESOURCE_ALIAS "ClosedHand"
)
set_source_files_properties("qt/resource/MagnifyCursor.png"
    PROPERTIES QT_RESOURCE_ALIAS "Magnify"
)
set_source_files_properties("qt/resource/OpenHandCursor.png"
    PROPERTIES QT_RESOURCE_ALIAS "OpenHand"
)
set_source_files_properties("qt/resource/PointingHandCursor.png"
    PROPERTIES QT_RESOURCE_ALIAS "PointingHand"
)
set_source_files_properties("qt/resource/UpArrowCursor.png"
    PROPERTIES QT_RESOURCE_ALIAS "UpArrow"
)
set_source_files_properties("qt/resource/detailviewIcon.png"
    PROPERTIES QT_RESOURCE_ALIAS "detailIcon"
)
set_source_files_properties("qt/resource/mark.png"
    PROPERTIES QT_RESOURCE_ALIAS "markIcon"
)
set_source_files_properties("qt/resource/moveIcon.png"
    PROPERTIES QT_RESOURCE_ALIAS "moveIcon"
)
set_source_files_properties("qt/resource/overviewIcon.png"
    PROPERTIES QT_RESOURCE_ALIAS "overIcon"
)
set_source_files_properties("qt/resource/zoomIcon.png"
    PROPERTIES QT_RESOURCE_ALIAS "zoomIcon"
)
set(resource2_resource_files
    "qt/resource/AliasingIcon.png"
    "qt/resource/ClosedHandCursor.png"
    "qt/resource/MagnifyCursor.png"
    "qt/resource/OpenHandCursor.png"
    "qt/resource/PointingHandCursor.png"
    "qt/resource/UpArrowCursor.png"
    "qt/resource/WindowIcon.png"
    "qt/resource/abort.png"
    "qt/resource/add.png"
    "qt/resource/addmark.png"
    "qt/resource/calculate.png"
    "qt/resource/cancel.png"
    "qt/resource/dem.png"
    "qt/resource/detailviewIcon.png"
    "qt/resource/disquette.png"
    "qt/resource/done.png"
    "qt/resource/edit.png"
    "qt/resource/equalmoveIcon.png"
    "qt/resource/equalzoomIcon.png"
    "qt/resource/exit.png"
    "qt/resource/exit2.png"
    "qt/resource/export.png"
    "qt/resource/exportKMLIcon.png"
    "qt/resource/exterior.png"
    "qt/resource/fd.png"
    "qt/resource/featdone.png"
    "qt/resource/fileopen.png"
    "qt/resource/filesave"
    "qt/resource/find.png"
    "qt/resource/fit.png"
    "qt/resource/fitbothIcon.png"
    "qt/resource/fitleftIcon.png"
    "qt/resource/fitrightIcon.png"
    "qt/resource/fixdetailviewIcon.png"
    "qt/resource/grid.png"
    "qt/resource/interior.ico"
    "qt/resource/interior.png"
    "qt/resource/mark.png"
    "qt/resource/markIcon.png"
    "qt/resource/match.png"
    "qt/resource/move.png"
    "qt/resource/moveIcon.png"
    "qt/resource/ortho.png"
    "qt/resource/orthoTiff.png"
    "qt/resource/overviewIcon.png"
    "qt/resource/ptadd.png"
    "qt/resource/ptedit.png"
    "qt/resource/ptremove.png"
    "qt/resource/remove.png"
    "qt/resource/removeall.png"
    "qt/resource/report.png"
    "qt/resource/searchfind"
    "qt/resource/seed.png"
    "qt/resource/select.png"
    "qt/resource/stereo.png"
    "qt/resource/table.png"
    "qt/resource/unmark.png"
    "qt/resource/zoomIcon.png"
)

qt_add_resources(e-foto "resource2"
    PREFIX
        "/icon"
    BASE
        "qt/resource"
    FILES
        ${resource2_resource_files}
)
set(resource3_resource_files
    "qt/resource/AboutMessage.html"
)

qt_add_resources(e-foto "resource3"
    PREFIX
        "/text"
    BASE
        "qt/resource"
    FILES
        ${resource3_resource_files}
)
set(shaders_resource_files
    "qt/resource/fshader.glsl"
    "qt/resource/vshader.glsl"
)

qt_add_resources(e-foto "shaders"
    PREFIX
        "/"
    BASE
        "qt/resource"
    FILES
        ${shaders_resource_files}
)

target_compile_definitions(e-foto PRIVATE ${PLATFORM_DEFINITIONS})

install(TARGETS e-foto
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(WIN32)
    qt_generate_deploy_app_script(
        TARGET e-foto
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${deploy_script})
endif()
