#include <algorithm>

#include <cbmc/cbmc_solvers.h>
#include <cbmc/bmc.h>

template<class test_runnert>
lazy_fitnesst<test_runnert>::lazy_fitnesst(test_runnert &test_runner) :
    test_runner(test_runner)
{
}

template<class test_runnert>
lazy_fitnesst<test_runnert>::~lazy_fitnesst()
{
}

template<class test_runnert>
template<class seedt>
void lazy_fitnesst<test_runnert>::seed(seedt &seeder)
{
  seeder(counterexamples);
}

template<class test_runnert>
void lazy_fitnesst<test_runnert>::add_test_case(const counterexamplet &ce)
{
  const counterexamplest::const_iterator end=counterexamples.end();
  if (end == std::find(counterexamples.begin(), counterexamples.end(), ce))
    counterexamples.push_back(ce);
}

template<class test_runnert>
typename lazy_fitnesst<test_runnert>::populationt::iterator lazy_fitnesst<
    test_runnert>::find_candidate(populationt &pop)
{
  const size_t ces=counterexamples.size();
  for (populationt::iterator it=pop.begin(); it != pop.end(); ++it)
    if (it->fitness == ces) return it;
  return pop.end();
}

template<class test_runnert>
typename lazy_fitnesst<test_runnert>::populationt::iterator lazy_fitnesst<
    test_runnert>::init(populationt &pop)
{
  const size_t ces=counterexamples.size();
  for (individualt &individual : pop)
  {
    size_t &executed=executed_test_cases[&individual];
    for (; executed < ces; ++executed)
      test_runner.run_test(individual, counterexamples[executed]);
    const populationt::iterator candidate=find_candidate(pop);
    if (pop.end() != candidate)
    {
      test_runner.join();
      return candidate;
    }
  }
  test_runner.join();
  return find_candidate(pop);
}

template<class test_runnert>
void lazy_fitnesst<test_runnert>::set_fitness(individualt &individual)
{
  individual.fitness=0u;
  for (const counterexamplet &ce : counterexamples)
    test_runner.run_test(individual, ce);
  test_runner.join();
  executed_test_cases[&individual]=counterexamples.size();
}

template<class test_runnert>
typename lazy_fitnesst<test_runnert>::individualt::fitnesst lazy_fitnesst<
    test_runnert>::get_target_fitness() const
{
  return counterexamples.size();
}
