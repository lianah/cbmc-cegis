/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_PROGRAM_H_
#define CEGIS_SYNTH_PROGRAM_H_

#include <goto-programs/goto_functions.h>

namespace Synth {
/**
 * @brief
 *
 * @details
 */
struct synth_programt
{
  /**
   * @brief
   *
   * @details
   */
  struct program_ranget
  {
    goto_programt::targett begin;
    goto_programt::targett end;
  };

  /**
   * @brief
   *
   * @details
   */
  struct meta_vars_positionst
  {
    goto_programt::targett Ix; // invariant
    goto_programt::targett Ix_prime;
    
    goto_programt::targett Gx; // loop guard

    goto_programt::targetst Rx;// ranking 
    goto_programt::targetst Rx_prime;
  };

  /**
   * @brief
   *
   * @details
   */
  struct loopt
  {
    exprt guard;
    program_ranget body;
    // goto_programt::targetst skolem_choices;
    meta_vars_positionst meta_variables;
  };
  typedef std::vector<loopt> loopst;

  symbol_tablet st;
  goto_functionst gf;
  loopst loops;
  goto_programt::targetst x0_choices; // do we need this?
  /**
   * @brief The range in the program relevant for synth analysis.
   *
   * @details Spans from the original beginning of the analysed program up to the
   * synth assertion.
   */
  program_ranget synth_range;
  exprt assertion;
  goto_programt::targett Ix0; 
  goto_programt::targett Ax;

  bool synth_ranking;

  /**
   * @brief
   *
   * @details
   */
  synth_programt();

  /**
   * @brief
   *
   * @details
   *
   * @param other
   */
  synth_programt(const synth_programt &other);

  /**
   * @brief
   *
   * @details
   *
   * @param st
   * @param gf
   */
  synth_programt(const symbol_tablet &st,
		 const goto_functionst &gf,
		 bool ranking);

  /**
   * @brief
   *
   * @details
   */
  ~synth_programt();

  /**
   * @brief
   *
   * @details
   *
   * @param st
   * @param gf
   */
  void set_goto_program(const symbol_tablet &st, const goto_functionst &gf);

  /**
   * @brief
   *
   * @details
   *
   * @param other
   *
   * @return
   */
  synth_programt &operator=(const synth_programt &other);

  void print() const;
};

} 
#endif /* CEGIS_SYNTH_PROGRAM_H_ */
