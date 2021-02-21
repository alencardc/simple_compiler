%option yylineno noyywrap

%{
  #include "parser.tab.h"

  int get_line_number(void);

  #define print_token(TOKEN) \
    printf("Line: %d | Token identified: %s | Token: " #TOKEN "\n", get_line_number(), yytext);

  
%}

digit [0-9]
integer {digit}+
exponent [eE][+-]?(([1-9]{digit}*)+|[0]+[1-9]{digit}*)
float {integer}(("."{integer})|{exponent}|("."{integer}){exponent}) 
char \'(\\.|[^\\])\'
string \"([^"\\\n]|\\.)*\"
chars_after_literals [^_a-zA-Z0-9.]

letter [_a-zA-Z]
identifier {letter}({letter}|{digit})*
id_end [^a-zA-Z0-9_]
special_chars [\,\;\:\(\)\[\]\{\}\+\-\|\*\/\<\>\=\!\&\%\#\^\.\$\?]

/*Comentários*/
white_space [ \t\r\v\f]
multiline_comment \/\*[^*]*\*+(?:[^/*][^/*][^*]*\*+)*\/
double_slash \/\/
singleline_comment {double_slash}.*

start_multiline \/\*
all_except_asterisk [^*]
asterisk_without_slash \*+[^/*]
end_multiline (\*+\/)
simpler_multiline_comment {start_multiline}({all_except_asterisk}|{asterisk_without_slash})*{end_multiline}

%x ERROR_STATE

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

 /* TYPE LITERALS */
{integer}/{chars_after_literals} { return TK_LIT_INT; }
{float}/{chars_after_literals} { return TK_LIT_FLOAT; }
false { return TK_LIT_FALSE; }
true { return TK_LIT_TRUE; }
{string}/{chars_after_literals} { return TK_LIT_STRING; }
{char}/{chars_after_literals} { return TK_LIT_CHAR; }

  /* SPECIAL CHARACTERS - e.g. ,;:() */
{special_chars} {return yytext[0]; }

  /* SPECIAL/COMPOSITE OPERATORS  */
"<=" {return TK_OC_LE; }
">=" {return TK_OC_GE; }
"==" {return TK_OC_EQ; }
"!=" {return TK_OC_NE; }
"&&" {return TK_OC_AND; }
"||" {return TK_OC_OR; }
">>" {return TK_OC_SL; }
"<<" {return TK_OC_SR; }

  /* IDENTIFIER */
{identifier} { return TK_IDENTIFICADOR; }

  /* COMMENTS AND CHARACTERS TO IGNORE */
{simpler_multiline_comment} {}
{singleline_comment} {}
{white_space}+ {}
\n {}

  /* ERROR IF NO PATTERN MATCHED */
. {
    yyless(0); 
    BEGIN(ERROR_STATE);
  }

 /*When in error state, grab all chars until the end of the line or ;*/
<ERROR_STATE>[^  \n;]* {
  BEGIN(INITIAL);
  return TOKEN_ERRO;
}
%%

int get_line_number(void) {
  return yylineno;
}
