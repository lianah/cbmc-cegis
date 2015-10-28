#include <cassert>
#include <cstdlib>

#include <util/bv_arithmetic.h>
#include <util/std_types.h>

#include <cegis/genetic/random_individual.h>

random_individualt::random_individualt(unsigned int seed, const typet &type,
    const instruction_set_info_factoryt &info_factory,
    const std::function<size_t(size_t)> &min_prog_sz,
    const std::function<size_t(size_t)> &max_prog_sz,
    const std::function<size_t(void)> &num_progs,
    const std::function<size_t(void)> &num_vars,
    const std::function<size_t(void)> &num_x0) :
    type(type), info_factory(info_factory), min_prog_sz(min_prog_sz), max_prog_sz(
        max_prog_sz), num_progs(num_progs), num_vars(num_vars), num_x0(num_x0)
{
  srand(seed);
}

random_individualt::~random_individualt()
{
}

size_t random_individualt::prog_size(const size_t index) const
{
  const size_t max=max_prog_sz(index);
  if (max == 0u) return 0u;
  const size_t min=min_prog_sz(index);
  if (min >= max) return min;
  const size_t diff=max - min;
  return min + rand() % (diff + 1);
}

program_individualt::instructiont::opcodet random_individualt::opcode()
{
  return rand() % info_factory.get_info().size();
}

// XXX: Symmetry breaking?
program_individualt::instructiont::opt random_individualt::op(
    const size_t instr_index) const
{
  return rand() % (num_vars() + instr_index);
}

void random_individualt::havoc(program_individualt::instructiont &instr,
    const size_t index)
{
  instr.opcode=opcode();
  const instruction_set_infot &info=info_factory.get_info();
  const instruction_set_infot::const_iterator num_ops=info.find(instr.opcode);
  assert(info.end() != num_ops);
  instr.ops.resize(num_ops->second);
  for (program_individualt::instructiont::opt &o : instr.ops)
    o=op(index);
}

void random_individualt::havoc(program_individualt::programt &prog,
    const size_t index)
{
  const size_t prog_sz=prog_size(index);
  prog.resize(prog_sz);
  for (size_t i=0; i < prog_sz; ++i)
    havoc(prog[i], i);
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
  progs.resize(num_progs());
  for (size_t i=0u; i < progs.size(); ++i)
    havoc(progs[i], i);
  ind.x0.resize(num_x0());
  for (unsigned int &x : ind.x0)
    x=x0();
}

unsigned int random_individualt::rand() const
{
  return ::rand();
}

size_t random_individualt::get_num_vars() const
{
  return num_vars();
}

size_t random_individualt::get_max_prog_size(const size_t prog_index) const
{
  return max_prog_sz(prog_index);
}
