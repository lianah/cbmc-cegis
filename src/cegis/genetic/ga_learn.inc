template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::ga_learnt(
    selectt &select, mutatet &mutate, crosst &cross, fitnesst &fitness,
    convertt &convert) :
    select(select), mutate(mutate), cross(cross), fitness(fitness), convert(
        convert)
{
  select.init(population);
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::~ga_learnt()
{
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
template<class seedt>
void ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::seed(seedt &seeder)
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
  // TODO: select.set_max_solution_size(max_solution_size);
  // TODO: mutate.set_max_solution_size(max_solution_size);
  // TODO: cross.set_max_solution_size(max_solution_size);
  // TODO: Run GA algorithm
  return false;
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
template<class itert>
bool ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::learn(itert first,
    const itert &last)
{
  for (; first != last; ++first)
    fitness.add_test_case(*first);
  // TODO: Run GA algorithm
  return false;
}

template<class selectt, class mutatet, class crosst, class fitnesst,
    class convertt>
void ga_learnt<selectt, mutatet, crosst, fitnesst, convertt>::show_candidate(
    messaget::mstreamt &os) const
{
  // TODO: Move goto solution printing to helper class.
}
