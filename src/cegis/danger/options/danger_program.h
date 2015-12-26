/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_DANGER_PROGRAM_H_
#define CEGIS_DANGER_PROGRAM_H_

#include <cegis/invariant/options/invariant_program.h>

/**
 * @brief
 *
 * @details
 */
class danger_programt: public invariant_programt
{
public:
  /**
   * @brief
   *
   * @details
   */
  struct danger_meta_vars_positionst
  {
    goto_programt::targetst Rx;
    goto_programt::targetst Sx;
    goto_programt::targetst Rx_prime;
  };

  /**
   * @brief
   *
   * @details
   */
  struct loopt: public invariant_loopt
  {
    goto_programt::targetst skolem_choices;
    danger_meta_vars_positionst danger_meta_variables;
  };
  typedef std::vector<loopt> loopst;

  loopst loops;
  goto_programt::targetst x0_choices;

  /**
   * @brief
   *
   * @details
   */
  danger_programt();

  /**
   * @brief
   *
   * @details
   *
   * @param other
   */
  danger_programt(const danger_programt &other);

  /**
   * @brief
   *
   * @details
   *
   * @param st
   * @param gf
   */
  danger_programt(const symbol_tablet &st, const goto_functionst &gf);

  /**
   * @brief
   *
   * @details
   */
  ~danger_programt();

  /**
   * @brief
   *
   * @details
   *
   * @param other
   *
   * @return
   */
  danger_programt &operator=(const danger_programt &other);
};

#endif /* CEGIS_DANGER_PROGRAM_H_ */
