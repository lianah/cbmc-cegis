#include <algorithm>

#include <cegis/util/program_helper.h>
#include <cegis/synth/constraint/synth_constraint_factory.h>
#include <cegis/synth/meta/meta_variable_names.h>
#include <cegis/synth/instrument/meta_variables.h>
#include <cegis/synth/symex/verify/restrict_counterexamples.h>

using namespace Synth;

namespace
{
bool is_assert(const goto_programt::instructiont &instr)
{
  return goto_program_instruction_typet::ASSERT == instr.type;
}

goto_programt::targett find_assertion(goto_programt &body)
{
  goto_programt::instructionst &i=body.instructions;
  const goto_programt::targett it=std::find_if(i.begin(), i.end(), &is_assert);
  assert(i.end() != it);
  return it;
}
}

void Synth::force_assertion_satisfaction(goto_functionst &gf, const size_t num_loops)
{
  goto_programt &body=get_synth_body(gf);
  goto_programt::targett pos=find_assertion(body);
  pos=body.insert_before(pos);
  pos->type=goto_program_instruction_typet::ASSUME;
  pos->source_location=default_synth_source_location();
  exprt::operandst op;
  for (size_t i=0; i < num_loops; ++i)
  {
    const notequal_exprt Ix=synth_component_as_bool(get_Ix(i));
    const notequal_exprt Gx=synth_component_as_bool(get_Gx(i));
    const equal_exprt not_Gx(Gx.lhs(), Gx.rhs());
    op.push_back(Ix);
    op.push_back(not_Gx);
  }
  pos->guard=conjunction(op);
}

void Synth::force_invariant_and_guard_satisfaction(goto_functionst &gf,
    const size_t num_loops)
{

}
