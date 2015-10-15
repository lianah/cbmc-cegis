#include <cegis/danger/meta/literals.h>
#include <cegis/danger/options/danger_program.h>
#include <cegis/danger/symex/learn/danger_body_provider.h>

danger_body_providert::danger_body_providert(const danger_programt &prog) :
    prog(prog)
{
}

danger_body_providert::~danger_body_providert()
{
}

const goto_programt &danger_body_providert::operator ()() const
{
  const irep_idt id(DANGER_EXECUTE);
  const goto_functionst::function_mapt &function_map=prog.gf.function_map;
  const goto_functionst::function_mapt::const_iterator it=function_map.find(id);
  assert(function_map.end() != it);
  const goto_function_templatet<goto_programt> &f=it->second;
  assert(f.body_available);
  return f.body;
}
