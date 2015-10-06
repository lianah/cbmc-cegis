/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_REPLACE_OPERATORS_H_
#define CEGIS_SYNTH_REPLACE_OPERATORS_H_

#include <goto-programs/goto_program.h>

namespace Synth {
typedef std::map<size_t, const irep_idt> synth_variable_namest;

/**
 * @brief
 *
 * @details
 *
 * @param st
 * @param first
 * @param last
 * @param names
 * @param rnames
 * @param op0
 * @param op1
 * @param op2
 * @param instr_idx
 */
void replace_ops_in_instr(const symbol_tablet &st,
    const goto_programt::targett &first, const goto_programt::targett &last,
    const synth_variable_namest &names, const synth_variable_namest &rnames,
    const size_t op0, const size_t op1, const size_t op2,
    const size_t instr_idx);
}
#endif /* CEGIS_SYNTH_REPLACE_OPERATORS_H_ */
