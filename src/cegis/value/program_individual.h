/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_GENETIC_INDIVIDUAL_H_
#define CEGIS_GENETIC_INDIVIDUAL_H_

#include <cstddef>
#include <deque>
#include <vector>

/**
 * @brief
 *
 * @details
 */
class program_individualt
{
public:
  /**
   * @brief
   *
   * @details
   */
  class instructiont
  {
  public:
    typedef unsigned char opcodet;
    typedef unsigned char opt;
    typedef std::vector<opt> opst;

    opcodet opcode;
    opst ops;

    /*instructiont();
    instructiont(const instructiont &other);
    ~instructiont();
    instructiont &operator=(const instructiont &other);*/

    /**
     * @brief
     *
     * @details
     *
     * @param n
     *
     * @return
     */
    opst::value_type &operator[](opst::size_type n);

    /**
     * @brief
     *
     * @details
     *
     * @param n
     *
     * @return
     */
    const opst::value_type &operator[](opst::size_type n) const;
  };

  typedef std::vector<instructiont> programt;
  typedef std::vector<programt> programst;
  typedef std::vector<unsigned int> nondet_choices;
  typedef size_t fitnesst;

  programst programs;
  nondet_choices x0;
  fitnesst fitness;
};

typedef std::deque<program_individualt> program_populationt;

#endif /* CEGIS_GENETIC_INDIVIDUAL_H_ */
