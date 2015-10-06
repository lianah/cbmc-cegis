/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_INSERT_CONSTRAINT_H_
#define CEGIS_SYNTH_INSERT_CONSTRAINT_H_

#include <deque>
#include <goto-programs/goto_program.h>

namespace Synth {
/**
 * @brief
 *
 * @details
 *
 * @param quantifiers
 * @param program
 */
void synth_insert_constraint(goto_programt::targetst &quantifiers,
    class synth_programt &program);

typedef std::deque<symbol_exprt> constraint_varst;

/**
 * @brief
 *
 * @details
 *
 * @param vars
 * @param program
 */
void get_synth_constraint_vars(constraint_varst &vars,
    const synth_programt &program);
}
#endif /* CEGIS_SYNTH_INSERT_CONSTRAINT_H_ */
