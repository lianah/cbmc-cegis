/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_INVARIANT_ADD_COUNTEREXAMPLES_H_
#define CEGIS_INVARIANT_ADD_COUNTEREXAMPLES_H_

#include <deque>
#include <functional>

#include <util/expr.h>

/**
 * @brief Counterexample type for this CEGIS component.
 *
 * @details Counterexamples give a set of values used for the state variables.
 */
typedef std::map<const irep_idt, exprt> counterexamplet;
typedef std::deque<counterexamplet> counterexamplest;

/**
 * @brief Constraint factory function.
 *
 * @details Provides the constraint to test counterexamples against (safety or danger).
 */
typedef std::function<exprt(size_t)> constraint_factoryt;

/**
 * @brief
 *
 * @details
 *
 * @param prog
 * @param ces
 */
void danger_add_learned_counterexamples(class invariant_programt &prog,
    const counterexamplest &ces, constraint_factoryt constraint);

#endif /* CEGIS_INVARIANT_ADD_COUNTEREXAMPLES_H_ */
