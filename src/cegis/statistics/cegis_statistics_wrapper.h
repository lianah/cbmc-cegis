/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_STATISTICS_WRAPPER_H_
#define CEGIS_STATISTICS_WRAPPER_H_

#include <chrono>

#include <util/message.h>

/**
 * @brief
 *
 * @details
 */
template<class learnt, class verifyt, class mstreamt>
class cegis_statistics_wrappert
{
  learnt &learner;
  verifyt &verifier;
  mstreamt &os;
  size_t max_solution_size;
  size_t num_ces;
  typedef std::chrono::milliseconds millisecondst;
  millisecondst learner_time;
  millisecondst verifier_time;
  std::chrono::high_resolution_clock::time_point start_time;
public:
  typedef typename learnt::counterexamplet counterexamplet;
  typedef typename learnt::counterexamplest counterexamplest;
  typedef typename learnt::candidatet candidatet;
  typedef typename verifyt::const_iterator const_iterator;

  /**
   * @brief
   *
   * @details
   *
   * @param learner
   * @param verifier
   * @param os
   */
  cegis_statistics_wrappert(learnt &learner, verifyt &verifier, mstreamt &os);

  /**
   * @brief
   *
   * @details
   */
  ~cegis_statistics_wrappert();

  template<class seedt>
  void seed(seedt &seed);

  const candidatet &next_candidate() const;

  bool learn(size_t max_solution_size);

  template<class itert>
  bool learn(itert first, const itert &last);

  void show_candidate(messaget::mstreamt &os) const;

  void verify(const candidatet &candidate);

  const_iterator counterexamples_begin() const;

  const_iterator counterexamples_end() const;

  bool has_counterexamples() const;

  bool success() const;
};

#include "cegis_statistics_wrapper.inc"

#endif /* CEGIS_STATISTICS_WRAPPER_H_ */
