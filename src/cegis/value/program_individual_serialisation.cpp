#include <util/bv_arithmetic.h>

#include <goto-programs/goto_trace.h>

#include <cegis/danger/meta/literals.h>
#include <cegis/danger/symex/learn/read_x0.h>
#include <cegis/value/program_individual_serialisation.h>

bool is_program_indivdual_decl(const goto_trace_stept &step)
{
  if (goto_trace_stept::DECL != step.type) return false;
  const exprt &value=step.full_lhs_value;
  if (ID_array != value.id()) return false;
  const typet &type=value.type().subtype();
  if (ID_struct != type.id()) return false;
  const std::string &tname=id2string(to_struct_type(type).get_tag());
  const char * const danger_tag=&DANGER_INSTRUCTION_TYPE_NAME[4];
  return std::string::npos != tname.find(danger_tag);
}

namespace
{
const program_individualt::instructiont::opt get_const_value(const exprt &expr)
{
  const bv_arithmetict bv(expr);
  return static_cast<program_individualt::instructiont::opt>(bv.to_integer().to_ulong());
}
}

program_individualt::instructiont to_program_individual_instruction(
    const struct_exprt &instr_rep)
{
  program_individualt::instructiont result;
  result.opcode=get_const_value(instr_rep.op0());
  result.ops.push_back(get_const_value(instr_rep.op1()));
  result.ops.push_back(get_const_value(instr_rep.op2()));
  result.ops.push_back(get_const_value(instr_rep.op3()));
  return result;
}

program_individualt to_program_individual(const danger_programt &prog,
    const goto_tracet &trace)
{
  program_individualt individual;
  individual.fitness=0u;
  for (const goto_trace_stept &step : trace.steps)
    if (is_program_indivdual_decl(step))
    {
      program_individualt::programt prog;
      for (const exprt &op : step.full_lhs_value.operands())
      {
        const struct_exprt &instr=to_struct_expr(op);
        prog.push_back(to_program_individual_instruction(instr));
      }
      individual.programs.push_back(prog);
    }
  danger_read_x0(individual, prog, trace);
  return individual;
}

void serialise(irept &sdu, const program_individualt &individual)
{
  // TODO: Implement
  assert(false);
}

void deserialise(program_individualt &individual, const irept &sdu)
{
  // TODO: Implement
  assert(false);
}
