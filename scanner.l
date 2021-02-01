%option yylineno

%{
  #include "tokens.h"

  int get_line_number(void);

  #define print_token(TOKEN) \
    printf("Line: %d | Token identified: %s | Token: " #TOKEN "\n", get_line_number(), yytext);
%}

digit [0-9]
integer {digit}+
exponent [eE][+-]?{integer}
float {integer}("."{integer})?{exponent}?
char \'(\\.|[^\\])\'
string \"([^"\\]|\\.)*\"

letter [_a-zA-Z]
identifier {letter}({letter}|{digit})*
id_end [^a-zA-Z0-9_]

%%

  /* IDENTIFIER */
{identifier}/{id_end} { return TK_IDENTIFICADOR; }

  /* TYPE LITERALS */
{integer} { return TK_LIT_INT; }
{float} { return TK_LIT_FLOAT; }
false { return TK_LIT_FALSE; }
true { return TK_LIT_TRUE; }
{string} { return TK_LIT_STRING; }
{char} { return TK_LIT_CHAR; }

%%

int get_line_number(void) {
  return yylineno;
}
