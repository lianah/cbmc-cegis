#include <cbmc/cbmc_solvers.h>
#include <cbmc/bmc.h>

template<class configt>
symex_test_runnert<configt>::symex_test_runnert(const optionst &options,
    configt &config) :
    options(options), config(config)
{
}

template<class configt>
symex_test_runnert<configt>::~symex_test_runnert()
{
}

template<class configt>
void symex_test_runnert<configt>::run_test(individualt &individual,
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
}

template<class configt>
void symex_test_runnert<configt>::join()
{
}
