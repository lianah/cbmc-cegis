#include <util/options.h>

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::ga_learnt(
    const optionst &options, selectt &select, mutatet &mutate, crosst &cross,
    fitnesst &fitness, convertt &convert) :
    options(options), select(select), mutate(mutate), cross(cross), fitness(
        fitness), convert(convert)
{
  select.init(pop);
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
  mutate.set_max_solution_size(max_solution_size);
  cross.set_max_solution_size(max_solution_size);
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
template<class itert>
bool ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::learn(itert first,
    const itert &last)
{
  for (; first != last; ++first)
    fitness.add_test_case(*first);
  const typename fitnesst::populationt::const_iterator it=fitness.init(pop);
  if (pop.end() != it) current_candidate; // TODO: Convert using convertt

  typedef typename selectt::individualt::fitnesst target_fitnesst;
  const target_fitnesst target_fitness=fitness.get_target_fitness();
  typename selectt::selectiont selection;
  bool evolving=true;
  while (evolving)
  {
    selection=select.select(pop);
    if (should_mutate(options))
    {
      if (!selection.can_mutate()) return false;
      mutate(selection.mutation_lhs(), selection.mutation_rhs());
    } else if (should_replace(options))
    {
      // TODO: Get random solution from selectt
    }
    else
    {
      if (!selection.can_cross()) return false;
      const typename selectt::couplest &couples=selection.get_couples();
      for (const typename selectt::couplet &couple : couples)
        cross(couple);
    }

    // TODO: Recheck fitness of selection.
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
