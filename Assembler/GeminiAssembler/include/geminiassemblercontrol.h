#ifndef GEMINIASSEMBLERCONTROL_H
#define GEMINIASSEMBLERCONTROL_H


#include "geminiassemblermodel.h"

#include <memory>
#include "geminiassemblerwindow.h"
class GeminiAssemblerWindow;

class GeminiAssemblerControl {
private:
  GeminiAssemblerModel *model;
  GeminiAssemblerWindow *view;

public:
  GeminiAssemblerControl();

  void setModel (GeminiAssemblerModel *m );
  void setView  (GeminiAssemblerWindow *v );

  bool convert_assembly_to_bytecode ();

  bool test_and_read_assembly_file();
  bool test_bytecode_file();
  bool validate_assembly_source();
};

#endif // GEMINIASSEMBLERCONTROL_H
