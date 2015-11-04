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

// XXX: Debug
#include <iostream>

#include <util/time_stopping.h>

static time_periodt init_time;
static time_periodt select_time;
static size_t select_count=0;
static time_periodt mutate_time;
static size_t mutate_count=0;
static time_periodt cross_time;
static time_periodt cross_fork_time;
static size_t cross_count=0;
static time_periodt replace_time;
static size_t replace_count=0;
// XXX: Debug

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
template<class itert>
bool ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::learn(itert first,
    const itert &last)
{
  for (; first != last; ++first)
    fitness.add_test_case(*first);
  std::cout << "<target_fitness>" << fitness.get_target_fitness()
      << "</target_fitness>" << std::endl;
  absolute_timet init_begin=current_time();
  const typename selectt::populationt::iterator it=fitness.init(pop);
  if (pop.end() != it)
  {
    convert.convert(current_candidate, *it);
    return true;
  }
  absolute_timet init_end=current_time();
  init_time+=init_end - init_begin;

  typename selectt::selectiont selection;
  size_t i=0; // XXX: Debug
  bool evolving=true;
  while (evolving)
  {
    absolute_timet init_begin=current_time(); // XXX: Debug
    selection=select.select(pop);
    absolute_timet init_end=current_time(); // XXX: Debug
    init_time+=init_end - init_begin; // XXX: Debug
    ++select_count; // XXX: Debug
    if (should_mutate(options))
    {
      absolute_timet mutate_begin=current_time(); // XXX: Debug
      if (!selection.can_mutate()) return false;
      typename selectt::individualt &lhs=selection.mutation_lhs();
      mutate(lhs, selection.mutation_rhs());
      absolute_timet mutate_end=current_time(); // XXX: Debug
      mutate_time+=mutate_end - mutate_begin; // XXX: Debug
      ++mutate_count; // XXX: Debug
      if (set_fitness(lhs)) return true;
    } else if (should_replace(options))
    {
      absolute_timet replace_begin=current_time(); // XXX: Debug
      typename selectt::individualt &ind=*selection.children.back();
      mutate.havoc(ind);
      absolute_timet replace_end=current_time(); // XXX: Debug
      replace_time+=replace_end - replace_begin; // XXX: Debug
      ++replace_count; // XXX: Debug
      if (set_fitness(ind)) return true;
    } else
    {
      if (!selection.can_cross()) return false;
      absolute_timet cross_begin=current_time(); // XXX: Debug
      cross(selection.get_parents(), selection.get_children());
      absolute_timet cross_end=current_time(); // XXX: Debug
      cross_time+=cross_end - cross_begin; // XXX: Debug
      absolute_timet cross_fork_begin=current_time(); // XXX: Debug
      for (const typename populationt::iterator &child : selection.get_children())
        if (set_fitness(*child)) return true;
      absolute_timet cross_fork_end=current_time(); // XXX: Debug
      cross_fork_time+=cross_fork_end - cross_fork_begin; // XXX: Debug
      ++cross_count; // XXX: Debug
    }
    // XXX: Debug
    if (++i % 100 == 0)
    {
      std::cout << "<init_time>" << init_time << "</init_time>" << std::endl;
      std::cout << "<select_time>" << select_time << "</select_time>"
          << std::endl;
      std::cout << "<select_count>" << select_count << "</select_count>"
          << std::endl;
      std::cout << "<mutate_time>" << mutate_time << "</mutate_time>"
          << std::endl;
      std::cout << "<mutate_count>" << mutate_count << "</mutate_count>"
          << std::endl;
      std::cout << "<replace_time>" << replace_time << "</replace_time>"
          << std::endl;
      std::cout << "<replace_count>" << replace_count << "</replace_count>"
          << std::endl;
      std::cout << "<cross_time>" << cross_time << "</cross_time>" << std::endl;
      std::cout << "<cross_fork_time>" << cross_fork_time
          << "</cross_fork_time>" << std::endl;
      std::cout << "<cross_count>" << cross_count << "</cross_count>"
          << std::endl;
    }
  }
  return false;
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
void ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::show_candidate(
    messaget::mstreamt &os) const
{
  convert.show(os, current_candidate);
}
