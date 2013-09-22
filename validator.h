#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <memory>
#include "gemini_types.h"

Error_lines validate_source (Source_code source_file, std::shared_ptr<Byte_code> byte_code);

#endif // VALIDATOR_H
