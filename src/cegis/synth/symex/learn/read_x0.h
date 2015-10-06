/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_READ_X0_H_
#define CEGIS_SYNTH_READ_X0_H_

namespace Synth {
/**
 * @brief
 *
 * @details
 *
 * @param result
 * @param prog
 * @param trace
 */
void synth_read_x0(class synth_goto_solutiont &result,
    const class synth_programt &prog, const class goto_tracet &trace);
}
#endif /* CEGIS_SYNTH_READ_X0_H_ */
