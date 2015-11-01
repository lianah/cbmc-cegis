#include <goto-instrument/dump_c.h>

#include <cegis/danger/fitness/concrete_fitness_source_provider.h>

concrete_fitness_source_providert::concrete_fitness_source_providert(
    const danger_programt &prog) :
    learn_config(prog)
{
}

concrete_fitness_source_providert::~concrete_fitness_source_providert()
{
}

#define STUB_MAX_SOLUTION_SIZE 28293u

std::string concrete_fitness_source_providert::operator ()()
{
  danger_learn_configt::counterexamplest empty;
  learn_config.process(empty, STUB_MAX_SOLUTION_SIZE);
  const symbol_tablet &st=learn_config.get_symbol_table();
  const goto_functionst &gf=learn_config.get_goto_functions();
  const namespacet ns(st);
  std::ostringstream oss;
  dump_c(gf, true, ns, oss);
  return oss.str();
}
