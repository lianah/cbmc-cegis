typedef std::chrono::high_resolution_clock clockt;

template<class learnt, class verifyt, class mstreamt>
cegis_statistics_wrappert<learnt, verifyt, mstreamt>::cegis_statistics_wrappert(
    learnt &learner, verifyt &verifier, mstreamt &os) :
    learner(learner), verifier(verifier), os(os), max_solution_size(1), num_ces(
        0), learner_time(0), verifier_time(0), start_time(clockt::now())
{
}

template<class learnt, class verifyt, class mstreamt>
cegis_statistics_wrappert<learnt, verifyt, mstreamt>::~cegis_statistics_wrappert()
{
}

template<class learnt, class verifyt, class mstreamt>
template<class seedt>
void cegis_statistics_wrappert<learnt, verifyt, mstreamt>::seed(seedt &seed)
{
  learner.seed(seed);
}

template<class learnt, class verifyt, class mstreamt>
const typename cegis_statistics_wrappert<learnt, verifyt, mstreamt>::candidatet &cegis_statistics_wrappert<
    learnt, verifyt, mstreamt>::next_candidate() const
{
  return learner.next_candidate();
}

template<class learnt, class verifyt, class mstreamt>
bool cegis_statistics_wrappert<learnt, verifyt, mstreamt>::learn(
    size_t max_solution_size)
{
  this->max_solution_size=max_solution_size;
  return learner.learn(max_solution_size);
}

typedef clockt::time_point time_pointt;

template<class learnt, class verifyt, class mstreamt>
template<class itert>
bool cegis_statistics_wrappert<learnt, verifyt, mstreamt>::learn(itert first,
    const itert &last)
{
  num_ces+=std::distance(first, last);
  time_pointt before=clockt::now();
  const bool result=learner.learn(first, last);
  time_pointt after=clockt::now();
  learner_time+=std::chrono::duration_cast<millisecondst>(after - before);
  return result;
}

template<class learnt, class verifyt, class mstreamt>
void cegis_statistics_wrappert<learnt, verifyt, mstreamt>::show_candidate(
    messaget::mstreamt &os) const
{
  os << "<stats>" << messaget::endl;
  os << "  <max_solution_size>" << max_solution_size << "</max_solution_size>" << messaget::endl;
  os << "  <num_ces>" << num_ces << "</num_ces>" << messaget::endl;
  os << "  <learning_time>" << learner_time.count() << "</learning_time>" << messaget::endl;
  os << "  <verifying_time>" << verifier_time.count() << "</verifying_time>" << messaget::endl;
  const time_pointt now=clockt::now();
  const millisecondst full_time(std::chrono::duration_cast<millisecondst>(now - start_time));
  os << "  <full_time>" << full_time.count() << "</full_time>" << messaget::endl;
  os << "  <candidate>" << messaget::endl;
  learner.show_candidate(os);
  os << "  </candidate>" << messaget::endl;
  os << "</stats>" << messaget::eom;
}

template<class learnt, class verifyt, class mstreamt>
void cegis_statistics_wrappert<learnt, verifyt, mstreamt>::verify(
    const candidatet &candidate)
{
  time_pointt before=clockt::now();
  verifier.verify(candidate);
  time_pointt after=clockt::now();
  verifier_time+=std::chrono::duration_cast<millisecondst>(after - before);
}

template<class learnt, class verifyt, class mstreamt>
typename cegis_statistics_wrappert<learnt, verifyt, mstreamt>::const_iterator cegis_statistics_wrappert<
    learnt, verifyt, mstreamt>::counterexamples_begin() const
{
  return verifier.counterexamples_begin();
}

template<class learnt, class verifyt, class mstreamt>
typename cegis_statistics_wrappert<learnt, verifyt, mstreamt>::const_iterator cegis_statistics_wrappert<
    learnt, verifyt, mstreamt>::counterexamples_end() const
{
  return verifier.counterexamples_end();
}

template<class learnt, class verifyt, class mstreamt>
bool cegis_statistics_wrappert<learnt, verifyt, mstreamt>::has_counterexamples() const
{
  return verifier.has_counterexamples();
}

template<class learnt, class verifyt, class mstreamt>
bool cegis_statistics_wrappert<learnt, verifyt, mstreamt>::success() const
{
  return verifier.success();
}