// $Id: allocation_checker.h,v 1.1 2018/05/13 16:48:32 ist424870 Exp $ -*- c++ -*-
#ifndef __GR8_SEMANTICS_ALLOCATION_CHECKER_H__
#define __GR8_SEMANTICS_ALLOCATION_CHECKER_H__

#include <string>
#include <iostream>
#include <cdk/symbol_table.h>
#include <cdk/ast/basic_node.h>
#include "targets/symbol.h"
#include "targets/basic_ast_visitor.h"

namespace gr8 {

  /**
   * Print nodes as XML elements to the output stream.
   */
  class allocation_checker: public basic_ast_visitor {
    int _counter;

  public:
    allocation_checker(std::shared_ptr<cdk::compiler> compiler) :
      basic_ast_visitor(compiler), _counter(0){
    }

  public:
    ~allocation_checker() {
      os().flush();
    }

  public:
    int assert_size(cdk::basic_node *node){
      _counter = 0;
      node->accept(this, 0);
      return _counter;
    }

    inline int counter(){
      return _counter;
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
