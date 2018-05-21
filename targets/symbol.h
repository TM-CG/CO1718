#ifndef __GR8_SEMANTICS_SYMBOL_H__
#define __GR8_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace gr8 {

  class symbol {
    basic_type *_type;
    std::string _name;
    long _value;
    int _toTrash;
    int _offset = 0;
    bool _def;
  public:
    std::vector<basic_type*> typesVector;

  public:
    symbol(basic_type *type, const std::string &name, long value) :
        _type(type), _name(name), _value(value), _def(false) {
    }

    //  symbol(basic_type *type, const std::string &name, long value)

    virtual ~symbol() {
      delete _type;
    }

    basic_type *type() const {
      return _type;
    }
    const std::string &name() const {
      return _name;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }
    void setOffset(int o){
      _offset = o;
    }
    int getOffset(){
      return _offset;
    }
    inline void setDef(){
      _def = true;
    }
    inline bool getDef(){
      return _def;
    }
    inline int toTrash(){
      return _toTrash;
    }
    inline void setToTrash(int i){
      _toTrash = i;
    }
  };

} // gr8

#endif
