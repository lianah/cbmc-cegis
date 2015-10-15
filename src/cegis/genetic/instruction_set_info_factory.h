/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_GENETIC_INSTRUCTION_SET_INFO_FACTORY_H_
#define CEGIS_GENETIC_INSTRUCTION_SET_INFO_FACTORY_H_

#include <functional>
#include <cegis/genetic/tournament_select.h>

/**
 * @brief
 *
 * @details
 */
class instruction_set_info_factoryt
{
  std::function<const class goto_programt &()> body_provider;
  tournament_selectt::instruction_set_infot info;
public:
  /**
   * @brief
   *
   * @details
   *
   * @param body
   */
  instruction_set_info_factoryt(const goto_programt &body);

  /**
   * @brief
   *
   * @details
   *
   * @param provider
   * @tparam goto_program_providert
   */
  template<class goto_program_providert>
  instruction_set_info_factoryt(const goto_program_providert &provider);

  /**
   * @brief
   *
   * @details
   */
  ~instruction_set_info_factoryt();

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  const tournament_selectt::instruction_set_infot &get_info() const;
};

#include "instruction_set_info_factory.inc"

#endif /* CEGIS_GENETIC_INSTRUCTION_SET_INFO_FACTORY_H_ */
