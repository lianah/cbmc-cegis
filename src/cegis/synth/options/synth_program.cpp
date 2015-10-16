#include <algorithm>

#include <cegis/util/program_helper.h>
#include <cegis/synth/options/synth_program.h>
#include <iostream>
using namespace Synth;

Synth::synth_programt::synth_programt()
{
}

Synth::synth_programt::synth_programt(const symbol_tablet &st,
				      const goto_functionst &gf, bool rank) :
  st(st),
  synth_ranking(rank)
{
  this->gf.copy_from(gf);
}

Synth::synth_programt::~synth_programt()
{
}

void Synth::synth_programt::set_goto_program(const symbol_tablet &st,
    const goto_functionst &gf)
{
  this->st=st;
  this->gf.clear();
  this->gf.copy_from(gf);
}

namespace
{
class copy_targett
{
  const copy_targett &fix;
  const goto_programt::instructionst &old_instrs;
  goto_programt::instructionst &new_instrs;
  bool synth_ranking;

public:
  copy_targett(const goto_functionst &old_gf,
	       goto_functionst &new_gf,
	       bool ranking)
    : fix(*this)
    , old_instrs(get_synth_body(old_gf).instructions)
    , new_instrs(get_synth_body(new_gf).instructions)
    , synth_ranking(ranking)
  {
  }
  
  goto_programt::targett operator()(const goto_programt::targett &target) const
  {
    const goto_programt::targett empty;
    if (empty == target) return empty;
    const goto_programt::const_targett old_target=target;
    const size_t old_distance=std::distance(old_instrs.begin(), old_target);
    goto_programt::targett new_target=new_instrs.begin();
    std::advance(new_target, old_distance);
    return new_target;
  }
  
  Synth::synth_programt::meta_vars_positionst operator()(
      const Synth::synth_programt::meta_vars_positionst &vars) const
  {
    Synth::synth_programt::meta_vars_positionst result;
    result.Gx=fix(vars.Gx);

    result.Ix=fix(vars.Ix);
    result.Ix_prime=fix(vars.Ix_prime);

    if (synth_ranking) {
      // copy ranking
      const goto_programt::targetst &old_r=vars.Rx;
      goto_programt::targetst &new_r=result.Rx;
      new_r.resize(old_r.size());
      std::transform(old_r.begin(), old_r.end(), new_r.begin(), fix);
    
      const goto_programt::targetst &old_rp=vars.Rx_prime;
      goto_programt::targetst &new_rp=result.Rx_prime;
      new_rp.resize(old_rp.size());
      std::transform(old_rp.begin(), old_rp.end(), new_rp.begin(), fix);
    }
    return result;
  }
  
  Synth::synth_programt::program_ranget operator()(
      const Synth::synth_programt::program_ranget &range) const
  {
    Synth::synth_programt::program_ranget result;
    result.begin=fix(range.begin);
    result.end=fix(range.end);
    return result;
  }
  
  Synth::synth_programt::loopt operator()(const Synth::synth_programt::loopt &loop) const
  {
    Synth::synth_programt::loopt result;
    result.body=fix(loop.body);
    result.guard=loop.guard;


    result.meta_variables=fix(loop.meta_variables);
    return result;
  }
};

synth_programt &assign(synth_programt &lhs, const synth_programt &rhs)
{
  const copy_targett fix(rhs.gf, lhs.gf, rhs.synth_ranking);

  const Synth::synth_programt::loopst &old_loops=rhs.loops;
  lhs.loops.resize(old_loops.size());
  std::transform(old_loops.begin(), old_loops.end(), lhs.loops.begin(), fix);

  const goto_programt::targetst &old_x0=rhs.x0_choices;
  lhs.x0_choices.resize(old_x0.size());
  std::transform(old_x0.begin(), old_x0.end(), lhs.x0_choices.begin(), fix);
  lhs.synth_range=fix(rhs.synth_range);
  lhs.assertion=rhs.assertion;

  lhs.Ix0=fix(rhs.Ix0);

  lhs.Ax=fix(rhs.Ax);

  lhs.synth_ranking = rhs.synth_ranking;
  return lhs;
}
}

Synth::synth_programt::synth_programt(const synth_programt &other) :
    st(other.st)
{
  gf.copy_from(other.gf);
  assign(*this, other);
}

synth_programt &Synth::synth_programt::operator =(const synth_programt &other)
{
  set_goto_program(other.st, other.gf);
  return assign(*this, other);
}

void Synth::synth_programt::print() const {
  gf.output(namespacet(st), std::cerr);
}
