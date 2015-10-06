#include <algorithm>

#include <ansi-c/c_types.h>

#include <util/arith_tools.h>

#include <cegis/synth/options/synth_program.h>
#include <cegis/util/program_helper.h>
#include <cegis/synth/instrument/meta_variables.h>
#include <cegis/synth/symex/learn/add_programs_to_learn.h>

using namespace Synth;

namespace
{
const char PROG_SUFFIX[]="_prog";
std::string get_prog_name(const symbol_tablet &st,
    const goto_programt::targett &decl)
{
  const irep_idt &base_id=st.lookup(get_affected_variable(*decl)).base_name;
  std::string base_name(id2string(base_id));
  return base_name+=PROG_SUFFIX;
}

const char SYNTH_EXECUTE[]="__CPROVER_synth_execute";
void execute(const symbol_tablet &st, goto_functionst &gf,
    const size_t max_solution_size, const goto_programt::targett &decl,
    const std::string &prog_base_name)
{
  goto_programt &body=get_synth_body(gf);
  goto_programt::targett pos=decl;
  goto_programt::targett execution=body.insert_after(++pos);
  execution->type=goto_program_instruction_typet::FUNCTION_CALL;
  execution->source_location=default_synth_source_location();
  code_function_callt call;
  call.function()=st.lookup(SYNTH_EXECUTE).symbol_expr();
  const std::string prog_name(get_synth_meta_name(prog_base_name));
  const symbol_exprt prog_symbol(st.lookup(prog_name).symbol_expr());
  const typet size_type(unsigned_int_type());
  const constant_exprt index(from_integer(0u, size_type));
  const index_exprt first_elem(prog_symbol, index);
  call.arguments().push_back(address_of_exprt(first_elem));
  const constant_exprt size(from_integer(max_solution_size, size_type));
  call.arguments().push_back(size);
  execution->code=call;
}

void execute(const symbol_tablet &st, goto_functionst &gf,
    const size_t max_solution_size, const goto_programt::targett &decl)
{
  execute(st, gf, max_solution_size, decl, get_prog_name(st, decl));
}

const char SYNTH_INSTRUCTION_TYPE_NAME[]="tag-__CPROVER_synth_instructiont";
goto_programt::targett add_program(synth_programt &prog,
    goto_programt::targett pos, const size_t max_solution_size,
    const goto_programt::targett &decl)
{
  symbol_tablet &st=prog.st;
  goto_functionst &gf=prog.gf;
  const std::string base_name(get_prog_name(st, decl));
  const typet size_type(unsigned_int_type());
  const constant_exprt size(from_integer(max_solution_size, size_type));
  const symbol_typet instr_type(SYNTH_INSTRUCTION_TYPE_NAME);
  const array_typet prog_type(instr_type, size);
  pos=declare_synth_variable(st, gf, pos, base_name, prog_type);
  execute(st, gf, max_solution_size, decl);
  return pos;
}

class declare_programst
{
  synth_programt &prog;
  const size_t max_solution_size;
  goto_programt::targett pos;
public:
  declare_programst(synth_programt &prog, const size_t max_solution_size,
      const goto_programt::targett &pos) :
      prog(prog), max_solution_size(max_solution_size), pos(pos)
  {
  }

  void operator()(const synth_programt::loopt &loop)
  {
    const symbol_tablet &st=prog.st;
    goto_functionst &gf=prog.gf;
    const synth_programt::meta_vars_positionst &meta=loop.meta_variables;
    pos=add_program(prog, pos, max_solution_size, meta.Dx);
    const std::string dx_prog_name=get_prog_name(st, meta.Dx);
    execute(st, gf, max_solution_size, meta.Dx_prime, dx_prog_name);
    const goto_programt::targetst &rx=meta.Rx;
    const goto_programt::targetst &rx_prime=meta.Rx_prime;
    if (!rx.empty() && !rx_prime.empty())
    {
      const goto_programt::targett rx_prog=*rx.rbegin();
      pos=add_program(prog, pos, max_solution_size, rx_prog);
      const std::string rx_prog_name=get_prog_name(st, rx_prog);
      execute(st, gf, max_solution_size, *rx_prime.rbegin(), rx_prog_name);
    }
    const goto_programt::targetst &sx=meta.Sx;
    if (!sx.empty())
      pos=add_program(prog, pos, max_solution_size, *sx.rbegin());
  }
};
}

void Synth::synth_add_programs_to_learn(Synth::synth_programt &prog,
    const size_t max_solution_size)
{
  const synth_programt::loopst &loops=prog.loops;
  if (loops.empty()) return;
  goto_programt::targett pos=prog.synth_range.begin;
  const declare_programst declare_progs(prog, max_solution_size, --pos);
  std::for_each(loops.begin(), loops.end(), declare_progs);
  const synth_programt::loopt first_loop=*loops.begin();
  const symbol_tablet &st=prog.st;
  const std::string D0=get_prog_name(st, first_loop.meta_variables.Dx);
  execute(st, prog.gf, max_solution_size, prog.Dx0, D0);
}
