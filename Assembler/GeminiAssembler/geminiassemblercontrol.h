#ifndef GEMINIASSEMBLERCONTROL_H
#define GEMINIASSEMBLERCONTROL_H


#include "geminiassemblermodel.h"

#include <memory>
#include "geminiassemblerwindow.h"
class GeminiAssemblerWindow;

class GeminiAssemblerControl {
private:
  std::shared_ptr<GeminiAssemblerModel> model;
  std::shared_ptr<GeminiAssemblerWindow> view;

public:
  GeminiAssemblerControl();

  void setModel (std::shared_ptr<GeminiAssemblerModel> m );
  void setView  (std::shared_ptr<GeminiAssemblerWindow> v );

  bool test_assembly_file ();
  bool test_bytecode_file ();

  bool convert_assembly_to_bytecode ();
};

#endif // GEMINIASSEMBLERCONTROL_H
