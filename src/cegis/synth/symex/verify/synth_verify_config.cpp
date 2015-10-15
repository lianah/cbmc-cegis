#include <cegis/synth/symex/verify/insert_constraint.h>
#include <cegis/synth/symex/verify/insert_candidate.h>
#include <cegis/synth/symex/verify/extract_counterexample.h>
#include <cegis/synth/symex/verify/synth_verify_config.h>

using namespace Synth;

Synth::synth_verify_configt::synth_verify_configt(const Synth::synth_programt &program, bool rank) :
  original_program(program), 
  synth_ranking(rank)
{
}

Synth::synth_verify_configt::~synth_verify_configt()
{
}

// this constructs the goto program to be verified by introducing the meta-variables
// at the proper locations
void Synth::synth_verify_configt::process(const candidatet &candidate)
{
  program=original_program;
  quantifiers.clear();
  synth_insert_constraint(quantifiers, program);
  synth_insert_candidate(program, candidate);
  program.gf.update();
}

const symbol_tablet &Synth::synth_verify_configt::get_symbol_table()
{
  return program.st;
}

const goto_functionst &Synth::synth_verify_configt::get_goto_functions()
{
  return program.gf;
}

void Synth::synth_verify_configt::convert(counterexamplest &counterexamples,
    const goto_tracet &trace)
{
  counterexamples.push_back(counterexamplet());
  synth_extract_counterexample(counterexamples.back(), trace, quantifiers);
}
