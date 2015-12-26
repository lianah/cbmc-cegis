#include <cegis/invariant/util/invariant_program_helper.h>
#include <cegis/invariant/options/target_copy_helper.h>
#include <cegis/invariant/options/invariant_program.h>

invariant_programt::invariant_programt()
{
}

invariant_programt::invariant_programt(const symbol_tablet &st,
    const goto_functionst &gf) :
    st(st)
{
  this->gf.copy_from(gf);
}

namespace
{
invariant_programt &assign(invariant_programt &lhs,
    const invariant_programt &rhs)
{
  const target_copy_helpert fix(get_entry_body(rhs.gf), get_entry_body(lhs.gf));
  lhs.invariant_range=fix(rhs.invariant_range);
  lhs.assertion=rhs.assertion;
  lhs.Ix0=fix(rhs.Ix0);
  lhs.Ax=fix(rhs.Ax);
  return lhs;
}
}

invariant_programt::invariant_programt(const invariant_programt &other) :
    st(other.st)
{
  gf.copy_from(other.gf);
  assign(*this, other);
}

invariant_programt &invariant_programt::operator =(
    const invariant_programt &other)
{
  st=other.st;
  gf.clear();
  gf.copy_from(other.gf);
  return assign(*this, other);
}

invariant_programt::~invariant_programt()
{
}
