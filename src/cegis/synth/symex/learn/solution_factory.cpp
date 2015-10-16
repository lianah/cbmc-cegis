#include <algorithm>

#include <util/bv_arithmetic.h>

#include <goto-programs/goto_trace.h>
#include <goto-programs/goto_functions.h>

#include <cegis/synth/meta/literals.h>
#include <cegis/synth/meta/meta_variable_names.h>
#include <cegis/synth/instrument/meta_variables.h>
#include <cegis/synth/value/synth_goto_solution.h>
#include <cegis/synth/options/synth_program.h>
#include <cegis/util/copy_instructions.h>
#include <cegis/util/program_helper.h>
#include <cegis/synth/symex/learn/instruction_set_factory.h>
#include <cegis/synth/symex/learn/replace_operators.h>
#include <cegis/synth/symex/learn/solution_factory.h>
#include <cegis/synth/symex/learn/read_x0.h>

using namespace Synth;

// LSH FIXME: does the order of the values matter?

namespace
{
const size_t get_const_value(const exprt &expr)
{
  const bv_arithmetict bv(expr);
  return bv.to_integer().to_ulong();
}

typedef std::map<size_t, const irep_idt> synth_variable_namest;
void reverse(synth_variable_namest &names, const synth_variable_idst &o)
{
  for (synth_variable_idst::const_iterator it=o.begin(); it != o.end(); ++it)
    names.insert(std::make_pair(it->second, it->first));
}

size_t create_temps(synth_variable_namest &rnames, const size_t num_tmp)
{
  for (size_t i=0; i < num_tmp; ++i)
    rnames.insert(std::make_pair(i, get_synth_meta_name(get_tmp(i))));
  return num_tmp;
}

typedef enum
{
  INV, RNK 
} prog_typet;

goto_programt::instructionst &get_prog(
    synth_goto_solutiont::synth_programst &progs, const prog_typet type,
    const size_t instr_idx)
{
  //  assert (progs.size());
  switch (type)
  {
  case INV:
    if (!instr_idx) progs.push_back(synth_goto_solutiont::synth_programt());
    return progs.back().invariant;
  case RNK:
    return progs.back().ranking;
  default:
    assert(false);
    return progs.back().invariant;
  }
}

class read_instrt
{
  synth_goto_solutiont::synth_programst &progs;
  const synth_programt &synth_prog;
  const synth_variable_namest &names;
  synth_variable_namest rnames;
  const instruction_sett &instrset;
  const size_t max_size;
  size_t loop_index;
  size_t insidx;
  prog_typet prog_type;

  // prepare rnames for next type of meta-variable
  void switch_prog()
  {
    insidx=0;
    rnames.clear();

    // LSH FIXME: hideous
    if (!synth_prog.synth_ranking) {
      const size_t idx=create_temps(rnames, max_size - 1);
      const std::string result_name(get_synth_meta_name(get_Ix(loop_index)));
      rnames.insert(std::make_pair(idx, result_name));
      prog_type=INV;
      return;
    }
    
    switch (prog_type)
    {
    case INV:
    {
      const size_t idx=create_temps(rnames, max_size - 1);
      const std::string result(get_synth_meta_name(get_Rx(loop_index, 0))); // XXX: Lexicographical ranking?
      rnames.insert(std::make_pair(idx, result));
      prog_type=RNK;
      break;
    }
    case RNK:
    {
      const size_t idx=create_temps(rnames, max_size - 1);
      const std::string result_name(get_synth_meta_name(get_Ix(loop_index)));
      rnames.insert(std::make_pair(idx, result_name));
      prog_type=INV;
      break;

      // const synth_programt::loopt &loop=synth_prog.loops[loop_index];
      // const size_t num_skolem=loop.skolem_choices.size();
      // const size_t num_temps=create_temps(rnames, max_size - num_skolem);
      // for (size_t i=num_temps; i < max_size; ++i)
      // {
      //   const size_t sk=i - num_temps;
      //   const std::string name(get_synth_meta_name(get_Sx(loop_index, sk)));
      //   rnames.insert(std::make_pair(i, name));
      // }
      // prog_type=SKO;
      // ++loop_index;
      // break;
    }
    // case SKO:
    // {
    //   const size_t idx=create_temps(rnames, max_size - 1);
    //   const std::string result_name(get_synth_meta_name(get_Dx(loop_index)));
    //   rnames.insert(std::make_pair(idx, result_name));
    //   prog_type=INV;
    //   break;
    // }
    }
  }
public:
  read_instrt(synth_goto_solutiont::synth_programst &progs,
      const synth_programt &synth_prog, const synth_variable_namest &names,
      const instruction_sett &instrset, const size_t max_size) :
      progs(progs), synth_prog(synth_prog), names(names), instrset(instrset), max_size(
          max_size), loop_index(0u), insidx(0u), prog_type(RNK)
      /*LSH FIXME: initial prog_type in trace should not be SKO */
  {
    switch_prog();
  }

  void operator()(const exprt &prog_arary_member)
  {
    const struct_exprt &instr_rep=to_struct_expr(prog_arary_member);
    const size_t opcode=get_const_value(instr_rep.op0());
    
    const instruction_sett::const_iterator instr_entry=instrset.find(opcode);
    assert(instrset.end() != instr_entry);

    goto_programt::instructionst &prog=get_prog(progs, prog_type, insidx);
    const goto_programt::instructionst &instr=instr_entry->second;
    goto_programt::targett first=prog.end();
    const goto_programt::targett last=prog.end();
    copy_instructionst copy_instr;
    for (goto_programt::const_targett it=instr.begin(); it != instr.end(); ++it)
    {
      prog.push_back(goto_programt::instructiont());
      goto_programt::targett new_instr=prog.end();
      copy_instr(--new_instr, it);
    }
    copy_instr.finalize();
    std::advance(first, -instr.size());
    const size_t op0=get_const_value(instr_rep.op1());
    const size_t op1=get_const_value(instr_rep.op2());
    const size_t op2=get_const_value(instr_rep.op3());
    const symbol_tablet &st=synth_prog.st;
    replace_ops_in_instr(st, first, last, names, rnames, op0, op1, op2, insidx);
    if (++insidx % max_size == 0)
    {
      synth_make_presentable(prog);
      switch_prog();
    }
  }
};

class read_instrt_reft
{
  read_instrt &ref;
public:
  read_instrt_reft(read_instrt &ref) :
      ref(ref)
  {
  }
  void operator()(const exprt &prog_arary_member)
  {
    ref(prog_arary_member);
  }
};

class extract_programt
{
  read_instrt read_instr;
public:
  extract_programt(synth_goto_solutiont::synth_programst &progs,
      const synth_programt &prog, const synth_variable_namest &names,
      const instruction_sett &instrset, const size_t max_size) :
      read_instr(progs, prog, names, instrset, max_size)
  {
  }

  void operator()(const goto_trace_stept &step)
  {
    if (goto_trace_stept::DECL != step.type) return;
    const exprt &value=step.full_lhs_value;
    if (ID_array != value.id()) return;
    const typet &type=value.type().subtype();
    if (ID_struct != type.id()) return;

    // only look at declarations of arrays with struct subtype
    
    const std::string &tname=id2string(to_struct_type(type).get_tag());
    const char * const synth_tag=&SYNTH_INSTRUCTION_TYPE_NAME[4];
    
    if (std::string::npos == tname.find(synth_tag)) return;
    const exprt::operandst &instructions=value.operands();
    read_instrt_reft read_instr(this->read_instr);
    std::for_each(instructions.begin(), instructions.end(), read_instr);
  }
};

void extract_programs(synth_goto_solutiont::synth_programst &progs,
    const synth_programt &prog, const goto_tracet &trace,
    const synth_variable_namest &names, const instruction_sett &instrset,
    const size_t max_size)
{
  const extract_programt extract(progs, prog, names, instrset, max_size);
  std::for_each(trace.steps.begin(), trace.steps.end(), extract);
}
}

void Synth::create_synth_solution(synth_goto_solutiont &result,
    const synth_programt &prog, const goto_tracet &trace,
    const synth_variable_idst &ids, const size_t max_size)
{
  synth_variable_namest names;
  reverse(names, ids);
  instruction_sett instr_set;
  extract_instruction_set(instr_set, prog.gf);
  synth_goto_solutiont::synth_programst &progs=result.synth_programs;
  // 
  extract_programs(progs, prog, trace, names, instr_set, max_size);
  // LSH TODO REMOVE:
  // synth_read_x0(result, prog, trace);
}
