%option c++ prefix="gr8_scanner_" outfile="gr8_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{
// make relevant includes before including the parser's tab file
#include <string>
#include <vector>
#include <iostream>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "gr8_parser.tab.h"

int whitespace = 0;
std::vector<int> indentation(1,0);
//return 0->push to vector
//return 1->pop from vector
//return 2->same block
int ident(int i){
	if (i > indentation.back() ){
		indentation.push_back(i);
		return 0;
	}
	else if(i < indentation.back()){
		indentation.pop_back();
		return 1;
	}
	else{
		return 2;
	}
}


// don't change this
#define yyerror LexerError
%}

ESC_HEPT				   [0-6]{1,3}
ID 						   [A-Za-z][0-9a-zA-Z-]*
HEPT					   0[0-6]+
DEC						   [0-9]+
INT_DEC					   0|[1-9][0-9]*
EXP               		   [eE][-+]?{DEC}
REAL   					   "."{DEC}|"."{DEC}{EXP}|{DEC}"."{DEC}|{DEC}"."{DEC}{EXP}|{DEC}{EXP} 					   

%x X_STRING X_COMMENT X_IGNORE_STRING X_INDENT X_COUNT X_EOF X_LINE
%%

"if"                   return tIF; 
"then"				   return tTHEN; 
"else"                 return tELSE; 
"elsif"				   return tELSEIF;
"post"                 return tPOST; 
"input"                return tREAD; 
"define"			   return tDEFINE;
"as"				   return tAS;
"public"			   return tPUBLIC;
"function"			   return tFUNCTION;
"small"				   return tSMALL;
"return"  			   return tRETURN;
"news"				   return tNEWS;
"tweet"				   return tTWEET; 
"stop"				   return tSTOP; 
"again"				   return tAGAIN; 
"plus"				   return '+'; 
"times"				   return '*'; 
"minus"				   return '-'; 
"over"				   return '/'; 
"modulus"			   return '%'; 
"below"				   return '<'; 
"above"				   return '>';
"not"				   return tNOT;
"and"				   return tAND;
"or"				   return tOR;
"equals"               return tEQ; 
"initially" 		   return tINIT;
"assign"			   return tASSIGN;
"to"				   return tTO;
"sweeping"			   return tSWEEP;
"from"			       return tFROM;
"do"				   return tDO;
"by"				   return tBY;
"procedure" 		   return tPROCEDURE;
"use"				   return tUSE;
"for"				   return tFOR;
"on"				   return tON;
"uses"				   return tUSES;
"huge"				   return tHUGE;
"fake"				   return tFAKE;
"objects"			   return tOBJECTS;
"cell"				   return tCELL;
"at"				   return tAT;
"null"				   return tNULL;
"?"					   return tADDRESS;

^[ \t]+				   yyless(0); whitespace = 0; yy_push_state(X_COUNT);

"..."[\n][ \t]*.  	   yyless(yyleng-1);		   

<X_COUNT>^[ \t]*$	   yy_pop_state();
<X_COUNT>[\t]		   whitespace += 8 - (whitespace % 8);
<X_COUNT>[ ]+		   whitespace += yyleng;
<X_COUNT>.		   	   yyless(0); yy_pop_state(); yy_push_state(X_INDENT);
<X_COUNT>[ ]\n 		   whitespace = 0; yy_pop_state();

<X_INDENT>.			   {yyless(0);
						int i = whitespace;
						int result = ident(i);
						if(result == 0){
							yy_pop_state();
							return '{';
						}
						if(result == 2){
							yy_pop_state();
						}
						if(result == 1){
							return '}';
						}
						}
<X_EOF><<EOF>>		{int i = whitespace;
				     int result = ident(i);
					 if(result == 2){
						yy_pop_state();
						return 0;
					 }
					 if(result == 1){
						return '}';
					 }
					}

<X_LINE>.			{int i = whitespace;
				     int result = ident(i);
					 if(result == 2){
						yy_pop_state();
						yyless(0);
					 }
					 if(result == 1){
					 	yyless(0);
						return '}';
					 }
					}

{ID}  				   yylval.s = new std::string(yytext); return tIDENTIFIER; 

\"                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"           yy_pop_state(); return tSTRING;
<X_STRING>"~0"		   yy_push_state(X_IGNORE_STRING); 
<X_STRING>"~"{ESC_HEPT}  { long i = strtol(yytext+1, nullptr, 7);
						   if(i>254){
						   	yyerror("Overflow...");
						   	}
						 	*yylval.s += (char) i;
						   } 
<X_STRING>"..."[\n][ \t]*.  yyless(yyleng-1);		   
<X_STRING>"~~"		   *yylval.s += '~';
<X_STRING>"~t"		   *yylval.s += '\t';
<X_STRING>"~n"		   *yylval.s += '\n';
<X_STRING>"~r"		   *yylval.s += '\r';
<X_STRING>"~\""	       *yylval.s += "\"";
<X_STRING>.            *yylval.s += yytext;
<X_STRING>\n           yyerror("newline in string");

<X_IGNORE_STRING>\"	   yy_pop_state(); yy_pop_state(); return tSTRING;
<X_IGNORE_STRING>~\"    ;
<X_IGNORE_STRING>\n    ;
<X_IGNORE_STRING>.	   ;

"<<"				   yy_push_state(X_COMMENT);
<X_COMMENT>"<<"		   yy_push_state(X_COMMENT);
<X_COMMENT>">>"		   yy_pop_state();
<X_COMMENT>.|\n        ;

"!!".*                 ; /* ignore comments */


{REAL}				    {   yylval.d = strtod(yytext, nullptr);
					    	if(yylval.d == -1)
					   			yyerror("Overflow");
			   		    	return tDOUBLE;
			   		    }

{HEPT}				   	{ 	yylval.i = strtol(yytext, nullptr, 7); 
                         	if(yylval.i == -1){ 
						 		yyerror("Overflow");
						 	}
					   		return tINTEGER;
					   	}

{INT_DEC}				{ 	yylval.i = strtol(yytext, nullptr, 10); 
                         	if(yylval.i == -1){ 
						 		yyerror("Overflow");
						 	}
					   		return tINTEGER;
					   	}
[(),]     			  return *yytext;

[ \t]+                 ; /* ignore whitespace */
[\n].				   if(yytext[1]!= ' ' && yytext[1]!= '\t') { yyless(1);whitespace = 0; yy_push_state(X_LINE);}else{ yyless(1); whitespace = 0; yy_push_state(X_COUNT); }return ';';
.                      yyerror("Unknown character!");

<<EOF>>				   whitespace=0; yy_push_state(X_EOF); return ';';
%%