#ifndef GEMINIASSEMBLERMODEL_H
#define GEMINIASSEMBLERMODEL_H

#include <string>
#include <memory>

#include "gemini_types.h"

class GeminiAssemblerModel {
private:
  std::string assembly_file_name;
  std::string bytecode_file_name;

  std::shared_ptr<Source_code> assembly_code;
  std::shared_ptr<Byte_code> byte_code;

  std::shared_ptr<Error_lines> error_lines;

public:
  GeminiAssemblerModel();
  void set_assembly_file_name(std::string s);
  std::string get_assembly_file_name ();
  std::string get_bytecode_file_name ();
  void set_bytecode_file_name(std::string s);
  void set_bytecode (std::shared_ptr<Byte_code> bc);
  void set_assembly (std::shared_ptr<Source_code> ac);
  void set_error_lines (Error_lines el);
  std::shared_ptr<Error_lines> get_error_lines ();
  std::shared_ptr<Source_code> get_source_code ();
  bool assembly_file_name_is_set ();
  bool bytecode_file_name_is_set ();
};

#endif // GEMINIASSEMBLERMODEL_H
