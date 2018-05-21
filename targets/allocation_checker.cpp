#include "targets/allocation_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void gr8::allocation_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_string_node(cdk::string_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_double_node(cdk::double_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_sweep_node(gr8::sweep_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}

void gr8::allocation_checker::do_tweet_node(gr8::tweet_node *const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_post_node(gr8::post_node *const node, int lvl) {
 //EMPTY
}

void gr8::allocation_checker::do_read_node(gr8::read_node *const node, int lvl) {
 //EMPTY
}

void gr8::allocation_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
 //EMPTY
}

void gr8::allocation_checker::do_identity_node(gr8::identity_node *const node, int lvl) {
 //EMPTY
}

void gr8::allocation_checker::do_alloc_node(gr8::alloc_node *const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_address_of_node(gr8::address_of_node *const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_and_node(cdk::and_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_or_node(cdk::or_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_add_node(cdk::add_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  //EMPTY
}
void gr8::allocation_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  //EMPTY
}
void gr8::allocation_checker::do_div_node(cdk::div_node * const node, int lvl) {
  //EMPTY
}
void gr8::allocation_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  //EMPTY
}
void gr8::allocation_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  //EMPTY
}
void gr8::allocation_checker::do_le_node(cdk::le_node * const node, int lvl) {
//Not used in gr8
}
void gr8::allocation_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
//Not used in gr8
}
void gr8::allocation_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  //EMPTY
}
void gr8::allocation_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
//Not used in gr8
}
void gr8::allocation_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_index_node(gr8::index_node *const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_again_node(gr8::again_node *const node, int lvl) {
  //verify
}
void gr8::allocation_checker::do_stop_node(gr8::stop_node *const node, int lvl) {
  // verify
}

void gr8::allocation_checker::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_if_node(gr8::if_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}

void gr8::allocation_checker::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  node->thenblock()->accept(this, lvl);
}

void gr8::allocation_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_data_node(cdk::data_node * const node, int lvl) {
  // Not used in gr8
}

void gr8::allocation_checker::do_block_node(gr8::block_node *const node, int lvl) {
  if(node->decls() != nullptr)
  node->decls()->accept(this, lvl);
} 

void gr8::allocation_checker::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}


void gr8::allocation_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  size_t vectorLen = node->size();
  for(size_t i = 0; i < vectorLen; i++){
    node->node(i)->accept(this, lvl + 2);
  }
}

void gr8::allocation_checker::do_null_node(gr8::null_node *const node, int lvl) {
 //EMPTY
}

void gr8::allocation_checker::do_not_node(cdk::not_node * const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_return_node(gr8::return_node *const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_var_declaration_node(gr8::var_declaration_node *const node, int lvl) {
  _counter = _counter + node->type()->size();
}

void gr8::allocation_checker::do_function_declaration_node(gr8::function_declaration_node *const node, int lvl) {
  //EMPTY
}

void gr8::allocation_checker::do_function_definition_node(gr8::function_definition_node *const node, int lvl) {
  node->block()->accept(this, lvl);
}

void gr8::allocation_checker::do_function_call_node(gr8::function_call_node *const node, int lvl) {
  //EMPTY
}

