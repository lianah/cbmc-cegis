#include <goto-programs/goto_trace.h>

#include <cegis/danger/meta/literals.h>

#include <cegis/value/program_individual_serialisation.h>
#include <cegis/instructions/instruction_set_factory.h>
#include <cegis/invariant/meta/meta_variable_names.h>
#include <cegis/invariant/instrument/meta_variables.h>
#include <cegis/invariant/util/copy_instructions.h>
#include <cegis/invariant/symex/learn/replace_operators.h>
#include <cegis/safety/meta/meta_variable_names.h>
#include <cegis/safety/options/safety_program.h>
#include <cegis/safety/symex/learn/solution_factory.h>

namespace
{
const goto_programt &get_execute_body(const goto_functionst &gf)
{
  typedef goto_functionst::function_mapt function_mapt;
  const function_mapt &function_map=gf.function_map;
  const function_mapt::const_iterator it=function_map.find(DANGER_EXECUTE);
  assert(function_map.end() != it);
  assert(it->second.body_available);
  return it->second.body;
}

void copy_instructions(goto_programt::instructionst &prog,
    const symbol_tablet &st, const invariant_variable_namest &var_names,
    const invariant_variable_namest &result_var_names,
    const program_individualt::instructiont &instr,
    const instruction_sett::value_type::second_type &entry,
    const size_t instr_idx)
{
  copy_instructionst copy_instr;
  for (goto_programt::const_targett it=entry.begin(); it != entry.end(); ++it)
  {
    prog.push_back(goto_programt::instructiont());
    goto_programt::targett new_instr=prog.end();
    copy_instr(--new_instr, it);
  }
  copy_instr.finalize();
  goto_programt::targett first=prog.end();
  std::advance(first, -entry.size());
  const program_individualt::instructiont::opst &ops=instr.ops;
  const size_t empty_op=0u;
  const size_t op0=!ops.empty() ? ops.front() : empty_op;
  const size_t op1=ops.size() >= 2 ? ops.at(1) : empty_op;
  const size_t op2=ops.size() >= 3 ? ops.at(2) : empty_op;
  replace_ops_in_instr(st, "", first, prog.end(), var_names, result_var_names,
      op0, op1, op2, instr_idx);
}

void extract_program(goto_programt::instructionst &prog,
    const symbol_tablet &st, const instruction_sett &instr_set,
    const invariant_variable_namest &var_names,
    const invariant_variable_namest &result_var_names, const size_t max_sz,
    const exprt::operandst &instructions)
{
  size_t instr_idx=0;
  for (const exprt &instruction : instructions)
  {
    const program_individualt::instructiont instr(
        to_program_individual_instruction(to_struct_expr(instruction)));
    const program_individualt::instructiont::opcodet opcode=instr.opcode;
    const instruction_sett::const_iterator instr_entry=instr_set.find(opcode);
    assert(instr_set.end() != instr_entry);
    copy_instructions(prog, st, var_names, result_var_names, instr,
        instr_entry->second, instr_idx++);
  }
}

size_t create_temps(invariant_variable_namest &rnames, const size_t num_tmp)
{
  for (size_t i=0; i < num_tmp; ++i)
    rnames.insert(std::make_pair(i, get_invariant_meta_name(get_tmp(i))));
  return num_tmp;
}
}

void create_safety_solution(safety_goto_solutiont &solution,
    const safety_programt &prog, const goto_tracet &trace,
    const invariant_variable_idst &var_ids, const size_t max_sz)
{
  instruction_sett instr_set;
  extract_instruction_set(instr_set, get_execute_body(prog.gf));
  invariant_variable_namest var_names;
  reverse_invariant_var_ids(var_names, var_ids);
  invariant_variable_namest result_var_names;
  assert(max_sz > 0);
  const size_t idx=create_temps(result_var_names, max_sz - 1);
  size_t loop_idx=0;
  for (const goto_trace_stept &step : trace.steps)
  {
    if (!is_program_indivdual_decl(step)) continue;
    solution.push_back(goto_programt::instructionst());
    const exprt::operandst &instrs=step.full_lhs_value.operands();
    result_var_names.erase(idx);
    const std::string result_name(get_invariant_meta_name(get_Ix(loop_idx++)));
    result_var_names.insert(std::make_pair(idx, result_name));
    extract_program(solution.back(), prog.st, instr_set, var_names,
        result_var_names, max_sz, instrs);
  }
}
