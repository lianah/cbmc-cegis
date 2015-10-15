#include <algorithm>

#include <goto-programs/goto_trace.h>

#include <cegis/danger/value/danger_goto_solution.h>
#include <cegis/danger/options/danger_program.h>
#include <cegis/danger/util/danger_program_helper.h>
#include <cegis/danger/meta/literals.h>
#include <cegis/danger/instrument/meta_variables.h>
#include <cegis/danger/symex/learn/read_x0.h>

namespace
{
bool is_placeholder_of(const goto_programt::targett &x0,
    const goto_programt::const_targett &placeholder)
{
  const goto_programt::instructiont &placeholder_instr=*placeholder;
  if (goto_program_instruction_typet::DECL != placeholder_instr.type)
    return false;
  std::string placeholder_base(DANGER_X0_PLACEHOLDER_PREFIX);
  placeholder_base+=id2string(get_affected_variable(*x0));
  const std::string placeholder_name(get_danger_meta_name(placeholder_base));
  return placeholder_name == id2string(get_affected_variable(placeholder_instr));
}

class extract_x0_choice
{
  danger_goto_solutiont &result;
  const goto_tracet &trace;
  goto_tracet::stepst::const_iterator current_step;
public:
  extract_x0_choice(danger_goto_solutiont &result, const goto_tracet &trace) :
      result(result), trace(trace)
  {
    assert(!trace.steps.empty());
    current_step=trace.steps.begin();
  }

  void operator()(const goto_programt::targett &x0)
  {
    const goto_tracet::stepst::const_iterator &end=trace.steps.end();
    while (end != current_step && !is_placeholder_of(x0, current_step->pc))
      ++current_step;
    assert(end != current_step);
    result.x0_choices.push_back(current_step->full_lhs_value);
  }
};
}

void danger_read_x0(danger_goto_solutiont &result, const danger_programt &prog,
    const goto_tracet &trace)
{
  const goto_programt::targetst &x0=prog.x0_choices;
  const extract_x0_choice extract(result, trace);
  std::for_each(x0.begin(), x0.end(), extract);
}
