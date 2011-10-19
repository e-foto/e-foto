TEMPLATE = app
TARGET = e-foto

# Build settings
DESTDIR = ../bin
OBJECTS_DIR = ../temp/obj
MOC_DIR = ../temp/moc
UI_DIR = ../temp/ui
RCC_DIR = ../temp/rcc
QT += opengl
DEPENDPATH += c/control \
    c/imageDisplay \
    c/infrastructure \
    c/interface \
    c/photogrammetry \
    c/xml_definitions \
    cv/infrastructure \
    qt/formDisplay \
    qt/imageDisplay \
    qt/infrastructure \
    qt/interface \
    qt/resource \
    sdl/infrastructure
INCLUDEPATH += . \
    c/control \
    c/infrastructure \
    c/photogrammetry \
    c/interface \
    c/imageDisplay \
    c/xml_definitions \
    cv/infrastructure \
    qt/interface \
    qt/imageDisplay \
    qt/formDisplay \
    qt/infrastructure \
    qt/resource \
	sdl/infrastructure #\
	#c/infrastructure/Eigen
unix { 
    INCLUDEPATH += /usr/include/opencv \
        /usr/include/SDL
    LIBS += -lGL \
        -lGLU \
        -lcv \
        -lhighgui \
        -lcvaux \
        -lSDL \
		-lSDL_image#\
	#	-lgomp
    HEADERS += CommonSDLMethods.h
    SOURCES += CommonSDLMethods.cpp
	#QMAKE_CXXFLAGS += -fopenmp
	#QMAKE_LFLAGS += -fopenmp
}
win32 { 
    INCLUDEPATH += C:/OpenCV2.0/include/opencv
	LIBS += "C:/OpenCV2.0\bin\libcv200.dll" \
		"C:/OpenCV2.0\bin\libcvaux200.dll" \
		"C:/OpenCV2.0\bin\libcxcore200.dll" \
		"C:/OpenCV2.0\bin\libcxts200.dll" \
		"C:/OpenCV2.0\bin\libhighgui200.dll"
}

# Input
# c/control/
HEADERS += EFotoManager.h \
    IOManager.h \
    ProjectManager.h \
    SRManager.h \
    PTManager.h \ # c/imageDisplay/
    AbstractSWidget.h \
    GLMethods.h \
    SExhibition.h \
    SImage.h \
    SMark.h \
    SMarker.h \
    STile.h \
    SViewport.h \ # c/infrastructure/
    AbstractAnalogCoordinate.h \
    AbstractDigitalCoordinate.h \
    CommonMethods.h \
    Coordinate.h \
    Dms.h \
    ConvertionsSystems.h \
    GeoSystem.h \
    EDom.h \
    EDomValidator.h \
    EObject.h \
    EParser.h \
    ETreeModel.h \
    Matrix.h \
	SparseMatrix.h \
    PositionMatrix.h \
    RectSupport.h \ # c/interface
    IOUserInterface.h \
    ProjectUserInterface.h \
    SRUserInterface.h \
    PTUserInterface.h \ # c/photogrammetry/
    AnalogFiductialMark.h \
    AnalogImageSpaceCoordinate.h \
    BundleAdjustment.h \
    CheckingPoint.h \
    ControlPoint.h \
    DigitalFiductialMark.h \
    DigitalImageSpaceCoordinate.h \
    EOQuality.h \
    ExteriorOrientation.h \
    Flight.h \
    FrameSensor.h \
    Image.h \
    InteriorOrientation.h \
    IOQuality.h \
    ObjectSpaceCoordinate.h \
    PhotogrammetricPoint.h \
    Point.h \
    ProjectHeader.h \
    Sensor.h \
    SensorWithFiducialMarks.h \
    SensorWithKnowDimensions.h \
    SensorWithKnowParameters.h \
    SpatialRessection.h \
    Terrain.h \ # c/xml_definitions/
    XMLAerial.h \
    XMLFlight.h \
    XMLProjectHeader.h \
    XMLSensor.h \
    XMLSensorWithFiducialMarks.h \
    XMLTerrain.h \
    XmlUpdater.h \ # cv/infrastructure/
    CommonCVMethods.h \ # qt/formDisplay
    AbstractForm.h \
    ControlButtons.h \
    FlightForm.h \
    HeaderForm.h \
    ImageForm.h \
    ImagesForm.h \
    PointForm.h \
    PointsForm.h \
    SensorForm.h \
    TerrainForm.h \ # qt/imageDisplay
    ImageView.h \
    SWidgetQt.h \
    AbstractDisplay.h \
    StereoService.h \
    StereoDisplay.h \
    RasterResource.h \
    Geometry.h \
    View.h \ # qt/infrastructure/
    AboutForm.h \
    LoadingScreen.h \
    CommonQtMethods.h \
    EfotoDoubleSpinBox.h \
    ScienceSpinBox.h \
    SigmaForm.h \
    ETableWidget.h \
    DmsEdit.h \
	WindowsSelectPage.h \
	FlightDirectionForm.h\
	#PhotoTriangulationParametersForm.h \
# qt/interface/
    IOUserInterface_Qt.h \
    ProjectUserInterface_Qt.h \
    SRUserInterface_Qt.h \
	PTUserInterface_Qt.h



# qt/formDisplay/
FORMS += FlightLayout.ui \
    HeaderLayout.ui \
    ImageLayout.ui \
    ImagesLayout.ui \
    LoadingScreen.ui \
    PointLayout.ui \
    PointsLayout.ui \
    SensorLayout.ui \
    TerrainLayout.ui \ # qt/infrastructure/
    AboutLayout.ui \
	#PhotoTriangulationParametersForm.ui\
    LoadingScreen.ui \ # qt/interface/
    FormProject.ui \
    IOForm.ui \
    SRForm.ui \
    FotoTriForm.ui \
	FlightDirectionForm.ui

# c/control/
SOURCES += EFotoManager.cpp \
    IOManager.cpp \
    ProjectManager.cpp \
    SRManager.cpp \
    PTManager.cpp \ # c/imageDisplay/
    AbstractSWidget.cpp \
    GLMethods.cpp \
    SExhibition.cpp \
    SImage.cpp \
    SMark.cpp \
    SMarker.cpp \
    STile.cpp \
    SViewport.cpp \ # c/infrastructure/
    AbstractAnalogCoordinate.cpp \
    AbstractDigitalCoordinate.cpp \
    CommonMethods.cpp \
    Coordinate.cpp \
    ConvertionsSystems.cpp \
    Dms.cpp \
    EDom.cpp \
    EDomValidator.cpp \
    EObject.cpp \
    EParser.cpp \
    ETreeModel.cpp \
    GeoSystem.cpp \
    Matrix.cpp \
	SparseMatrix.cpp \
    PositionMatrix.cpp \
    RectSupport.cpp \ # c/interface/
    IOUserInterface.cpp \
    ProjectUserInterface.cpp \
    SRUserInterface.cpp \
	PTUserInterface.cpp \
	#PhotoTriangulationParametersForm.cpp \
	# photogrammetry/
    AnalogFiductialMark.cpp \
    AnalogImageSpaceCoordinate.cpp \
    BundleAdjustment.cpp \
    CheckingPoint.cpp \
    ControlPoint.cpp \
    DigitalFiductialMark.cpp \
    DigitalImageSpaceCoordinate.cpp \
    EOQuality.cpp \
    ExteriorOrientation.cpp \
    Flight.cpp \
    FrameSensor.cpp \
    Image.cpp \
    InteriorOrientation.cpp \
    IOQuality.cpp \
    ObjectSpaceCoordinate.cpp \
    PhotogrammetricPoint.cpp \
    Point.cpp \
    ProjectHeader.cpp \
    Sensor.cpp \
    SensorWithFiducialMarks.cpp \
    SensorWithKnowDimensions.cpp \
    SensorWithKnowParameters.cpp \
    SpatialRessection.cpp \
    Terrain.cpp \ # c/xml_definitions/
    XMLAerial.cpp \
    XMLFlight.cpp \
    XMLProjectHeader.cpp \
    XMLSensor.cpp \
    XMLSensorWithFiducialMarks.cpp \
    XMLTerrain.cpp \
    XmlUpdater.cpp \ # cv/infrastructure/
    CommonCVMethods.cpp \ # qt/formDisplay/
    ControlButtons.cpp \
    FlightForm.cpp \
    HeaderForm.cpp \
    ImageForm.cpp \
    ImagesForm.cpp \
    PointForm.cpp \
    PointsForm.cpp \
    SensorForm.cpp \
    TerrainForm.cpp \ # qt/imageDisplay/
    ImageView.cpp \
    SWidgetQt.cpp \
    AbstractDisplay.cpp \
    StereoService.cpp \
    StereoDisplay.cpp \
    RasterResource.cpp \
    Geometry.cpp \
    View.cpp \ # qt/infrastructure/
    AboutForm.cpp \
    LoadingScreen.cpp \
    CommonQtMethods.cpp \
    DmsEdit.cpp \
    ETableWidget.cpp \
    EfotoDoubleSpinBox.cpp \
    ScienceSpinBox.cpp \
    SigmaForm.cpp \
	WindowsSelectPage.cpp \
	FlightDirectionForm.cpp \
	#PhotoTriangulationParametersForm.cpp \
	 # qt/interface/
    IOUserInterface_Qt.cpp \
	ProjectUserInterface_Qt.cpp \
    SRUserInterface_Qt.cpp \
    PTUserInterface_Qt.cpp \
	main.cpp
# qt/resource/
RESOURCES += resource.qrc
ABOUTDIR = qt/infrastructure/
unix { 
    MYDATA = $$system(date -u +%Y.%m.%d) # a versao release so precisa da data e olhe la!
    
    # MYDATA = $$system(date -u +%y.%m.%d) # Data com o ano usando apenas com 2 digitos
    MYREV = $$system(svnversion)
    MYREV ~= s/[a-z]|[A-Z]/
    CONFIG(release, debug|release) { # Verifica se esta em modo RELEASE
        system(sed -r s/[0-9]{4}\.[0-9]{2}\.[0-9]{2}/$${MYDATA}/ -i $${ABOUTDIR}/AboutLayout.ui)# atualiza o data do BUILD AboutLayout.ui com a data da compilaçao
        system(sed -r s/Revision\ [0-9]{3}/Revision\ $${MYREV}/ -i $${ABOUTDIR}/AboutLayout.ui)# atualiza a revis�o do AboutLayout
        !build_pass:message(Release build! UNIX)# Essa linha pode ser suprimida, isso so aparecera na saida do compilador(Compile Output)
    }
    else:!build_pass:message(Debug build! UNIX)# Essa linha pode ser suprimida, isso so aparecera na saida do compilador(Compile Output)
}
win32 { 
    MYDATA = $$system(for /F \"usebackq tokens=1,2,3,4* delims=/| \" %a in (\'%date%\') do @echo %d %c %b %a)
    MYDATA ~= s/\D/
    MYYEAR = $$member(MYDATA,0).
    MYMONTH = $$member(MYDATA,1).
    MYDAY = $$member(MYDATA,2)
    MYDATA = $$MYYEAR$$MYMONTH$$MYDAY
    
    # MYDATA2 = $${MYDATA}
    # MYDATA2 ~= s/^[0-9]{3}/ #Retira os trs primeiros digitos do Ano na data, ou seja, yy.mm.dd
    MYREV = $$system(subWCrev ../)
    MYREV = $$find(MYREV, [0-9]{3}.)
    MYREV ~= s/\D/ # Pega tudo menos numeros(Non-digit)
    MYREV ~= s/^[0-9]{3}/ # Elimina os trs primeiros digitos
    !build_pass:message(rev: $${MYREV})
    !build_pass:message(data: $${MYDATA})
    CONFIG(release, debug|release) { # Verifica se esta em modo RELEASE
        system(sed -r s/[0-9]{4}\.[0-9]{2}\.[0-9]{2}/$${MYDATA}/ -i $${ABOUTDIR}/AboutLayout.ui)# atualiza o data do BUILD AboutLayout.ui com a data da compilaçao
        system(sed -r s/Revision\" \"[0-9]{3}/Revision\" \"$${MYREV}/ -i $${ABOUTDIR}/AboutLayout.ui)# atualiza a revis�o do AboutLayout
        !build_pass:message(Release build! WIN32)# Essa linha pode ser suprimida, isso s� aparecer� na saida do compilador(Compile Output)
    }
    else:!build_pass:message(Debug build! WIN32)# Essa linha pode ser suprimida, isso s� aparecer� na saida do compilador(Compile Output)
}
