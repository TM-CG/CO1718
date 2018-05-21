#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void gr8::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

void gr8::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if(!getInsideFunction()){
    _pf.SDOUBLE(node->value());
  }
  _pf.DOUBLE(node->value()); // push a double
}

void gr8::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  int lbl1;
  int lbl2;

  _pf.INT(0);
  _pf.EQ();
  _pf.JZ(mklbl(lbl1 = ++_lbl)); 
  _pf.INT(1);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
  _pf.INT(0);
  _pf.LABEL(mklbl(lbl2));  
}

void gr8::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;
    int lbl1, lbl2; 
    node->left()->accept(this, lvl); 

    _pf.INT(0); 
    _pf.NE();
    _pf.JZ(mklbl(lbl1 = ++_lbl)); //salta logo para o fim (nao avalia left) -> caso false 
    node->right()->accept(this, lvl); 
    _pf.INT(0); 
    _pf.NE(); 
    _pf.JZ(mklbl(lbl1 = lbl1)); //salta para o fim -> caso false 
    _pf.INT(1); _pf.JMP(mklbl(lbl2 = ++_lbl)); //salta para o fim -> caso false 
    _pf.ALIGN(); 
    _pf.LABEL(mklbl(lbl1 = lbl1)); 
    _pf.INT(0); 
    _pf.ALIGN(); 
    _pf.LABEL(mklbl(lbl2 = lbl2)); 
}

void gr8::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  int lbl2;

  node->left()->accept(this, lvl);
  _pf.INT(0);
  _pf.EQ();
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->right()->accept(this, lvl);
  _pf.INT(0);
  _pf.EQ();
  _pf.JZ(mklbl(lbl1 = lbl1));
  _pf.INT(0);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1 = lbl1));
  _pf.INT(1);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl2 = lbl2));
}

void gr8::postfix_writer::do_sweep_node(gr8::sweep_node *const node, int lvl) {
  int lblUP, lblDOWN, lblCOND, lblENDSWEEP, lblINCR, lblCONDRET;
  ASSERT_SAFE_EXPRESSIONS;
  node->init()->accept(this, lvl); //coloco o valor inicial na pilha
  node->iterator()->accept(this, lvl); //coloco endereço do lval na pilha
  _pf.STINT(); //atribuição do valor inicial ao lvalue
  //fica a saber se é sweep up ou down

  //teste condiçao
  _pf.ALIGN();
  _pf.LABEL(mklbl(lblCOND = ++_lbl));
  node->increment()->accept(this, lvl); //mete o incremento na pilha
  _pf.INT(0);
  _pf.GE();
  _pf.JZ(mklbl(lblDOWN = ++_lbl)); //sweep down
  _pf.JMP(mklbl(lblUP = ++_lbl)); //sweep up

  //voltar depois de fazer o compartamento espicifco do tipo do sweep
  _pf.ALIGN();
  _pf.LABEL(mklbl(lblCONDRET= ++_lbl));
  _pf.JMP(mklbl(lblINCR = ++_lbl));//salta para incremento
    //sweep up or down

  //incremento
  _pf.ALIGN();
  _pf.LABEL(mklbl(lblINCR));
  node->iterator()->accept(this, lvl);//mete o endereço do iterador na pilha
  _pf.LDINT();//meto na pilha o valor do iterador
  node->increment()->accept(this, lvl);
  _pf.ADD();
  node->iterator()->accept(this, lvl); //coloco endereço do lval na pilha
  _pf.STINT(); //lvalue guarda valor incrementado
  _pf.JMP(mklbl(lblCOND));



  //sweep up
  _pf.ALIGN();
  _pf.LABEL(mklbl(lblUP));
  node->iterator()->accept(this, lvl);//mete o endereço do iterador na pilha
  _pf.LDINT();//meto na pilha o valor do iterador
  node->condition()->accept(this, lvl);
  _pf.GT();
  _pf.JNZ(mklbl(lblENDSWEEP = ++_lbl));//nao verifica a condicao
  againVector.push_back(lblINCR);
  stopVector.push_back(lblENDSWEEP);
  node->block()->accept(this, lvl);
  _pf.JMP(mklbl(lblCONDRET));

  //sweep down
  _pf.ALIGN();
  _pf.LABEL(mklbl(lblDOWN));
  node->iterator()->accept(this, lvl);//mete o endereço do iterador na pilha
  _pf.LDINT();//meto na pilha o valor do iterador
  node->condition()->accept(this, lvl);
  _pf.LT();
  _pf.JNZ(mklbl(lblENDSWEEP));//nao verifica a condicao
  node->block()->accept(this, lvl);
  _pf.JMP(mklbl(lblCONDRET));

  //fim do sweep
  _pf.ALIGN();
  _pf.LABEL(mklbl(lblENDSWEEP));
  stopVector.pop_back();
  againVector.pop_back();

}

void gr8::postfix_writer::do_again_node(gr8::again_node *const node, int lvl) {
  int again = node->argument(), label;
  label = againVector[againVector.size() - again];
  _pf.JMP(mklbl(label));
}

void gr8::postfix_writer::do_address_of_node(gr8::address_of_node *const node, int lvl) {
  // EMPTY
}

void gr8::postfix_writer::do_identity_node(gr8::identity_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
}

void gr8::postfix_writer::do_block_node(gr8::block_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _symtab.push();
  if(node->decls() != nullptr)
    node->decls()->accept(this, lvl);
  node->inst()->accept(this, lvl);
  _symtab.pop();
}

void gr8::postfix_writer::do_var_declaration_node(gr8::var_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(!getInsideFunction()){
    if(node->value() == nullptr){
      _pf.BSS();
      _pf.ALIGN();
      _pf.GLOBAL(node->id(), _pf.OBJ());
      _pf.LABEL(node->id());
      _pf.SALLOC(node->type()->size());
    } else{
      _pf.DATA();
      _pf.ALIGN();
      _pf.GLOBAL(node->id(), _pf.OBJ());
      _pf.LABEL(node->id());
      _actualType = node->type();
      node->value()->accept(this, lvl); //must be a literal
    } 
  }else{
    if(argumentsSequence()){
      _symtab.find_local(node->id())->setOffset(_offset);
      _offset = _offset + node->type()->size();
    }else{
      _offset = _offset + node->type()->size();
      _symtab.find_local(node->id())->setOffset(-_offset);
    }
    if(node->value() != nullptr){
      node->value()->accept(this, lvl);
      _pf.LOCAL(-_offset);
      if(node->type()->name() == basic_type::TYPE_INT)
        _pf.STINT();
      else if(node->type()->name() == basic_type::TYPE_DOUBLE)
        _pf.STDOUBLE();
      else if(node->type()->name() == basic_type::TYPE_STRING){
        _pf.STINT();
      }
      else if(node->type()->name() == basic_type::TYPE_POINTER){
        _pf.STINT();
      }
    }
  }
}

void gr8::postfix_writer::do_tweet_node(gr8::tweet_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_POINTER) {
    _pf.CALL("printi");
    _pf.TRASH(4);
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8);
  }
    else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void gr8::postfix_writer::do_index_node(gr8::index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->pointer()->accept(this, lvl);
  node->index()->accept(this, lvl);
  if(node->pointer()->type()->subtype()->name() == basic_type::TYPE_DOUBLE){
    _pf.INT(8);
  }else
    _pf.INT(4);
  _pf.MUL();
  _pf.ADD();
}

void gr8::postfix_writer::do_stop_node(gr8::stop_node *const node, int lvl) {
  int stop = node->argument(), label;
  label = stopVector[stopVector.size() - stop];
  _pf.JMP(mklbl(label));
}

void gr8::postfix_writer::do_alloc_node(gr8::alloc_node *const node, int lvl) {
  node->argument()->accept(this, lvl);
  if(node->type()->subtype()->name() == basic_type::TYPE_DOUBLE){
    _pf.INT(8);
  }else{
    _pf.INT(4);
  }
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}

void gr8::postfix_writer::do_function_declaration_node(gr8::function_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  setInsideFunction(true);
   _offset = 8;
  _symtab.push();
  argumentsSequenceSet(true);
  if(node->param() != nullptr)
    node->param()->accept(this, lvl);
  argumentsSequenceSet(false);
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(node->id());
  symbol->setToTrash(_offset-8);
  _offset = 0;
  _symtab.pop();
  setInsideFunction(false);

}

void gr8::postfix_writer::do_function_definition_node(gr8::function_definition_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int alloc_size;
  alloc_size = _checker.assert_size(node);
  _actualReturn = node->type();
  if(node->id() == "covfefe"){
    _pf.EXTERN("argc");
    _pf.EXTERN("argv");
    _pf.EXTERN("readi");
    _pf.EXTERN("printi");
    _pf.EXTERN("prints");
    _pf.EXTERN("println");
    _pf.EXTERN("printd");
    _pf.TEXT();
    _pf.ALIGN();
    _pf.GLOBAL("_main", _pf.FUNC());
    _pf.LABEL("_main");
  }
  else{
    _pf.TEXT();
    _pf.ALIGN();
    _pf.GLOBAL(node->id(), _pf.FUNC());
    _pf.LABEL(node->id());
  }
  setInsideFunction(true);
  if(alloc_size == 0){
    _pf.START();
  }else if (alloc_size > 0){
    _pf.ENTER(alloc_size);
  }else{
    throw std::string("something went wrong");
  }
  _offset = 8;
  _symtab.push();
  argumentsSequenceSet(true);
  if(node->param() != nullptr)
    node->param()->accept(this, lvl);
  argumentsSequenceSet(false);
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(node->id());
  symbol->setToTrash(_offset-8);
  _offset = 0;
  node->block()->accept(this, lvl);
  _symtab.pop();
  _pf.LEAVE();
  _pf.RET();
  setInsideFunction(false);
}

void gr8::postfix_writer::do_return_node(gr8::return_node *const node, int lvl) {
  //TODO return double e damos int
  ASSERT_SAFE_EXPRESSIONS;
  if(node->expression() != nullptr){
    node->expression()->accept(this, lvl);
  }
  if(node->expression()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.STFVAL64();
  else if(node->expression()->type()->name() == basic_type::TYPE_INT && _actualReturn->name() == basic_type::TYPE_DOUBLE){
    _pf.I2D();
    _pf.STFVAL64();
  }else
    _pf.STFVAL32();
  _pf.LEAVE();
  _pf.RET();
}

void gr8::postfix_writer::do_function_call_node(gr8::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(node->id());
  _actualId = new std::string(node->id());
  argumentsSequenceCallSet(true);
  if(node->arguments() != nullptr)
    node->arguments()->accept(this, lvl);
  argumentsSequenceCallSet(false);
  _pf.CALL(node->id());
  _pf.TRASH(symbol->toTrash());
  if(node->type() != nullptr){
    if(node->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.LDFVAL64();
    if(node->type()->name() == basic_type::TYPE_INT ||
     node->type()->name() == basic_type::TYPE_POINTER || node->type()->name() == basic_type::TYPE_STRING)
      _pf.LDFVAL32();
  }
}

void gr8::postfix_writer::do_null_node(gr8::null_node *const node, int lvl) {
  _pf.INT(0);
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(argumentsSequenceCall()){
    std::shared_ptr<gr8::symbol> symbol = _symtab.find(*_actualId);
    for (size_t i = node->size(); i > 0; i--) {
      if(symbol->typesVector[i-1] != nullptr){
        if(symbol->typesVector[i-1]->name() == basic_type::TYPE_DOUBLE)
          setI2D(true);
        else
          setI2D(false);
      }
      node->node(i-1)->accept(this, lvl);
    }
  }else{
    for (size_t i = 0; i < node->size(); i++) {
      node->node(i)->accept(this, lvl);
    }
  }
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if(!getInsideFunction()){
    if(_actualType->name() == basic_type::TYPE_DOUBLE){
      _pf.SDOUBLE(node->value());
    }else
      _pf.SINT(node->value());
  }else{
    _pf.INT(node->value());
    if(getI2D()){
      _pf.I2D();
    }
  }
}

void gr8::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
    int lbl1;

    /* generate the string */
    _pf.RODATA(); // strings are DATA readonly
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
    _pf.SSTRING(node->value()); // output string characters
    if(getInsideFunction()){
      _pf.TEXT(); // return to the TEXT segment
      _pf.ADDR(mklbl(lbl1)); // the string to be printed
    }else{
      _pf.DATA();
      _pf.SADDR(mklbl(lbl1));
    }
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
  //TODO NEG DE DOUBLE
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT){
    _pf.I2D();
  }
  if(node->type()->name() == basic_type::TYPE_INT){
    _pf.ADD(); 
  }else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DADD();
  }else if(node->type()->name() ==  basic_type::TYPE_POINTER){
    if(node->type()->subtype()->name() == basic_type::TYPE_POINTER || node->type()->subtype()->name() == basic_type::TYPE_INT ){
      _pf.INT(4);
      _pf.MUL();
    }else if(node->type()->subtype()->name() == basic_type::TYPE_DOUBLE){
      _pf.INT(8);
      _pf.MUL();
    }
    _pf.ADD();
  }
}

void gr8::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_INT){
    _pf.SUB(); 
  }else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DSUB();
  }else if(node->type()->name() ==  basic_type::TYPE_POINTER){
    _pf.SUB();
  }
}

void gr8::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_INT){
    _pf.MUL(); 
  }else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DMUL();
  }
}

void gr8::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_INT){
    _pf.DIV(); 
  }else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DDIV();
  }
}

void gr8::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}

void gr8::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_INT){
    _pf.LT(); 
  }else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DCMP();
  }
}

void gr8::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_INT){
    _pf.LE(); 
  }else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DCMP();
  }
}

void gr8::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_INT){
    _pf.GE(); 
  }else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DCMP();
  }
}

void gr8::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_INT){
    _pf.GT(); 
  }else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DCMP();
  }
}
void gr8::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void gr8::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(node->name());
  _actualId = new std::string(node->name());
  if(symbol->getOffset() == 0){
    _pf.ADDR(node->name());  
  }else{
    _pf.LOCAL(symbol->getOffset());
  }
}

void gr8::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(*_actualId);

  if(node->type()->name() == basic_type::TYPE_INT){
    _pf.LDINT();
  }else if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.LDDOUBLE();
  }else if(node->type()->name() == basic_type::TYPE_STRING){
    _pf.LDINT();
  }else if(node->type()->name() == basic_type::TYPE_POINTER && symbol->getOffset()!=0)
    _pf.LDINT();
}

void gr8::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  if(node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE && node->rvalue()->type()->name() == 
  basic_type::TYPE_INT){
    _pf.I2D();
  }   
  node->lvalue()->accept(this, lvl); // where to store the value 
  if(node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.STDOUBLE();
  } // initialize it to 0 (zero)
  else{
    _pf.STINT(); // store the value at address
  }
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if(node->argument()->type() != nullptr){
    if (node->argument()->type()->name() == basic_type::TYPE_INT) {
     _pf.TRASH(4); // delete the evaluated value
    } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
      _pf.TRASH(4); // delete the evaluated value's address
    } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
      _pf.TRASH(8); // delete the evaluated value's address
    }
      else {
      std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
      exit(1);
    }
  }
}

void gr8::postfix_writer::do_post_node(gr8::post_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_POINTER) {
    _pf.CALL("printi");
    _pf.TRASH(4);
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8);
  }
    else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_read_node(gr8::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.LDFVAL32();
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_if_node(gr8::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}
