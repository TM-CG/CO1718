#ifndef __GR8_SWEEPNODE_H__
#define __GR8_SWEEPNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include <cdk/ast/basic_node.h>

namespace gr8 {

  /**
   * Class for describing sweep-cycle nodes.
   */
  class sweep_node: public cdk::basic_node {
    cdk::lvalue_node *_iterator;
    cdk::expression_node *_init;
    cdk::expression_node *_increment;
    cdk::expression_node *_condition;
    block_node *_block;

  public:
    inline sweep_node(int lineno, cdk::lvalue_node *iterator, cdk::expression_node *init, cdk::expression_node *increment, 
      cdk::expression_node *condition, block_node *block) : basic_node(lineno), _iterator(iterator), _init(init),
      _increment(increment), _condition(condition), _block(block) {
    }

  public:
    inline cdk::lvalue_node *iterator() {
      return _iterator;
    }
    inline cdk::expression_node *init() {
      return _init;
    }
    inline cdk::expression_node *increment() {
      return _increment;
    }
    inline cdk::expression_node *condition() {
      return _condition;
    }
    inline block_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_sweep_node(this, level);
    }

  };

} // gr8

#endif
