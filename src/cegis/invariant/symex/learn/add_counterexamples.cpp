#include <algorithm>

#include <ansi-c/c_types.h>

#include <util/arith_tools.h>

#include <cegis/invariant/util/invariant_program_helper.h>
#include <cegis/invariant/instrument/meta_variables.h>
#include <cegis/invariant/meta/literals.h>
#include <cegis/invariant/options/invariant_program.h>
#include <cegis/invariant/symex/learn/add_counterexamples.h>

namespace
{
typedef std::map<const irep_idt, array_exprt> array_valuest;

typedef counterexamplet::const_iterator ceitt;
class create_x_array_valuest
{
  array_valuest &vals;
  const counterexamplet &prototype;
  const size_t num_ces;
public:
  array_valuest::value_type to_array(const ceitt &it, const exprt &size)
  {
    const array_exprt array(array_typet(it->second.type(), size));
    return std::make_pair(it->first, array);
  }

  create_x_array_valuest(array_valuest &vals, const counterexamplet &prototype,
      const size_t num_ces) :
      vals(vals), prototype(prototype), num_ces(num_ces)
  {
    const constant_exprt size(from_integer(num_ces, unsigned_int_type()));
    for (ceitt it=prototype.begin(); it != prototype.end(); ++it)
      vals.insert(to_array(it, size));
  }

  void operator()(const counterexamplet &ce) const
  {
    for (ceitt it=ce.begin(); it != ce.end(); ++it)
      vals[it->first].copy_to_operands(it->second);
  }
};

const char X_CHOICE_PREFIX[]=CEGIS_PREFIX "x_choice_";
void declare_x_arrays(symbol_tablet &st, goto_functionst &gf,
    goto_programt::targett pos, const array_valuest &vals)
{
  for (array_valuest::const_iterator it=vals.begin(); it != vals.end(); ++it)
  {
    std::string base_name(X_CHOICE_PREFIX);
    base_name+=id2string(it->first);
    const array_exprt &value=it->second;
    const typet &type=value.type();
    pos=declare_invariant_variable(st, gf, pos, base_name, type);
    pos=assign_invariant_variable(st, gf, pos, base_name, value);
  }
}

const char X_INDEX[]=CEGIS_PREFIX "x_index";
symbol_exprt get_index(const symbol_tablet &st)
{
  const std::string index_name(get_invariant_meta_name(X_INDEX));
  return st.lookup(index_name).symbol_expr();
}

const char X_LABEL[]=CEGIS_PREFIX"x_loop";
goto_programt::targett add_ce_loop(invariant_programt &prog,
    const size_t ces_size, const bool use_x0_ce)
{
  symbol_tablet &st=prog.st;
  goto_functionst &gf=prog.gf;
  goto_programt::targett pos=prog.invariant_range.begin;
  const typet size_type(unsigned_int_type());
  pos=declare_invariant_variable(st, gf, --pos, X_INDEX, size_type);
  const constant_exprt first_index(from_integer(0, size_type));
  pos=assign_invariant_variable(st, gf, pos, X_INDEX, first_index);
  goto_programt::targett loop_head=pos;
  (++loop_head)->labels.push_back(X_LABEL);
  goto_programt &body=get_entry_body(gf);
  pos=insert_before_preserve_labels(body, prog.invariant_range.end);
  pos->type=goto_program_instruction_typet::ASSIGN;
  pos->source_location=default_invariant_source_location();
  const symbol_exprt index(get_index(st));
  const constant_exprt one(from_integer(1, size_type));
  const code_assignt inc(index, plus_exprt(index, one));
  pos->code=inc;
  pos=body.insert_after(pos);
  pos->type=goto_program_instruction_typet::GOTO;
  pos->source_location=default_invariant_source_location();
  pos->function=goto_functionst::entry_point();
  pos->targets.push_back(loop_head);
  pos->loop_number=0u;
  const size_t loop_limit=use_x0_ce ? ces_size + 1 : ces_size;
  const constant_exprt num_ces(from_integer(loop_limit, size_type));
  const binary_relation_exprt cond(index, ID_lt, num_ces);
  pos->guard=cond;
  return pos;
}

class assign_ce_valuet
{
  const symbol_tablet &st;
  goto_functionst &gf;
  goto_programt::targett pos;
  goto_programt::targett goto_pos;
  const bool use_x0_ce;
public:
  void add_x0_case(const size_t ces_size)
  {
    const typet size_type(unsigned_int_type());
    const constant_exprt num_ces(from_integer(ces_size, size_type));
    const symbol_exprt index(get_index(st));
    const equal_exprt cond(index, num_ces);
    pos->guard=cond;
    goto_pos=pos;
  }

  assign_ce_valuet(invariant_programt &prog, const size_t ces_size,
      const bool use_x0_ce) :
      st(prog.st), gf(prog.gf), use_x0_ce(use_x0_ce)
  {
    const invariant_programt::invariant_loopst loops(prog.get_loops());
    assert(!loops.empty());
    pos=loops.front()->meta_variables.Ix;
    ++pos;
    if (use_x0_ce)
    {
      pos=get_entry_body(gf).insert_after(pos);
      pos->type=goto_program_instruction_typet::GOTO;
      pos->source_location=default_invariant_source_location();
      add_x0_case(ces_size);
    }
  }

  void operator()(const std::pair<const irep_idt, exprt> &assignment)
  {
    std::string base_name(X_CHOICE_PREFIX);
    base_name+=id2string(assignment.first);
    const std::string array_name(get_invariant_meta_name(base_name));
    const symbol_exprt array(st.lookup(array_name).symbol_expr());
    const index_exprt rhs(array, get_index(st));
    const symbol_exprt lhs(st.lookup(assignment.first).symbol_expr());
    pos=invariant_assign(st, gf, pos, lhs, rhs);
  }

  void finalize_x0_case()
  {
    if (use_x0_ce) goto_pos->targets.push_back(++pos);
  }
};

void assign_ce_values(invariant_programt &prog, const counterexamplet &ce,
    const size_t num_ces, const bool use_x0_ce)
{
  const assign_ce_valuet assign_value(prog, num_ces, use_x0_ce);
  std::for_each(ce.begin(), ce.end(), assign_value).finalize_x0_case();
}

void create_constraints(invariant_programt &prog,
    const constraint_factoryt &constraint)
{
  goto_programt::targett pos=prog.invariant_range.end;
  std::advance(pos, -3);
  goto_programt &body=get_entry_body(prog.gf);
  pos=body.insert_after(pos);
  pos->type=goto_program_instruction_typet::ASSUME;
  pos->source_location=default_invariant_source_location();
  pos->guard=constraint(prog.get_loops().size());
}

void add_final_assertion(invariant_programt &prog,
    const goto_programt::targett &loop_end)
{
  goto_programt &body=get_entry_body(prog.gf);
  goto_programt::targett assertion=body.insert_after(loop_end);
  assertion->type=goto_program_instruction_typet::ASSERT;
  assertion->source_location=default_invariant_source_location();
  assertion->guard=false_exprt();
}
}

void invariant_add_learned_counterexamples(invariant_programt &prog,
    const counterexamplest &ces, const constraint_factoryt constraint,
    const bool x0_ce)
{
  if (ces.empty()) return;
  array_valuest vals;
  const counterexamplet &prototype=*ces.begin();
  const size_t ces_count=ces.size();
  const create_x_array_valuest create_values(vals, prototype, ces_count);
  std::for_each(ces.begin(), ces.end(), create_values);
  symbol_tablet &st=prog.st;
  goto_functionst &gf=prog.gf;
  goto_programt::targett pos=prog.invariant_range.begin;
  declare_x_arrays(st, gf, --pos, vals);
  const goto_programt::targett loop_end=add_ce_loop(prog, ces.size(), x0_ce);
  assign_ce_values(prog, prototype, ces_count, x0_ce);
  create_constraints(prog, constraint);
  add_final_assertion(prog, loop_end);
}