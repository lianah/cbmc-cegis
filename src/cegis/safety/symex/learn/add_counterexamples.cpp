#include <algorithm>

#include <cegis/safety/options/safety_program.h>
#include <cegis/safety/constraint/safety_constraint_factory.h>
#include <cegis/safety/symex/learn/add_counterexamples.h>

void safety_add_learned_counterexamples(safety_programt &prog,
    const safety_goto_cest &ces, constraint_factoryt constraint)
{
  if (ces.empty()) return;
  // TODO: Implement for multiple loops (change constraint, instrumentation)
  counterexamplest x0s;
  std::transform(ces.begin(), ces.end(), std::back_inserter(x0s),
      [](const safety_goto_cet &ce)
      { return ce.x0;});
  counterexamplest first_loop_only;
  std::transform(ces.begin(), ces.end(), std::back_inserter(first_loop_only),
      [](const safety_goto_cet &ce)
      { assert(!ce.x.empty()); return ce.x.front();});
  const std::string x0_pre(X0_CHOICE_PREFIX);
  const std::string x_pre(X_CHOICE_PREFIX);
  invariant_declare_x_choice_arrays(prog, x0s, x0_pre);
  invariant_declare_x_choice_arrays(prog, first_loop_only, x_pre);
  const size_t sz=ces.size();
  const goto_programt::targett loop_end=invariant_add_ce_loop(prog, sz, false);
  goto_programt::targett pos=prog.invariant_range.begin;
  invariant_assign_ce_values(prog, x0s.front(), x0s.size(), x0_pre, pos, false);
  pos=prog.get_loops().front()->meta_variables.Ix;
  const size_t first_loop_sz=first_loop_only.size();
  invariant_assign_ce_values(prog, first_loop_only.front(), first_loop_sz,
      x_pre, pos, false);
  invariant_add_constraint(prog, constraint, loop_end);
}
