#ifndef __LEX_VALUE_H__
#define __LEX_VALUE_H__

#include <stdbool.h>

typedef enum eTokenValueType {
  CHAR_VAL,
  STRING_VAL,
  INTEGER_VAL,
  FLOAT_VAL,
  BOOL_VAL,
  NO_VAL = -1,
} TokenValueType;

typedef union sTokenValue {
  int i_val;
  double f_val;
  bool b_val;
  char* s_value;
} TokenValue;

typedef enum eTokenType {
  TOKEN_LITERAL,
  TOKEN_IDENTIFIER,
  TOKEN_COMPOSTO,
  TOKEN_OPERATOR,
  TOKEN_SPECIAL,
} TokenType;

typedef struct sLexValue {
  int line_number;
  TokenType token_type;
  TokenValueType value_type;
  TokenValue token_value;
} LexValue;

LexValue create_lex_value(TokenType token_type, TokenValue value, TokenValueType val_type, int line);

#endif // __LEX_VALUE_H__
