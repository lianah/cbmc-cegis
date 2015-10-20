#include <algorithm>
#include <cstdlib>
#include <cassert>

#include <util/std_types.h>
#include <util/bv_arithmetic.h>

#include <cegis/genetic/tournament_select.h>

#define DEFAULT_TOURNAMENT_ROUNDS 10u

tournament_selectt::tournament_selectt(
    const instruction_set_info_factoryt &instruction_set_info,
    const size_t num_progs, const size_t max_prog_size,
    const std::function<size_t(void)> &num_vars, const size_t rounds) :
    info_factory(instruction_set_info), num_progs(num_progs), max_prog_size(
        max_prog_size), num_vars(num_vars), rounds(rounds)
{
}

tournament_selectt::~tournament_selectt()
{
}

namespace
{
unsigned int rand_x0(const size_t width)
{
  unsignedbv_typet type(width);
  type.set(ID_C_c_type, ID_signed_int);
  const unsigned int wordmask=bv_spect(type).max_value().to_ulong();
  const unsigned int r=rand() % 6u;
  switch (r)
  {
  case 0:
    return 0;
  case 1:
    return 1;
  case 2:
    return wordmask;
  case 3:
    return 1 << (width - 1);
  case 4:
    return (1 << (width - 1)) - 1;
  case 5:
    return rand();
  }
}
}

void tournament_selectt::init(populationt &pop)
{
  const size_t prog_size_limit=max_prog_size + 1;
  const instruction_set_infot &info=info_factory.get_info();
  const size_t opcode_limit=info.size() + 1;
  const size_t num_vars=this->num_vars();
  for (program_individualt &ind : pop)
  {
    program_individualt::programst &progs=ind.programs;
    progs.resize(num_progs);
    for (program_individualt::programt &prog : progs)
    {
      const size_t prog_size=rand() % prog_size_limit;
      prog.resize(prog_size);
      for (size_t i=0; i < prog.size(); ++i)
      {
        program_individualt::instructiont &instr=prog[i];
        instr.opcode=rand() % opcode_limit;
        const instruction_set_infot::const_iterator num_ops=info.find(
            instr.opcode);
        assert(info.end() != num_ops);
        instr.ops.resize(num_ops->second);
        for (program_individualt::instructiont::opt &op : instr.ops)
          op=rand() % (num_vars + i);
      }
    }
    // TODO: Provide num_x0
    //for (program_individualt::nondet_choices::value_type &x0 : ind.x0)
  }
}

namespace
{
#define ARENA_SIZE 2u

class arenat
{
  typedef tournament_selectt::populationt::iterator contestantt;
  typedef std::pair<contestantt, contestantt> matcht;
  typedef typename tournament_selectt::populationt::value_type::fitnesst fitnesst;
  std::array<matcht, ARENA_SIZE> matches;
  const contestantt no_contestant;
  size_t fight_count;

  bool fill_contestant(contestantt &lhs, const contestantt &rhs)
  {
    if (no_contestant != lhs) return false;
    lhs=rhs;
    return true;
  }

  bool fill_matches(const contestantt &contestant)
  {
    for (matcht &match : matches)
    {
      if (fill_contestant(match.first, contestant)) return true;
      if (fill_contestant(match.second, contestant)) return true;
    }
    return false;
  }

  bool does_underdog_win() const
  {
    return (fight_count / 2) % 2;
  }

  bool fight(contestantt &champion, const contestantt &challenger)
  {
    const bool underdog_wins=does_underdog_win();
    ++fight_count;
    const bool champ_is_underdog=champion->fitness < challenger->fitness;
    if (underdog_wins == champ_is_underdog) return false;
    champion=challenger;
    return true;
  }

  contestantt &get_first(matcht &match)
  {
    return does_underdog_win() ? match.second : match.first;
  }

  contestantt &get_second(matcht &match)
  {
    return does_underdog_win() ? match.first : match.second;
  }

  bool contains(const contestantt &contestant)
  {
    auto pred=[&contestant](const matcht &match)
    {
      return match.first == contestant || match.second == contestant;
    };
    return matches.end() != std::find_if(matches.begin(), matches.end(), pred);
  }
public:
  arenat(tournament_selectt::populationt &pop) :
      no_contestant(pop.end()), fight_count(0u)
  {
    matches.fill(std::make_pair(no_contestant, no_contestant));
  }

  bool add_contestant(const contestantt &contestant)
  {
    if (contains(contestant)) return false;
    if (fill_matches(contestant)) return true;
    fight_count=0u;
    for (matcht &match : matches)
    {
      contestantt &first=get_first(match);
      contestantt &second=get_second(match);
      const contestantt champion=first;
      if (fight(first, contestant))
      {
        second=champion;
        break;
      }
      if (fight(second, contestant)) break;
    }
    return true;
  }

  void select(tournament_selectt::selectiont &selection)
  {
    fight_count=0;
    for (matcht &match : matches)
    {
      selection.push_back(get_first(match));
      ++fight_count;
      selection.push_back(get_second(match));
      ++fight_count;
    }
  }
};
}

tournament_selectt::selectiont tournament_selectt::select(populationt &pop)
{
  arenat arena(pop);
  for (size_t contestants=0; contestants < rounds;)
  {
    const populationt::size_type idx=rand() % pop.size();
    populationt::iterator contestant=pop.begin();
    std::advance(contestant, rand() % pop.size());
    if (arena.add_contestant(contestant)) ++contestants;
  }
  tournament_selectt::selectiont selection;
  arena.select(selection);
  return selection;
}
