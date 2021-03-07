#include "lex_value.h"

LexValue create_lex_value(TokenType token_type, TokenValue value, TokenValueType val_type, int line) {

  return (LexValue){
    .line_number = line,
    .token_type = token_type,
    .value_type = val_type,
    .token_value = value,
  };
}

LexValue create_integer(int value, int line) {
  return create_lex_value(TOKEN_LITERAL, (TokenValue) value, INTEGER_VAL, line);
}

LexValue create_float(double value, int line) {
  return create_lex_value(TOKEN_LITERAL, (TokenValue) value, FLOAT_VAL, line);
}

LexValue create_string(char *value, int line) {
  return create_lex_value(TOKEN_LITERAL, (TokenValue) value, STRING_VAL, line);
}

LexValue create_char(char *value, int line) {
  return create_lex_value(TOKEN_LITERAL, (TokenValue) value, CHAR_VAL, line);
}

LexValue create_bool(bool value, int line) {
  return create_lex_value(TOKEN_LITERAL, (TokenValue) value, BOOL_VAL, line);
}

LexValue create_identifier(char *value, int line) {
  return create_lex_value(TOKEN_IDENTIFIER, (TokenValue) value, STRING_VAL, line);
}

LexValue create_compound_op(char *value, int line) {
  return create_lex_value(TOKEN_COMPOUND_OP, (TokenValue) value, STRING_VAL, line);
}