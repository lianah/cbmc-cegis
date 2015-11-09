#include <algorithm>

#include <cegis/danger/symex/verify/insert_constraint.h>
#include <cegis/danger/symex/verify/insert_candidate.h>
#include <cegis/danger/symex/verify/extract_counterexample.h>
#include <cegis/danger/symex/verify/danger_verify_config.h>

danger_verify_configt::danger_verify_configt(const danger_programt &program) :
    original_program(program)
{
}

danger_verify_configt::~danger_verify_configt()
{
}

void danger_verify_configt::process(const candidatet &candidate)
{
  program=original_program;
  quantifiers.clear();
  danger_insert_constraint(quantifiers, program);
  danger_insert_candidate(program, candidate);
  program.gf.update();
}

const symbol_tablet &danger_verify_configt::get_symbol_table() const
{
  return program.st;
}

const goto_functionst &danger_verify_configt::get_goto_functions() const
{
  return program.gf;
}

goto_functionst &danger_verify_configt::get_goto_functions()
{
  return program.gf;
}

void danger_verify_configt::convert(counterexamplest &counterexamples,
    const goto_tracet &trace)
{
  counterexamples.push_back(counterexamplet());
  danger_extract_counterexample(counterexamples.back(), trace, quantifiers);
}

size_t danger_verify_configt::get_number_of_loops() const
{
  return original_program.loops.size();
}

exprt::operandst danger_verify_configt::get_loop_guards() const
{
  exprt::operandst loop_guards;
  const danger_programt::loopst &loops=original_program.loops;
  std::transform(loops.begin(), loops.end(), std::back_inserter(loop_guards),
      [](const danger_programt::loopt &loop)
      { return loop.guard;});
  return loop_guards;
}
