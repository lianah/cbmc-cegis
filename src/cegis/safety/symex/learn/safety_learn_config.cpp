#include <cegis/danger/meta/literals.h>

#include <cegis/invariant/symex/learn/instrument_vars.h>
#include <cegis/invariant/symex/learn/invariant_library.h>
#include <cegis/invariant/symex/learn/add_invariant_programs_to_learn.h>
#include <cegis/invariant/symex/learn/add_counterexamples.h>
#include <cegis/safety/options/safety_program_printer.h>
#include <cegis/safety/constraint/safety_constraint_factory.h>
#include <cegis/safety/symex/learn/add_variable_refs.h>
#include <cegis/safety/symex/learn/solution_factory.h>
#include <cegis/safety/symex/learn/safety_learn_config.h>

safety_learn_configt::safety_learn_configt(const safety_programt &program) :
    original_program(program), num_consts(0)
{
}

safety_learn_configt::~safety_learn_configt()
{
}

// XXX: Debug
#include <iostream>
// XXX: Debug

void safety_learn_configt::process(const counterexamplest &ces,
    const size_t max_sz)
{
  program=original_program;
  var_ids.clear();
  const symbol_tablet &st=program.st;
  num_consts=get_invariant_variable_ids(st, var_ids);
  const size_t num_vars=var_ids.size();
  null_message_handlert msg;
  const std::string name(DANGER_EXECUTE);
  add_invariant_library(program, msg, num_vars, num_consts, max_sz, name);
  add_safety_learning_variable_refs(program, var_ids, max_sz);
  goto_functionst &gf=program.gf;
  link_result_var(st, gf, var_ids.size(), max_sz, program.Ix0);
  add_invariant_progs_to_learn(program, max_sz);
  const invariant_programt &prog=program;
  const invariant_programt::const_invariant_loopst loops(prog.get_loops());
  const invariant_programt::invariant_loopt &first_loop=*loops.front();
  const std::string I0=get_prog_var_name(st, first_loop.meta_variables.Ix);
  execute_inv_prog(st, gf, max_sz, program.Ix0, I0);
  invariant_add_learned_counterexamples(program, ces, create_safety_constraint,
      false);
  gf.update();
  // XXX: Debug
  std::cout << "<learn_prog>" << std::endl;
  const namespacet ns(st);
  gf.output(ns, std::cout);
  std::cout << "</learn_prog>" << std::endl;
  // XXX: Debug
}

void safety_learn_configt::set_word_width(const size_t word_width_in_bits)
{
  // TODO: Implement!
}

const symbol_tablet &safety_learn_configt::get_symbol_table() const
{
  return program.st;
}

const goto_functionst &safety_learn_configt::get_goto_functions() const
{
  return program.gf;
}

void safety_learn_configt::convert(candidatet &current_candidate,
    const goto_tracet &trace, const size_t max_sz)
{
  current_candidate.clear();
  create_safety_solution(current_candidate, program, trace, var_ids, max_sz);
}

void safety_learn_configt::show_candidate(messaget::mstreamt &os,
    const candidatet &candidate)
{
  print_safety_program(os, program, candidate);
}
