#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void gr8::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

void gr8::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}

void gr8::type_checker::do_tweet_node(gr8::tweet_node *const node, int lvl) {
  node->argument()->accept(this, lvl+2);
  if(node->argument()->type()->name() == basic_type::TYPE_POINTER){
    throw std::string("Tweet cannot have pointers as argument");
  }
}

void gr8::type_checker::do_post_node(gr8::post_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() == basic_type::TYPE_POINTER){
    throw std::string("Post cannot have pointers as argument");
  }
}

void gr8::type_checker::do_read_node(gr8::read_node *const node, int lvl) {
//TODO CONFIRM POST-FIX
}
//-------------UNARY EXPRESSIONS--------------------------------------------------------------

void gr8::type_checker::processUnaryExpressionNumbers(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  unsigned long int argument =  node->argument()->type()->name();
  if(argument == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
  else if(argument == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  else
    throw std::string("Argument must be a int or double"); 
  }

void gr8::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpressionNumbers(node, lvl);
}

void gr8::type_checker::do_identity_node(gr8::identity_node *const node, int lvl) {
  processUnaryExpressionNumbers(node, lvl);
}

void gr8::type_checker::do_alloc_node(gr8::alloc_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("Argument must be a int in alloc"); 
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
} 

void gr8::type_checker::do_address_of_node(gr8::address_of_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
}
//-------------bINARY EXPRESSIONS--------------------------------------------------------------

void gr8::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  //TODO VERIFICAR PARA AS OPERAçÔES COM PONTEIROS
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl +2);
  unsigned long int left_type =  node->left()->type()->name();
  unsigned long int right_type =  node->left()->type()->name();  
  if(left_type == basic_type::TYPE_INT){
    if(right_type == basic_type::TYPE_INT)
      node->type(new basic_type(4, basic_type::TYPE_INT));
    else if(right_type == basic_type::TYPE_DOUBLE)
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    else if(right_type == basic_type::TYPE_POINTER)
      node->type(new basic_type(4, basic_type::TYPE_POINTER));
    else
      throw std::string("Wrong type in right argument of binary expression");
  }
  else if(left_type == basic_type::TYPE_DOUBLE){
    if(right_type == basic_type::TYPE_INT)
      node->type(new basic_type(4, basic_type::TYPE_DOUBLE));
    else if(right_type == basic_type::TYPE_DOUBLE)
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    else
      throw std::string("Wrong type in right argument of binary expression");
  }
  else if(left_type == basic_type::TYPE_POINTER){
    if(right_type == basic_type::TYPE_INT)
      node->type(new basic_type(4, basic_type::TYPE_POINTER));
    else if(right_type == basic_type::TYPE_DOUBLE)
      node->type(new basic_type(8, basic_type::TYPE_POINTER));
    else
      throw std::string("Wrong type in right argument of binary expression");
  }
  else
    throw std::string("Wrong type in left argument of binary expression");
}

void gr8::type_checker::processBinaryExpressionInts(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in right argument of binary expression");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::processBinaryExpressionNumbers(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  unsigned long int left_type =  node->left()->type()->name();
  unsigned long int right_type =  node->left()->type()->name();  
  if(left_type == basic_type::TYPE_INT){
    if(right_type == basic_type::TYPE_INT)
      node->type(new basic_type(4, basic_type::TYPE_INT));
    else if(right_type == basic_type::TYPE_DOUBLE)
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    else
      throw std::string("Wrong type in right argument of binary expression");
  }
  else if(left_type == basic_type::TYPE_DOUBLE){
    if(right_type == basic_type::TYPE_INT)
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    else if(right_type == basic_type::TYPE_DOUBLE)
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    else
      throw std::string("Wrong type in right argument of binary expression");
  }
  else
   throw std::string("Wrong type in left argument of binary expression");
  }


void gr8::type_checker::do_and_node(cdk::and_node * const node, int lvl) {
  processBinaryExpressionInts(node, lvl + 2);
}
void gr8::type_checker::do_or_node(cdk::or_node * const node, int lvl) {
  processBinaryExpressionInts(node, lvl + 2);
}
void gr8::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpressionNumbers(node, lvl);
}
void gr8::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpressionNumbers(node, lvl);
}
void gr8::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpressionNumbers(node, lvl);
}
void gr8::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpressionNumbers(node, lvl);
}
void gr8::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
//Not used in gr8
}
void gr8::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
//Not used in gr8
}
void gr8::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpressionNumbers(node, lvl);
}
void gr8::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
//Not used in gr8
}
void gr8::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------
void gr8::type_checker::do_index_node(gr8::index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  
  node->pointer()->accept(this, lvl + 2);
  if(node->pointer()->type()->name() != basic_type::TYPE_POINTER){
    throw std::string("Memory location must be a pointer");
  }

  node->index()->accept(this, lvl + 2);
  if(node->index()->type()->name() != basic_type::TYPE_INT){
    throw std::string("Index must be an int.");
  }
  if(node->pointer()->type()->subtype() != nullptr)
    node->type(node->pointer()->type()->subtype());
}

void gr8::type_checker::do_again_node(gr8::again_node *const node, int lvl) {
  //verify
}
void gr8::type_checker::do_stop_node(gr8::stop_node *const node, int lvl) {
  // verify
}

void gr8::type_checker::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void gr8::type_checker::do_if_node(gr8::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void gr8::type_checker::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void gr8::type_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id + "undeclared";
  }
}

void gr8::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void gr8::type_checker::do_data_node(cdk::data_node * const node, int lvl) {
  // Not used in gr8
}

void gr8::type_checker::do_block_node(gr8::block_node *const node, int lvl) {
  if(node->decls() != nullptr)
    node->decls()->accept(this, lvl + 2);
  node->inst()->accept(this, lvl + 2);
} 

void gr8::type_checker::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}


void gr8::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  unsigned long int lvalue_type = node->lvalue()->type()->name();
  if (lvalue_type != basic_type::TYPE_INT && lvalue_type != basic_type::TYPE_DOUBLE &&
  lvalue_type != basic_type::TYPE_STRING && lvalue_type != basic_type::TYPE_POINTER ) throw std::string(
      "wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  unsigned long int rvalue_type =  node->rvalue()->type()->name();
  if (rvalue_type != lvalue_type){
    if(lvalue_type == basic_type::TYPE_DOUBLE && rvalue_type == basic_type::TYPE_INT){
    }else{
      throw std::string("wrong type in right argument of assignment expression");  
    }
  }
  if(node->lvalue()->type()->name() == basic_type::TYPE_POINTER && node->rvalue()->type()->name() == basic_type::TYPE_POINTER){
    node->rvalue()->type(node->lvalue()->type());
  }
  node->type(node->lvalue()->type());
}

void gr8::type_checker::do_sequence_node(cdk::sequence_node * const node, int lvl) {}

void gr8::type_checker::do_null_node(gr8::null_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
}

void gr8::type_checker::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("Argument must be a int"); 
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_sweep_node(gr8::sweep_node *const node, int lvl) {
  node->iterator()->accept(this, lvl + 2);
  node->init()->accept(this, lvl + 2);
  node->increment()->accept(this, lvl + 2);
  node->condition()->accept(this, lvl + 2);
  if(node->condition()->type()->name() != basic_type::TYPE_INT)
    throw std::string("Condition not valid.");
  node->block()->accept(this, lvl + 2);
}

void gr8::type_checker::do_return_node(gr8::return_node *const node, int lvl) {
  node->expression()->accept(this, lvl + 2);
}

void gr8::type_checker::do_var_declaration_node(gr8::var_declaration_node *const node, int lvl) {
  //TODO ver caso do scope 
  const std::string &id = node->id();
  std::shared_ptr<gr8::symbol> symbol = _symtab.find_local(id);
  if(symbol == nullptr) {
    symbol = std::make_shared<gr8::symbol>(node->type(), id, 0);    
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);
    if(node->value() != nullptr){ 
      node->value()->accept(this, lvl + 2);
      if(node->value()->type()->name() == basic_type::TYPE_POINTER)
        node->value()->type()->_subtype = node->type()->_subtype;
    }
  } else {
    throw std::string("Symbol already in use " + id);
  }
}

void gr8::type_checker::do_function_declaration_node(gr8::function_declaration_node *const node, int lvl) {
  const std::string &id = node->id();
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(id);

  if (symbol == nullptr) {
    if(node->type() != nullptr)
      symbol = std::make_shared<gr8::symbol>(new basic_type(node->type()->size(), node->type()->name()), id, 0);    
    else
      symbol = std::make_shared<gr8::symbol>(nullptr, id, 0);    
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);
  } else {
    throw std::string("Symbol already in use " + id);
  }
  if(node->param() != nullptr){
    for(size_t i = 0; i < node->param()->size(); i++){
      cdk::typed_node *castedNode = dynamic_cast<cdk::typed_node*>(node->param()->node(i));
      if(castedNode != nullptr){
        symbol->typesVector.push_back(castedNode->type());
      }
    }
  }
}

void gr8::type_checker::do_function_definition_node(gr8::function_definition_node *const node, int lvl) {
  const std::string &id = node->id();
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(id);
  if (symbol == nullptr) {
      if(node->type() != nullptr)
        symbol = std::make_shared<gr8::symbol>(new basic_type(node->type()->size(), node->type()->name()), id, 0);    
      else
        symbol = std::make_shared<gr8::symbol>(nullptr, id, 0);    
      symbol->setDef();
      _symtab.insert(id, symbol);
      _parent->set_new_symbol(symbol); 
    }else{
      if(!symbol->getDef())
        symbol->setDef();
      else
        throw std::string("Function name already defined.");
    }
  if(node->param() != nullptr){
    for(size_t i = 0; i < node->param()->size(); i++){
      cdk::typed_node *castedNode = dynamic_cast<cdk::typed_node*>(node->param()->node(i));
      if(castedNode != nullptr){
        symbol->typesVector.push_back(castedNode->type());
      }
    }
  }
  node->block()->accept(this, lvl + 2);
}

void gr8::type_checker::do_function_call_node(gr8::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->id();
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(id);
  if (symbol != nullptr) {
    if(node->arguments() != nullptr)
      node->arguments()->accept(this, lvl + 2);
  } else {
    throw std::string("Symbol must be in symbol table");
  }
  node->type(symbol->type());
}

