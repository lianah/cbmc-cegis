#include <cegis/genetic/dynamic_test_runner_helper.h>
#include <cegis/safety/genetic/dynamic_safety_test_runner.h>

dynamic_safety_test_runnert::dynamic_safety_test_runnert(
    const std::function<std::string(void)> &source_code_provider,
    const std::function<size_t(size_t)> &max_prog_sz) :
    source_code_provider(source_code_provider), max_prog_sz(max_prog_sz), shared_library(
    LIBRARY_PREFIX,
    LIBRARY_SUFFIX), handle(0), fitness_tester(0)
{
}

dynamic_safety_test_runnert::~dynamic_safety_test_runnert()
{
  close_fitness_tester_library(handle, fitness_tester);
}

void dynamic_safety_test_runnert::run_test(individualt &ind,
    const counterexamplet &ce, const std::function<void(bool)> on_complete)
{
  prepare_fitness_tester_library(handle, fitness_tester, source_code_provider,
      shared_library());
  // TODO: Implement
}

void dynamic_safety_test_runnert::join()
{
}
