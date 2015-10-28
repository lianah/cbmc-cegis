#include <util/mp_arith.h>

#include <cegis/danger/options/danger_program.h>
#include <cegis/danger/instrument/meta_variables.h>
#include <cegis/genetic/genetic_constant_strategy.h>

namespace
{
const char CONSTANT_PREFIX[]="DANGER_CONSTANT_NONDET_";
}

size_t genetic_constant_strategy(danger_programt &program,
    const size_t max_length)
{
  typet type=danger_meta_type(); // XXX: Currently single data type
  type.set(ID_C_constant, true);
  // TODO: Multiply by number of programs and loops?
  symbol_tablet &st=program.st;
  goto_functionst &gf=program.gf;
  goto_programt::targett pos=program.danger_range.begin;
  for (size_t i=0; i < max_length; ++i)
  {
    std::string name(CONSTANT_PREFIX);
    pos=declare_danger_variable(st, gf, pos, name+=integer2string(i), type);
  }
  return max_length;
}
