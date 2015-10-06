/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_ADD_INVARIANT_AND_TEMP_VARIABLES_H_
#define CEGIS_SYNTH_ADD_INVARIANT_AND_TEMP_VARIABLES_H_

namespace Synth {

class synth_programt;

/**
 * @brief
 *
 * @details
 *
 * @param program
 * @param max_program_length
 */
 void add_synth_invariants_and_temp_variables(Synth::synth_programt &program,
    const size_t max_program_length);

} 
#endif /* CEGIS_SYNTH_ADD_INVARIANT_AND_TEMP_VARIABLES_H_ */
