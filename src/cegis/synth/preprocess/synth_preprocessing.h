/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_PREPROCESSING_H_
#define CEGIS_SYNTH_PREPROCESSING_H_

#include <cegis/synth/options/synth_program.h>
#include <cegis/synth/constant/constant_strategy.h>

namespace Synth {

/**
 * @brief
 *
 * @details
 */
class synth_preprocessingt
{
  Synth::synth_programt original_program;
  Synth::synth_programt current_program;
  Synth::constant_strategyt constant_strategy;
  bool synth_ranking;
public:
  /**
   * @brief
   *
   * @details
   *
   * @param st
   * @param gf
   * @param constant_strategy
   */
  synth_preprocessingt(const symbol_tablet &st,
		       const goto_functionst &gf,
		       constant_strategyt constant_strategy,
		       bool ranking);

  /**
   * @brief
   *
   * @details
   */
  ~synth_preprocessingt();

  /**
   * @brief Provides the minimum solution size.
   *
   * @details Properties like the number of skolem choices dictate the minimum
   * solution size.
   */
  size_t get_min_solution_size() const;

  /**
   * @brief
   *
   * @details
   */
  void operator()();

  /**
   * @brief
   *
   * @details
   *
   * @param max_solution_length
   */
  void operator()(const size_t max_solution_length);

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  const synth_programt &get_synth_program() const;
};

} 
#endif /* CEGIS_SYNTH_PREPROCESSING_H_ */
