#include <cegis/genetic/symex_fitness.h>

symex_fitnesst::symex_fitnesst()
{
}

symex_fitnesst::~symex_fitnesst()
{
}

void symex_fitnesst::add_test_case(const counterexamplet &ce)
{
  counterexamples.push_back(ce);
}

symex_fitnesst::populationt::const_iterator symex_fitnesst::init(
    populationt &pop) const
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

void symex_fitnesst::set_fitness(individualt &individual) const
{
  individual.fitness=0u;
  for (const counterexamplet &ce : counterexamples)
  {
    // TODO: execute each test case. fitness=sum of successful runs
  }
}

symex_fitnesst::individualt::fitnesst symex_fitnesst::get_target_fitness() const
{
  return counterexamples.size();
}
