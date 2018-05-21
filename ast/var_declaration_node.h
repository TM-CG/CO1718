#ifndef __GR8_VARDECLARATIONNODE_H__
#define __GR8_VARDECLARATIONNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>
#include <cdk/ast/typed_node.h>


namespace gr8 {

  /**
   * Class for describing Variable Declaration nodes.
   */
  class var_declaration_node: public cdk::typed_node {
    bool _public;
    bool _use;
    std::string _id;
    cdk::expression_node *_value;

  public:
    inline var_declaration_node(int lineno, bool publicFlag, basic_type *type, bool use, std::string id, cdk::expression_node *value) :
        cdk::typed_node(lineno), _public(publicFlag), _use(use), _id(id), _value(value){
        this->type(type);
    }

  public:
    inline std::string id(){
      return _id;
    }

    inline bool publicFlag(){
      return _public;
    }

    inline bool use(){
      return _use;
    }

    inline cdk::expression_node* value(){
      return _value;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_declaration_node(this, level);
    }

  };

} // gr8

#endif
