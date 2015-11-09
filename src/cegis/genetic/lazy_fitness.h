/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_GENETIC_SYMEX_FITNESS_H_
#define CEGIS_GENETIC_SYMEX_FITNESS_H_

#include <cegis/value/program_individual.h>
#include <util/expr.h>

/**
 * @brief
 *
 * @details
 */
template<class test_runnert>
class lazy_fitnesst
{
public:
  typedef std::map<const irep_idt, exprt> counterexamplet;
  typedef std::deque<counterexamplet> counterexamplest;
  typedef program_populationt populationt;
  typedef program_individualt individualt;
private:
  test_runnert &test_runner;
  counterexamplest counterexamples;
  std::map<individualt *, size_t> executed_test_cases;

  populationt::iterator find_candidate(populationt &pop);
public:
  /**
   * @brief
   *
   * @details
   *
   * @param test_runner
   */
  lazy_fitnesst(test_runnert &);

  /**
   * @brief
   *
   * @details
   */
  ~lazy_fitnesst();

  /**
   * @brief
   *
   * @details
   *
   * @param seed
   * @tparam seedt
   */
  template<class seedt>
  void seed(seedt &seed);

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
  populationt::iterator init(populationt &pop);

  /**
   * @brief
   *
   * @details
   *
   * @param individual
   */
  void set_fitness(individualt &individual, bool debug=false); // XXX: Debug

  /**
   * @brief
   *
   * @details
   */
  individualt::fitnesst get_target_fitness() const;
};

#include "lazy_fitness.inc"

#endif /* CEGIS_GENETIC_SYMEX_FITNESS_H_ */
