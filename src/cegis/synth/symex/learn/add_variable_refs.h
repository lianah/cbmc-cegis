/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_INSTRUMENT_VARIABLE_REFS_H_
#define CEGIS_SYNTH_INSTRUMENT_VARIABLE_REFS_H_

#include <util/irep.h>

namespace Synth {
typedef std::map<const irep_idt, size_t> synth_variable_idst;

/**
 * @brief
 *
 * @details
 *
 * @param prog
 * @param ids
 * @param max_solution_size
 *
 * @return
 */
void synth_add_variable_refs(class synth_programt &prog,
    const synth_variable_idst &ids, const size_t max_solution_size);

/**
 * @brief
 *
 * @details
 *
 * @param st
 * @params ids
 *
 * @return
 */
size_t get_synth_variable_ids(const class symbol_tablet &st,
    synth_variable_idst &ids);
}
#endif /* CEGIS_SYNTH_INSTRUMENT_VARIABLE_REFS_H_ */
