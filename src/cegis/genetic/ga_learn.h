/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_GENETIC_GA_LEARN_H_
#define CEGIS_GENETIC_GA_LEARN_H_

#include <cegis/danger/value/danger_goto_solution.h>

/**
 * @brief
 *
 * @details
 */
template<class selectt, class mutatet, class crosst, class fitnesst, class convertt>
class ga_learnt
{
public:
  typedef typename convertt::candidatet candidatet;
  typedef typename fitnesst::counterexamplet counterexamplet;
  typedef typename selectt::populationt populationt;
  typedef typename selectt::selectiont selectiont;
private:
  const class optionst &options;
  selectt &select;
  mutatet &mutate;
  crosst &cross;
  fitnesst &fitness;
  convertt &convert;
  populationt pop;
  selectiont selection;
  candidatet current_candidate;
  bool is_population_initialised;

  bool set_fitness(typename selectt::individualt &ind);
public:
  /**
   * @brief
   *
   * @details
   *
   * @param options
   * @param select
   * @param mutate
   * @param cross
   * @param fitness
   * @param convert
   */
  ga_learnt(const optionst &options, selectt &select, mutatet &mutate,
      crosst &cross, fitnesst &fitness, convertt &convert);

  /**
   * @brief
   *
   * @details
   */
  ~ga_learnt();


  /**
   * @brief
   *
   * @details
   *
   * @param seed
   */
  template<class seedt>
  void seed(seedt &seed);

  /**
   * @brief Provides the next candidate.
   *
   * @details Provides the last candidate generated using learn.
   *
   * @return The next candidate.
   */
  const candidatet &next_candidate() const;

  /**
   * @brief Generates a candidate solution.
   *
   * @details Generates a new candidate based on previously received counterexamples.
   * This operation is useful when the set of counterexamples remains the same and only
   * the maximum solution size has changed.
   *
   * @param max_solution_size The new maximum solution size.
   *
   * @return <code>true</code> if learning was successful, <code>false</code>
   * if no new candidate could be generated.
   */
  bool learn(size_t max_solution_size);

  /**
   * @brief Generates a candidate solution.
   *
   * @details Receives set of counterexample from the verification oracle
   * and adds it to its information base. Generates a new candidate
   * based on received counterexamples.
   *
   * @param first The first iterator of the counterexample set.
   * @param last The last iterator of the counterexample set.
   *
   * @return <code>true</code> if learning was successful, <code>false</code>
   * if no new candidate could be generated.
   */
  template<class itert>
  bool learn(itert first, const itert &last);

  /**
   * @brief Displays the last candidate.
   *
   * @details Prints the last candidate generated using learn.
   *
   * @param os The stream to output the candidate.
   */
  void show_candidate(messaget::mstreamt &os) const;
};

#include "ga_learn.inc"

#endif /* CEGIS_GENETIC_GA_LEARN_H_ */
