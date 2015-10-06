#include <util/cprover_prefix.h>

#include <ansi-c/expr2c.h>

#include <cegis/synth/options/synth_program.h>
#include <cegis/synth/instrument/meta_variables.h>
#include <cegis/synth/constant/add_constant.h>

using namespace Synth;

namespace
{
const char BUILTIN[]="<built-in-additions>";

bool is_builtin(const goto_programt::targett &pos)
{
  const std::string &file=id2string(pos->source_location.get_file());
  return file.empty() || BUILTIN == file;
}

const char NS_SEP[]="::";
bool is_meta_or_not_global(const symbolt &symbol)
{
  if (ID_code == symbol.type.id()) return true;
  const std::string &name=id2string(symbol.name);
  if (std::string::npos != name.find("#return_value")) return true;
  if (std::string::npos != name.find(CPROVER_PREFIX)) return true;
  return std::string::npos != name.find(NS_SEP);
}

bool contains_constant(const symbol_tablet &st, const exprt &value)
{
  typedef symbol_tablet::symbolst symbolst;
  exprt lhs=value;
  lhs.add_source_location()=source_locationt();
  const symbolst &s=st.symbols;
  for (symbolst::const_iterator it=s.begin(); it != s.end(); ++it)
  {
    const symbolt &symbol=it->second;
    if (is_meta_or_not_global(symbol)) continue;
    exprt rhs=symbol.value;
    rhs.add_source_location()=lhs.source_location();
    if (lhs == rhs) return true;
  }
  return false;
}

const char CONSTANT_PREFIX[]="SYNTH_CONSTANT_";
}

void Synth::add_synth_constant(synth_programt &program, const exprt &value)
{
  symbol_tablet &st=program.st;
  if (contains_constant(st, value)) return;
  const namespacet ns(st);
  std::string name(CONSTANT_PREFIX);
  name+=expr2c(value, ns);
  add_synth_constant(program, name, value);
}

void Synth::add_synth_constant(synth_programt &prog, const std::string &name,
    const exprt &value)
{
  goto_programt::targett pos=prog.synth_range.begin;
  while (is_builtin (pos))
    ++pos;
  typet type(value.type());
  type.set(ID_C_constant, true);
  symbol_tablet &st=prog.st;
  create_synth_symbol(st, name, type).value=value;
  pos=synth_assign_user_variable(st, prog.gf, pos, name, value);
}
