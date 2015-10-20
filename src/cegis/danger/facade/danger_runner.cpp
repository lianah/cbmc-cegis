#include <util/cmdline.h>
#include <util/message.h>

#include <cegis/facade/cegis.h>
#include <cegis/genetic/ga_learn.h>
#include <cegis/genetic/tournament_select.h>
#include <cegis/genetic/instruction_set_info_factory.h>
#include <cegis/seed/null_seed.h>
#include <cegis/seed/literals_seed.h>
#include <cegis/symex/cegis_symex_learn.h>
#include <cegis/symex/cegis_symex_verify.h>
#include <cegis/statistics/cegis_statistics_wrapper.h>

#include <cegis/danger/constant/constant_strategy.h>
#include <cegis/danger/constant/default_constant_strategy.h>
#include <cegis/danger/preprocess/danger_preprocessing.h>
#include <cegis/danger/symex/learn/add_variable_refs.h>
#include <cegis/danger/symex/learn/danger_learn_config.h>
#include <cegis/danger/symex/learn/danger_body_provider.h>
#include <cegis/danger/symex/verify/danger_verify_config.h>
#include <cegis/danger/symex/verify/parallel_danger_verifier.h>
#include <cegis/danger/facade/danger_runner.h>

namespace

{
#define DANGER_STATISTICS "danger-statistics"
#define DANGER_MAX_SIZE "danger-max-size"

typedef messaget::mstreamt mstreamt;

template<class learnt, class verifyt, class preproct>
int run_statistics(mstreamt &os, const optionst &options, learnt &learn,
    verifyt &verify, preproct &preproc)
{
  null_seedt seed;
  const size_t max_prog_size=options.get_unsigned_int_option(DANGER_MAX_SIZE);
  if (!options.get_bool_option(DANGER_STATISTICS))
    return run_cegis(learn, verify, preproc, seed, max_prog_size, os);
  cegis_statistics_wrappert<learnt, verifyt, mstreamt> stat(learn, verify, os);
  return run_cegis(stat, stat, preproc, seed, max_prog_size, os);
}

#define DANGER_PARALLEL_VERIFY "danger-parallel-verify"

template<class learnert, class preproct>
int run_parallel(mstreamt &os, const optionst &options,
    const danger_programt &prog, learnert &learn, preproct &preproc)
{
  danger_verify_configt verify_config(prog);
  if (options.get_bool_option(DANGER_PARALLEL_VERIFY))
  {
    parallel_danger_verifiert verify(options, verify_config);
    return run_statistics(os, options, learn, verify, preproc);
  }
  cegis_symex_verifyt<danger_verify_configt> verify(options, verify_config);
  return run_statistics(os, options, learn, verify, preproc);
}

#define DANGER_GENETIC "danger-genetic"

class mutatet
{
};

class crosst
{
};

class fitnesst
{
public:
  typedef std::map<const irep_idt, exprt> counterexamplet;

  template<class seedt>
  void seed(seedt &seed)
  {
  }

  void add_test_case(const counterexamplet &ce)
  {
  }
};

class convertt
{
public:
  typedef danger_goto_solutiont candidatet;
};

template<class preproct>
int run_genetic(mstreamt &os, const optionst &opt, const danger_programt &prog,
    preproct &preproc)
{
  if (opt.get_bool_option(DANGER_GENETIC))
  {
    const danger_body_providert body(prog);
    instruction_set_info_factoryt info_factory(body);
    const size_t num_progs=prog.loops.size() * 3u;
    const size_t max_prog_size=opt.get_unsigned_int_option(DANGER_MAX_SIZE);
    const size_t rounds=opt.get_unsigned_int_option("danger-genetic-rounds");
    std::function<size_t(void)> num_vars=[&prog]()
    {
      danger_variable_idst ids;
      get_danger_variable_ids(prog.st, ids);
      return ids.size();
    };
    tournament_selectt select(info_factory, num_progs, max_prog_size, num_vars,
        rounds);
    mutatet mutate;
    crosst cross;
    fitnesst fitness;
    convertt convert;
    ga_learnt<tournament_selectt, mutatet, crosst, fitnesst, convertt> learn(
        select, mutate, cross, fitness, convert);
    return run_parallel(os, opt, prog, learn, preproc);
  }
  danger_learn_configt learn_config(prog);
  cegis_symex_learnt<danger_learn_configt> learn(opt, learn_config);
  return run_parallel(os, opt, prog, learn, preproc);
}
}

int run_danger(const optionst &options, mstreamt &result,
    const symbol_tablet &st, const goto_functionst &gf)
{
  // TODO: If genetic: only nondet constants! (part of x0)
  const constant_strategyt strategy=default_constant_strategy;
  danger_preprocessingt preproc(st, gf, strategy);
  const danger_programt &prog=preproc.get_danger_program();
  return run_genetic(result, options, prog, preproc);
}
