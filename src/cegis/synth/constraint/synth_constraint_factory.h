/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_CONSTRAINT_FACTORY_H_
#define CEGIS_SYNTH_CONSTRAINT_FACTORY_H_

#include <util/std_expr.h>

namespace Synth {

/**
 * @brief
 *
 * @details
 *
 * @param number_of_loops
 *
 * @return
 */
exprt create_synth_constraint_init(const size_t number_of_loops);
exprt create_synth_constraint_inductive(const size_t number_of_loops);
exprt create_synth_constraint_strong(const size_t number_of_loops);
exprt create_synth_constraint_rank(const size_t number_of_loops);

/**
 * @brief
 *
 * @details
 *
 * @param base_name
 *
 * @return
 */
notequal_exprt synth_component_as_bool(const std::string &base_name);

} 
 
#endif /* CEGIS_SYNTH_CONSTRAINT_FACTORY_H_ */
