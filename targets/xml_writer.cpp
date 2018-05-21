#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

std::string typeToString(basic_type *type){
  if(type->name()==basic_type::TYPE_INT) return "int";
  if(type->name()==basic_type::TYPE_DOUBLE) return "double";
  if(type->name()==basic_type::TYPE_BOOLEAN) return "boolean";
  if(type->name()==basic_type::TYPE_STRING) return "string";
  if(type->name()==basic_type::TYPE_POINTER) return (typeToString(type->subtype()) + "*");
  return "notype";
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void gr8::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void gr8::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}
//---------------------------------------------------------------------------

/*void gr8::xml_writer::do_program_node(gr8::program_node * const node, int lvl) {
  openTag(node, lvl);
  node->statements()->accept(this, lvl + 4);
  closeTag(node, lvl);
}*/

//---------------------------------------------------------------------------

void gr8::xml_writer::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_post_node(gr8::post_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_read_node(gr8::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_if_node(gr8::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}

void gr8::xml_writer::do_sweep_node(gr8::sweep_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("lvalue", lvl + 2);
  node->iterator()->accept(this, lvl + 4);
  closeTag("lvalue", lvl + 2);
  openTag("init", lvl + 2);
  node->init()->accept(this, lvl + 4);
  closeTag("init", lvl + 2);
  openTag("increment", lvl + 2);
  node->increment()->accept(this, lvl + 4);
  closeTag("increment", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_tweet_node(gr8::tweet_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

void gr8::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

void gr8::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

void gr8::xml_writer::do_null_node(gr8::null_node *const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//TODO NOT SURE
void gr8::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_again_node(gr8::again_node *const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<argument>" << node->argument() << "</argument>" << std::endl;
  closeTag(node, lvl);
}

void gr8::xml_writer::do_stop_node(gr8::stop_node *const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<argument>" << node->argument() << "</argument>" << std::endl;
  closeTag(node, lvl);
}

void gr8::xml_writer::do_block_node(gr8::block_node *const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if(node->decls() != nullptr){
    openTag("decls", lvl + 2);
    node->decls()->accept(this, lvl + 4);
    closeTag("decls", lvl + 2);
  }
  if(node->inst() != nullptr){
    openTag("inst", lvl + 2);
    node->inst()->accept(this, lvl + 4);
    closeTag("inst", lvl + 2);
  }
  closeTag(node, lvl);
}

void gr8::xml_writer::do_address_of_node(gr8::address_of_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("lvalue", lvl + 2);
  node->lvalue()->accept(this, lvl + 4);
  closeTag("lvalue", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_index_node(gr8::index_node *const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("pointer", lvl + 2);
  node->pointer()->accept(this, lvl + 4);
  closeTag("pointer", lvl + 2);
  openTag("index", lvl + 2);
  node->index()->accept(this, lvl + 4);
  closeTag("index", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_identity_node(gr8::identity_node *const node, int lvl) {
  do_unary_expression(node, lvl);
}

void gr8::xml_writer::do_alloc_node(gr8::alloc_node *const node, int lvl) {
  do_unary_expression(node, lvl);
}

void gr8::xml_writer::do_return_node(gr8::return_node *const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if(node->expression() != nullptr){
    openTag("expression", lvl + 2);
    node->expression()->accept(this, lvl + 4);
    closeTag("expression", lvl + 2);
  }
  closeTag(node, lvl);
}

void gr8::xml_writer::do_var_declaration_node(gr8::var_declaration_node *const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" <<"is_public=" 
  << ((node->publicFlag()) ? "true" : "false" )<< " is_use=" << ((node->use()) ? "true" : "false")
  << " type=" << typeToString(node->type()) << " id=" << node->id()<< std::endl;
  if(node->value() != nullptr){
    openTag("value", lvl + 2);
    node->value()->accept(this, lvl + 4);
    closeTag("value", lvl + 2);
  }
  closeTag(node, lvl);
}

void gr8::xml_writer::do_function_declaration_node(gr8::function_declaration_node *const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" <<"is_public=" << ((node->publicFlag()) ? "true" : "false") 
  << "type=" << typeToString(node->type()) << "id" << node->id() <<std::endl;
  if(node->param()!=nullptr){
    openTag("param", lvl + 2);
    node->param()->accept(this, lvl + 4);
    closeTag("param", lvl + 2);
  }
  closeTag(node, lvl);
}

void gr8::xml_writer::do_function_definition_node(gr8::function_definition_node *const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" <<"is_public=" 
  << ((node->publicFlag()) ? "true" : "false") << " type=" << typeToString(node->type()) 
  << " id=" << node->id() << std::endl;
  if(node->param()!= nullptr){
    openTag("param", lvl + 2);
    node->param()->accept(this, lvl + 4);
    closeTag("param", lvl + 2);
  }
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_function_call_node(gr8::function_call_node *const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << "id" 
  << node->id() << "</" << node->label() << ">" << std::endl;
  if(node->arguments()!= nullptr){
    openTag("arguments", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("arguments", lvl + 2);
  }
  closeTag(node, lvl);
}

