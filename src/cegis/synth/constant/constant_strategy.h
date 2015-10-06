/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_CONSTANT_STRATEGY_H_
#define CEGIS_SYNTH_CONSTANT_STRATEGY_H_

#include <cstddef>

namespace Synth {
/**
 * @brief
 *
 * @details
 *
 * @param program
 * @param max_length
 *
 * @return
 */
typedef size_t (*constant_strategyt)(class Synth::synth_programt &program,
    const size_t max_length);

} 
#endif /* CEGIS_SYNTH_CONSTANT_STRATEGY_H_ */
