#include "geminiassemblerwindow.h"
#include <QApplication>

#include <memory>

#include "geminiassemblermodel.h"
#include "geminiassemblercontrol.h"



int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  std::shared_ptr<GeminiAssemblerWindow> w = std::shared_ptr<GeminiAssemblerWindow> {new GeminiAssemblerWindow};
  std::shared_ptr<GeminiAssemblerModel> gm = std::shared_ptr<GeminiAssemblerModel> { new GeminiAssemblerModel};
  std::shared_ptr<GeminiAssemblerControl> gc = std::shared_ptr<GeminiAssemblerControl> {new GeminiAssemblerControl};

  w->setModel(gm);
  gc->setModel(gm);
//  gc->setView(w);

  w->show();
  
  return a.exec();
}
