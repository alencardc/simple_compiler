#include "lex_value.h"

LexValue create_lex_value(TokenType token_type, TokenValue value, TokenValueType val_type, int line) {

  return (LexValue){
    .line_number = line,
    .token_type = token_type,
    .value_type = val_type,
    .token_value = value,
  };
}