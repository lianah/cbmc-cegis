/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
 Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

 \*******************************************************************/

#ifndef CEGIS_GENETIC_TOURNAMENT_SELECT_H_
#define CEGIS_GENETIC_TOURNAMENT_SELECT_H_

#include <array>
#include <deque>
#include <map>

#include <cegis/genetic/individual.h>
#include <cegis/genetic/instruction_set_info_factory.h>

#define CEGIS_TOURNAMENT_POPULATION_SIZE 2000

/**
 * @brief
 *
 * @details
 */
class tournament_selectt
{
private:
  instruction_set_info_factoryt info_factory;
  const size_t num_progs;
  const size_t max_prog_size;
  std::function<size_t(void)> num_vars;
  const size_t rounds;
public:
  typedef std::array<program_individualt, CEGIS_TOURNAMENT_POPULATION_SIZE> populationt;
  typedef std::deque<populationt::iterator> selectiont;

  /**
   * @brief
   *
   * @details
   *
   * @param instruction_set_info
   * @param num_progs
   * @param max_prog_size
   * @param num_vars
   * @param rounds
   */
  tournament_selectt(const instruction_set_info_factoryt &instruction_set_info,
      size_t num_progs, size_t max_prog_size,
      const std::function<size_t(void)> &num_vars, size_t rounds);

  /**
   * @brief
   *
   * @details
   */
  ~tournament_selectt();

  /**
   * @brief
   *
   * @details
   *
   * @param population
   */
  void init(populationt &population);

  /**
   * @brief
   *
   * @details
   *
   * @param population
   *
   * @return
   */
  selectiont select(populationt &population);
};

#endif /* CEGIS_GENETIC_TOURNAMENT_SELECT_H_ */
