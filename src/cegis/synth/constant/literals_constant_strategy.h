/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_LITERALS_CONSTANT_STRATEGY_H_
#define CEGIS_SYNTH_LITERALS_CONSTANT_STRATEGY_H_

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
size_t literals_constant_strategy(class Synth::synth_programt &program,
				  const size_t max_length);
}
#endif /* CEGIS_SYNTH_LITERALS_CONSTANT_STRATEGY_H_ */
