#include <algorithm>

#include <cbmc/cbmc_solvers.h>
#include <cbmc/bmc.h>

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
void symex_fitnesst<configt>::add_test_case(const counterexamplet &ce)
{
  const counterexamplest::const_iterator end=counterexamples.end();
  if (end == std::find(counterexamples.begin(), counterexamples.end(), ce))
    counterexamples.push_back(ce);
}

template<class configt>
typename symex_fitnesst<configt>::populationt::iterator symex_fitnesst<configt>::find_candidate(
    populationt &pop)
{
  const size_t ces=counterexamples.size();
  for (populationt::iterator it=pop.begin(); it != pop.end(); ++it)
    if (it->fitness == ces) return it;
  return pop.end();
}

template<class configt>
typename symex_fitnesst<configt>::populationt::iterator symex_fitnesst<configt>::init(
    populationt &pop)
{
  const size_t ces=counterexamples.size();
  for (individualt &individual : pop)
  {
    size_t &executed=executed_test_cases[&individual];
    for (size_t i=executed; i < ces; ++i, ++executed)
      test_runner.run_test(individual, counterexamples[i]);
    const populationt::iterator candidate=find_candidate(pop);
    if (pop.end() != candidate)
    {
      // XXX: test_runner.abandon();
      test_runner.join();
      return candidate;
    }
  }
  test_runner.join();
  return find_candidate(pop);
}

template<class configt>
void symex_fitnesst<configt>::set_fitness(individualt &individual)
{
  individualt::fitnesst &fitness=individual.fitness;
  fitness=0u;
  for (const counterexamplet &ce : counterexamples)
    test_runner.run_test(individual, ce);
  test_runner.join();
  executed_test_cases[&individual]=counterexamples.size();
}

template<class configt>
typename symex_fitnesst<configt>::individualt::fitnesst symex_fitnesst<configt>::get_target_fitness() const
{
  return counterexamples.size();
}
