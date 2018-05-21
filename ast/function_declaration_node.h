#ifndef __GR8_FUNCTIONDECLARATIONNODE_H__
#define __GR8_FUNCTIONDECLARATIONNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/typed_node.h>
#include <cdk/basic_type.h>

namespace gr8 {

  /**
   * Class for describing Variable Declaration nodes.
   */
  class function_declaration_node: public cdk::typed_node {
    bool _public;
    std::string _id;
    cdk::sequence_node *_param;

  public:
    inline function_declaration_node(int lineno, bool publicFlag, basic_type *type, std::string id,
    cdk::sequence_node *param) :
        cdk::typed_node(lineno), _public(publicFlag), _id(id), _param(param){
        this->type(type);
    }

  public:
    inline std::string id(){
      return _id;
    }

    inline cdk::sequence_node* param(){
      return _param;
    }

    inline bool publicFlag(){
      return _public;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // gr8

#endif
