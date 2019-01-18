# Build Settings
#DESTDIR = build/bin
#OBJECTS_DIR = build/temp/obj
#MOC_DIR = build/temp/moc
#UI_DIR = build/temp/ui
#RCC_DIR = build/temp/rcc
#QT += opengl
#QMAKE_CXXFLAGS += -std=c++11 -Wall


#VAR = foobar => Assign value to variable when qmake is run
#$$VAR => QMake variable's value at the time qmake is run
#$${VAR} => QMake variable's value at the time qmake is run (identical but enclosed to separate from surrounding text)
#$(VAR) => Contents of an Environment variable at the time Makefile (not qmake) is run
#$$(VAR) =>Contents of an Environment variable at the time qmake (not Makefile) is run
#$$[...] This means "configuration option that were set when Qt was built"
#message($$[QT_INSTALL_BINS]) gives:
#C:\Qt\Qt5.0.2\5.0.2\msvc2010_opengl\bin
#!build_pass: evita mutiplas passagens durante as múltiplas passagem do processo de build
#https://docs.huihoo.com/qt/4.5/supported-platforms.html

#UNIX##############################################
unix {
# Set libshape usage
    LIBS += -lshp
    LIBS += -lGL -lGLU
#Estou usando Qt5.11 instalado em opt se usar isso vai achar qt5.5, vai dar conflito.
#Além disso não precisa disso na compilação via IDE
#    LIBS += -L/usr/lib/x86_64-linux-gnu/
# Set gdal usage
    LIBS += -L/usr/include/gdal -lgdal
    INCLUDEPATH += /usr/include/gdal
    DEPENDPATH += /usr/include/gdal
# install settings
    DEFINES += unix
    target.path = /usr/bin
    desk.path = /usr/share/applications
    desk.files += efoto.desktop
    icon.path = /usr/share/applications/pixmaps
    icon.files += efoto-icon.png

    INSTALLS += target desk icon
    INCLUDEPATH += /usr/include/
    DEPENDPATH += /usr/include/

    # Build Settings
    DESTDIR = build/bin
    OBJECTS_DIR = build/temp/obj
    MOC_DIR = build/temp/moc
    UI_DIR = build/temp/ui
    RCC_DIR = build/temp/rcc
    QT += opengl
    QMAKE_CXXFLAGS += -std=c++11 -Wall
}

#linux-g++:contains(QMAKE_HOST.arch, x86):{
#       LIB=lib32
#       message("Compiling for 32bit system")
#    }
#linux-g++:contains(QMAKE_HOST.arch, x86):{
#       LIB=lib64
#       message("Compiling for 64bit system")
#}

#Windows###########################################

win32{
message("QT_ARCH: $$QT_ARCH")
message("QMAKE_HOST.arch: $$QMAKE_HOST.arch")

        #desliguei o modo config e release, mas se alguém setar vai para pasta debug
	#se necessário daria para configurar as 4 opções
	CONFIG-=debug_and_release
	CONFIG-=debug_and_release_target
	#Common parammeters for windows
	QT += opengl
    QMAKE_CXXFLAGS += -std=c++11 -Wall	
#O QT_ARCH não funcionou bem com o mysys2, então mudei para usar o g++ e qmake

win32-g++:contains(QMAKE_HOST.arch, x86):{
#Windows 32bits install
	#Config and Release common parammeters
    INCLUDEPATH += /mingw32/include
    DEPENDPATH += /mingw32/include
	#Set gdal
    LIBS += -L/mingw32/lib/ -lgdal
#Debug settings	
    !build_pass:CONFIG(debug, debug|release) {
    !build_pass:message($$find(CONFIG, "debug"))
    !build_pass:message("Windows de 32 bits")
    #Debug install settings
    debug: DESTDIR = build_debug_32bits/bin
    debug: OBJECTS_DIR = build_debug_32bits/temp/obj
    debug: MOC_DIR = build_debug_32bits/temp/moc
    debug: UI_DIR = build_debug_32bits/temp/ui
    debug: RCC_DIR = build_debug_32bits/temp/rcc
}
#Release settings
    !build_pass:CONFIG(release, debug|release) {
    !build_pass:message($$find(CONFIG, "release"))
    !build_pass:message("Windows de 32 bits")
    #Release Settings
    release: DESTDIR = build_release_32bits/bin
    release: OBJECTS_DIR = build_release_32bits/temp/obj
    release: MOC_DIR = build_release_32bits/temp/moc
    release: UI_DIR = build_release_32bits/temp/ui
    release: RCC_DIR = build_release_32bits/temp/rcc
}
}

win32-g++:contains(QMAKE_HOST.arch, x86_64):{

#Windows 64bits install
	#Config and Release common parammeters
    INCLUDEPATH += /mingw64/include
    DEPENDPATH += /mingw64/include
	#Set gdal
    LIBS += -L/mingw64/lib/ -lgdal
#Debug settings
    !build_pass:CONFIG(debug, debug|release) {
    !build_pass:message($$find(CONFIG, "debug"))
    !build_pass:message("Windows de 64 bits")
    #Build Debug
    debug: DESTDIR = build_debug_64bits/bin
    debug: OBJECTS_DIR = build_debug_64bits/temp/obj
    debug: MOC_DIR = build_debug_64bits/temp/moc
    debug: UI_DIR = build_debug_64bits/temp/ui
    debug: RCC_DIR = build_debug_64bits/temp/rcc
}
#Release settings
    !build_pass:CONFIG(release, debug|release) {
    !build_pass:message($$find(CONFIG, "release"))
    !build_pass:message("Windows de 64 bits")
    #Release settings
    release: DESTDIR = build_release_64bits/bin
    release: OBJECTS_DIR = build_release_64bits/temp/obj
    release: MOC_DIR = build_release_64bits/temp/moc
    release: UI_DIR = build_release_64bits/temp/ui
    release: RCC_DIR = build_release_64bits/temp/rcc
}
}
}
#Descobrir versão do QT
QT_VERSION = $$[QT_VERSION]
!build_pass:message($$QT_VERSION)
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0)
QT_VER_MIN = $$member(QT_VERSION, 1)
#se quiser impedir de rodar com QT que não seja QT4 pode usar isso
# e-foto icon
RC_FILE = efotoIcon.rc

