/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_GOTO_SOLUTION_H_
#define CEGIS_SYNTH_GOTO_SOLUTION_H_

#include <goto-programs/goto_program.h>

namespace Synth {
/**
 * @brief
 *
 * @details
 */
struct synth_goto_solutiont
{
  /**
   * @brief
   *
   * @details
   */
  struct synth_programt
  {
    goto_programt::instructionst invariant;
    goto_programt::instructionst ranking;
  };
  typedef std::vector<synth_programt> synth_programst;
  synth_programst synth_programs;
  
  // LSH FIXME: what are the non-det choices again? just so you can reverse engineer the solution
  // and know what was universally quantified
  /* typedef std::vector<exprt> nondet_choicest; */
  /* nondet_choicest x0_choices; */
};
}

#endif /* CEGIS_SYNTH_GOTO_SOLUTION_H_ */
