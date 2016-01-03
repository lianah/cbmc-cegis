/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SAFETY_SOLUTION_FACTORY_H_
#define CEGIS_SAFETY_SOLUTION_FACTORY_H_

#include <cegis/safety/value/safety_goto_solution.h>

typedef std::map<const irep_idt, size_t> invariant_variable_idst;

/**
 * @brief
 *
 * @details
 *
 * @param solution
 * @param prog
 * @param trace
 * @param var_ids
 * @param max_solution_size
 */
void create_safety_solution(safety_goto_solutiont &solution,
    const class safety_programt &prog, const class goto_tracet &trace,
    const invariant_variable_idst &var_ids, size_t max_solution_size);

#endif /* CEGIS_SAFETY_SOLUTION_FACTORY_H_ */
