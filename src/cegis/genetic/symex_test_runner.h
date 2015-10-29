/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_GENETIC_SYMEX_TEST_RUNNER_H_
#define CEGIS_GENETIC_SYMEX_TEST_RUNNER_H_

#include <cegis/value/program_individual.h>
#include <util/expr.h>

template<class configt>
class symex_test_runnert
{
public:
  typedef std::map<const irep_idt, exprt> counterexamplet;
  typedef program_individualt individualt;
private:
  class bool_pipet
  {
    int fd[2u];
    individualt *individual;
  public:
    bool_pipet(individualt *individual);
    pid_t child_pid;
    void run_test(const class optionst &options, configt &config,
        const counterexamplet &ce);
    void join();
  };
  typedef std::deque<bool_pipet> taskst;
  taskst tasks;
  const optionst &options;
  configt &config;

  void cleanup();
public:
  /**
   * @brief
   *
   * @details
   *
   * @param options
   * @param config
   */
  symex_test_runnert(const optionst &options, configt &config);

  /**
   * @brief
   *
   * @details
   */
  ~symex_test_runnert();

  /**
   * @brief
   *
   * @details
   *
   * @param ind
   * @param ce
   */
  void run_test(individualt &ind, const counterexamplet &ce);

  /**
   * @brief
   *
   * @details
   */
  void join();

  /**
   * @brief
   *
   * @details
   */
  void abandon();
};

#include "symex_test_runner.inc"

#endif /* CEGIS_GENETIC_SYMEX_TEST_RUNNER_H_ */
