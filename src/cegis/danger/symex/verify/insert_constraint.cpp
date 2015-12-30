#include <algorithm>

#include <util/cprover_prefix.h>
#include <util/expr_util.h>

#include <cegis/invariant/util/invariant_constraint_variables.h>
#include <cegis/invariant/util/invariant_program_helper.h>
#include <cegis/danger/constraint/danger_constraint_factory.h>
#include <cegis/danger/options/danger_program.h>
#include <cegis/invariant/instrument/meta_variables.h>
#include <cegis/danger/symex/verify/insert_constraint.h>

namespace
{
class quantifyt
{
  goto_programt::targetst &quantifiers;
  goto_programt::targett pos;
  goto_programt &body;
public:
  quantifyt(goto_programt::targetst &quantifiers,
      const goto_programt::targett &pos, danger_programt &program) :
      quantifiers(quantifiers), pos(pos), body(get_entry_body(program.gf))
  {
  }

  void operator()(const symbol_exprt &var)
  {
    pos=body.insert_after(pos);
    pos->type=goto_program_instruction_typet::ASSIGN;
    pos->source_location=default_invariant_source_location();
    pos->code=code_assignt(var, side_effect_expr_nondett(var.type()));
    quantifiers.push_back(pos);
  }
};

void add_universal_quantifier(goto_programt::targetst &quantifiers,
    danger_programt &program)
{
  invaraint_symbol_set vars(create_empty_symbol_set());
  collect_counterexample_variables(vars, program);
  goto_programt::targett Ix=program.loops.front().meta_variables.Ix;
  const quantifyt quantify(quantifiers, --Ix, program);
  std::for_each(vars.begin(), vars.end(), quantify);
}

void add_final_assertion(danger_programt &program)
{
  goto_programt::targett pos=program.invariant_range.end;
  pos=get_entry_body(program.gf).insert_after(--pos);
  pos->type=goto_program_instruction_typet::ASSERT;
  pos->source_location=default_invariant_source_location();
  pos->guard=create_danger_constraint(program.loops.size());
}
}

void danger_insert_constraint(goto_programt::targetst &quantifiers,
    danger_programt &program)
{
  add_universal_quantifier(quantifiers, program);
  add_final_assertion(program);
}

void danger_limit_ce(const goto_programt::targetst &quantifiers,
    danger_programt &program, size_t width)
{
  const symbol_tablet &st=program.st;
  goto_programt &body=get_entry_body(program.gf);
  for (const goto_programt::targett &quantifier : quantifiers)
  {
    const irep_idt &var=get_affected_variable(*quantifier);
    const goto_programt::targett assume=body.insert_after(quantifier);
    assume->type=goto_program_instruction_typet::ASSUME;
    assume->source_location=default_invariant_source_location();
    const symbol_exprt expr=st.lookup(var).symbol_expr();
    const typet &type=expr.type();
    constant_exprt pattern=to_constant_expr(gen_zero(type));
    std::string value=id2string(pattern.get_value());
    const size_t value_sz=value.size();
    assert(width <= value.size());
    for (size_t i=0; i < value_sz - width; ++i)
      value[i]='1';
    pattern.set_value(value);
    const bitand_exprt bitand_expr(expr, pattern);
    const exprt zero(gen_zero(type));
    const equal_exprt condition(bitand_expr, zero);
    assume->guard=condition;
  }
}
