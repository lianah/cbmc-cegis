/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_LIBRARY_H_
#define CEGIS_SYNTH_LIBRARY_H_

#include <cstddef>

class message_handlert;

namespace Synth {
/**
 * @brief Load the interpreter into the program. 
 *
 * @details
 *
 * @param prog
 * @param msg
 * @param num_vars
 * @param num_consts
 * @param max_solution_size
 */
  void add_synth_library(class Synth::synth_programt &prog,
			 message_handlert &msg, const size_t num_vars,
			 const size_t num_consts,
			 const size_t max_solution_size);
}
#endif /* CEGIS_SYNTH_LIBRARY_H_ */
