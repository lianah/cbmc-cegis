/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_ADD_COUNTEREXAMPLES_H_
#define CEGIS_SYNTH_ADD_COUNTEREXAMPLES_H_

#include <deque>

#include <util/expr.h>

namespace Synth {

/**
 * @brief Counterexample type for this CEGIS component.
 *
 * @details Counterexamples give a set of values used for the state variables.
 */
typedef std::map<const irep_idt, exprt> counterexamplet;

typedef std::deque<counterexamplet> counterexamplest;

/**
 * @brief
 *
 * @details
 *
 * @param prog
 * @param ces
 */
void synth_add_learned_counterexamples(class synth_programt &prog,
    const counterexamplest &ces);

} 
#endif /* CEGIS_SYNTH_ADD_COUNTEREXAMPLES_H_ */
