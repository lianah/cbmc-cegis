/*******************************************************************

Module: Counterexample-Guided Inductive Synthesis

Author: Daniel Kroening, kroening@kroening.com
        Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/

#ifndef CEGIS_SYNTH_INSERT_CANDIDATE_H_
#define CEGIS_SYNTH_INSERT_CANDIDATE_H_

namespace Synth {
/**
 * @brief Candidate solution type for this CEGIS component.
 *
 * @details Solutions are provided as a set of GOTO function bodies
 * (goto_programt::instructionst) for function names.
 */
typedef class synth_goto_solutiont candidatet;

/**
 * @brief
 *
 * @details
 *
 * @param program
 * @param candidate
 */
void synth_insert_candidate(synth_programt &program,
    const candidatet &candidate);
}
#endif /* CEGIS_SYNTH_INSERT_CANDIDATE_H_ */
