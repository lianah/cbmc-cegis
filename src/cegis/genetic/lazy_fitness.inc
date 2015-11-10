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
  //if (end == std::find(counterexamples.begin(), counterexamples.end(), ce))
  assert (end == std::find(counterexamples.begin(), counterexamples.end(), ce));
    counterexamples.push_back(ce);
}

template<class test_runnert>
typename lazy_fitnesst<test_runnert>::populationt::iterator lazy_fitnesst<
    test_runnert>::find_candidate(populationt &pop)
{
  for (const populationt::value_type &ind : pop)
    for (const populationt::value_type::programt &prog : ind.programs)
      for (size_t i=0; i < prog.size(); ++i)
        for (const populationt::value_type::programt::value_type::opt &op : prog[i].ops)
          assert (op < (5+i));
  const size_t ces=get_target_fitness();
  for (populationt::iterator it=pop.begin(); it != pop.end(); ++it)
    if (it->fitness == ces) return it;
  return pop.end();
}

size_t get_bounty(const size_t index)
{
  /*size_t bounty=1u;
  bounty+=index / 5u;
  return bounty;*/
  return 1u;
}

template<class test_runnert>
typename lazy_fitnesst<test_runnert>::populationt::iterator lazy_fitnesst<
    test_runnert>::init(populationt &pop)
{
  const counterexamplest &ces=counterexamples;
  const size_t ce_count=ces.size();
  for (individualt &individual : pop)
  {
    size_t &executed=executed_test_cases[&individual];
    for (; executed < ce_count; ++executed)
      test_runner.run_test(individual, ces[executed], get_bounty(executed));
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
  const size_t num_ces=counterexamples.size();
  for (size_t i=0; i < num_ces; ++i)
    test_runner.run_test(individual, counterexamples[i], get_bounty(i));
  test_runner.join();
  executed_test_cases[&individual]=counterexamples.size();
}

template<class test_runnert>
typename lazy_fitnesst<test_runnert>::individualt::fitnesst lazy_fitnesst<
    test_runnert>::get_target_fitness() const
{
  size_t fitness=0;
  const size_t end=counterexamples.size();
  for (size_t i=0u; i < end; ++i)
    fitness+=get_bounty(i);
  return fitness;
}
