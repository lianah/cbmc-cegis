#include <cegis/invariant/util/invariant_program_helper.h>
#include <cegis/invariant/symex/verify/insert_program.h>
#include <cegis/safety/meta/meta_variable_names.h>
#include <cegis/safety/options/safety_program.h>
#include <cegis/safety/symex/verify/insert_candidate.h>

void safety_insert_candidate(safety_programt &program,
    const safety_goto_solutiont &candidate)
{
  const safety_programt::safety_loopst &loops=program.safety_loops;
  const size_t size=loops.size();
  assert(size == candidate.size());
  goto_programt &body=get_entry_body(program.gf);
  for (size_t i=0; i < size; ++i)
  {
    const invariant_programt::invariant_loopt &loop=loops[i];
    const goto_programt::instructionst &prog=candidate[i];
    const invariant_programt::meta_vars_positionst &meta=loop.meta_variables;
    insert_program(body, meta.Ix, prog);
    insert_program(body, meta.Ix_prime, prog, get_Ix(i), get_Ix_prime(i));
  }
}
