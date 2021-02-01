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
special_chars [\,\;\:\(\)\[\]\{\}\+\-\|\*\/\<\>\=\!\&\%\#\^\.\$]

%%
  /* RESERVED WORDS */
int { return TK_PR_INT; }
float { return TK_PR_FLOAT; }
bool { return TK_PR_BOOL; }
char { return TK_PR_CHAR; }
string { return TK_PR_STRING; }
if { return TK_PR_IF; }
then { return TK_PR_THEN; }
else { return TK_PR_ELSE; }
while { return TK_PR_WHILE; }
do { return TK_PR_DO; }
input { return TK_PR_INPUT; }
output { return TK_PR_OUTPUT; }
return { return TK_PR_RETURN; }
const { return TK_PR_CONST; }
static { return TK_PR_STATIC; }
foreach { return TK_PR_FOREACH; }
for { return TK_PR_FOR; }
switch { return TK_PR_SWITCH; }
case { return TK_PR_CASE; }
break { return TK_PR_BREAK; }
continue { return TK_PR_CONTINUE; }
class { return TK_PR_CLASS; }
private { return TK_PR_PRIVATE; }
public { return TK_PR_PUBLIC; }
protected { return TK_PR_PROTECTED; }
end { return TK_PR_END; }
default { return TK_PR_DEFAULT; }

  /* SPECIAL CHARACTERS - e.g. ,;:() */
{special_chars} {return yytext[0]; }

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
