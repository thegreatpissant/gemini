#ifndef GEMINIASSEMBLERMODEL_H
#define GEMINIASSEMBLERMODEL_H

#include <string>
#include <memory>

#include "../../gemini_types.h"

class GeminiAssemblerModel {
private:
  std::string assembly_file_name;
  std::string bytecode_file_name;

  Source_code assembly_code;
  Byte_code byte_code;

  Error_lines error_lines;

public:
  GeminiAssemblerModel();
  void set_assembly_file_name(std::string s);
  void set_bytecode_file_name(std::string s);
  void set_bytecode (Byte_code bc);
  void set_assembly (Source_code ac);
  void set_error_lines (Error_lines el);
  Error_lines get_error_lines ();
  Source_code get_source_code ();
  bool assembly_file_name_is_set ();
  bool bytecode_file_name_is_set ();
};

#endif // GEMINIASSEMBLERMODEL_H
