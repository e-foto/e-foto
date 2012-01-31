TEMPLATE = app
TARGET = e-foto

# Build settings
DESTDIR = ../bin
OBJECTS_DIR = ../temp/obj
MOC_DIR = ../temp/moc
UI_DIR = ../temp/ui
RCC_DIR = ../temp/rcc

QT += opengl

unix {
INCLUDEPATH += \
			/usr/include/opencv
LIBS += \
			-lGL \
			-lGLU \
			-lcv \
			-lhighgui \
			-lcvaux
}

win32 {
	INCLUDEPATH += C:/OpenCV2.0/include/opencv
	LIBS += "C:/OpenCV2.0\bin\libcv200.dll" \
		"C:/OpenCV2.0\bin\libcvaux200.dll" \
		"C:/OpenCV2.0\bin\libcxcore200.dll" \
		"C:/OpenCV2.0\bin\libcxts200.dll" \
		"C:/OpenCV2.0\bin\libhighgui200.dll"
}

DEPENDPATH += \
			  ../trunk_candidata/c/control \
			  ../trunk_candidata/c/imageDisplay \
			  ../trunk_candidata/c/imageProcessing \
			  ../trunk_candidata/c/infrastructure \
			  ../trunk_candidata/c/interface \
			  ../trunk_candidata/c/photogrammetry \
			  ../trunk_candidata/c/xml_definitions \
			  ../trunk_candidata/cv/infrastructure \
			  ../trunk_candidata/qt/formDisplay \
			  ../trunk_candidata/qt/imageDisplay \
			  ../trunk_candidata/qt/infrastructure \
			  ../trunk_candidata/qt/interface \
			  ../trunk_candidata/qt/resource

INCLUDEPATH += . \
			   ../trunk_candidata/c/infrastructure \
			   ../trunk_candidata/c/photogrammetry \
			   ../trunk_candidata/c/control \
			   ../trunk_candidata/c/interface \
			   ../trunk_candidata/qt/interface \
			   ../trunk_candidata/c/imageProcessing \
			   ../trunk_candidata/qt/imageDisplay \
			   ../trunk_candidata/qt/infrastructure \
			   ../trunk_candidata/c/imageDisplay \
			   ../trunk_candidata/c/xml_definitions \
			   ../trunk_candidata/qt/formDisplay \
			   ../trunk_candidata/cv/infrastructure \
			   ../trunk_candidata/qt/resource

# Input
HEADERS += \
		   #c/control/
		   DEMManager.h \
		   EFotoManager.h \
		   IOManager.h \
		   OrthoManager.h \
		   ProjectManager.h \
		   PTManager.h \
		   SPManager.h \
		   SRManager.h \
		   #c/imageDisplay/
		   AbstractSWidget.h \
		   GLMethods.h \
		   SExhibition.h \
		   SImage.h \
		   SMark.h \
		   SMarker.h \
		   STile.h \
		   SViewport.h \
		   #c/imageProcessing/
		   ImageMatching.h \
		   Interpolation.h \
		   LeastSquaresMatching.h \
		   NormalizedCrossCorrelation.h \
		   RadiometricTransformation.h \
		   #c/infrastructure/
		   AbstractAnalogCoordinate.h \
		   AbstractDigitalCoordinate.h \
		   CommonMethods.h \
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
		   RectSupport.h \
		   SparseMatrix.h \
		   #c/interface/
		   DEMUserInterface.h \
		   IOUserInterface.h \
		   OrthoUserInterface.h \
		   ProjectUserInterface.h \
		   PTUserInterface.h \
		   SPUserInterface.h \
		   SRUserInterface.h \
		   #c/photogrammetry/
		   AnalogFiductialMark.h \
		   AnalogImageSpaceCoordinate.h \
		   BundleAdjustment.h \
		   CheckingPoint.h \
		   ControlPoint.h \
		   DemFeatures.h \
		   DemGrid.h \
		   DigitalFiductialMark.h \
		   DigitalImageSpaceCoordinate.h \
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
		   PhotogrammetricPoint.h \
		   Point.h \
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
		   Terrain.h \
		   #c/xml_definitions/
		   XMLAerial.h \
		   XMLFlight.h \
		   XMLProjectHeader.h \
		   XMLSensor.h \
		   XMLSensorWithFiducialMarks.h \
		   XMLTerrain.h \
		   XmlUpdater.h \
		   #cv/infrastructure/
		   CommonCVMethods.h \
		   #qt/formDisplay/
		   AbstractForm.h \
		   ControlButtons.h \
		   FlightForm.h \
		   HeaderForm.h \
		   ImageForm.h \
		   ImagesForm.h \
		   PointForm.h \
		   PointsForm.h \
		   SensorForm.h \
		   TerrainForm.h \
		   #qt/imageDisplay/
		   GeometryResource.h \
		   ImageView.h \
		   ImageViewers.h \
		   #PhotoTools.h \
		   RasterResource.h \
		   SingleDisplay.h \
		   SingleScene.h \
		   SingleTools.h \
		   StereoDisplay.h \
		   StereoScene.h \
		   StereoTools.h \
		   SWidgetQt.h \
		   #qt/infrastructure/
		   AboutForm.h \
		   CommonQtMethods.h \
		   DmsEdit.h \
		   EfotoDoubleSpinBox.h \
		   EPolygon.h \
		   ETableWidget.h \
		   FlightDirectionForm.h \
		   LoadingScreen.h \
		   ScienceSpinBox.h \
		   SigmaForm.h \
		   WindowsSelectPage.h \
		   #qt/interface/
		   DEMUserInterface_Qt.h \
		   IOUserInterface_Qt.h \
		   OrthoUserInterface_Qt.h \
		   ProgressWindow_Qt.h \
		   ProjectUserInterface_Qt.h \
		   PTUserInterface_Qt.h \
		   SPUserInterface_Qt.h \
                   SRUserInterface_Qt.h \
                   #stereoPlotterProject
                   testTable.h

SOURCES += \
		   #c/control/
		   DEMManager.cpp \
		   EFotoManager.cpp \
		   IOManager.cpp \
		   OrthoManager.cpp \
		   ProjectManager.cpp \
		   PTManager.cpp \
		   SPManager.cpp \
		   SRManager.cpp \
		   #c/imageDisplay/
		   AbstractSWidget.cpp \
		   GLMethods.cpp \
		   SExhibition.cpp \
		   SImage.cpp \
		   SMark.cpp \
		   SMarker.cpp \
		   STile.cpp \
		   SViewport.cpp \
		   #c/imageProcessing/
		   ImageMatching.cpp \
		   Interpolation.cpp \
		   LeastSquaresMatching.cpp \
		   NormalizedCrossCorrelation.cpp \
		   RadiometricTransformation.cpp \
		   #c/infrastructure/
		   AbstractAnalogCoordinate.cpp \
		   AbstractDigitalCoordinate.cpp \
		   CommonMethods.cpp \
		   ConvertionsSystems.cpp \
		   Coordinate.cpp \
		   Dms.cpp \
		   EDom.cpp \
		   EDomValidator.cpp \
		   ETreeModel.cpp \
		   GeoSystem.cpp \
		   Matrix.cpp \
		   PositionMatrix.cpp \
		   RectSupport.cpp \
		   SparseMatrix.cpp \
		   #c/interface/
		   DEMUserInterface.cpp \
		   IOUserInterface.cpp \
		   OrthoUserInterface.cpp \
		   ProjectUserInterface.cpp \
		   PTUserInterface.cpp \
		   SPUserInterface.cpp \
		   SRUserInterface.cpp \
		   #c/photogrammetry/
		   AnalogFiductialMark.cpp \
		   AnalogImageSpaceCoordinate.cpp \
		   BundleAdjustment.cpp \
		   CheckingPoint.cpp \
		   ControlPoint.cpp \
		   DemFeatures.cpp \
		   DemGrid.cpp \
		   DigitalFiductialMark.cpp \
		   DigitalImageSpaceCoordinate.cpp \
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
		   PhotogrammetricPoint.cpp \
		   Point.cpp \
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
		   Terrain.cpp \
		   #c/xml_definitions/
		   XMLAerial.cpp \
		   XMLFlight.cpp \
		   XMLProjectHeader.cpp \
		   XMLSensor.cpp \
		   XMLSensorWithFiducialMarks.cpp \
		   XMLTerrain.cpp \
		   XmlUpdater.cpp \
		   #cv/infrastructure/
		   CommonCVMethods.cpp \
		   #qt/formDisplay/
		   ControlButtons.cpp \
		   FlightForm.cpp \
		   HeaderForm.cpp \
		   ImageForm.cpp \
		   ImagesForm.cpp \
		   PointForm.cpp \
		   PointsForm.cpp \
		   SensorForm.cpp \
		   TerrainForm.cpp \
		   #qt/imageDisplay/
		   GeometryResource.cpp \
		   ImageView.cpp \
		   ImageViewers.cpp \
		   #PhotoTools.cpp \
		   RasterResource.cpp \
		   SingleDisplay.cpp \
		   SingleScene.cpp \
		   SingleTools.cpp \
		   StereoDisplay.cpp \
		   StereoScene.cpp \
		   StereoTools.cpp \
		   SWidgetQt.cpp \
		   #qt/infrastructure/
		   AboutForm.cpp \
		   CommonQtMethods.cpp \
		   DmsEdit.cpp \
		   EfotoDoubleSpinBox.cpp \
		   EPolygon.cpp \
		   ETableWidget.cpp \
		   FlightDirectionForm.cpp \
		   LoadingScreen.cpp \
		   ScienceSpinBox.cpp \
		   SigmaForm.cpp \
		   WindowsSelectPage.cpp \
		   #qt/interface/
		   DEMUserInterface_Qt.cpp \
		   IOUserInterface_Qt.cpp \
                   #main.cpp \
		   OrthoUserInterface_Qt.cpp \
		   ProgressWindow_Qt.cpp \
		   ProjectUserInterface_Qt.cpp \
		   PTUserInterface_Qt.cpp \
		   SPUserInterface_Qt.cpp \
                   SRUserInterface_Qt.cpp \
                   #stereoPlotterProject
                   testTable.cpp \
                   testMain.cpp

FORMS += \
		  #qt/formDisplay/
		  FeatEditor.ui \
		  FeatsEditor.ui \
		  FlightLayout.ui \
		  HeaderLayout.ui \
		  ImageLayout.ui \
		  ImagesLayout.ui \
		  PointLayout.ui \
		  PointsLayout.ui \
		  SensorLayout.ui \
		  SpEditor.ui \
		  TerrainLayout.ui \
		  #qt/infrastructure/
		  AboutLayout.ui \
		  FlightDirectionForm.ui \
		  LoadingScreen.ui \
		  #qt/interface/
		  DemExtraction.ui \
		  EfotoViewer.ui \
		  FormProject.ui \
		  FotoTriForm.ui \
		  Imageview.ui \
		  IOForm.ui \
		  OrthoForm.ui \
		  ProgressWindow.ui \
		  SeedEditor.ui \
		  SRForm.ui

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
		#system(sed -r s/Revision\" \"[0-9]{3}/Revision\" \"$${MYREV}/ -i $${ABOUTDIR}/AboutLayout.ui)# atualiza a revis�o do AboutLayout
		!build_pass:message(Release build! WIN32)# Essa linha pode ser suprimida, isso s� aparecer� na saida do compilador(Compile Output)
	}
	else:!build_pass:message(Debug build! WIN32)# Essa linha pode ser suprimida, isso s� aparecer� na saida do compilador(Compile Output)
}
#linha para colocar o icone do e-foto no executavel
RC_FILE = efotoIcon.rc
