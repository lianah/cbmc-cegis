#include <util/mp_arith.h>

#include <cegis/danger/constant/add_constant.h>
#include <cegis/danger/instrument/meta_variables.h>

#include <cegis/genetic/genetic_constant_strategy.h>

namespace
{
const char CONSTANT_PREFIX[]="DANGER_CONSTANT_NONDET_";
}

size_t genetic_constant_strategy(danger_programt &program,
    const size_t max_length)
{
  const typet type=danger_meta_type(); // XXX: Currently single data type
  // TODO: Multiply by number of programs and loops?
  for (size_t i=0; i < max_length; ++i)
  {
    std::string name(CONSTANT_PREFIX);
    add_danger_constant(program, name+=integer2string(i), type);
  }
  return max_length;
}
