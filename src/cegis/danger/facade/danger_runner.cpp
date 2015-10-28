#include <cstdlib>

#include <util/cmdline.h>
#include <util/message.h>

#include <cegis/facade/cegis.h>
#include <cegis/genetic/ga_learn.h>
#include <cegis/genetic/random_individual.h>
#include <cegis/genetic/tournament_select.h>
#include <cegis/genetic/instruction_set_info_factory.h>
#include <cegis/genetic/random_mutate.h>
#include <cegis/genetic/random_cross.h>
#include <cegis/genetic/symex_fitness.h>
#include <cegis/genetic/genetic_constant_strategy.h>
#include <cegis/genetic/genetic_preprocessing.h>
#include <cegis/seed/null_seed.h>
#include <cegis/seed/literals_seed.h>
#include <cegis/symex/cegis_symex_learn.h>
#include <cegis/symex/cegis_symex_verify.h>
#include <cegis/statistics/cegis_statistics_wrapper.h>

#include <cegis/danger/constant/constant_strategy.h>
#include <cegis/danger/constant/default_constant_strategy.h>
#include <cegis/danger/instrument/meta_variables.h>
#include <cegis/danger/preprocess/danger_preprocessing.h>
#include <cegis/danger/symex/learn/add_variable_refs.h>
#include <cegis/danger/symex/learn/danger_learn_config.h>
#include <cegis/danger/symex/learn/danger_body_provider.h>
#include <cegis/danger/symex/verify/danger_verify_config.h>
#include <cegis/danger/symex/verify/parallel_danger_verifier.h>
#include <cegis/danger/symex/fitness/danger_fitness_config.h>
#include <cegis/danger/facade/danger_runner.h>

namespace

{
#define DANGER_STATISTICS "cegis-statistics"
#define DANGER_MAX_SIZE "cegis-max-size"

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

#define DANGER_PARALLEL_VERIFY "cegis-parallel-verify"

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

namespace
{
class variable_counter_helper
{
  const danger_programt &prog;
  bool counted;
  size_t num_vars;
  size_t num_consts;

  const size_t &count(size_t &value)
  {
    if (!counted)
    {
      danger_variable_idst ids;
      num_consts=get_danger_variable_ids(prog.st, ids);
      num_vars=ids.size();
      counted=true;
    }
    return value;
  }
public:
  variable_counter_helper(const danger_programt &prog) :
      prog(prog), counted(false), num_vars(0), num_consts(0)
  {
  }

  size_t get_num_vars()
  {
    return count(num_vars);
  }

  size_t get_num_consts()
  {
    return count(num_consts);
  }
};

bool is_genetic(const optionst &opt)
{
  return opt.get_bool_option("cegis-genetic");
}

class lazy_sizet
{
  const std::function<size_t(void)> func;
  size_t size;
  bool initialised;
public:
  lazy_sizet(const std::function<size_t(void)> &func) :
      func(func), size(0), initialised(false)
  {
  }

  size_t operator()()
  {
    if (initialised) return size;
    initialised=true;
    return size=func();
  }
};

#define SKOLEM_PROG_INDEX 1u

class min_danger_prog_sizet
{
  lazy_sizet &preproc_min_prog_size;
  const size_t user_min_prog_size;
public:
  min_danger_prog_sizet(lazy_sizet &preproc_min_prog_size, const optionst &opt) :
      preproc_min_prog_size(preproc_min_prog_size), user_min_prog_size(
          opt.get_unsigned_int_option("cegis-min-size"))
  {
  }

  size_t operator()(const size_t prog_index) const
  {
    if (SKOLEM_PROG_INDEX == prog_index)
      return std::max(user_min_prog_size, preproc_min_prog_size());
    return user_min_prog_size;
  }
};

class max_danger_prog_sizet
{
  lazy_sizet &num_x0;
  const size_t user_max_prog_size;
public:
  max_danger_prog_sizet(lazy_sizet &num_x0, const optionst &opt) :
      num_x0(num_x0), user_max_prog_size(
          opt.get_unsigned_int_option("cegis-max-size"))
  {
  }

  size_t operator()(const size_t prog_index) const
  {
    if (SKOLEM_PROG_INDEX == prog_index)
    {
      const size_t number_of_x0_vars=num_x0();
      if (number_of_x0_vars == 0u) return 0u;
      return std::max(number_of_x0_vars, user_max_prog_size);
    }
    return user_max_prog_size;
  }
};
}

template<class preproct>
int run_genetic(mstreamt &os, const optionst &opt, const danger_programt &prog,
    preproct &preproc)
{
  if (is_genetic(opt))
  {
    // Danger program properties and GA settings
    const unsigned int seed=opt.get_unsigned_int_option("cegis-seed");
    danger_body_providert body(prog);
    instruction_set_info_factoryt info_fac(std::ref(body));
    const size_t pop_size=opt.get_unsigned_int_option("cegis-genetic-popsize");
    const lazy_sizet num_progs([&prog]()
    { return prog.loops.size() * 3u;});
    lazy_sizet num_x0([&prog]()
    { return prog.x0_choices.size();});
    lazy_sizet preproc_min_size([&preproc]()
    { return preproc.get_min_solution_size();});
    const min_danger_prog_sizet min_prog_sz(preproc_min_size, opt);
    const max_danger_prog_sizet max_prog_sz(num_x0, opt);
    const size_t rounds=opt.get_unsigned_int_option("cegis-genetic-rounds");
    variable_counter_helper counter(prog);
    const lazy_sizet num_vars([&counter]()
    { return counter.get_num_vars();});
    const lazy_sizet num_consts([&counter]()
    { return counter.get_num_consts();});

    // Set-up genetic algorithm
    const typet type=danger_meta_type(); // XXX: Currently single user data type.
    random_individualt rnd(seed, type, info_fac, min_prog_sz, max_prog_sz,
        num_progs, num_vars, num_x0);
    tournament_selectt select(rnd, pop_size, rounds);
    random_mutatet mutate(rnd, num_consts);
    random_crosst cross(rnd);
    danger_fitness_configt config(info_fac, prog);
    typedef symex_fitnesst<danger_fitness_configt> fitnesst;
    fitnesst fitness(opt, config);
    ga_learnt<tournament_selectt, random_mutatet, random_crosst, fitnesst,
        danger_fitness_configt> learn(opt, select, mutate, cross, fitness,
        config);
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
  srand(options.get_unsigned_int_option("cegis-seed"));
  const bool is_gen=is_genetic(options);
  const constant_strategyt str=
      is_gen ? genetic_constant_strategy : default_constant_strategy;
  danger_preprocessingt preproc(st, gf, str);
  const danger_programt &prog=preproc.get_danger_program();
  genetic_preprocessingt<danger_preprocessingt> gen_preproc(options, preproc);
  return run_genetic(result, options, prog, gen_preproc);
}
