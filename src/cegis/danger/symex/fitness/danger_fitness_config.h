/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_DANGER_FITNESS_CONFIG_H_
#define CEGIS_DANGER_FITNESS_CONFIG_H_

#include <cegis/danger/options/danger_program.h>

/**
 * @brief
 *
 * @details
 */
class danger_fitness_configt
{
  const danger_programt &original_program;
  danger_programt program_with_constraint;
  danger_programt program;
  goto_programt::targetst quantifiers;
  bool constraint_inserted;
  bool program_contains_ce;
public:
  typedef class danger_goto_solutiont candidatet;
  typedef std::map<const irep_idt, exprt> counterexamplet;
  typedef class program_individualt individualt;

  /**
   * @brief
   *
   * @details
   *
   * @param prog
   */
  danger_fitness_configt(const danger_programt &prog);

  /**
   * @brief
   *
   * @details
   */
  ~danger_fitness_configt();

  /**
   * @brief
   *
   * @details
   *
   * @param candidate
   */
  void set_candidate(const candidatet &candidate);

  /**
   * @brief
   *
   * @details
   *
   * @param ce
   */
  void set_test_case(const counterexamplet &ce);

  /**
   * @brief
   *
   * @details
   *
   * @param current_candidate
   * @param ind
   */
  void convert(candidatet &current_candidate, const individualt &ind) const;

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  const class symbol_tablet &get_symbol_table() const;

  /**
   * @brief
   *
   * @details
   *
   * @return
   */
  const class goto_functionst &get_goto_functions() const;
};

#endif /* CEGIS_DANGER_FITNESS_CONFIG_H_ */
