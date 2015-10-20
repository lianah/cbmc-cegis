/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
 Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

 \*******************************************************************/

#ifndef CEGIS_GENETIC_TOURNAMENT_SELECT_H_
#define CEGIS_GENETIC_TOURNAMENT_SELECT_H_

#include <map>

#include <cegis/genetic/individual.h>
#include <cegis/genetic/instruction_set_info_factory.h>

/**
 * @brief
 *
 * @details
 */
class tournament_selectt
{
private:
  instruction_set_info_factoryt info_factory;
  const size_t pop_size;
  const size_t num_progs;
  const std::function<size_t(void)> initial_max_prog_size;
  const std::function<size_t(void)> num_vars;
  const std::function<size_t(void)> num_x0;
  const size_t rounds;
  const typet type;
public:
  typedef program_populationt populationt;
  typedef populationt::value_type individualt;
  typedef std::deque<populationt::iterator> couplet;
  typedef std::deque<couplet> couplest;
  class selectiont
  {
  public:
    couplest couples;

    bool can_mutate() const;
    bool can_cross() const;
    populationt::value_type &mutation_lhs();
    const populationt::value_type &mutation_rhs() const;
    const couplest &get_couples() const;
    void push_back(const populationt::iterator &individual);
  };

  /**
   * @brief
   *
   * @details
   *
   * @param instruction_set_info
   * @param pop_size
   * @param num_progs
   * @param initial_max_prog_size
   * @param num_vars
   * @param num_x0
   * @param rounds
   */
  tournament_selectt(const instruction_set_info_factoryt &instruction_set_info,
      size_t pop_size, size_t num_progs,
      const std::function<size_t(void)> &initial_max_prog_size,
      const std::function<size_t(void)> &num_vars,
      const std::function<size_t(void)> &num_x0, size_t rounds,
      const typet &type);

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
