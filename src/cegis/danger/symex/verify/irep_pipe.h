/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_IREP_PIPE_H_
#define CEGIS_IREP_PIPE_H_

/**
 * @brief
 *
 * @details
 */
class irep_pipet
{
  int fd[2u];
  bool read_closed;
  bool write_closed;
  bool close_on_destuction;
public:
  /**
   * @brief
   *
   * @details
   */
  irep_pipet();

  /**
   * @brief
   *
   * @details
   */
  ~irep_pipet();

  /**
   * @brief
   *
   * @details
   */
  void close_read();

  /**
   * @brief
   *
   * @details
   */
  void close_write();

  /**
   * @brief
   *
   * @details
   *
   * @param sdu
   */
  void send(const class irept &sdu) const;

  /**
   * @brief
   *
   * @details
   *
   * @param sdu
   */
  void receive(irept &sdu) const;

  /**
   * @brief
   *
   * @details
   */
  void auto_close();
};

#endif /* CEGIS_IREP_PIPE_H_ */
