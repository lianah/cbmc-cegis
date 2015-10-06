#include <algorithm>

#include <goto-programs/goto_trace.h>

#include <cegis/synth/value/synth_goto_solution.h>
#include <cegis/synth/options/synth_program.h>
#include <cegis/util/program_helper.h>
#include <cegis/synth/symex/learn/read_x0.h>

using namespace Synth;

namespace
{
class extract_x0_choice
{
  synth_goto_solutiont &result;
  const goto_tracet &trace;
  goto_tracet::stepst::const_iterator current_step;
public:
  extract_x0_choice(synth_goto_solutiont &result, const goto_tracet &trace) :
      result(result), trace(trace)
  {
    assert(!trace.steps.empty());
    current_step=trace.steps.begin();
  }

  void operator()(const goto_programt::targett &target)
  {
    while (trace.steps.end() != current_step && target != current_step->pc)
      ++current_step;
    assert(trace.steps.end() != current_step);
    result.x0_choices.push_back(current_step->full_lhs_value);
  }
};
}

void Synth::synth_read_x0(synth_goto_solutiont &result, const synth_programt &prog,
    const goto_tracet &trace)
{
  const goto_programt::targetst &x0=prog.x0_choices;
  const extract_x0_choice extract(result, trace);
  std::for_each(x0.begin(), x0.end(), extract);
}
