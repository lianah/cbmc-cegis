#include <util/arith_tools.h>
#include <cegis/danger/instrument/meta_variables.h>
#include <cegis/danger/util/danger_program_helper.h>
#include <cegis/danger/value/danger_goto_solution.h>
#include <cegis/danger/symex/learn/add_variable_refs.h>
#include <cegis/danger/symex/learn/solution_factory.h>
#include <cegis/danger/symex/verify/insert_constraint.h>
#include <cegis/danger/symex/verify/insert_candidate.h>
#include <cegis/value/program_individual.h>
#include <cegis/genetic/instruction_set_info_factory.h>
#include <cegis/danger/symex/fitness/danger_fitness_config.h>

danger_fitness_configt::danger_fitness_configt(
    instruction_set_info_factoryt &info_fac, const danger_programt &prog) :
    info_fac(info_fac), original_program(prog), constraint_inserted(false), program_contains_ce(
        false), max_solution_size(0u)
{
}

danger_fitness_configt::~danger_fitness_configt()
{
}

void danger_fitness_configt::convert(candidatet &current_candidate,
    const individualt &ind)
{
  danger_variable_idst ids;
  get_danger_variable_ids(original_program.st, ids);
  const instruction_sett &instr_set=info_fac.get_instructions();
  create_danger_solution(current_candidate, original_program, ind, instr_set,
      ids, max_solution_size);
}

void danger_fitness_configt::set_candidate(const candidatet &candidate)
{
  if (!constraint_inserted)
  {
    program_with_constraint=original_program;
    danger_insert_constraint(quantifiers, program_with_constraint);
    constraint_inserted=true;
  }
  program=program_with_constraint;
  program_contains_ce=false;
  danger_insert_candidate(program, candidate);
}

void danger_fitness_configt::set_test_case(const counterexamplet &ce)
{
  if (quantifiers.empty()) return;
  goto_functionst &gf=program.gf;
  for (goto_programt::targett quantifier : quantifiers)
  {
    const irep_idt &var=get_affected_variable(*quantifier);
    const counterexamplet::const_iterator it=ce.find(var);
    if (ce.end() == it) continue;
    symbol_tablet &st=program.st;
    if (program_contains_ce)
    {
      goto_programt::targett assignment=quantifier;
      get_danger_body(gf).instructions.erase(++assignment);
    }
    danger_assign_user_variable(st, gf, quantifier, var, it->second);
  }
  gf.update();
  program_contains_ce=true;
}

const symbol_tablet &danger_fitness_configt::get_symbol_table() const
{
  return program.st;
}

const goto_functionst &danger_fitness_configt::get_goto_functions() const
{
  return program.gf;
}

void danger_fitness_configt::set_max_solution_size(const size_t size)
{
  max_solution_size=size;
}