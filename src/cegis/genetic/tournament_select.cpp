#include <algorithm>
#include <cstdlib>

#include <cegis/genetic/random_individual.h>
#include <cegis/genetic/tournament_select.h>

// XXX: Debug
#include <iostream>

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
typedef tournament_selectt::populationt::iterator contestantt;

class fitness_is_less_thant
{
  const contestantt no_contestant;
public:
  fitness_is_less_thant(const contestantt &no_contestant) :
      no_contestant(no_contestant)
  {
  }

  bool operator()(const contestantt &lhs, const contestantt &rhs) const
  {
    if (lhs == rhs) return false;
    if (no_contestant == lhs) return false;
    if (no_contestant == rhs) return true;
    return lhs->fitness < rhs->fitness;
  }
};

class fitness_is_greater_thant
{
  const contestantt no_contestant;
public:
  fitness_is_greater_thant(const contestantt &no_contestant) :
      no_contestant(no_contestant)
  {
  }

  bool operator()(const contestantt &lhs, const contestantt &rhs) const
  {
    if (lhs == rhs) return false;
    if (no_contestant == lhs) return false;
    if (no_contestant == rhs) return true;
    return lhs->fitness > rhs->fitness;
  }
};

#define MATCH_SIZE 2u

class arenat
{
  typedef std::multiset<contestantt, fitness_is_greater_thant> winnerst;
  typedef std::multiset<contestantt, fitness_is_less_thant> loserst;
  typedef std::pair<contestantt, contestantt> matcht;
  typedef typename tournament_selectt::populationt::value_type::fitnesst fitnesst;
  const contestantt no_contestant;
  winnerst winners;
  loserst losers;

  bool contains(const contestantt &c)
  {
    if (winners.end() != std::find(winners.begin(), winners.end(), c))
      return true;
    return losers.end() != std::find(losers.begin(), losers.end(), c);
  }
public:
  arenat(tournament_selectt::populationt &pop) :
      no_contestant(pop.end()), winners(no_contestant), losers(no_contestant)
  {
  }

  bool add_contestant(contestantt contestant)
  {
    if (contains(contestant)) return false;
    winners.insert(contestant);
    if (winners.size() > MATCH_SIZE)
    {
      winnerst::iterator it=winners.end();
      losers.insert(*--it);
      winners.erase(it);
    }
    if (losers.size() > MATCH_SIZE)
    {
      loserst::iterator it=losers.end();
      losers.erase(--it);
    }
    return true;
  }

  void select(tournament_selectt::selectiont &selection)
  {
    assert(MATCH_SIZE == winners.size());
    assert(MATCH_SIZE == losers.size());
    std::copy(winners.begin(), winners.end(),
        std::back_inserter(selection.parents));
    std::copy(losers.begin(), losers.end(),
        std::back_inserter(selection.children));
  }
};

// XXX: Debug
size_t parent_fitness_sum=0;
size_t parent_max_fitness=0;
size_t parent_min_fitness=0;
size_t cross_count=0;
// XXX: Debug
}

tournament_selectt::selectiont tournament_selectt::select(populationt &pop)
{
  arenat arena(pop);
  for (size_t contestants=0; contestants < rounds;)
  {
    populationt::iterator contestant=pop.begin();
    std::advance(contestant, rand() % pop.size());
    if (arena.add_contestant(contestant)) ++contestants;
  }
  tournament_selectt::selectiont selection;
  arena.select(selection);
  assert(selection.parents[0]->fitness >= selection.parents[1]->fitness);
  assert(selection.parents[1]->fitness >= selection.children[0]->fitness);
  assert(selection.children[0]->fitness <= selection.children[1]->fitness);
  // XXX: Debug
  const size_t ff=selection.parents[0]->fitness;
  parent_max_fitness=std::max(parent_max_fitness, ff);
  parent_min_fitness=std::min(parent_min_fitness, ff);
  parent_fitness_sum+=ff;
  const size_t mf=selection.parents[1]->fitness;
  parent_max_fitness=std::max(parent_max_fitness, mf);
  parent_min_fitness=std::min(parent_min_fitness, mf);
  parent_fitness_sum+=mf;
  // XXX: Debug
  if (++cross_count % 10000 == 0)
  {
    std::cout << "<parent_max_fitness>" << parent_max_fitness
        << "</parent_max_fitness>" << std::endl;
    std::cout << "<parent_min_fitness>" << parent_min_fitness
        << "</parent_min_fitness>" << std::endl;
    std::cout << "<parent_avg_fitness>"
        << ((double) parent_fitness_sum / (double) cross_count) / 2.0
        << "</parent_avg_fitness>" << std::endl;
  }
  // XXX: Debug
  return selection;
}
