#include <goto-programs/safety_checker.h>

#include <cbmc/cbmc_solvers.h>
#include <cbmc/bmc.h>

#define CEGIS_SYMEX_DEFAULT_MIN_WORD_WIDTH 2u

template<class learn_configurationt>
cegis_symex_learnt<learn_configurationt>::cegis_symex_learnt(
    const optionst &options, learn_configurationt &config) :
    options(options), config(config), word_width(
    CEGIS_SYMEX_DEFAULT_MIN_WORD_WIDTH), max_solution_size(1u)
{
}

template<class learn_configurationt>
cegis_symex_learnt<learn_configurationt>::~cegis_symex_learnt()
{
}

template<class learn_configurationt>
template<class seedt>
void cegis_symex_learnt<learn_configurationt>::seed(seedt &seed)
{
  seed(counterexamples);
}

template<class learn_configurationt>
const typename cegis_symex_learnt<learn_configurationt>::candidatet &cegis_symex_learnt<
    learn_configurationt>::next_candidate() const
{
  return current_candidate;
}

template<class learn_configurationt>
bool cegis_symex_learnt<learn_configurationt>::learn(
    const size_t max_solution_size)
{
  this->max_solution_size=max_solution_size;
  return learn(counterexamples.end(), counterexamples.end());
}

namespace
{
safety_checkert::resultt cegis_learn_run_bmc(goto_tracet &trace,
    const symbol_tablet &st, const goto_functionst &gf, message_handlert &msg,
    const optionst &opt)
{
  cbmc_solverst solvers(opt, st, msg);
  const std::unique_ptr<cbmc_solverst::solvert> solver=solvers.get_solver();
  bmct bmc(opt, st, msg, solver->prop_conv());

  const safety_checkert::resultt bmc_result=bmc(gf);
  if (safety_checkert::UNSAFE != bmc_result) return bmc_result;
  trace=bmc.safety_checkert::error_trace;
  return safety_checkert::UNSAFE;
}
}

template<class learn_configurationt>
safety_checkert::resultt cegis_symex_learnt<learn_configurationt>::run_bmc(
    message_handlert &msg, const optionst &opt)
{
  const symbol_tablet &st=config.get_symbol_table();
  cbmc_solverst solvers(opt, st, msg);
  const std::unique_ptr<cbmc_solverst::solvert> solver=solvers.get_solver();
  bmct bmc(opt, st, msg, solver->prop_conv());

  const safety_checkert::resultt bmc_result=bmc(config.get_goto_functions());
  if (safety_checkert::UNSAFE != bmc_result) return bmc_result;
  const goto_tracet &trace=bmc.safety_checkert::error_trace;
  config.process(counterexamples, max_solution_size);
  config.convert(current_candidate, trace, max_solution_size);
  return safety_checkert::UNSAFE;
}

template<class learn_configurationt>
template<class itert>
bool cegis_symex_learnt<learn_configurationt>::learn(itert first,
    const itert &last)
{
  std::copy(first, last, std::back_inserter(counterexamples));
  if (counterexamples.empty()) return true;

  const size_t full_width=32u;
  for (const counterexamplet &ce : counterexamples)
    for (const typename counterexamplet::value_type &assignment : ce)
    {
      const exprt &expr=assignment.second;
      if (ID_constant != expr.id()) continue;
      const std::string &value=id2string(to_constant_expr(expr).get_value());
      const std::string::size_type first_one=value.find('1');
      if (std::string::npos == first_one) continue;
      const size_t value_width=value.size() - first_one;
      if (value_width <= word_width) continue;
      if (value_width > 16) word_width=32u;
      else if (value_width > 8) word_width=16u;
      else if (value_width > 4) word_width=8u;
      else if (value_width > 2) word_width=4u;
    }

  null_message_handlert msg;
  optionst o(options);
  o.set_option("unwinding-assertions", false);
  std::string unwindset("__CPROVER_danger_execute.0:");
  unwindset+=integer2string(max_solution_size + 1);
  //learn_options.set_option("unwindset", unwindset);
  const symbol_tablet &st=config.get_symbol_table();
  const goto_functionst &gf=config.get_goto_functions();
  safety_checkert::resultt full_width_result=safety_checkert::ERROR;
  goto_tracet full_width_trace;
  for (; word_width < full_width; word_width*=2)
  {
    config.process(counterexamples, max_solution_size);
    config.set_word_width(word_width);
    const safety_checkert::resultt limited_result=run_bmc(msg, o);
    if (safety_checkert::UNSAFE == limited_result) return true;
    if (safety_checkert::ERROR == limited_result) return false;
    if (safety_checkert::ERROR == full_width_result)
    {
      config.process(counterexamples, max_solution_size);
      full_width_result=cegis_learn_run_bmc(full_width_trace, st, gf, msg, o);
      if (safety_checkert::UNSAFE != full_width_result) return false;
    }
  }
  config.process(counterexamples, max_solution_size);
  if (safety_checkert::ERROR == full_width_result) full_width_result=
      cegis_learn_run_bmc(full_width_trace, st, gf, msg, o);
  if (safety_checkert::UNSAFE != full_width_result) return false;
  config.convert(current_candidate, full_width_trace, max_solution_size);
  return true;
}

template<class learn_configurationt>
void cegis_symex_learnt<learn_configurationt>::show_candidate(
    messaget::mstreamt &os) const
{
  config.show_candidate(os, current_candidate);
}
