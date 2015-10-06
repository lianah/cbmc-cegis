/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_SOLUTION_FACTORY_H_
#define CEGIS_SYNTH_SOLUTION_FACTORY_H_

#include <util/irep.h>

class goto_tracet;

namespace Synth {
typedef std::map<const irep_idt, size_t> synth_variable_idst;

/**
 * @brief
 *
 * @details
 *
 * @param result
 * @param prog
 * @param trace
 * @param ids
 * @param max_solution_size
 *
 * @return
 */
void create_synth_solution(class synth_goto_solutiont &result,
    const class synth_programt &prog, const goto_tracet &trace,
    const synth_variable_idst &ids, const size_t max_solution_size);
}
#endif /* CEGIS_SYNTH_SOLUTION_FACTORY_H_ */
