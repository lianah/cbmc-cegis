/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_LEARN_CONFIG_H_
#define CEGIS_SYNTH_LEARN_CONFIG_H_

#include <deque>

#include <util/message.h>

#include <cegis/synth/options/synth_program.h>
#include <cegis/synth/value/synth_goto_solution.h>


namespace Synth {

/**
 * @brief
 *
 * @details
 */
class synth_learn_configt
{
  /**
   * @brief
   *
   * @details
   */
  typedef std::map<const irep_idt, size_t> synth_variable_idst;

  const Synth::synth_programt &original_program;
  synth_programt program;
  synth_variable_idst var_ids;
public:
  /**
   * @brief Counterexample type for this CEGIS component.
   *
   * @details Counterexamples give a set of values used for the state variables.
   */
  typedef std::map<const irep_idt, exprt> counterexamplet;
  typedef std::deque<counterexamplet> counterexamplest;

  /**
   * @brief Candidate solution type for this CEGIS component.
   *
   * @details Solutions are provided as a set of GOTO function bodies
   * (goto_programt::instructionst) for function names.
   */
  typedef synth_goto_solutiont candidatet;

  /**
   * @brief
   *
   * @details
   *
   * @param program
   */
  synth_learn_configt(const Synth::synth_programt &program);

  /**
   * @brief
   *
   * @details
   */
  ~synth_learn_configt();

  /**
   * @brief
   *
   * @details
   *
   * @param counterexamples
   * @param max_solution_size
   */
  void process(const counterexamplest &counterexamples,
      size_t max_solution_size);

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  const symbol_tablet &get_symbol_table() const;

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  const goto_functionst &get_goto_functions() const;

  /**
   * @brief
   *
   * @details
   *
   * @param current_candidate
   * @param trace
   * @param max_solution_size
   */
  void convert(candidatet &current_candidate, const class goto_tracet &trace,
      size_t max_solution_size);

  /**
   * @brief
   *
   * @details
   *
   * @param os
   * @param candidate
   */
  void show_candidate(messaget::mstreamt &os, const candidatet &candidate);
};
} 

#endif /* CEGIS_SYNTH_LEARN_CONFIG_H_ */
