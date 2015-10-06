/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_INSTRUCTION_SET_FACTORY_H_
#define CEGIS_SYNTH_INSTRUCTION_SET_FACTORY_H_

#include <goto-programs/goto_program.h>

namespace Synth {

typedef std::map<size_t, goto_programt::instructionst> instruction_sett;

/**
 * @brief
 *
 * @details
 *
 * @param instruction_set
 * @param gf
 */
void extract_instruction_set(instruction_sett &instruction_set,
    const class goto_functionst &gf);
}
#endif /* CEGIS_SYNTH_INSTRUCTION_SET_FACTORY_H_ */
