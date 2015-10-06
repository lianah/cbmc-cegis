/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_STORE_NONDET_CHOICES_H_
#define CEGIS_SYNTH_STORE_NONDET_CHOICES_H_

namespace Synth {
/**
 * @brief
 *
 * @details
 *
 * @param program
 */
void store_skolem_choices(class synth_programt &program);

/**
 * @brief
 *
 * @details
 *
 * @param program
 */
void store_x0_choices(synth_programt &program);

}
#endif /* CEGIS_SYNTH_STORE_NONDET_CHOICES_H_ */
