#ifndef __GR8_ADDRESSOFNODE_H__
#define __GR8_ADDRESSOFNODE_H__

#include <cdk/ast/lvalue_node.h>
#include <cdk/ast/expression_node.h>

namespace gr8 {

  /**
   * Class for describing address_of nodes.
   */
  class address_of_node: public cdk::expression_node {
    cdk::lvalue_node *_lvalue;

  public:
    inline address_of_node(int lineno, cdk::lvalue_node *argument) :
        cdk::expression_node(lineno), _lvalue(argument) {
    }

  public:
    inline cdk::lvalue_node* lvalue(){
      return _lvalue;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_address_of_node(this, level);
    }

  };

} // gr8

#endif
