/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_PROGRAM_INDIVIDUAL_SERIALISATION_H_
#define CEGIS_PROGRAM_INDIVIDUAL_SERIALISATION_H_

#include <util/std_expr.h>

#include <cegis/value/program_individual.h>

/**
 * @brief
 *
 * @details
 *
 * @param step
 *
 * @return
 */
bool is_program_indivdual_decl(const class goto_trace_stept &step);

/**
 * @brief
 *
 * @details
 *
 * @param expr
 *
 * @return
 */
program_individualt::instructiont to_program_individual_instruction(
    const struct_exprt &expr);

/**
 * @brief
 *
 * @details
 *
 * @param trace
 *
 * @return
 */
program_individualt to_program_individual(const class danger_programt &prog,
    const class goto_tracet &trace);

/**
 * @brief
 *
 * @details
 *
 * @param sdu
 * @param individual
 */
void serialise(irept &sdu, const program_individualt &individual);

/**
 * @brief
 *
 * @details
 *
 * @param sdu
 * @param individual
 */
void deserialise(program_individualt &individual, const irept &sdu);

#endif /* CEGIS_PROGRAM_INDIVIDUAL_SERIALISATION_H_ */
