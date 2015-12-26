#include <algorithm>

#include <cegis/invariant/options/target_copy_helper.h>
#include <cegis/invariant/util/invariant_program_helper.h>
#include <cegis/danger/options/danger_program.h>

danger_programt::danger_programt()
{
}

danger_programt::danger_programt(const symbol_tablet &st,
    const goto_functionst &gf) :
    invariant_programt(st, gf)
{
}

danger_programt::~danger_programt()
{
}

namespace
{
class copy_targett
{
  const target_copy_helpert fix;
public:
  copy_targett(const goto_functionst &old_gf, goto_functionst &new_gf) :
      fix(get_entry_body(old_gf), get_entry_body(new_gf))
  {
  }
  goto_programt::targett operator()(const goto_programt::targett &target) const
  {
    return fix(target);
  }
  danger_programt::danger_meta_vars_positionst operator()(
      const danger_programt::danger_meta_vars_positionst &vars) const
  {
    danger_programt::danger_meta_vars_positionst result;
    const goto_programt::targetst &old_r=vars.Rx;
    goto_programt::targetst &new_r=result.Rx;
    new_r.resize(old_r.size());
    std::transform(old_r.begin(), old_r.end(), new_r.begin(), fix);
    const goto_programt::targetst &old_s=vars.Sx;
    goto_programt::targetst &new_s=result.Sx;
    new_s.resize(old_s.size());
    std::transform(old_s.begin(), old_s.end(), new_s.begin(), fix);
    const goto_programt::targetst &old_rp=vars.Rx_prime;
    goto_programt::targetst &new_rp=result.Rx_prime;
    new_rp.resize(old_rp.size());
    std::transform(old_rp.begin(), old_rp.end(), new_rp.begin(), fix);
    return result;
  }
  danger_programt::loopt operator()(const danger_programt::loopt &loop) const
  {
    danger_programt::loopt result;
    fix(result, loop);
    goto_programt::targetst &new_s=result.skolem_choices;
    const goto_programt::targetst &old_s=loop.skolem_choices;
    std::transform(old_s.begin(), old_s.end(), std::back_inserter(new_s), fix);
    result.danger_meta_variables=operator()(loop.danger_meta_variables);
    return result;
  }
};

danger_programt &assign(danger_programt &lhs, const danger_programt &rhs)
{
  const copy_targett fix(rhs.gf, lhs.gf);
  const danger_programt::loopst &old_loops=rhs.loops;
  lhs.loops.resize(old_loops.size());
  std::transform(old_loops.begin(), old_loops.end(), lhs.loops.begin(), fix);
  const goto_programt::targetst &old_x0=rhs.x0_choices;
  lhs.x0_choices.resize(old_x0.size());
  std::transform(old_x0.begin(), old_x0.end(), lhs.x0_choices.begin(), fix);
  return lhs;
}
}

danger_programt::danger_programt(const danger_programt &other) :
    invariant_programt(other)
{
  assign(*this, other);
}

danger_programt &danger_programt::operator =(const danger_programt &other)
{
  invariant_programt::operator =(other);
  return assign(*this, other);
}

invariant_programt::invariant_loopst danger_programt::get_loops() const
{
  std::vector<const invariant_programt::invariant_loopt *> result(loops.size());
  std::transform(loops.begin(), loops.end(), result.begin(),
      [](const loopt &loop)
      { return &loop;});
  return result;
}
