#include <cbmc/cbmc_solvers.h>
#include <cbmc/bmc.h>

template<class configt>
symex_fitnesst<configt>::symex_fitnesst(const optionst &options,
    configt &config) :
    options(options), config(config)
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
void symex_fitnesst<configt>::add_test_case(const counterexamplet &ce)
{
  counterexamples.push_back(ce);
}

template<class configt>
typename symex_fitnesst<configt>::populationt::const_iterator symex_fitnesst<
    configt>::init(populationt &pop) const
{
  const individualt::fitnesst target=get_target_fitness();
  for (populationt::iterator it=pop.begin(); it != pop.end(); ++it)
  {
    individualt &individual=*it;
    set_fitness(individual);
    if (target == individual.fitness) return it;
  }
  return pop.end();
}

#include <iostream> // XXX: Debug

template<class configt>
void symex_fitnesst<configt>::set_fitness(individualt &individual) const
{
  individualt::fitnesst &fitness=individual.fitness;
  fitness=0u;
  //if (counterexamples.empty()) return;
  if (counterexamples.empty())
  {
    std::cout << "<fitness>" << fitness << "</fitness>" << std::endl;
    return;
  }
  danger_goto_solutiont converted;
  config.convert(converted, individual);
  config.set_candidate(converted);
  //null_message_handlert msg;
  ui_message_handlert msg(ui_message_handlert::PLAIN, "");
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
  std::cout << "<fitness>" << fitness << "</fitness>" << std::endl;
}

template<class configt>
typename symex_fitnesst<configt>::individualt::fitnesst symex_fitnesst<configt>::get_target_fitness() const
{
  return counterexamples.size();
}
