#include <cegis/invariant/symex/verify/insert_constraint.h>
#include <cegis/invariant/symex/verify/extract_counterexample.h>
#include <cegis/safety/constraint/safety_constraint_factory.h>
#include <cegis/safety/value/safety_goto_ce.h>
#include <cegis/safety/symex/verify/insert_candidate.h>
#include <cegis/safety/symex/verify/safety_verify_config.h>

safety_verify_configt::safety_verify_configt(const safety_programt &prog) :
    original_program(prog)
{
}

safety_verify_configt::~safety_verify_configt()
{
}

// XXX: Debug
#include <iostream>
// XXX: Debug

void safety_verify_configt::process(const candidatet &candidate)
{
  program=original_program;
  quantifiers.clear();
  invariant_insert_constraint(quantifiers, program, create_safety_constraint);
  safety_insert_candidate(program, candidate);
  program.gf.update();
  // XXX: Debug
  std::cout << "<verify_prog>" << std::endl;
  const namespacet ns(program.st);
  program.gf.output(ns, std::cout);
  std::cout << "</verify_prog>" << std::endl;
  // XXX: Debug
}

const symbol_tablet &safety_verify_configt::get_symbol_table() const
{
  return program.st;
}

const goto_functionst &safety_verify_configt::get_goto_functions() const
{
  return program.gf;
}

void safety_verify_configt::convert(counterexamplest &counterexamples,
    const goto_tracet &trace)
{
  counterexamples.push_back(counterexamplet());
  // TODO: Implement for multiple loops (change constraint, instrumentation)
  counterexamplet &new_ce=counterexamples.back();
  counterexamplet::assignments_per_loopt &x=new_ce.x;
  x.push_back(counterexamplet::assignmentst());
  invariant_extract_counterexample(x.back(), trace, quantifiers);
}
