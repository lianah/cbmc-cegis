/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_DANGER_READ_X0_H_
#define CEGIS_DANGER_READ_X0_H_

class goto_tracet;

namespace Danger {
/**
 * @brief
 *
 * @details
 *
 * @param result
 * @param prog
 * @param trace
 */
void danger_read_x0(class danger_goto_solutiont &result,
    const class danger_programt &prog, const goto_tracet &trace);
}
#endif /* CEGIS_DANGER_READ_X0_H_ */
