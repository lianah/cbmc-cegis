/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_SYMEX_PROGRAM_INSTRUMENTER_H_
#define CEGIS_SYNTH_SYMEX_PROGRAM_INSTRUMENTER_H_

namespace Synth {
/**
 * @brief
 *
 * @details
 *
 * @param prog
 */
  void synth_add_programs_to_learn(class Synth::synth_programt &prog,
				   const size_t max_solution_size);

} 
#endif /* CEGIS_SYNTH_SYMEX_PROGRAM_INSTRUMENTER_H_ */
