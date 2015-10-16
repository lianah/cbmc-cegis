#include <util/simplify_expr.h>

#include <cegis/util/program_helper.h>
#include <cegis/synth/options/synth_program.h>
#include <cegis/synth/preprocess/remove_loops_and_assertion.h>
#include <iostream>
using namespace Synth;

namespace
{
bool handle_assertion_removal(synth_programt &program,
    goto_programt::instructionst &instrs, const goto_programt::targett &target)
{
  const goto_programt::instructiont &instr=*target;
  if (goto_program_instruction_typet::ASSERT != instr.type) return false;
  const namespacet ns(program.st);
  assert(program.assertion.id().empty());
  program.assertion=instr.guard;
  goto_programt::targett &end=program.synth_range.end;
  end=target;
  --end;
  goto_programt::targett &last_loop_end=program.loops.back().body.end;
  const bool is_last_loop_end=last_loop_end == target;
  instrs.erase(target);
  ++end;
  if (is_last_loop_end) last_loop_end=end;
  return true;
}

void handle_loop_removal(synth_programt &program,
    goto_programt::instructionst &instrs, goto_programt::targett &target)
{
  const goto_programt::instructiont &instr=*target;
  if (!instr.is_backwards_goto()) return;
  const namespacet ns(program.st);
  const goto_programt::targett goto_target=instr.get_target();
  synth_programt::loopt loop;
  if (instr.guard.is_true())
  {
    // LSH FIXME: it could be the case that the goto is pointing to a declaration actually
    // e.g. while (nondet())
    // problem is assignment in loop condition (may arise in for loops to?)
    exprt guard=goto_target->guard;
    if (ID_not == guard.id()) loop.guard=to_not_expr(guard).op();
    else loop.guard=simplify_expr(not_exprt(guard), ns);
    loop.body.begin=goto_target;
    ++loop.body.begin;
    instrs.erase(goto_target);
  } else
  {
    loop.guard=simplify_expr(instr.guard, ns);
    loop.body.begin=goto_target;
  }
  assert(!loop.guard.id().empty());
  loop.body.end=target;
  --loop.body.end;
  instrs.erase(target--);
  ++loop.body.end;
  program.loops.push_back(loop);
}
}

void Synth::synth_remove_loops_and_assertion(Synth::synth_programt &program)
{
  goto_programt &body=get_synth_body(program.gf);
  goto_programt::instructionst &instrs=body.instructions;
  program.synth_range.begin=instrs.begin();
  for (goto_programt::targett it=instrs.begin(); it != instrs.end(); ++it)
  {
    if (handle_assertion_removal(program, instrs, it)) break;
    handle_loop_removal(program, instrs, it);
  }
}
