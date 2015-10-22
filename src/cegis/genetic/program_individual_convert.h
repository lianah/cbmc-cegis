/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_GENETIC_PROGRAM_INDIVIDUAL_CONVERT_H_
#define CEGIS_GENETIC_PROGRAM_INDIVIDUAL_CONVERT_H_

/**
 * @brief
 *
 * @details
 */
class program_individual_convertt
{
public:
  typedef class danger_goto_solutiont candidatet;
  typedef class program_individualt individualt;

  /**
   * @brief
   *
   * @details
   */
  program_individual_convertt();

  /**
   * @brief
   *
   * @details
   */
  ~program_individual_convertt();

  /**
   * @brief
   *
   * @details
   *
   * @param candidate
   * @param ind
   */
  void convert(candidatet &candidate, const individualt &ind);
};

#endif /* CEGIS_GENETIC_PROGRAM_INDIVIDUAL_CONVERT_H_ */
