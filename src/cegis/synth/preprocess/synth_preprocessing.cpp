#include <algorithm>

#include <goto-programs/goto_inline.h>

#include <cegis/synth/preprocess/remove_loops_and_assertion.h>
#include <cegis/synth/preprocess/store_nondet_choices.h>
#include <cegis/synth/preprocess/add_invariants_and_temp_variables.h>
#include <cegis/synth/preprocess/synth_preprocessing.h>

#include <iostream>

using namespace Synth;

Synth::synth_preprocessingt::synth_preprocessingt(const symbol_tablet &st,
						  const goto_functionst &gf,
						  constant_strategyt constant_strategy,
						  bool ranking)
  : original_program(st, gf, ranking)
  , constant_strategy(constant_strategy)
{
}

Synth::synth_preprocessingt::~synth_preprocessingt()
{
}

// namespace {
// bool cmp(const synth_programt::loopt &lhs, const synth_programt::loopt &rhs)
// {
//   return lhs.skolem_choices.size() < rhs.skolem_choices.size();
// }
// }

size_t Synth::synth_preprocessingt::get_min_solution_size() const
{
  return 1u;
  // const synth_programt::loopst &l=original_program.loops; // should be number of functions to synthesize
  // size_t sklm=std::max_element(l.begin(), l.end(), &cmp)->skolem_choices.size();
  // return std::max(sklm, size_t(1u));
}

void Synth::synth_preprocessingt::operator ()()
{
  const namespacet ns(original_program.st);
  null_message_handlert null_msg;
  goto_inline(original_program.gf, ns, null_msg);
  // std::cerr << "synth_preprocessingt post-inline " << std::endl;
  // original_program.print();
  synth_remove_loops_and_assertion(original_program);

  // std::cerr << "synth_preprocessingt post-remove_loops_and_assertions " << std::endl;
  // original_program.print();

  // no need for invariant
  // store_skolem_choices(original_program);

  std::cerr << "synth_preprocessingt::operator() " << std::endl;
  original_program.print();

  current_program=original_program;
  // TODO print (dump C, print goto)
  
}

void Synth::synth_preprocessingt::operator ()(const size_t max_length)
{
  current_program=original_program;
  constant_strategy(current_program, max_length);
  store_x0_choices(current_program);
  // adding SSA variables depending on length 
  add_synth_invariants_and_temp_variables(current_program, max_length);
}

const synth_programt &Synth::synth_preprocessingt::get_synth_program() const
{
  return current_program;
}
