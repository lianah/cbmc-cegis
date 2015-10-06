/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_VERIFY_CONFIG_H_
#define CEGIS_SYNTH_VERIFY_CONFIG_H_

#include <deque>

#include <cegis/synth/options/synth_program.h>

namespace Synth {

/**
 * @brief
 *
 * @details
 */
class synth_verify_configt
{
  const Synth::synth_programt &original_program;
  Synth::synth_programt program;
  goto_programt::targetst quantifiers;
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
  typedef class synth_goto_solutiont candidatet;

  /**
   * @brief
   *
   * @details
   *
   * @param program
   */
  synth_verify_configt(const synth_programt &program);

  /**
   * @brief
   *
   * @details
   */
  ~synth_verify_configt();

  /**
   * @brief
   *
   * @details
   *
   * @param candidate
   */
  void process(const candidatet &candidate);

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  const symbol_tablet &get_symbol_table();

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  const goto_functionst &get_goto_functions();

  /**
   * @brief
   *
   * @details
   *
   * @param counterexamples
   * @param trace
   */
  void convert(counterexamplest &counterexamples,
      const class goto_tracet &trace);
};
}
#endif /* CEGIS_SYNTH_VERIFY_CONFIG_H_ */
