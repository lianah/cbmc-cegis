/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_ADD_CONSTANT_H_
#define CEGIS_SYNTH_ADD_CONSTANT_H_

namespace Synth {

/**
 * @brief
 *
 * @details
 *
 * @param program
 * @param value
 */
void add_synth_constant(class synth_programt &program,
    const class exprt &value);

/**
 * @brief
 *
 * @details
 *
 * @param program
 * @param name
 * @param value
 */
void add_synth_constant(synth_programt &program, const std::string &name,
    const exprt &value);

} 
#endif /* CEGIS_SYNTH_ADD_CONSTANT_H_ */
