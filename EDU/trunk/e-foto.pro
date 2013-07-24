TEMPLATE = app
TARGET = e-foto

# Build settings
DESTDIR = ../bin
OBJECTS_DIR = ../temp/obj
MOC_DIR = ../temp/moc
UI_DIR = ../temp/ui
RCC_DIR = ../temp/rcc
QT += opengl
unix:LIBS += -lGL \
    -lGLU
win32:
DEPENDPATH += c/control \
    c/imageDisplay \
    c/imageProcessing \
    c/infrastructure \
    c/interface \
    c/photogrammetry \
    c/xml_definitions \
    qt/formDisplay \
    qt/imageDisplay \
    qt/infrastructure \
    qt/interface \
    qt/resource
INCLUDEPATH += . \
    c/infrastructure \
    c/photogrammetry \
    c/control \
    c/interface \
    qt/interface \
    c/imageProcessing \
    qt/imageDisplay \
    qt/infrastructure \
    c/imageDisplay \
    c/xml_definitions \
    qt/formDisplay \
    qt/resource

# Input
# c/control/
HEADERS += DEMManager.h \
    EFotoManager.h \
    IOManager.h \
    OrthoManager.h \
    ProjectManager.h \
    PTManager.h \
    ReportManager.h \
    SPManager.h \
    SRManager.h \ # c/imageDisplay/
# AbstractSWidget.h \
# GLMethods.h \
# SExhibition.h \
# SImage.h \
# SMark.h \
# SMarker.h \
# STile.h \
# SViewport.h \
# c/imageProcessing/
    ImageMatching.h \
    Interpolation.h \
    LeastSquaresMatching.h \
    NormalizedCrossCorrelation.h \
    RadiometricTransformation.h \ # c/infrastructure/
    AbstractDetectorCoordinate.h \
    AbstractImageCoordinate.h \ # CommonMethods.h \
    ConvertionsSystems.h \
    Coordinate.h \
    Dms.h \
    EDom.h \
    EDomValidator.h \
    EObject.h \
    ETreeModel.h \
    GeoSystem.h \
    Matrix.h \
    PositionMatrix.h \
    ProgressPublisher.h \
    RectSupport.h \
    SparseMatrix.h \
    TreeFeatures.h \ # c/interface/
    DEMUserInterface.h \
    IOUserInterface.h \
    OrthoUserInterface.h \
    ProjectUserInterface.h \
    PTUserInterface.h \
    ReportUserInterface.h \
    SPUserInterface.h \
    SRUserInterface.h \ # c/photogrammetry/
    DetectorFiducialMark.h \
    DetectorSpaceCoordinate.h \
    Dummies.h \
    BundleAdjustment.h \
    DemFeatures.h \
    DemGrid.h \
    ImageFiducialMark.h \
    ImageSpaceCoordinate.h \
    EOQuality.h \
    ExteriorOrientation.h \
    Flight.h \
    FrameSensor.h \
    Image.h \
    InteriorOrientation.h \
    IOQuality.h \
    MatchingPoints.h \
    MatchingPointsGrid.h \
    ObjectSpaceCoordinate.h \
    Orthorectification.h \
    Point.h \
    Project.h \
    ProjectHeader.h \
    ProjectiveRay.h \
    RayTester.h \
    Sensor.h \
    SensorWithFiducialMarks.h \
    SensorWithKnowDimensions.h \
    SensorWithKnowParameters.h \
    SpatialIntersection.h \
    SpatialRessection.h \
    StereoPair.h \
    Terrain.h \ # c/xml_definitions/
    XMLAerial.h \
    XMLFlight.h \
    XMLProjectHeader.h \
    XMLSensor.h \
    XMLSensorWithFiducialMarks.h \
    XMLTerrain.h \
    XmlUpdater.h \ # cv/infrastructure/
# CommonCVMethods.h \
# qt/formDisplay/
    AbstractForm.h \
    ControlButtons.h \
    FlightForm.h \
    HeaderForm.h \
    ImageForm.h \
    ImagesForm.h \
    PointForm.h \
    PointsForm.h \
    SensorForm.h \
    TerrainForm.h \ # qt/imageDisplay/
    GeometryResource.h \ # ImageView.h \
    ImageViewers.h \ # PhotoTools.h \
    RasterResource.h \
    SingleDisplay.h \
    SingleScene.h \
    SingleTools.h \
    StereoDisplay.h \
    StereoScene.h \
    StereoTools.h \ # SWidgetQt.h \
# qt/infrastructure/
    AboutForm.h \ # CommonQtMethods.h \
    DmsEdit.h \
    EfotoDoubleSpinBox.h \
    EPolygon.h \
    ETableWidget.h \
    FlightDirectionForm.h \
    LoadingScreen.h \
    ScienceSpinBox.h \
    SigmaForm.h \
    WindowsSelectPage.h \ # qt/interface/
    DEMUserInterface_Qt.h \
    IOUserInterface_Qt.h \
    OrthoUserInterface_Qt.h \
    ProgressWindow_Qt.h \
    ProjectUserInterface_Qt.h \
    PTUserInterface_Qt.h \
    ReportUserInterface_Qt.h \
    SPUserInterface_Qt.h \
    SRUserInterface_Qt.h \
    c/photogrammetry/PhotoTriReport.h

# c/control/
SOURCES += DEMManager.cpp \
    EFotoManager.cpp \
    IOManager.cpp \
    OrthoManager.cpp \
    ProjectManager.cpp \
    PTManager.cpp \
    ReportManager.cpp \
    SPManager.cpp \
    SRManager.cpp \ # c/imageDisplay/
# AbstractSWidget.cpp \
# GLMethods.cpp \
# SExhibition.cpp \
# SImage.cpp \
# SMark.cpp \
# SMarker.cpp \
# STile.cpp \
# SViewport.cpp \
# c/imageProcessing/
    ImageMatching.cpp \
    Interpolation.cpp \
    LeastSquaresMatching.cpp \
    NormalizedCrossCorrelation.cpp \
    RadiometricTransformation.cpp \ # c/infrastructure/
    AbstractDetectorCoordinate.cpp \
    AbstractImageCoordinate.cpp \ # CommonMethods.cpp \
    ConvertionsSystems.cpp \
    Coordinate.cpp \
    Dms.cpp \
    EDom.cpp \
    EDomValidator.cpp \
    ETreeModel.cpp \
    GeoSystem.cpp \
    Matrix.cpp \
    PositionMatrix.cpp \
    ProgressPublisher.cpp \
    RectSupport.cpp \
    SparseMatrix.cpp \
    TreeFeatures.cpp \ # c/interface/
    DEMUserInterface.cpp \
    IOUserInterface.cpp \
    OrthoUserInterface.cpp \
    ProjectUserInterface.cpp \
    PTUserInterface.cpp \
    ReportUserInterface.cpp \
    SPUserInterface.cpp \
    SRUserInterface.cpp \ # c/photogrammetry/
    DetectorFiducialMark.cpp \
    DetectorSpaceCoordinate.cpp \
    Dummies.cpp \
    BundleAdjustment.cpp \
    DemFeatures.cpp \
    DemGrid.cpp \
    ImageFiducialMark.cpp \
    ImageSpaceCoordinate.cpp \
    EOQuality.cpp \
    ExteriorOrientation.cpp \
    Flight.cpp \
    FrameSensor.cpp \
    Image.cpp \
    InteriorOrientation.cpp \
    IOQuality.cpp \
    MatchingPoints.cpp \
    MatchingPointsGrid.cpp \
    ObjectSpaceCoordinate.cpp \
    Orthorectification.cpp \
    Point.cpp \
    Project.cpp \
    ProjectHeader.cpp \
    ProjectiveRay.cpp \
    RayTester.cpp \
    Sensor.cpp \
    SensorWithFiducialMarks.cpp \
    SensorWithKnowDimensions.cpp \
    SensorWithKnowParameters.cpp \
    SpatialIntersection.cpp \
    SpatialRessection.cpp \
    StereoPair.cpp \
    Terrain.cpp \ # c/xml_definitions/
    XMLAerial.cpp \
    XMLFlight.cpp \
    XMLProjectHeader.cpp \
    XMLSensor.cpp \
    XMLSensorWithFiducialMarks.cpp \
    XMLTerrain.cpp \
    XmlUpdater.cpp \ # cv/infrastructure/
# CommonCVMethods.cpp \
# qt/formDisplay/
    ControlButtons.cpp \
    FlightForm.cpp \
    HeaderForm.cpp \
    ImageForm.cpp \
    ImagesForm.cpp \
    PointForm.cpp \
    PointsForm.cpp \
    SensorForm.cpp \
    TerrainForm.cpp \ # qt/imageDisplay/
    GeometryResource.cpp \ # ImageView.cpp \
    ImageViewers.cpp \ # PhotoTools.cpp \
    RasterResource.cpp \
    SingleDisplay.cpp \
    SingleScene.cpp \
    SingleTools.cpp \
    StereoDisplay.cpp \
    StereoScene.cpp \
    StereoTools.cpp \ # SWidgetQt.cpp \
# qt/infrastructure/
    AboutForm.cpp \ # CommonQtMethods.cpp \
    DmsEdit.cpp \
    EfotoDoubleSpinBox.cpp \
    EPolygon.cpp \
    ETableWidget.cpp \
    FlightDirectionForm.cpp \
    LoadingScreen.cpp \
    ScienceSpinBox.cpp \
    SigmaForm.cpp \
    WindowsSelectPage.cpp \ # qt/interface/
    DEMUserInterface_Qt.cpp \
    IOUserInterface_Qt.cpp \
    main.cpp \
    OrthoUserInterface_Qt.cpp \
    ProgressWindow_Qt.cpp \
    ProjectUserInterface_Qt.cpp \
    PTUserInterface_Qt.cpp \
    ReportUserInterface_Qt.cpp \
    SPUserInterface_Qt.cpp \
    SRUserInterface_Qt.cpp \
    c/photogrammetry/PhotoTriReport.cpp

# qt/formDisplay/
FORMS += FeatEditor.ui \
    FeatsEditor.ui \
    FlightLayout.ui \
    HeaderLayout.ui \
    ImageLayout.ui \
    ImagesLayout.ui \
    PointLayout.ui \
    PointsLayout.ui \
    SensorLayout.ui \
    SpEditor.ui \
    TerrainLayout.ui \ # qt/infrastructure/
    AboutLayout.ui \
    FlightDirectionForm.ui \
    LoadingScreen.ui \ # qt/interface/
    DemExtraction.ui \
    EfotoViewer.ui \
    FormProject.ui \
    FotoTriForm.ui \
    Imageview.ui \
    IOForm.ui \
    OrthoForm.ui \
    OrthoQualityEditor.ui \
    ProgressWindow.ui \
    ReportForm.ui \
    SeedEditor.ui \
    SRForm.ui \
    Stereoplotter.ui
RESOURCES += resource.qrc

# Autochange AboutForm code
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
        
        # system(sed -r s/Revision\" \"[0-9]{3}/Revision\" \"$${MYREV}/ -i $${ABOUTDIR}/AboutLayout.ui)# atualiza a revis�o do AboutLayout
        !build_pass:message(Release build! WIN32)# Essa linha pode ser suprimida, isso s� aparecer� na saida do compilador(Compile Output)
    }
    else:!build_pass:message(Debug build! WIN32)# Essa linha pode ser suprimida, isso s� aparecer� na saida do compilador(Compile Output)
}

# linha para colocar o icone do e-foto no executavel
RC_FILE = efotoIcon.rc
