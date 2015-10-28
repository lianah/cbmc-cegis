#include <algorithm>

#include <cbmc/cbmc_solvers.h>
#include <cbmc/bmc.h>

// XXX: Debug
#include <iostream>

template<class configt>
symex_fitnesst<configt>::symex_fitnesst(const optionst &options,
    configt &config) :
    options(options), config(config), test_runner(options, config)
{
}

template<class configt>
symex_fitnesst<configt>::~symex_fitnesst()
{
}

template<class configt>
template<class seedt>
void symex_fitnesst<configt>::seed(seedt &seed) const
{
  // TODO: Implement!
}

template<class configt>
void symex_fitnesst<configt>::run_test(individualt &individual,
    const counterexamplet &ce)
{
  individualt::fitnesst &fitness=individual.fitness;
  danger_goto_solutiont converted;
  config.convert(converted, individual);
  config.set_candidate(converted);
  null_message_handlert msg;
  config.set_test_case(ce);
  const symbol_tablet &st=config.get_symbol_table();
  cbmc_solverst solvers(options, st, msg);
  const goto_functionst &gf=config.get_goto_functions();
  const std::unique_ptr<cbmc_solverst::solvert> solver=solvers.get_solver();
  bmct bmc(options, st, msg, solver->prop_conv());
  if (safety_checkert::SAFE == bmc(gf)) ++fitness;
  /*std::cout << "<run_test>" << std::endl;
   std::cout << "  <fitness>" << fitness << "</fitness>" << std::endl;
   std::cout << "  <target>" << get_target_fitness() << "</target>" << std::endl;
   std::cout << "</run_test>" << std::endl;*/
}

template<class configt>
void symex_fitnesst<configt>::add_test_case(populationt &pop,
    const counterexamplet &ce)
{
  const counterexamplest::const_iterator end=counterexamples.end();
  if (end == std::find(counterexamples.begin(), counterexamples.end(), ce))
    counterexamples.push_back(ce);
  for (individualt &individual : pop)
    test_runner.run_test(individual, ce);
  test_runner.join();
  assert(false);
}

template<class configt>
void symex_fitnesst<configt>::set_fitness(individualt &individual) const
{
  individualt::fitnesst &fitness=individual.fitness;
  fitness=0u;
  if (counterexamples.empty()) return;
  danger_goto_solutiont converted;
  config.convert(converted, individual);
  config.set_candidate(converted);
  null_message_handlert msg;
  for (const counterexamplet &ce : counterexamples)
  {
    config.set_test_case(ce);
    const symbol_tablet &st=config.get_symbol_table();
    cbmc_solverst solvers(options, st, msg);
    const goto_functionst &gf=config.get_goto_functions();
    const std::unique_ptr<cbmc_solverst::solvert> solver=solvers.get_solver();
    bmct bmc(options, st, msg, solver->prop_conv());
    if (safety_checkert::SAFE == bmc(gf)) ++fitness;
  }
  /*std::cout << "<set_fitness>" << std::endl;
   std::cout << "  <fitness>" << fitness << "</fitness>" << std::endl;
   std::cout << "  <target>" << get_target_fitness() << "</target>" << std::endl;
   std::cout << "</set_fitness>" << std::endl;*/
}

template<class configt>
typename symex_fitnesst<configt>::individualt::fitnesst symex_fitnesst<configt>::get_target_fitness() const
{
  return counterexamples.size();
}
