/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_INVARIANT_PROGRAM_H_
#define CEGIS_INVARIANT_PROGRAM_H_

#include <goto-programs/goto_functions.h>

/**
 * @brief
 *
 * @details
 */
class invariant_programt
{
public:
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
    goto_programt::targett Ix;
    goto_programt::targett Gx;
    goto_programt::targett Ix_prime;
  };

  /**
   * @brief
   *
   * @details
   */
  struct invariant_loopt
  {
    exprt guard;
    program_ranget body;
    meta_vars_positionst meta_variables;
  };

  symbol_tablet st;
  goto_functionst gf;
  /**
   * @brief The range in the program relevant for invariant analysis.
   *
   * @details Spans from the original beginning of the analysed program up to the
   * assertion.
   */
  program_ranget invariant_range;
  exprt assertion;
  goto_programt::targett Ix0;
  goto_programt::targett Ax;

  /**
   * @brief
   *
   * @details
   */
  invariant_programt();

  /**
   * @brief
   *
   * @details
   *
   * @param st
   * @param gf
   */
  invariant_programt(const symbol_tablet &st, const goto_functionst &gf);

  /**
   * @brief
   *
   * @details
   *
   * @param other
   */
  invariant_programt(const invariant_programt &other);

  /**
   * @brief
   *
   * @details
   */
  ~invariant_programt();

  /**
   * @brief
   *
   * @details
   *
   * @param other
   *
   * @return
   */
  invariant_programt &operator=(const invariant_programt &other);
};

#endif /* CEGIS_INVARIANT_PROGRAM_H_ */
