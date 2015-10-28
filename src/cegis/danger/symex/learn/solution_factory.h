/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_DANGER_SOLUTION_FACTORY_H_
#define CEGIS_DANGER_SOLUTION_FACTORY_H_

#include <cegis/value/program_individual.h>

typedef std::map<size_t, goto_programt::instructionst> instruction_sett;
typedef std::map<const irep_idt, size_t> danger_variable_idst;

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
void create_danger_solution(class danger_goto_solutiont &result,
    const class danger_programt &prog, const class goto_tracet &trace,
    const danger_variable_idst &ids, const size_t max_solution_size);

/**
 * @brief
 *
 * @details
 *
 * @param result
 * @param prog
 * @param ind
 * @param instr
 * @param ids
 * @param max_solution_size
 */
void create_danger_solution(danger_goto_solutiont &result,
    const danger_programt &prog, const program_individualt &ind,
    const instruction_sett &instr, const danger_variable_idst &ids,
    const size_t max_solution_size);

#endif /* CEGIS_DANGER_SOLUTION_FACTORY_H_ */
