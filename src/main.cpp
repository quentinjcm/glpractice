#include <iostream>
#include "QtGui/QGuiApplication"
#include "QSurfaceFormat"
#include "QTGLScene.hpp"

int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);

  // setting up the gl format
  QSurfaceFormat format;
  //samplse
  format.setSamples(4);
  //gl version
  #if defined( __APPLE__)
    format.setMajorVersion(4);
    format.setMinorVersion(1);
  #else
    format.setMajorVersion(4);
    format.setMinorVersion(3);
  #endif
  //profile
  format.setProfile(QSurfaceFormat::CoreProfile);
  //depth buffer size
  format.setDepthBufferSize(24);

  //creating window
  QTGLScene window;
  window.setFormat(format);
  window.resize(1024, 720);
  window.show();
  return app.exec();
}

