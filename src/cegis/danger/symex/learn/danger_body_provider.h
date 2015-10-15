/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_DANGER_BODY_PROVIDER_H_
#define CEGIS_DANGER_BODY_PROVIDER_H_

/**
 * @brief
 *
 * @details
 */
class danger_body_providert
{
  const class danger_programt &prog;
public:
  /**
   * @brief
   *
   * @details
   *
   * @param prog
   */
  danger_body_providert(const danger_programt &prog);

  /**
   * @brief
   *
   * @details
   */
  ~danger_body_providert();

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  const class goto_programt &operator()() const;
};

#endif /* CEGIS_DANGER_BODY_PROVIDER_H_ */
