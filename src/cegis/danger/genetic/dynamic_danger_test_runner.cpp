#include <cstdlib>

#include <util/bv_arithmetic.h>
#include <cegis/invariant/meta/literals.h>
#include <cegis/genetic/dynamic_test_runner_helper.h>
#include "dynamic_danger_test_runner.h"

dynamic_danger_test_runnert::dynamic_danger_test_runnert(
    const std::function<std::string(void)> &source_code_provider,
    const std::function<size_t(size_t)> &max_prog_sz) :
    source_code_provider(source_code_provider), max_prog_sz(max_prog_sz), shared_library(
    LIBRARY_PREFIX,
    LIBRARY_SUFFIX), handle(0), fitness_tester(0)
{
}

dynamic_danger_test_runnert::~dynamic_danger_test_runnert()
{
  if (fitness_tester) close_fitness_tester_library(handle);
}

void dynamic_danger_test_runnert::run_test(individualt &ind,
    const counterexamplet &ce, const std::function<void(bool)> on_complete)
{
  prepare_fitness_tester_library(handle, fitness_tester, source_code_provider,
      shared_library());
  std::deque<unsigned int> args;
  for (const std::pair<const irep_idt, exprt> &assignment : ce)
  {
    const bv_arithmetict arith(assignment.second);
    const mp_integer::llong_t v=arith.to_integer().to_long();
    args.push_back(static_cast<unsigned int>(v));
  }
  const individualt::programst &progs=ind.programs;
  const size_t num_progs=progs.size();
  for (size_t i=0; i < num_progs; ++i)
  {
    if (max_prog_sz(i) == 0u) continue;
    const individualt::programt &prog=progs[i];
    assert(!prog.empty());
    args.push_back(static_cast<unsigned int>(prog.size()));
    for (const individualt::instructiont &instr : prog)
    {
      args.push_back(static_cast<unsigned int>(instr.opcode));
      size_t op_count=0;
      for (const individualt::instructiont::opt &op : instr.ops)
      {
        args.push_back(static_cast<unsigned int>(op));
        ++op_count;
      }
      for (; op_count < 3u; ++op_count)
        args.push_back(0u);
    }
  }
  for (const individualt::x0t::value_type &x0 : ind.x0)
    args.push_back(static_cast<unsigned int>(x0));

  const int argc=args.size();

  std::vector<unsigned int> argv;
  argv.resize(argc);

  for (int i=0; i < argc; ++i)
    argv[i]=args[i];

  on_complete(EXIT_SUCCESS == fitness_tester(argv.data()));
}

void dynamic_danger_test_runnert::join()
{
}
