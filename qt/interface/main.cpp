#include <iostream>
#include <QStyleFactory>
#include <QStyle>
#include <QSurfaceFormat>
#include <QApplication>
#include <QTranslator>
#include <QGuiApplication>
#include <QPalette>
#include <QStyleHints>
#include "LoadingScreen.h"
#include "EFotoManager.h"

// Function to check if the system is in dark mode
inline bool isDarkMode() {
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
  const auto scheme = QGuiApplication::styleHints()->colorScheme();
  return scheme == Qt::ColorScheme::Dark;
#else
  const QPalette defaultPalette;
  const auto text = defaultPalette.color(QPalette::WindowText);
  const auto window = defaultPalette.color(QPalette::Window);
  return text.lightness() > window.lightness();
#endif // QT_VERSION
}

int main(int argc, char **argv)
{
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication efoto(argc, argv);

  efoto.setStyle(QStyleFactory::create("Fusion"));

  QPalette palette = efoto.palette();

  if (isDarkMode()) {
    // System is in dark mode; set a light theme for the application
    palette.setColor(QPalette::Window, Qt::white);         // Window background
    palette.setColor(QPalette::WindowText, Qt::black);     // Window text
    palette.setColor(QPalette::Base, Qt::white);           // Background of text input widgets
    palette.setColor(QPalette::Text, Qt::black);           // Text in input widgets
    palette.setColor(QPalette::Button, Qt::lightGray);     // Button background
    palette.setColor(QPalette::ButtonText, Qt::black);     // Button text
  } else {
    // System is in light mode; use the default or custom light theme
    palette.setColor(QPalette::Window, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, Qt::white);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Button, Qt::lightGray);
    palette.setColor(QPalette::ButtonText, Qt::black);
  }

  efoto.setPalette(palette);
  efoto.setStyleSheet("QToolTip { color: #000000; background-color: #ffffdc; border: none; }");

  efoto.quitOnLastWindowClosed();
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  QSurfaceFormat::setDefaultFormat(format);

  qApp->processEvents();
  br::uerj::eng::efoto::EFotoManager manager;
  manager.setInterfaceType("Qt");
  if (argc == 2)
  {
    ++argv;
    manager.exec(*argv);
  }
  else
  {
    manager.exec();
  }

  return efoto.exec();
}
