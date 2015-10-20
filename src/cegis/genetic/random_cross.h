/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_GENETIC_RANDOM_CROSS_H_
#define CEGIS_GENETIC_RANDOM_CROSS_H_

#include <cegis/genetic/individual.h>

/**
 * @brief
 *
 * @details
 */
class random_crosst
{
public:
  typedef program_populationt populationt;
  typedef std::deque<populationt::iterator> individualst;

  /**
   * @brief
   *
   * @details
   */
  random_crosst();

  /**
   * @brief
   *
   * @details
   */
  ~random_crosst();

  /**
   * @brief
   *
   * @details
   *
   * @param size
   */
  void set_max_solution_size(size_t size);

  /**
   * @brief
   *
   * @details
   *
   * @param couple
   */
  void operator()(const individualst &couple);
};

#endif /* CEGIS_GENETIC_RANDOM_CROSS_H_ */
