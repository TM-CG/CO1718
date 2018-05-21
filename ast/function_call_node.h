#ifndef __GR8_FUNCTIONCALLNODE_H__
#define __GR8_FUNCTIONCALLNODE_H__

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>

namespace gr8 {

  /**
   * Class for describing Function call nodes.
   */
  class function_call_node: public cdk::expression_node{
    std::string _id;
    cdk::sequence_node *_arguments;

  public:
    inline function_call_node(int lineno, std::string id, cdk::sequence_node *arguments) :
        cdk::expression_node(lineno), _id(id), _arguments(arguments){
    }

  public:
    inline std::string id(){
      return _id;
    }

    inline cdk::sequence_node* arguments(){
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // gr8

#endif
