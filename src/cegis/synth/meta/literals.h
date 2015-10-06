/*******************************************************************

 Module: Counterexample-Guided Inductive Synthesis

 Author: Daniel Kroening, kroening@kroening.com
         Pascal Kesseli, pascal.kesseil@cs.ox.ac.uk

\*******************************************************************/


#ifndef CEGIS_SYNTH_LITERALS_H_
#define CEGIS_SYNTH_LITERALS_H_

namespace Synth {

#define SYNTH_PREFIX "__CPROVER_synth_"
#define SYNTH_MODULE "<builtin-library-synth>"

#define SYNTH_EXECUTE "__CPROVER_synth_execute"
#define SYNTH_INSTRUCTION_TYPE_NAME "tag-__CPROVER_synth_instructiont"
#define SYNTH_OPS "__CPROVER_synth_OPS"
#define SYNTH_RESULT_OPS "__CPROVER_synth_RESULT_OPS"
#define SYNTH_TMP_PREFIX SYNTH_PREFIX"tmp_"
#define PRIME_SUFFIX "_prime"

}  
#endif /* CEGIS_SYNTH_LITERALS_H_ */
