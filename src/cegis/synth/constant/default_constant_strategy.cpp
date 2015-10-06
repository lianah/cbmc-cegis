#include <util/arith_tools.h>
#include <util/bv_arithmetic.h>

#include <cegis/synth/instrument/meta_variables.h>
#include <cegis/synth/constant/add_constant.h>
#include <cegis/synth/constant/literals_constant_strategy.h>
#include <cegis/synth/constant/default_constant_strategy.h>

namespace
{
const char NONDET_PREFIX[]="SYNTH_CONSTANT_NONDET_";
}

size_t Synth::default_constant_strategy(Synth::synth_programt &program,
    const size_t max_length)
{
  const typet type(synth_meta_type());
  const bv_spect spec(type);
  add_synth_constant(program, from_integer(spec.max_value().to_ulong(), type));
  add_synth_constant(program, from_integer(0u, type));
  return 2u + literals_constant_strategy(program, max_length);
  /*for (size_t i=0; i < max_length; ++i)
  {
    const side_effect_expr_nondett value(type);
    std::string name(NONDET_PREFIX);
    add_synth_constant(program, name+=integer2string(i), value);
  }
  return 2u + max_length + literals_constant_strategy(program, max_length);*/
}
