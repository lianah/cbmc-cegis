#include <algorithm>

#include <goto-programs/goto_trace.h>

#include <cegis/danger/symex/verify/extract_counterexample.h>

// XXX: Debug
#include <iostream>

namespace
{
class is_pc_equalt
{
  const goto_programt::const_targett &lhs;
public:
  is_pc_equalt(const goto_trace_stept &lhs) :
      lhs(lhs.pc)
  {
  }

  bool operator()(const goto_programt::targett &rhs) const
  {
    return lhs == rhs;
  }
};

class extract_counterexamplet
{
  counterexamplet &result;
  goto_programt::targetst q;

  bool should_extract(const goto_trace_stept &step)
  {
    const is_pc_equalt pred(step);
    const size_t original_size=q.size();
    q.erase(std::remove_if(q.begin(), q.end(), pred), q.end());
    return q.size() != original_size;
  }
public:
  extract_counterexamplet(counterexamplet &result,
      const goto_programt::targetst &quantifiers) :
      result(result), q(quantifiers)
  {
  }

  bool is_done() const
  {
    return q.empty();
  }

  void operator()(const goto_trace_stept &step)
  {
    if (!should_extract(step)) return;
    std::cout << "<extract_ce>" << step.pc->code.to_string() << "</extract_ce>" << std::endl; // XXX: Debug
    std::cout << "<ce_value>" << step.lhs_object_value.to_string() << "</ce_value>" << std::endl; // XXX: Debug
    const symbol_exprt &lhs=step.lhs_object;
    result.insert(std::make_pair(lhs.get_identifier(), step.lhs_object_value));
  }
};
}

void danger_extract_counterexample(counterexamplet &result,
    const goto_tracet &trace, const goto_programt::targetst &quantifiers)
{
  result.clear();
  const goto_tracet::stepst &s=trace.steps;
  extract_counterexamplet extract(result, quantifiers);
  typedef goto_tracet::stepst::const_iterator itt;
  for (itt it=s.begin(); it != s.end() && !extract.is_done(); ++it)
    extract(*it);
}
