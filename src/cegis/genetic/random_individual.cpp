#include <cassert>
#include <cstdlib>

#include <util/bv_arithmetic.h>
#include <util/std_types.h>

#include <cegis/genetic/random_individual.h>

random_individualt::random_individualt(unsigned int seed, const typet &type,
    const instruction_set_info_factoryt &info_factory, size_t num_progs,
    const std::function<size_t(void)> &num_vars,
    const std::function<size_t(void)> &num_x0) :
    type(type), info_factory(info_factory), num_vars(num_vars), num_x0(num_x0), num_progs(
        num_progs), prog_size_limit(0u), vars_limit(0u), x0_limit(0u), opcode_limit(
        0u)
{
  srand(seed);
}

random_individualt::~random_individualt()
{
}

void random_individualt::set_max_prog_size(const size_t size)
{
  prog_size_limit=size + 1;
  vars_limit=num_vars() + 1;
  x0_limit=num_x0() + 1;
  opcode_limit=info_factory.get_info().size() + 1;
}

size_t random_individualt::prog_size() const
{
  return rand() % prog_size_limit;
}

program_individualt::instructiont::opcodet random_individualt::opcode() const
{
  return rand() % opcode_limit;
}

// XXX: Symmetry breaking?
program_individualt::instructiont::opt random_individualt::op() const
{
  return rand() % vars_limit;
}

void random_individualt::havoc(program_individualt::instructiont &instr)
{
  instr.opcode=opcode();
  const instruction_set_infot &info=info_factory.get_info();
  const instruction_set_infot::const_iterator num_ops=info.find(instr.opcode);
  assert(info.end() != num_ops);
  instr.ops.resize(num_ops->second);
  for (program_individualt::instructiont::opt &o : instr.ops)
    o=op();
}

void random_individualt::havoc(program_individualt::programt &prog)
{
  const size_t prog_size=rand() % prog_size_limit;
  prog.resize(prog_size);
  for (program_individualt::instructiont &instr : prog)
    havoc(instr);
}

program_individualt::nondet_choices::value_type random_individualt::x0() const
{
  return rand();
}

program_individualt::nondet_choices::value_type random_individualt::constant() const
{
  const bv_spect spec(type);
  const unsigned int width=spec.width;
  const unsigned int wordmask=spec.max_value().to_ulong();
  const unsigned int r=rand() % 6u;
  switch (r)
  {
  case 0:
    return 0;
  case 1:
    return 1;
  case 2:
    return wordmask;
  case 3:
    return 1 << (width - 1);
  case 4:
    return (1 << (width - 1)) - 1;
  default:
    return rand();
  }
}

void random_individualt::havoc(program_individualt &ind)
{
  program_individualt::programst &progs=ind.programs;
  progs.resize(num_progs);
  for (program_individualt::programt &prog : progs)
    havoc(prog);
  ind.x0.resize(x0_limit - 1);
  for (unsigned int &x : ind.x0)
    x=x0();
}

unsigned int random_individualt::rand() const
{
  return rand();
}
