#include <util/namespace_utils.h>

#include <ansi-c/c_types.h>

#include <goto-programs/goto_functions.h>

#include <cegis/synth/meta/literals.h>
#include <cegis/util/program_helper.h>
#include <cegis/synth/instrument/meta_variables.h>

using namespace Synth;

namespace
{
const char NS_SEP[]="::";
}
std::string Synth::get_synth_meta_name(const std::string &base_name)
{
  std::string name=id2string(goto_functionst::entry_point());
  name+=NS_SEP;
  return name+=base_name;
}

goto_programt::targett Synth::declare_synth_variable(symbol_tablet &st,
    goto_functionst &gf, const goto_programt::targett &pos,
    const std::string &base_name, const typet &type)
{
  const std::string symbol_name(get_synth_meta_name(base_name));
  create_synth_symbol(st, symbol_name, type);
  const symbol_exprt symbol(symbol_name, type);
  const goto_programt::targett decl=get_synth_body(gf).insert_after(pos);
  decl->type=goto_program_instruction_typet::DECL;
  decl->code=code_declt(symbol);
  decl->source_location=default_synth_source_location();
  return decl;
}

symbolt &Synth::create_synth_symbol(symbol_tablet &st, const std::string &full_name,
    const typet &type)
{
  symbolt new_symbol;
  new_symbol.name=full_name;
  new_symbol.type=type;
  new_symbol.base_name=full_name;
  new_symbol.pretty_name=new_symbol.base_name;
  new_symbol.location=default_synth_source_location();
  new_symbol.mode=ID_C;
  new_symbol.module=SYNTH_MODULE;
  new_symbol.is_thread_local=true;
  new_symbol.is_static_lifetime=false;
  new_symbol.is_file_local=true;
  new_symbol.is_lvalue=true;
  assert(!st.add(new_symbol));
  return st.lookup(new_symbol.name);
}

goto_programt::targett Synth::synth_assign(const symbol_tablet &st,
    goto_functionst &gf, const goto_programt::targett &insert_after_pos,
    const exprt &lhs, const exprt &rhs)
{
  goto_programt &body=get_synth_body(gf);
  goto_programt::targett assign=body.insert_after(insert_after_pos);
  assign->type=goto_program_instruction_typet::ASSIGN;
  assign->source_location=default_synth_source_location();
  const namespacet ns(st);
  const typet &type=lhs.type();
  if (type_eq(type, rhs.type(), ns)) assign->code=code_assignt(lhs, rhs);
  else assign->code=code_assignt(lhs, typecast_exprt(rhs, type));
  return assign;
}

goto_programt::targett Synth::synth_assign_user_variable(const symbol_tablet &st,
    goto_functionst &gf, const goto_programt::targett &insert_after_pos,
    const irep_idt &name, const exprt &value)
{
  const symbol_exprt lhs(st.lookup(name).symbol_expr());
  return synth_assign(st, gf, insert_after_pos, lhs, value);
}

goto_programt::targett Synth::assign_synth_variable(const symbol_tablet &st,
    goto_functionst &gf, const goto_programt::targett &insert_after_pos,
    const std::string &base_name, const exprt &value)
{
  const std::string name(get_synth_meta_name(base_name));
  return synth_assign_user_variable(st, gf, insert_after_pos, name, value);
}

typet Synth::synth_meta_type()
{
  return unsigned_int_type();
}

source_locationt Synth::default_synth_source_location()
{
  source_locationt loc;
  loc.set_file(SYNTH_MODULE);
  loc.set_function(goto_functionst::entry_point());
  return loc;
}
