#include <algorithm>

#include <cegis/util/program_helper.h>
#include <cegis/synth/options/synth_program.h>
#include <cegis/synth/preprocess/store_nondet_choices.h>

using namespace Synth;

// namespace
// {
// void store_skolem_choices_for_loop(synth_programt::loopt &loop)
// {
//   const synth_programt::program_ranget &range=loop.body;
//   for (goto_programt::targett it=range.begin; it != range.end; ++it)
//     if (is_nondet(it)) loop.skolem_choices.push_back(it);
// }
// }

// void Synth::store_skolem_choices(synth_programt &program)
// {
//   synth_programt::loopst &loops=program.loops;
//   std::for_each(loops.begin(), loops.end(), &store_skolem_choices_for_loop);
// }

namespace
{
void store_x0_choices_for_range(synth_programt &program,
    const goto_programt::targett &begin, const goto_programt::targett &end)
{
  for (goto_programt::targett it=begin; it != end; ++it)
    if (is_nondet(it)) program.x0_choices.push_back(it);
}
}

void Synth::store_x0_choices(synth_programt &program)
{
  goto_programt::targett begin=program.synth_range.begin;
  goto_programt::targett end;
  typedef synth_programt::loopst loopst;
  const loopst &loops=program.loops;
  for (loopst::const_iterator it=loops.begin(); it != loops.end(); ++it)
  {
    end=it->body.begin;
    store_x0_choices_for_range(program, begin, end);
    begin=it->body.end;
  }
  end=program.synth_range.end;
  store_x0_choices_for_range(program, begin, end);
}
