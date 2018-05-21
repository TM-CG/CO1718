#ifndef __GR8_FUNCTIONDEFINITIONNODE_H__
#define __GR8_FUNCTIONDEFINITIONNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/typed_node.h>

namespace gr8 {

  /**
   * Class for describing Function definition nodes.
   */
  class function_definition_node: public cdk::typed_node {
    bool _public;
    std::string _id;
    cdk::sequence_node *_param;
    block_node *_block;

  public:
    inline function_definition_node(int lineno, bool publicFlag, basic_type *type, std::string id,
      cdk::sequence_node *param, block_node *block) :
        cdk::typed_node(lineno), _public(publicFlag), _id(id), _param(param), _block(block){
        this->type(type);
    }

  public:
    inline std::string id(){
      return _id;
    }

    inline cdk::sequence_node* param(){
      return _param;
    }

    inline block_node* block(){
      return _block;
    }

    inline bool publicFlag(){
      return _public;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // gr8

#endif
