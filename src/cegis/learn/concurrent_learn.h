/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_CONCURRENT_LEARN_H_
#define CEGIS_CONCURRENT_LEARN_H_

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

  concurrent_learnt();
  ~concurrent_learnt();
};

#include "concurrent_learn.inc"

#endif /* CEGIS_CONCURRENT_LEARN_H_ */
