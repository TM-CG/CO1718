#ifndef __GR8_SEMANTICS_POSTFIX_WRITER_H__
#define __GR8_SEMANTICS_POSTFIX_WRITER_H__

#include <string>
#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/emitters/basic_postfix_emitter.h>
#include "targets/allocation_checker.h"
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"

namespace gr8 {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<gr8::symbol> &_symtab;
    cdk::basic_postfix_emitter &_pf;
    int _lbl;
    gr8::allocation_checker _checker;
    bool _insideFunction;
    std::vector<std::string> labels;
    int _offset = 0;
    int toAllocateInt = 0;
    std::vector<int> againVector;
    std::vector<int> stopVector;   
    bool _argumentsSec;
    bool _argumentsCallSec;
    int _toTrash;
    std::string *_actualId;
    bool I2D;
    basic_type* _actualReturn = nullptr;
    basic_type* _actualType = nullptr;


  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<gr8::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0), _checker(compiler), _insideFunction(false), 
        _argumentsSec(false), _argumentsCallSec(false), I2D(false), _actualReturn(nullptr), _actualType(nullptr){ 
    }

  public:
    ~postfix_writer() {
      os().flush();
    }

  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }

    inline int getInt(){
      return toAllocateInt;
    }

    inline void setInt(int i){
      toAllocateInt = i;
    }

    inline void setInsideFunction(bool arg){
      _insideFunction = arg;
    }

    inline bool getInsideFunction(){
      return _insideFunction;
    }

    inline bool argumentsSequence(){
      return _argumentsSec;
    }

    inline void argumentsSequenceSet(bool arg){
      _argumentsSec = arg;
    }

    inline void argumentsSequenceCallSet(bool arg){
      _argumentsCallSec = arg;
    }

    inline bool argumentsSequenceCall(){
      return _argumentsCallSec;
    }

    inline void setI2D(bool t){
      I2D = t;
    }

    inline bool getI2D(){
      return I2D;
    }

  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

  };

} // gr8

#endif
