#include <util/options.h>

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::ga_learnt(
    const optionst &options, selectt &select, mutatet &mutate, crosst &cross,
    fitnesst &fitness, convertt &convert) :
    options(options), select(select), mutate(mutate), cross(cross), fitness(
        fitness), convert(convert), is_population_initialised(false)
{
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::~ga_learnt()
{
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
template<class seedt>
void ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::seed(
    seedt &seeder)
{
  fitness.seed(seeder);
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
const typename ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::candidatet &ga_learnt<
    selectt, mutatet, crosst, fitnesst, convertt>::next_candidate() const
{
  return current_candidate;
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
bool ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::learn(
    const size_t max_solution_size)
{
  if (!is_population_initialised)
  {
    select.init(pop);
    is_population_initialised=true;
  }
  std::vector<counterexamplet> empty;
  return learn(empty.end(), empty.end());
}

namespace
{
bool roll_rate(const int rate)
{
  return rand() < RAND_MAX / 100 * rate;
}

bool should_mutate(const optionst &opt)
{
  return roll_rate(opt.get_unsigned_int_option("cegis-genetic-mutation-rate"));
}

bool should_replace(const optionst &opt)
{
  return roll_rate(opt.get_unsigned_int_option("cegis-genetic-replace-rate"));
}
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
bool ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::set_fitness(
    typename selectt::individualt &ind)
{
  fitness.set_fitness(ind);
  typedef typename selectt::individualt::fitnesst target_fitnesst;
  const target_fitnesst target_fitness=fitness.get_target_fitness();
  const bool have_solution=(target_fitness == ind.fitness);
  if (have_solution) convert.convert(current_candidate, ind);
  return have_solution;
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
template<class itert>
bool ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::learn(itert first,
    const itert &last)
{
  for (; first != last; ++first)
    fitness.add_test_case(*first);
  const typename fitnesst::populationt::const_iterator it=fitness.init(pop);
  if (pop.end() != it)
  {
    convert.convert(current_candidate, *it);
    return true;
  }

  typename selectt::selectiont selection;
  bool evolving=true;
  while (evolving)
  {
    selection=select.select(pop);
    if (should_mutate(options))
    {
      if (!selection.can_mutate()) return false;
      typename selectt::individualt &lhs=selection.mutation_lhs();
      mutate(lhs, selection.mutation_rhs());
      if (set_fitness(lhs)) return true;
    } else if (should_replace(options))
    {
      typename selectt::individualt &ind=*selection.children.back();
      mutate.havoc(ind);
      if (set_fitness(ind)) return true;
    } else
    {
      if (!selection.can_cross()) return false;
      cross(selection.get_parents(), selection.get_children());
      for (const typename populationt::iterator &child : selection.get_children())
        if (set_fitness(*child)) return true;
    }
  }
  return false;
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
void ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::show_candidate(
    messaget::mstreamt &os) const
{
  // TODO: Move goto solution printing to helper class.
}
