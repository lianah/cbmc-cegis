/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_GENETIC_RANDOM_INDIVIDUAL_H_
#define CEGIS_GENETIC_RANDOM_INDIVIDUAL_H_

#include <util/type.h>

#include <cegis/genetic/individual.h>
#include <cegis/genetic/instruction_set_info_factory.h>

/**
 * @brief
 *
 * @details
 */
class random_individualt
{
  const typet type;
  instruction_set_info_factoryt info_factory;
  const std::function<size_t(void)> num_vars;
  const std::function<size_t(void)> num_x0;
  size_t num_progs;
  size_t prog_size_limit;
  size_t vars_limit;
  size_t x0_limit;
  size_t opcode_limit;
public:
  /**
   * @brief
   *
   * @details
   *
   * @param seed
   * @param type
   * @param instruction_set_info_factory
   * @param num_progs
   * @param num_vars
   * @param num_x0
   */
  random_individualt(unsigned int seed, const typet &type,
      const instruction_set_info_factoryt &info_factory, size_t num_progs,
      const std::function<size_t(void)> &num_vars,
      const std::function<size_t(void)> &num_x0);

  /**
   * @brief
   *
   * @details
   */
  ~random_individualt();

  /**
   * @brief
   *
   * @details
   *
   * @param size
   */
  void set_max_prog_size(size_t size);

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  program_individualt::programt::size_type prog_size() const;

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  program_individualt::instructiont::opcodet opcode() const;

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  program_individualt::instructiont::opt op() const;

  /**
   * @brief
   *
   * @details
   *
   * @param instr
   */
  void havoc(program_individualt::instructiont &instr);

  /**
   * @brief
   *
   * @details
   *
   * @param prog
   */
  void havoc(program_individualt::programt &prog);

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  program_individualt::nondet_choices::value_type x0() const;

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  program_individualt::nondet_choices::value_type constant() const;

  /**
   * @brief
   *
   * @details
   *
   * @param ind
   */
  void havoc(program_individualt &ind);

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  unsigned int rand() const;
};

#endif /* CEGIS_GENETIC_RANDOM_INDIVIDUAL_H_ */
