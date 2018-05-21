%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;	/* integer value */
  double                d;  /* double value */
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node; /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::string_node      *snode;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  gr8::block_node      *block;
  basic_type      *type;
};

%token <i> tINTEGER
%token <type> tSMALL tFAKE tHUGE tNEWS
%token <s> tIDENTIFIER tSTRING
%token <d> tDOUBLE
%token tIF tREAD tBEGIN tEND tSTOP tAGAIN tPOST tRETURN tFUNCTION tSMALL
%token tTHEN tELSEIF tDEFINE tAS tPUBLIC tFUNCTION tSMALL tCONTINUE
%token tTWEET tTIMES tMINUS tOVER tMODULUS tBELOW tABOVE tNOT tAND tOR
%token tINIT tASSIGN tTO tSWEEP tFROM tDO tBY tPROCEDURE tUSE tFOR tON
%token tOBJECTS tCELL tAT tADDRESS tPLUS tUSES tNULL 

%nonassoc tIFX
%nonassoc tELSE

%left tOR tAND
%nonassoc tNOT
%left '>' '<' tEQ
%left '+' '-' tOBJECTS
%left '*' '/' '%'
%right tTWEET tRETURN tPOST 
%nonassoc tUNARY tADDRESS '(' ')' 
%right tASSIGN

%type <s> string
%type <node> stmt file var func elseif decl 
%type <sequence> listi listd vars exprs
%type <expression> expr lvals
%type <lvalue> lval 
%type <type> type typeSMALL typeHUGE typeNEWS
%type <block> block
%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file   : listd { compiler->ast(new cdk::sequence_node(LINE, $1));  }
       |       { compiler->ast(new cdk::sequence_node(LINE, nullptr));  }
       ;

block  : '{' listd '}'           { $$ = new gr8::block_node(LINE, $2); }
       | '{' listi '}'           { $$ = new gr8::block_node(LINE, $2); }
       | '{' listd listi '}'     { $$ = new gr8::block_node(LINE, $2, $3); }
       | '{' '}'                 { $$ = new gr8::block_node(LINE, nullptr, nullptr); }
       ;

listd  : decl                   { $$ = new cdk::sequence_node(LINE, $1); }
       | listd decl             { $$ = new cdk::sequence_node(LINE, $2, $1); }
       ;

listi  : stmt	                  { $$ = new cdk::sequence_node(LINE, $1); }
	     | listi stmt             { $$ = new cdk::sequence_node(LINE, $2, $1); }
	     ;

decl   : var ";"                    { $$ = $1; }
       | func                       { $$ = $1; }
       ;

func   : tPUBLIC type tFUNCTION tIDENTIFIER tUSES vars ";"                       { $$ = new gr8::function_declaration_node(LINE, true, $2, *$4, $6); }
       | tPUBLIC tPROCEDURE tIDENTIFIER tUSES vars ";"                   { $$ = new gr8::function_declaration_node(LINE, true, nullptr, *$3, $5); }
       | type tFUNCTION tIDENTIFIER tUSES vars ";"                       { $$ = new gr8::function_declaration_node(LINE, false, $1, *$3, $5); }
       | tPROCEDURE tIDENTIFIER tUSES vars ";"                            { $$ = new gr8::function_declaration_node(LINE, false, nullptr, *$2, $4); }
       | tPUBLIC type tFUNCTION tIDENTIFIER     ";"                          { $$ = new gr8::function_declaration_node(LINE, true, $2, *$4, nullptr); }
       | tPUBLIC tPROCEDURE tIDENTIFIER                ";"                       { $$ = new gr8::function_declaration_node(LINE, true, nullptr, *$3, nullptr); }
       | type tFUNCTION tIDENTIFIER                       ";"                    { $$ = new gr8::function_declaration_node(LINE, false, $1, *$3, nullptr); }
       | tPROCEDURE tIDENTIFIER                              ";"                 { $$ = new gr8::function_declaration_node(LINE, false, nullptr, *$2, nullptr); }
       | tDEFINE tPUBLIC type tFUNCTION tIDENTIFIER tON vars tAS ";" block         { $$ = new gr8::function_definition_node(LINE, true, $3, *$5, $7, $10); }
       | tDEFINE tPUBLIC tPROCEDURE tIDENTIFIER tON vars tAS ";" block            { $$ = new gr8::function_definition_node(LINE, true, nullptr, *$4, $6, $9); }
       | tDEFINE type tFUNCTION tIDENTIFIER tON vars tAS ";" block                { $$ = new gr8::function_definition_node(LINE, false, $2, *$4, $6, $9); }
       | tDEFINE tPROCEDURE tIDENTIFIER tON vars tAS ";" block                    { $$ = new gr8::function_definition_node(LINE, false, nullptr, *$3, $5, $8); }
       | tDEFINE tPUBLIC type tFUNCTION tIDENTIFIER tAS ";" block %prec tUNARY    { $$ = new gr8::function_definition_node(LINE, true, $3, *$5, nullptr, $8); }
       | tDEFINE tPUBLIC tPROCEDURE tIDENTIFIER tAS ";" block                     { $$ = new gr8::function_definition_node(LINE, true, nullptr, *$4, nullptr, $7); }
       | tDEFINE type tFUNCTION tIDENTIFIER tAS ";" block                         { $$ = new gr8::function_definition_node(LINE, true, $2, *$4, nullptr, $7); }
       | tDEFINE tPROCEDURE tIDENTIFIER tAS ";" block                             { $$ = new gr8::function_definition_node(LINE, true, nullptr, *$3, nullptr, $6); }
       ;

vars   : var                                              { $$ = new cdk::sequence_node(LINE, $1); }
       | vars ',' var                                     { $$ = new cdk::sequence_node(LINE, $3, $1); }
       ;

var    : tPUBLIC type tIDENTIFIER                         { $$ = new gr8::var_declaration_node(LINE, true, $2, true, *$3, nullptr); }
       | tUSE type tIDENTIFIER                            { $$ = new gr8::var_declaration_node(LINE, false, $2, true, *$3, nullptr); }
       | tPUBLIC type tIDENTIFIER '(' tINIT expr ')'       { $$ = new gr8::var_declaration_node(LINE, true, $2, false, *$3, $6); }
       | tUSE type tIDENTIFIER '(' tINIT expr ')'          { $$ = new gr8::var_declaration_node(LINE, true, $2, true, *$3, $6); }
       | type tIDENTIFIER                                 { $$ = new gr8::var_declaration_node(LINE, false, $1, false, *$2, nullptr); }
       | type tIDENTIFIER '(' tINIT expr ')'              { $$ = new gr8::var_declaration_node(LINE, false, $1, false, *$2, $5); }
       ;

type   : typeSMALL                                        { $$ = $1; }
       | tSMALL                                           { $$ = new basic_type(4, basic_type::TYPE_INT); }
       | tHUGE                                            { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
       | typeHUGE                                         { $$ = $1; }
       | tNEWS                                            { $$ = new basic_type(4, basic_type::TYPE_STRING); }
       | typeNEWS                                         { $$ = $1; }
       ;

typeSMALL : tSMALL tFAKE                                  { $$ = new basic_type(4, basic_type::TYPE_POINTER); $2 = new basic_type(4, basic_type::TYPE_INT); $$->_subtype = $2; }
          | tSMALL typeSMALL                              { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype =$2; }
          ;

typeHUGE  : tHUGE tFAKE                                   { $$ = new basic_type(4, basic_type::TYPE_POINTER); $2 = new basic_type(8, basic_type::TYPE_DOUBLE); $$->_subtype = $2; }
          | tHUGE typeHUGE                                { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype =$2; }
          ;

typeNEWS  : tFAKE tNEWS                                   { $$ = new basic_type(4, basic_type::TYPE_POINTER); $2 = new basic_type(4, basic_type::TYPE_STRING); $$->_subtype = $2; }
          | tFAKE typeNEWS                                { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2; }
          ;
                 
stmt : expr ";"                                              { $$ = new gr8::evaluation_node(LINE, $1); }
 	   | tPOST expr   ";"                                      { $$ = new gr8::post_node(LINE, $2); }
     | tTWEET expr  ";"                                      { $$ = new gr8::tweet_node(LINE, $2); }
     | tAGAIN      ";"                                       { $$ = new gr8::again_node(LINE, 1); }
     | tAGAIN tINTEGER   ";"                                 { $$ = new gr8::again_node(LINE, $2); }
     | tSTOP     ";"                                         { $$ = new gr8::stop_node(LINE, 1); }
     | tSTOP tINTEGER    ";"                                 { $$ = new gr8::stop_node(LINE, $2); }
     | tASSIGN expr tTO lval      ";"                        { $$ = new cdk::assignment_node(LINE, $4, $2); }
     | tRETURN      ";"                                      { $$ = new gr8::return_node(LINE, nullptr); }
     | tRETURN expr ";"                                      { $$ = new gr8::return_node(LINE, $2); }
     | tIF expr tTHEN ";" block %prec tIFX                   { $$ = new gr8::if_node(LINE, $2, $5); }
     | tIF expr tTHEN ";" block tELSE ";" block              { $$ = new gr8::if_else_node(LINE, $2, $5, $8); }
     | tIF expr tTHEN ";" block elseif                             { $$ = new gr8::if_else_node(LINE, $2, $5, $6); }   
     | tSWEEP lval tFROM expr tTO expr tBY expr tDO ";" block      { $$ = new gr8::sweep_node(LINE, $2, $4, $8, $6, $11); }
     | tSWEEP lval tFROM expr tTO expr tDO ";" block               { $$ = new gr8::sweep_node(LINE, $2, $4,  new cdk::integer_node(LINE, 1) , $6, $9); }
     | block                                                   {  $$ = $1; }
     ;

elseif : tELSEIF expr tTHEN ";" block                         { $$ = new gr8::if_node(LINE, $2, $5); }
       | tELSEIF expr tTHEN ";" block tELSE ";" block             { $$ = new gr8::if_else_node(LINE, $2, $5, $8);}
       | tELSEIF expr tTHEN ";" block elseif                  { $$ = new gr8::if_else_node(LINE, $2, $5, $6); }
       ;
        
exprs : expr                 { $$ = new cdk::sequence_node(LINE, $1); }
      | exprs ',' expr       { $$ = new cdk::sequence_node(LINE, $3, $1); }
      ;
      

expr : tINTEGER                        { $$ = new cdk::integer_node(LINE, $1); }
	   | string                          { $$ = new cdk::string_node(LINE, $1); }
     | tDOUBLE                         { $$ = new cdk::double_node(LINE, $1); }
     | tNULL                           { $$ = new gr8::null_node(LINE); }
     | expr tOBJECTS                   { $$ = new gr8::alloc_node(LINE, $1); }
     | '-' expr %prec tUNARY           { $$ = new cdk::neg_node(LINE, $2); }
     | '+' expr %prec tUNARY           { $$ = new gr8::identity_node(LINE, $2); }
     | expr '+' expr                   { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr                   { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr                   { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr                   { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr                   { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr                   { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr                   { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tAND expr                  { $$ = new cdk::and_node(LINE, $1, $3); }
     | expr tOR expr                   { $$ = new cdk::or_node(LINE, $1, $3); }
     | tNOT expr                       { $$ = new cdk::not_node(LINE, $2); }
     | expr tEQ expr	                 { $$ = new cdk::eq_node(LINE, $1, $3); }
     | '(' expr ')'                    { $$ = $2; }
     | tUSE exprs tFOR tIDENTIFIER     { $$ = new gr8::function_call_node(LINE, *$4, $2); }
     | tDO tIDENTIFIER                 { $$ = new gr8::function_call_node(LINE, *$2, nullptr); }
     | tREAD                           { $$ = new gr8::read_node(LINE); }
     | lvals                           {  $$ = $1; }
     ;

string : tSTRING string                { $$ = new std::string(*$1); *$$ += *$2; }
       | tSTRING                       { $$= new std::string(*$1); }
       ;

lvals   : lval %prec tIFX                 { $$ = new cdk::rvalue_node(LINE, $1);}
        | lval tADDRESS                   { $$ = new gr8::address_of_node(LINE, $1); }
        ;

lval   : tIDENTIFIER                        { $$ = new cdk::identifier_node(LINE, $1); }
       | tCELL expr tAT expr %prec tUNARY   { $$ = new gr8::index_node(LINE, $4, $2); }
       ;

%%
