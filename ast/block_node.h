#ifndef __GR8_BLOCKNODE_H__
#define __GR8_BLOCKNODE_H__

#include <cdk/ast/sequence_node.h>

namespace gr8{

  class block_node: public cdk::basic_node {
    cdk::sequence_node *_decls, *_inst;

  public:
    //Block without variable declarations
    inline block_node(int lineno, cdk::sequence_node *argument) :
        cdk::basic_node(lineno), _inst(argument) {
    }

    //Block with variable declarations
    inline block_node(int lineno, cdk::sequence_node *argument1,
      cdk::sequence_node *argument2) :
        cdk::basic_node(lineno), _decls(argument1), _inst(argument2) {
    }

  public:
    inline cdk::sequence_node *decls() {
      return _decls;
    }

    inline cdk::sequence_node *inst(){
      return _inst;
    }

    void accept(basic_ast_visitor *sp, int level){
      sp->do_block_node(this, level);
    }

  };

} //gr8

#endif
