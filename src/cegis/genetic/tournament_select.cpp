#include <algorithm>
#include <cstdlib>

#include <cegis/genetic/random_individual.h>
#include <cegis/genetic/tournament_select.h>

#define MUTATION_OPS 2u

bool tournament_selectt::selectiont::can_mutate() const
{
  return parents.size() >= MUTATION_OPS;
}

#define NUM_PARENTS 2u
#define NUM_CHILDREN 2u

bool tournament_selectt::selectiont::can_cross() const
{
  return parents.size() >= NUM_PARENTS && children.size() >= NUM_CHILDREN;
}

tournament_selectt::individualt &tournament_selectt::selectiont::mutation_lhs()
{
  return *parents[1];
}

const tournament_selectt::individualt &tournament_selectt::selectiont::mutation_rhs() const
{
  return *parents.front();
}

const tournament_selectt::individualst &tournament_selectt::selectiont::get_parents() const
{
  return parents;
}

const tournament_selectt::individualst &tournament_selectt::selectiont::get_children() const
{
  return children;
}

tournament_selectt::tournament_selectt(random_individualt &random,
    size_t pop_size, size_t rounds) :
    random(random), pop_size(pop_size), rounds(rounds)
{
}

tournament_selectt::~tournament_selectt()
{
}

void tournament_selectt::init(populationt &pop)
{
  pop.resize(pop_size);
  for (program_individualt &ind : pop)
    random.havoc(ind);
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

  tournament_selectt::individualst &get_container(
      tournament_selectt::selectiont &selection)
  {
    const size_t count=fight_count % 4;
    if (count == 0 || count == 1) return selection.parents;
    return selection.children;
  }

  void select(tournament_selectt::selectiont &selection)
  {
    fight_count=0;
    for (matcht &match : matches)
    {
      get_container(selection).push_back(get_first(match));
      ++fight_count;
      get_container(selection).push_back(get_second(match));
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
