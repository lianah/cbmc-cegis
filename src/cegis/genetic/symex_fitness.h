/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_GENETIC_SYMEX_FITNESS_H_
#define CEGIS_GENETIC_SYMEX_FITNESS_H_

#include <cegis/genetic/individual.h>
#include <util/expr.h>

/**
 * @brief
 *
 * @details
 */
class symex_fitnesst
{
public:
  typedef std::map<const irep_idt, exprt> counterexamplet;
  typedef std::deque<counterexamplet> counterexamplest;
private:
  counterexamplest counterexamples;
public:
  typedef program_individualt individualt;
  typedef program_populationt populationt;

  /**
   * @brief
   *
   * @details
   */
  symex_fitnesst();

  /**
   * @brief
   *
   * @details
   */
  ~symex_fitnesst();

  /**
   * @brief
   *
   * @details
   *
   * @param seed
   * @tparam seedt
   */
  template<class seedt>
  void seed(seedt &seed) const;

  /**
   * @brief
   *
   * @details
   *
   * @param ce
   */
  void add_test_case(const counterexamplet &ce);

  /**
   * @brief
   *
   * @details
   *
   * @param pop
   */
  populationt::const_iterator init(populationt &pop) const;

  /**
   * @brief
   *
   * @details
   *
   * @param individual
   */
  void set_fitness(individualt &individual) const;

  /**
   * @brief
   *
   * @details
   */
  individualt::fitnesst get_target_fitness() const;
};

#include "symex_fitness.inc"

#endif /* CEGIS_GENETIC_SYMEX_FITNESS_H_ */
