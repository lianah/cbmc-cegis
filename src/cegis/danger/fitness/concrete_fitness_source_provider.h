/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_CONCRETE_FITNESS_SOURCE_PROVIDER_H_
#define CEGIS_CONCRETE_FITNESS_SOURCE_PROVIDER_H_

#include <cegis/danger/symex/learn/danger_learn_config.h>

/**
 * @brief
 *
 * @details
 */
class concrete_fitness_source_providert
{
private:
  danger_learn_configt learn_config;
public:
  /**
   * @brief
   *
   * @details
   *
   * @param prog
   */
  concrete_fitness_source_providert(const danger_programt &prog);

  /**
   * @brief
   *
   * @details
   */
  ~concrete_fitness_source_providert();

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  std::string operator()();
};

#endif /* CEGIS_CONCRETE_FITNESS_SOURCE_PROVIDER_H_ */
