#include "geminiassemblerwindow.h"
#include <QApplication>

#include <memory>

#include "geminiassemblermodel.h"
#include "geminiassemblercontrol.h"



int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  GeminiAssemblerWindow w;
  GeminiAssemblerModel  gm;
  GeminiAssemblerControl gc;

  w.setModel( &gm );
  w.setControl ( &gc );
  gc.setModel( &gm );
  gc.setView( &w );

  w.show();
  return a.exec();
}
