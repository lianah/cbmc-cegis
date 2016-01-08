#include <cstdlib>

#include <util/options.h>

#include <cegis/facade/cegis.h>
#include <cegis/statistics/cegis_statistics_wrapper.h>
#include <cegis/symex/cegis_symex_learn.h>
#include <cegis/symex/cegis_symex_verify.h>
#include <cegis/seed/null_seed.h>
#include <cegis/invariant/options/parameters.h>
#include <cegis/invariant/constant/constant_strategy.h>
#include <cegis/invariant/constant/default_constant_strategy.h>
#include <cegis/safety/preprocessing/safety_preprocessing.h>
#include <cegis/safety/symex/learn/safety_learn_config.h>
#include <cegis/safety/symex/verify/safety_verify_config.h>
#include <cegis/safety/facade/safety_runner.h>

namespace
{
typedef messaget::mstreamt mstreamt;

template<class learnt, class verifyt, class preproct>
int run_user_interface(mstreamt &os, const optionst &options, learnt &learn,
    verifyt &verify, preproct &preproc)
{
  null_seedt seed;
  const size_t max_prog_size=options.get_unsigned_int_option(CEGIS_MAX_SIZE);
  if (!options.get_bool_option(CEGIS_STATISTICS))
    return run_cegis(learn, verify, preproc, seed, max_prog_size, os);
  cegis_statistics_wrappert<learnt, verifyt, mstreamt> stat(learn, verify, os);
  return run_cegis(stat, stat, preproc, seed, max_prog_size, os);
}

template<class learn_configt>
struct genetic_traits
{
};

template<>
struct genetic_traits<safety_learn_configt>
{
};

template<class learnt, class verifyt, class preproct>
int run_backend(mstreamt &os, const optionst &opt, const safety_programt &prog,
    learnt &learn, verifyt &verify, preproct &preproc)
{
  if (!opt.get_bool_option(CEGIS_GENETIC))
    return run_user_interface(os, opt, learn, verify, preproc);
  // TODO: Adde genetic case
}
}

int run_safety(optionst &options, mstreamt &result, const symbol_tablet &st,
    const goto_functionst &gf)
{
  srand(options.get_unsigned_int_option(CEGIS_SEED));
  const constant_strategyt constant_strategy(default_constant_strategy);
  safety_preprocessingt preproc(options, st, gf, constant_strategy);
  const safety_programt &safety_program=preproc.get_safety_program();
  safety_learn_configt learn_cfg(safety_program);
  typedef cegis_symex_learnt<safety_preprocessingt, safety_learn_configt> learnt;
  learnt learn(options, preproc, learn_cfg);
  safety_verify_configt verify_cfg(safety_program);
  cegis_symex_verifyt<safety_verify_configt> verify(options, verify_cfg);
  return run_backend(result, options, safety_program, learn, verify, preproc);
}
