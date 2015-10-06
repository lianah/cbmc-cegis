/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_DEFAULT_CONSTANT_STRATEGY_H_
#define CEGIS_SYNTH_DEFAULT_CONSTANT_STRATEGY_H_

#include <cstddef>

namespace Synth {
/**
 * @brief Modifies program by adding new SYNTH_CONSTANT_x variables
 * assigned to constant value x. Constants added are 0, MAX, and  c-1, c , c+1
 * for all the constants c occuring in the input program.
 *
 * @details
 *
 * @param program
 * @param max_length
 *
 * @return
 */
void default_constant_strategy(class synth_programt &program,
    const size_t max_length);

} 
#endif /* CEGIS_SYNTH_DEFAULT_CONSTANT_STRATEGY_H_ */
