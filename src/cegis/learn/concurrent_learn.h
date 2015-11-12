/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_CONCURRENT_LEARN_H_
#define CEGIS_CONCURRENT_LEARN_H_

#include <functional>

#include <util/irep.h>
#include <util/task_pool.h>

/**
 * @brief
 *
 * @details
 */
template<class learner1t, class learner2t>
class concurrent_learnt
{
public:
  typedef typename learner1t::candidatet candidatet;
  typedef typename learner1t::counterexamplet counterexamplet;
  typedef typename learner1t::counterexamplest counterexamplest;
  typedef std::function<void(irept &, const candidatet &)> serialisert;
  typedef std::function<void(candidatet &, const irept &)> deserialisert;
private:
  learner1t &learner1;
  learner2t &learner2;
  task_poolt task_pool;
  const serialisert serialiser;
  const deserialisert deserialiser;
public:
  /**
   * @brief
   *
   * @details
   *
   * @param learner1
   * @param learner2
   * @param serialiser
   * @param deserialiser
   */
  concurrent_learnt(learner1t &learner1, learner2t &learner2,
      serialisert serialiser, deserialisert deserialiser);

  /**
   * @brief
   *
   * @details
   */
  ~concurrent_learnt();
};

#include "concurrent_learn.inc"

#endif /* CEGIS_CONCURRENT_LEARN_H_ */
