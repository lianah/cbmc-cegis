/* FUNCTION: __CPROVER_synth_execute */

// why void* and not just unsigned int* ?
const void *__CPROVER_synth_OPS[__CPROVER_synth_number_of_ops];
void *__CPROVER_synth_RESULT_OPS[__CPROVER_synth_max_solution_size];

typedef unsigned char synth_opcodet;
typedef unsigned char synth_opt;
struct __CPROVER_synth_instructiont
{
  synth_opcodet opcode;
  synth_opt op0;
  synth_opt op1;
  synth_opt op2;
};

#define __CPROVER_synth_max_instruction 5u
#define __CPROVER_synth_commutative 3u


void __CPROVER_synth_execute(struct __CPROVER_synth_instructiont *program,
                              unsigned char size)
{
  for (unsigned char i = 0; i < size; ++i)
  {
    // Why use define instead of assignment to local variable?    
    synth_opcodet synth_opcode = program[i].opcode;
    __CPROVER_assume(synth_opcode <= __CPROVER_synth_max_instruction);
    synth_opt synth_op0_id = program[i].op0;
    synth_opt synth_op1_id = program[i].op1;
    synth_opt synth_op2_id = program[i].op2;

    // __CPROVER_synth_number_of_vars is number of input and output vars
    // and does not include temporaries due to SSA
    unsigned char synth_max_op_index = __CPROVER_synth_number_of_vars + i;

    __CPROVER_assume(synth_op0_id < synth_max_op_index &&
		     synth_op1_id < synth_max_op_index &&
		     // SSA use only previous temp results
		     (synth_op0_id >= __CPROVER_synth_number_of_consts ||
		      synth_op1_id >= __CPROVER_synth_number_of_consts) &&
		     // All operands cannot be constants
                     (synth_opcode >= __CPROVER_synth_commutative || synth_op0_id <= synth_op1_id) &&
                     (synth_op2_id == 0)
		     );

    // Pointers because in the learn goto program you assign the reference to the local variables
    const unsigned int * const op0_ptr=__CPROVER_synth_OPS[synth_op0_id];
    const unsigned int * const op1_ptr=__CPROVER_synth_OPS[synth_op1_id];
    const unsigned int * const op2_ptr=__CPROVER_synth_OPS[synth_op2_id];
    __CPROVER_assume(op0_ptr && op1_ptr && op2_ptr);  // No null pointers in op array

    const unsigned int op0=*op0_ptr;
    const unsigned int op1=*op1_ptr;
    const unsigned int op2=*op2_ptr;

  #define synth_sop0 ((int) op0)
  #define synth_sop1 ((int) op1)


   unsigned int result;
   if (synth_opcode == 0) {
      __CPROVER_synth_opcode_0: result=op0 + op1; 
   } else if (synth_opcode == 1) {
      __CPROVER_synth_opcode_1: result= op0 == op1;
   } else if (synth_opcode == 2) {
      __CPROVER_synth_opcode_2: result= op0 * op1;
   } else if (synth_opcode == 3) {
      __CPROVER_synth_opcode_3: result=synth_sop0 < synth_sop1;
   } else if (synth_opcode == 4) {
      __CPROVER_synth_opcode_4: result=synth_sop0 <= synth_sop1;
   } else {
      __CPROVER_synth_opcode_5: result=op0 - op1;
   }  

   *(unsigned int *)__CPROVER_synth_RESULT_OPS[i]=result;
  }
}
