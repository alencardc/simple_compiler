%{
	#include "tokens.h"

	int line_number = 1;
%}

	/* Definicao de algumas expressoes */
DIGIT [0-9]
ALPHA [A-Za-z_]
SIGN [+-]
DELIMITER [ \n[:punct:]]
INT {SIGN}?{DIGIT}+
CHAR \'[^']\'
STRING \"[^("|\n)]*\"
BOOL_LITERAL_TRUE "TRUE"|"true"
BOOL_LITERAL_FALSE "FALSE"|"false"
SPECIAL [\,\;\:\(\)\[\]\{\}\+\-\|\*\/\<\>\=\!\&\%\#\^\.\$]

%x MULTILINE_COMMENT

%%
 /* Guarda o numero de linhas */
\n {line_number++;}

 /* Caracteres que devem ser ignorados */
" " {}
"	" {}
\r {}

 /* Ignora comentarios simples */
\/\/.* {}

 /* Ignora comentarios multi-linha */
\/\* {BEGIN(MULTILINE_COMMENT);}							/* Entra no estado MULTILINE_COMMENT */
<MULTILINE_COMMENT>[^*\n]* {}               	/* Ignora qualquer CHAR que nao for * ou \n */
<MULTILINE_COMMENT>\*[^*/\n]* {}            	/* Ignora "*" nao seguido de "/" */
<MULTILINE_COMMENT>\n {line_number++;}       	/* Conta quebra de linhas em comentarios */
<MULTILINE_COMMENT>\*\/ {BEGIN(INITIAL);}    	/* Fim do comentario, retorna ao estado INITIAL */
 
  /* Identifica os literais da linguagem */
{INT}/{DELIMITER} {return TK_LIT_INT;}

{INT}\.({DIGIT}+|[Ee]{INT})/{DELIMITER} {return TK_LIT_FLOAT;}

{CHAR}/{DELIMITER} {return TK_LIT_CHAR;}

{STRING}/{DELIMITER} {return TK_LIT_STRING;}

{BOOL_LITERAL_TRUE}/{DELIMITER} {return TK_LIT_TRUE;}
{BOOL_LITERAL_FALSE}/{DELIMITER} {return TK_LIT_FALSE;}
 
 /* Identifica as palavras reservadas da linguagem */
"int" {return TK_PR_INT;}
"float" {return TK_PR_FLOAT;}
"bool" {return TK_PR_BOOL;}
"char" {return TK_PR_CHAR;}
"string" {return TK_PR_STRING;}
"if" {return TK_PR_IF;}
"then" {return TK_PR_THEN;}
"else" {return TK_PR_ELSE;}
"while" {return TK_PR_WHILE;}
"do" {return TK_PR_DO;}
"input" {return TK_PR_INPUT;}
"output" {return TK_PR_OUTPUT;}
"return" {return TK_PR_RETURN;}
"const" {return TK_PR_CONST;}
"static" {return TK_PR_STATIC;}
"foreach" {return TK_PR_FOREACH;}
"for" {return TK_PR_FOR;}
"switch" {return TK_PR_SWITCH;}
"case" {return TK_PR_CASE;}
"break" {return TK_PR_BREAK;}
"continue" {return TK_PR_CONTINUE;}
"class" {return TK_PR_CLASS;}
"public" {return TK_PR_PUBLIC;}
"private" {return TK_PR_PRIVATE;}
"protected" {return TK_PR_PROTECTED;}
"end" {return TK_PR_END;}
"default" {return TK_PR_DEFAULT;}
 
 /* Identifica os operadores compostos da linguagem */
"<=" {return TK_OC_LE;}
">=" {return TK_OC_GE;}
"==" {return TK_OC_EQ;}
"!=" {return TK_OC_NE;}
"&&" {return TK_OC_AND;}
"||" {return TK_OC_OR;}
">>" {return TK_OC_SR;}
"<<" {return TK_OC_SL;}
 
 /* Identifica os caracteres especiais da linguagem */
{SPECIAL} {return yytext[0];}

	/* Identifica os identificadores da linguagem */
{ALPHA}({ALPHA}|{DIGIT})* {return TK_IDENTIFICADOR;}

 /* Para qualquer outro padrão, retorna o token de erro */
. {return TOKEN_ERRO;}

%%

 /* Funcao que retorna a contagem de linhas */
int get_line_number(){
	return line_number;
}