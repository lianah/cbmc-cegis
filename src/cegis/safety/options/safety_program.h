/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SAFETY_PROGRAM_H_
#define CEGIS_SAFETY_PROGRAM_H_

/**
 * @brief
 *
 * @details
 */
class safety_programt
{
public:
  /**
   * @brief
   *
   * @details
   */
  safety_programt();

  /**
   * @brief
   *
   * @details
   *
   * @param other
   */
  safety_programt(const class danger_programt &other);

  /**
   * @brief
   *
   * @details
   *
   * @param st
   * @param gf
   */
  safety_programt(const class symbol_tablet &st, const class goto_functionst &gf);

  /**
   * @brief
   *
   * @details
   */
  ~safety_programt();
};

#endif /* CEGIS_SAFETY_PROGRAM_H_ */
