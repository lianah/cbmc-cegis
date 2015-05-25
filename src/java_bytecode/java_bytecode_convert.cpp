/*******************************************************************\

Module: JAVA Bytecode Language Conversion

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

#include <numeric>
#include <set>
#include <stack>

#include <util/string2int.h>
#include <util/std_expr.h>
#include <util/arith_tools.h>
#include <util/ieee_float.h>
#include <util/i2string.h>
#include <util/expr_util.h>

#include <ansi-c/c_types.h>

#include "java_types.h"
#include "java_bytecode_convert.h"
#include "java_bytecode_vtable.h"
#include "bytecode_info.h"

class patternt
{
public:
  explicit inline patternt(const char *_p):p(_p)
  {
  }

  // match with '?'  
  friend bool operator==(const irep_idt &what, const patternt &pattern)
  {
    for(unsigned i=0; i<what.size(); i++)
      if(pattern.p[i]==0)
        return false;
      else if(pattern.p[i]!='?' && pattern.p[i]!=what[i])
        return false;

    return pattern.p[what.size()]==0;
  }

protected:
  const char *p;
};

class java_bytecode_convertt:public messaget
{
public:
  java_bytecode_convertt(
    symbol_tablet &_symbol_table,
    message_handlert &_message_handler):
    messaget(_message_handler),
    symbol_table(_symbol_table)
  {
  }

  void operator()(const java_bytecode_parse_treet &parse_tree)
  {
    convert(parse_tree);
  }

  typedef java_bytecode_parse_treet::classt classt;
  typedef java_bytecode_parse_treet::membert membert;
  typedef java_bytecode_parse_treet::instructiont instructiont;
  typedef membert::instructionst instructionst;

protected:
  symbol_tablet &symbol_table;

  irep_idt current_method;
  unsigned number_of_parameters;

  // JVM local variables
  symbol_exprt &variable(
    std::map<irep_idt, symbol_exprt> &variables,
    const exprt &arg,
    char type_char)
  {
    irep_idt number=to_constant_expr(arg).get_value();
    std::string prefix=(unsafe_string2unsigned(id2string(number))<number_of_parameters)?"arg":"local";
    irep_idt base_name=prefix+id2string(number)+type_char;
    irep_idt identifier=id2string(current_method)+"::"+id2string(base_name);
    const std::map<irep_idt, symbol_exprt>::iterator variable(variables.find(identifier));

    if(variables.end() != variable)
    {
      symbol_exprt &cached(variable->second);
      if(!is_reference_type(type_char))
        cached.type() = java_type(type_char);
      return cached;
    }

    symbol_exprt result(identifier, java_type(type_char));
    result.set(ID_C_base_name, base_name);
    std::pair<std::map<irep_idt, symbol_exprt>::iterator, bool> it(variables.insert(std::make_pair(identifier, result)));
    assert(it.second);

    return it.first->second;
  }

  // temporary variables
  unsigned tmp_counter;

  symbol_exprt tmp_variable(const typet &type)
  {
    irep_idt base_name="tmp"+i2string(tmp_counter++);
    irep_idt identifier=id2string(current_method)+"::"+id2string(base_name);
    symbol_exprt result(identifier, type);
    result.set(ID_C_base_name, base_name);
    return result;
  }

  // JVM program locations
  irep_idt label(const irep_idt &address)
  {
    return "pc"+id2string(address);
  }

  // JVM Stack
  typedef std::vector<exprt> stackt;
  stackt stack;

  exprt::operandst pop(unsigned n)
  {
    if(stack.size()<n)
      throw "malformed bytecode (pop too high)";

    exprt::operandst operands;
    operands.resize(n);
    for(unsigned i=0; i<n; i++)
      operands[i]=stack[stack.size()-n+i];

    stack.resize(stack.size()-n);
    return operands;
  }

  void push(const exprt::operandst &o)
  {
    stack.resize(stack.size()+o.size());

    for(unsigned i=0; i<o.size(); i++)
      stack[stack.size()-o.size()+i]=o[i];
  }

  // conversion
  void convert(const java_bytecode_parse_treet &parse_tree);
  void convert(const classt &c);
  void convert(symbolt &class_symbol, const membert &m);
  void convert(const instructiont &i);
  typet convert(const typet &type);
  codet convert_instructions(const instructionst &);

  static const bytecode_infot &get_bytecode_info(const irep_idt &statement);
};

/*******************************************************************\

Function: java_bytecode_convertt::convert

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void java_bytecode_convertt::convert(
  const java_bytecode_parse_treet &parse_tree)
{
  for(java_bytecode_parse_treet::classest::const_iterator
      it=parse_tree.classes.begin();
      it!=parse_tree.classes.end();
      it++)
    convert(*it);
}

namespace {
const char JAVA_NS[] = "java::";
irept make_base(const irep_idt &base_name) {
  irept base;
  base.id(ID_base);
  const symbol_typet type(JAVA_NS + id2string(base_name));
  base.add(ID_type, type);
  return base;
}

const char ID_interfaces[] = "interfaces";
}

/*******************************************************************\

Function: java_bytecode_convertt::convert

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void java_bytecode_convertt::convert(const classt &c)
{
  class_typet class_type;

  class_type.set_tag(c.name);
  class_type.set(ID_base_name, c.name);

  if(!c.extends.empty())
    class_type.bases().push_back(make_base(c.extends));

  irept &impl=class_type.add(ID_interfaces);
  const std::list<irep_idt> &ifc=c.implements;

  for(std::list<irep_idt>::const_iterator it=ifc.begin();
      it!=ifc.end(); ++it)
  {
    const irept base=make_base(*it);
    class_type.bases().push_back(base); // TODO: Useful?
    impl.get_sub().push_back(base);
  }

  // produce class symbol
  symbolt new_symbol;
  new_symbol.base_name=c.name;
  new_symbol.pretty_name=c.name;
  new_symbol.name=JAVA_NS + id2string(c.name);
  new_symbol.type=class_type;
  new_symbol.mode=ID_java;
  new_symbol.is_type=true;

  class_type.set(ID_name, new_symbol.name);

  for(classt::memberst::const_iterator
      it=c.members.begin();
      it!=c.members.end();
      it++)
    convert(new_symbol, *it);

  if(symbol_table.add(new_symbol))
    throw "failed to add class symbol";
}

namespace {
char java_type(const typet &type)
{
  const irep_idt &id(type.id());
  if (ID_signedbv == id) {
    const unsigned int width(type.get_unsigned_int(ID_width));
    if(java_int_type().get_unsigned_int(ID_width) == width)
      return 'i';
    else if(java_long_type().get_unsigned_int(ID_width) == width)
      return 'l';
    else if(java_short_type().get_unsigned_int(ID_width) == width)
      return 's';
    else if(java_byte_type().get_unsigned_int(ID_width) == width)
      return 'b';
  } else if(ID_unsignedbv == id) {
    return 'c';
  } else if(ID_floatbv == id) {
    const unsigned int width(type.get_unsigned_int(ID_width));
    if(java_float_type().get_unsigned_int(ID_width) == width)
      return 'f';
    else if(java_double_type().get_unsigned_int(ID_width) == width)
      return 'd';
  } else if(ID_bool == id) {
    return 'z';
  }
  return 'a';
}

const size_t SLOTS_PER_INTEGER(1u);
const size_t INTEGER_WIDTH(64u);
size_t count_slots(const size_t value, const code_typet::parametert &param) {
  const unsigned int width(param.type().get_unsigned_int(ID_width));
  return value + SLOTS_PER_INTEGER + width / INTEGER_WIDTH;
}

size_t get_variable_slots(const code_typet::parametert &param) {
  return count_slots(0, param);
}

size_t count_java_parameter_slots(const code_typet::parameterst &p) {
  return std::accumulate(p.begin(), p.end(), 0, &count_slots);
}

bool is_contructor(const irep_idt &class_name, const class_typet::methodt &method) {
  const std::string &name(id2string(class_name));
  std::string needle("java::");
  needle += name;
  needle += ".";
  needle += name;
  needle += ":()V";
  return id2string(method.get_name()).find(needle) != std::string::npos;
}

void cast_if_necessary(binary_relation_exprt &condition) {
  exprt &lhs(condition.lhs());
  exprt &rhs(condition.rhs());
  const typet &lhs_type(lhs.type());
  if(lhs_type == rhs.type()) return;
  rhs = typecast_exprt(rhs, lhs_type);
}
}

/*******************************************************************\

Function: java_bytecode_convertt::convert

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void java_bytecode_convertt::convert(
  symbolt &class_symbol,
  const membert &m)
{
  class_typet &class_type=to_class_type(class_symbol.type);

  typet member_type=java_type_from_string(m.signature);

  if(member_type.id()==ID_code)
  {
    const irep_idt method_identifier=
      id2string(class_symbol.name)+"."+id2string(m.name)+":"+m.signature;

    code_typet &code_type=to_code_type(member_type);
    code_typet::parameterst &parameters=code_type.parameters();

    // do we need to add 'this'?
    if(!m.is_static)
    {
      code_typet::parametert this_p;
      //symbol_typet class_type(class_symbol.name);
      // TODO: Actual type?
      empty_typet class_type;
      this_p.set(ID_C_this, true);
      // TODO: java_reference_type
      this_p.type()=pointer_typet(class_type);
      parameters.insert(parameters.begin(), this_p);
    }

    // assign names to parameters
    for(size_t i=0, param_index=0;
        i < parameters.size(); ++i)
    {
      irep_idt base_name="arg"+i2string(param_index);
      const typet &type(parameters[i].type());
      irep_idt identifier=id2string(method_identifier)+"::"+id2string(base_name)+java_type(type);
      parameters[i].set_base_name(base_name);
      parameters[i].set_identifier(identifier);

      // add to symbol table
      symbolt parameter_symbol;
      parameter_symbol.base_name=base_name;
      parameter_symbol.mode=ID_java;
      parameter_symbol.name=identifier;
      parameter_symbol.type=parameters[i].type();
      parameter_symbol.is_lvalue=true;
      parameter_symbol.is_state_var=true;
      symbol_table.add(parameter_symbol);
      param_index+=get_variable_slots(parameters[i]);
    }

    class_type.methods().push_back(class_typet::methodt());
    class_typet::methodt &method=class_type.methods().back();

    method.set_base_name(m.base_name);
    method.set_name(method_identifier);

    const bool is_virtual=!m.is_static;

    method.set(ID_abstract, m.is_abstract);
    method.set(ID_is_virtual, is_virtual);

    if(is_virtual)
      set_virtual_name(method);

    if(is_contructor(class_symbol.base_name, method))
      method.set(ID_constructor, true);

    method.type()=member_type;

    // create method symbol
    symbolt method_symbol;
    method_symbol.mode=ID_java;
    method_symbol.name=method.get_name();
    method_symbol.base_name=method.get_base_name();
    method_symbol.pretty_name=id2string(class_symbol.pretty_name)+"."+
                              id2string(method.get_base_name())+"()";
    method_symbol.type=member_type;
    current_method=method_symbol.name;
    number_of_parameters=count_java_parameter_slots(parameters);
    tmp_counter=0;
    method_symbol.value=convert_instructions(m.instructions);
    symbol_table.add(method_symbol);

    if(is_virtual)
      create_vtable_type_and_pointer(symbol_table, class_symbol);
  }
  else
  {
    class_type.components().push_back(class_typet::componentt());
    class_typet::componentt &component=class_type.components().back();

    component.set_name(m.name);
    component.set_base_name(m.base_name);
    component.type()=member_type;
  }
}

/*******************************************************************\

Function: java_bytecode_convertt::get_bytecode_info

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

const bytecode_infot &java_bytecode_convertt::get_bytecode_info(
  const irep_idt &statement)
{
  for(const bytecode_infot *p=bytecode_info; p->mnemonic!=0; p++)
    if(statement==p->mnemonic) return *p;

  throw std::string("failed to find bytecode mnemonic `")+
        id2string(statement)+"'";
}

namespace {

irep_idt get_if_cmp_operator(const irep_idt &stmt)
{
  if(stmt == patternt("if_?cmplt")) return ID_lt;
  if(stmt == patternt("if_?cmple")) return ID_le;
  if(stmt == patternt("if_?cmpgt")) return ID_gt;
  if(stmt == patternt("if_?cmpge")) return ID_ge;
  if(stmt == patternt("if_?cmpeq")) return ID_equal;
  if(stmt == patternt("if_?cmpne")) return ID_notequal;

  throw "Unhandled java comparison instruction";
}

constant_exprt as_number(const mp_integer value, const typet &type)
{
  const unsigned int java_int_width(type.get_unsigned_int(ID_width));
  const std::string significant_bits(integer2string(value, 2));
  std::string binary_width(java_int_width - significant_bits.length(), '0');
  return constant_exprt(binary_width += significant_bits, type);
}

#if 0
constant_exprt as_int(const mp_integer value)
{
  return as_number(value, java_int_type());
}

constant_exprt as_width(const size_t width)
{
  return as_int(width);
}

constant_exprt java_integer_width()
{
  return as_width(32u);
}
#endif

const size_t JAVA_NS_LENGTH(6u);

const typet get_java_type(const irept &type)
{
  const irep_idt &type_id=type.get(ID_identifier);

  const irep_idt &id=type_id.empty()?type.id():type_id;

  if(ID_boolean == id)
    return java_boolean_type();
  else if(ID_char == id)
    return java_char_type();
  else if(ID_float == id)
    return java_float_type();
  else if(ID_double == id)
    return java_double_type();
  else if(ID_byte == id)
    return java_byte_type();
  else if(ID_short == id)
    return java_short_type();
  else if(ID_int == id)
    return java_int_type();
  else if(ID_long == id)
    return java_long_type();

  const std::string &type_name=
    id2string(type.find(ID_type).get(ID_identifier));

  const bool has_ns=std::string::npos != type_name.find(JAVA_NS);
  
  const std::string name=
    has_ns ? type_name.substr(JAVA_NS_LENGTH, type_name.size() - JAVA_NS_LENGTH) : type_name;
  
  const char first_char=name[0];
  
  if ('[' == first_char || '(' == first_char)
    return java_type_from_string(id2string(name));

  return pointer_typet(static_cast<const typet &>(type.find(ID_type)));
}

const char METHOD_NAME_SEP('.');

std::string to_field_name(const exprt &arg)
{
  const std::string &id(id2string(arg.get(ID_identifier)));
  const std::string::size_type pos(id.find_last_of(METHOD_NAME_SEP) + 1);
  return id.substr(pos);
}

member_exprt to_member(const exprt &obj, const exprt &field)
{
  const dereference_exprt lhs(obj, obj.type().subtype());
  return member_exprt(lhs, to_field_name(field), field.type());
}
}

/*******************************************************************\

Function: java_bytecode_convertt::convert_instructions

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

codet java_bytecode_convertt::convert_instructions(
  const instructionst &instructions)
{
  // first pass: get targets

  std::set<irep_idt> targets;

  for(instructionst::const_iterator
      i_it=instructions.begin();
      i_it!=instructions.end();
      i_it++)
  {
    if(i_it->statement=="goto" ||
       i_it->statement==patternt("if_?cmp??") ||
       i_it->statement==patternt("if??") ||
       i_it->statement=="ifnonnull" ||
       i_it->statement=="ifnull")
    {
      assert(!i_it->args.empty());
      targets.insert(label(to_constant_expr(i_it->args[0]).get_value()));
    }
    else if(i_it->statement=="tableswitch")
    {
    }
    else if(i_it->statement=="lookupswitch")
    {
    }
  }

  std::map<irep_idt, symbol_exprt> loc_vars;
  code_blockt code;

  for(instructionst::const_iterator
      i_it=instructions.begin();
      i_it!=instructions.end();
      i_it++)
  {
    codet c=code_skipt();

    irep_idt statement=i_it->statement;
    exprt arg0=i_it->args.size()>=1?i_it->args[0]:nil_exprt();
    exprt arg1=i_it->args.size()>=2?i_it->args[1]:nil_exprt();

    const bytecode_infot &bytecode_info=get_bytecode_info(statement);

    // deal with _idx suffixes
    if(statement.size()>=2 &&
       statement[statement.size()-2]=='_' &&
       isdigit(statement[statement.size()-1]))
    {
      arg0=constant_exprt(
        std::string(id2string(statement), statement.size()-1, 1),
        integer_typet());
      statement=std::string(id2string(statement), 0, statement.size()-2);
    }

    exprt::operandst op=pop(bytecode_info.pop);
    exprt::operandst results;
    results.resize(bytecode_info.push, nil_exprt());

    if(statement=="aconst_null")
    {
      assert(results.size()==1);
      results[0]=gen_zero(java_reference_type(void_typet()));
    }
    else if(statement=="athrow")
    {
      assert(op.size()==1 && results.size()==1);
      side_effect_expr_throwt throw_expr;
      throw_expr.copy_to_operands(op[0]);
      c=code_expressiont(throw_expr);
      results[0]=op[0];
    }
    else if(statement=="checkcast")
    {
      assert(op.size()==1 && results.size()==1);
      results[0]=op[0];
    }
    else if(statement=="invokedynamic")
    {
      // not used in Java, will need to investigate what it does
    }
    else if(statement=="invokeinterface" ||
            statement=="invokespecial" ||
            statement=="invokevirtual" ||
            statement=="invokestatic")
    {
      const bool use_this(statement != "invokestatic");
      const bool is_virtual(statement == "invokevirtual" || statement == "invokeinterface");

      code_typet &code_type(to_code_type(arg0.type()));
      code_typet::parameterst &parameters(code_type.parameters());

      if(use_this)
      {
        if(parameters.empty() || !parameters[0].get_bool(ID_C_this))
        {
          // TODO: java_reference_type
          const empty_typet empty;
          const pointer_typet this_pointer(empty);
          code_typet::parametert this_p(this_pointer);
          this_p.set(ID_C_this, true);
          parameters.insert(parameters.begin(), this_p);
        }
      }

      code_function_callt call;
      call.arguments() = pop(parameters.size());
      const typet &return_type(code_type.return_type());
      if (ID_empty != return_type.id()) {
        call.lhs() = tmp_variable(return_type);
        results.resize(1);
        results[0] = call.lhs();
      }

      if(is_virtual)
      {
        const exprt &this_arg(call.arguments().front());
        call.function() = make_vtable_function(arg0, this_arg);
      } else
        call.function() = arg0;
      c = call;
    }
    else if(statement=="return")
    {
      assert(op.empty() && results.empty());
      code_returnt code_return;
      c=code_return;
    }
    else if(statement==patternt("?return"))
    {
      assert(op.size()==1 && results.empty());
      code_returnt code_return(op[0]);
      c=code_return;
    }
    else if(statement==patternt("?astore"))
    {
      assert(op.size() == 3 && results.empty());
      exprt &array_pointer(op[0]);
      if(ID_pointer != array_pointer.type().id() || ID_struct != array_pointer.type().subtype().id())
      {
        const typet generic_array_type(java_array_type(empty_typet()));
        array_pointer = typecast_exprt(array_pointer, generic_array_type);
        array_pointer.type() = generic_array_type;
      }
      const dereference_exprt array(op[0], op[0].type().subtype());
      const char type_char(statement[0]);
      const typet elem_type(java_type(type_char));
      const empty_typet empty;
      const pointer_typet array_type(empty);
      const member_exprt data(array, "data", array_type);
      const dereference_exprt element(plus_exprt(data, op[1]), elem_type);
      c = code_assignt(element, op[2]);
    }
    else if(statement==patternt("?store"))
    {
      // store value into some local variable
      assert(op.size()==1 && results.empty());
      symbol_exprt &var(variable(loc_vars, arg0, statement[0]));
      const bool is_array('a' == statement[0]);
      if (is_array) {
        var.type() = op[0].type();
      }
      c = code_assignt(var, op[0]);
    }
    else if(statement==patternt("?aload"))
    {
      assert(op.size() == 2 && results.size() == 1);
      const dereference_exprt array(op[0], op[0].type().subtype());
      const typet elem_type(java_type(statement[0]));
      const empty_typet empty;
      const pointer_typet array_type(empty);
      const member_exprt data(array, "data", array_type);
      const typecast_exprt pointer(data, pointer_typet(elem_type));
      const typecast_exprt index(op[1], signed_long_int_type());
      const dereference_exprt element(plus_exprt(pointer, index), elem_type);
      const irep_idt &elem_type_id(elem_type.id());
      if((ID_signedbv == elem_type_id || ID_unsignedbv == elem_type_id) &&
          elem_type.get_unsigned_int(ID_width) < 32u)
        results[0]=typecast_exprt(element, java_int_type());
      else
        results[0]=element;
    }
    else if(statement==patternt("?load"))
    {
      // load a value from a local variable
      results[0]=variable(loc_vars, arg0, statement[0]);
    }
    else if(statement=="ldc" || statement=="ldc_w" ||
            statement=="ldc2" || statement=="ldc2_w")
    {
      assert(op.empty() && results.size()==1);
      results[0]=arg0;
    }
    else if(statement=="goto" || statement=="goto_w")
    {
      assert(op.empty() && results.empty());
      irep_idt number=to_constant_expr(arg0).get_value();
      code_gotot code_goto(label(number));
      c=code_goto;
    }
    else if(statement=="iconst_m1")
    {
      assert(results.size()==1);
      results[0]=from_integer(-1, java_int_type());
    }
    else if(statement==patternt("?const"))
    {
      assert(results.size() == 1);
      const char type_char(statement[0]);
      const bool is_double('d' == type_char);
      const bool is_float('f' == type_char);
      if (is_double || is_float)
      {
        const ieee_float_spect spec(
            is_float ?
                ieee_float_spect::single_precision() :
                ieee_float_spect::double_precision());

        ieee_floatt value(spec);
        const typet &arg_type(arg0.type());
        if(ID_integer == arg_type.id())
          value.from_integer(arg0.get_int(ID_value));
        else
          value.from_expr(to_constant_expr(arg0));

        results[0] = value.to_expr();
      }
      else
      {
        const unsigned int value(arg0.get_unsigned_int(ID_value));
        const typet type(java_type(statement[0]));
        results[0] = as_number(value, type);
      }
    }
    else if(statement==patternt("?ipush"))
    {
      assert(results.size()==1);
      results[0]=typecast_exprt(arg0, java_int_type());
    }
    else if(statement==patternt("if_?cmp??"))
    {
      irep_idt number=to_constant_expr(arg0).get_value();
      assert(op.size()==2 && results.empty());
      code_ifthenelset code_branch;
      const irep_idt cmp_op(get_if_cmp_operator(statement));
      binary_relation_exprt condition(op[0], cmp_op, op[1]);
      cast_if_necessary(condition);
      code_branch.cond()=condition;
      code_branch.then_case()=code_gotot(label(number));
      c=code_branch;
    }
    else if(statement==patternt("if??"))
    {
      const irep_idt id=
        statement=="ifeq"?ID_equal:
        statement=="ifne"?ID_notequal:
        statement=="iflt"?ID_lt:
        statement=="ifge"?ID_ge:
        statement=="ifgt"?ID_gt:
        statement=="ifle"?ID_le:
        (assert(false), "");

      irep_idt number=to_constant_expr(arg0).get_value();
      assert(op.size()==1 && results.empty());
      code_ifthenelset code_branch;
      code_branch.cond()=binary_relation_exprt(op[0], id, gen_zero(op[0].type()));
      code_branch.then_case()=code_gotot(label(number));
      c=code_branch;
    }
    else if(statement==patternt("ifnonnull"))
    {
      irep_idt number=to_constant_expr(arg0).get_value();
      assert(op.size()==1 && results.empty());
      code_ifthenelset code_branch;
      const typecast_exprt lhs(op[0], pointer_typet());
      const exprt rhs(gen_zero(lhs.type()));
      code_branch.cond()=binary_relation_exprt(lhs, ID_notequal, rhs);
      code_branch.then_case()=code_gotot(label(number));
      c=code_branch;
    }
    else if(statement==patternt("ifnull"))
    {
      assert(op.size()==1 && results.empty());
      irep_idt number=to_constant_expr(arg0).get_value();
      code_ifthenelset code_branch;
      const typecast_exprt lhs(op[0], pointer_typet());
      const exprt rhs(gen_zero(lhs.type()));
      code_branch.cond()=binary_relation_exprt(lhs, ID_equal, rhs);
      code_branch.then_case()=code_gotot(label(number));
      c=code_branch;
    }
    else if(statement=="iinc")
    {
      code_assignt code_assign;
      code_assign.lhs()=variable(loc_vars, arg0, 'i');
      code_assign.rhs()=plus_exprt(variable(loc_vars, arg0, 'i'), typecast_exprt(arg1, java_int_type()));
      c=code_assign;
    }
    else if(statement==patternt("?xor"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=bitxor_exprt(op[0], op[1]);
    }
    else if(statement==patternt("?or"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=bitor_exprt(op[0], op[1]);
    }
    else if(statement==patternt("?and"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=bitand_exprt(op[0], op[1]);
    }
    else if(statement==patternt("?shl"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=shl_exprt(op[0], op[1]);
    }
    else if(statement==patternt("?shr"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=ashr_exprt(op[0], op[1]);
    }
    else if(statement==patternt("?ushr"))
    {
      assert(op.size()==2 && results.size()==1);
      const typet type(java_type(statement[0]));
      const unsigned int width(type.get_unsigned_int(ID_width));
      typet target(unsigned_long_int_type());
      target.set(ID_width, width);
      const typecast_exprt lhs(op[0], target);
      const typecast_exprt rhs(op[1], target);
      results[0]=lshr_exprt(lhs, rhs);
    }
    else if(statement==patternt("?add"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=plus_exprt(op[0], op[1]);
    }
    else if(statement==patternt("?sub"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=minus_exprt(op[0], op[1]);
    }
    else if(statement==patternt("?div"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=div_exprt(op[0], op[1]);
    }
    else if(statement==patternt("?mul"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=mult_exprt(op[0], op[1]);
    }
    else if(statement==patternt("?neg"))
    {
      assert(op.size()==1 && results.size()==1);
      results[0]=unary_minus_exprt(op[0], op[0].type());
    }
    else if(statement=="frem" || statement=="drem")
    {
      assert(op.size()==2 && results.size()==1);
      std::string function("java::__CPROVER.__java_bytecode_");
      const bool is_float(statement == "frem");
      if(is_float) {
        function += "frem:(FF)F";
      } else {
        function += "drem:(DD)D";
      }
      const typet type(is_float ? java_float_type() : java_double_type());
      code_typet function_type;
      function_type.return_type() = type;
      const code_typet::parametert param(type);
      function_type.parameters().push_back(param);
      function_type.parameters().push_back(param);
      code_function_callt call;
      call.lhs() = tmp_variable(type);
      call.function() = symbol_exprt(function, function_type);
      call.arguments().push_back(op[0]);
      call.arguments().push_back(op[1]);
      results[0]=call.lhs();
      c = call;
    }
    else if(statement==patternt("?rem"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=mod_exprt(op[0], op[1]);
    }
    else if(statement==patternt("?cmp"))
    {
    assert(op.size() == 2 && results.size() == 1);

      // The integer result on the stack is:
      //  0 if op[0] equals op[1]
      // -1 if op[0] is less than op[1]
      //  1 if op[0] is greater than op[1]

      const typet t(java_int_type());
      results[0]=
        if_exprt(binary_relation_exprt(op[0], ID_equal, op[1]), gen_zero(t),
        if_exprt(binary_relation_exprt(op[0], ID_gt, op[1]), from_integer(1, t),
        from_integer(-1, t)));
    }
    else if(statement==patternt("?cmp?"))
    {
      assert(op.size()==2 && results.size()==1);
      const floatbv_typet type(to_floatbv_type(java_type(statement[0])));
      const ieee_float_spect spec(type);
      const ieee_floatt nan(ieee_floatt::NaN(spec));
      const constant_exprt nan_expr(nan.to_expr());
      const int nan_value(statement[4] == 'l' ? -1 : 1);
      const typet result_type(java_int_type());
      const exprt nan_result(from_integer(nan_value, result_type));
      // (value1 == NaN || value2 == NaN) ? nan_value : value1  < value2 ? -1 : value2 < value1  1 ? 1 : 0;
      // (value1 == NaN || value2 == NaN) ? nan_value : value1 == value2 ? 0  : value1 < value2 -1 ? 1 : 0;
      results[0]=
        if_exprt(or_exprt(ieee_float_equal_exprt(nan_expr, op[0]), ieee_float_equal_exprt(nan_expr, op[1])), nan_result,
        if_exprt(ieee_float_equal_exprt(op[0], op[1]), gen_zero(result_type),
        if_exprt(binary_relation_exprt(op[0], ID_lt, op[1]), from_integer(-1, result_type), from_integer(1, result_type))));
    }
    else if(statement==patternt("?cmpl"))
    {
      assert(op.size()==2 && results.size()==1);
      results[0]=binary_relation_exprt(op[0], ID_lt, op[1]);
    }
    else if(statement=="dup")
    {
      assert(op.size()==1 && results.size()==2);
      results[0]=results[1]=op[0];
    }
    else if(statement=="dup_x1")
    {
      assert(op.size()==2 && results.size()==3);
      results[0]=op[1];
      results[1]=op[0];
      results[2]=op[1];
    }
    else if(statement=="dconst")
    {
      assert(op.empty() && results.size()==1);
    }
    else if(statement=="fconst")
    {
      assert(op.empty() && results.size()==1);
    }
    else if(statement=="getfield")
    {
      assert(op.size()==1 && results.size()==1);
      results[0]=to_member(op[0], arg0);
    }
    else if(statement=="getstatic")
    {
      assert(op.empty() && results.size()==1);
      results[0]=arg0;
    }
    else if(statement=="putfield")
    {
      assert(op.size()==2 && results.size()==0);
      c = code_assignt(to_member(op[0], arg0), op[1]);
    }
    else if(statement=="putstatic")
    {
      assert(op.size()==1 && results.empty());
      c=code_assignt(arg0, op[0]);
    }
    else if(statement==patternt("?2?")) // i2c etc.
    {
      assert(op.size()==1 && results.size()==1);
      results[0]=typecast_exprt(op[0], java_type(statement[2]));
    }
    else if(statement=="new")
    {
      // use temporary since the stack symbol might get duplicated
      assert(op.empty() && results.size()==1);
      const pointer_typet ref_type(arg0.type());
      const exprt tmp(tmp_variable(ref_type));
      exprt new_expr(side_effect_exprt(ID_java_new, ref_type));
      new_expr.operands().resize(1);
      c = code_assignt(tmp, new_expr);
      const typecast_exprt rhs(tmp, ref_type); // XXX: Necessary?
      results[0]=rhs;
    }
    else if(statement=="newarray" || statement=="anewarray" || statement=="multianewarray")
    {
      assert(op.size()==1 && results.size()==1);
      exprt init_dim(from_integer(1, java_int_type()));
      if(statement == "multianewarray")
        init_dim = arg1;

      typet element_type(get_java_type(arg0));
      if(ID_bool == element_type.id())
        element_type = java_byte_type();
      const typet ref_type(java_array_type(element_type));
      side_effect_exprt java_new_array(ID_java_new_array, ref_type);
      java_new_array.operands().resize(1);
      java_new_array.copy_to_operands(op[0]);
      const constant_exprt element_type_carrier(element_type);
      java_new_array.copy_to_operands(element_type_carrier);
      java_new_array.copy_to_operands(init_dim);
      const exprt tmp(tmp_variable(ref_type));
      c = code_assignt(tmp, java_new_array);
      results[0]=tmp;
    }
    else if(statement=="anewarray" && false)
    {
      // use temporary since the stack symbol might get duplicated
      assert(op.size()==1 && results.size()==1);

      /*reference_typet ref_type(arg0.type());
      exprt tmp=tmp_variable(ref_type);
      exprt new_expr=side_effect_exprt(ID_java_new_array, ref_type);
      new_expr.operands().resize(2);
      new_expr.op1()=op[0]; // number of elements
      c=code_assignt(tmp, new_expr);
      results[0]=tmp;*/
      assert(!"TODO: Implement anewarray");
    }
    else if(statement=="arraylength")
    {
      assert(op.size() == 1 && results.size() == 1);
      const dereference_exprt array(op[0]);
      const member_exprt length(array, "length", java_int_type());
      results[0]=length;
    }
    else if(statement=="tableswitch")
    {
      assert(op.size()==1 && results.size()==1);
      c=codet(statement);
      c.copy_to_operands(op[0]);
    }
    else if(statement=="lookupswitch")
    {
      assert(op.size()==1 && results.size()==1);
      c=codet(statement);
      c.copy_to_operands(op[0]);
    }
    else
    {
      c=codet(statement);
      c.operands()=op;
    }

    push(results);

    {
      irep_idt l=label(i2string(i_it->address));

      if(targets.find(l)!=targets.end())
        code.add(code_labelt(l, c));
      else if(c.get_statement()!=ID_skip)
        code.add(c);
    }
  }

  return code;
}

/*******************************************************************\

Function: java_bytecode_convertt::convert

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

typet java_bytecode_convertt::convert(const typet &type)
{
  if(type.id()==ID_void)
    return empty_typet();
  else if(type.id()==ID_code)
  {
    code_typet code_type=to_code_type(type); // copy, will change

    code_type.return_type()=convert(code_type.return_type());

    code_typet::parameterst &parameters=code_type.parameters();

    for(code_typet::parameterst::iterator
        it=parameters.begin();
        it!=parameters.end();
        it++)
    {
      it->type()=convert(it->type());
    }

    return code_type;
  }
  else
    return type;
}

/*******************************************************************\

Function: java_bytecode_convert

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

bool java_bytecode_convert(
  const java_bytecode_parse_treet &parse_tree,
  symbol_tablet &symbol_table,
  const std::string &module,
  message_handlert &message_handler)
{
  java_bytecode_convertt java_bytecode_convert(
    symbol_table, message_handler);

  try
  {
    java_bytecode_convert(parse_tree);
    return false;
  }

  catch(int)
  {
  }

  catch(const char *e)
  {
    java_bytecode_convert.error() << e << messaget::eom;
  }

  catch(const std::string &e)
  {
    java_bytecode_convert.error() << e << messaget::eom;
  }

  return true;
}
