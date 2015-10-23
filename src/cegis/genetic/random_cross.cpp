#include <cassert>
#include <cstdlib>

#include <cegis/genetic/random_individual.h>
#include <cegis/genetic/random_cross.h>

random_crosst::random_crosst(random_individualt &random) :
    random(random), max_prog_sz(0u)
{
}

random_crosst::~random_crosst()
{
}

void random_crosst::set_max_solution_size(const size_t size)
{
  max_prog_sz=size;
}

namespace
{
void fix_result_ops(random_crosst::programt::value_type &instr,
    const size_t org_index, const size_t new_index, const size_t num_vars)
{
  for (random_crosst::programt::value_type::opt &op : instr.ops)
  {
    if (op < num_vars) continue;
    op-=(org_index - new_index);
    op%=(num_vars + new_index);
  }
}
}

void random_crosst::operator ()(const individualst &parents,
    const individualst &children)
{
  assert(parents.size() >= 2 && children.size() >= 2);
  const size_t prog_limit=parents.front()->programs.size() + 1;
  const size_t target_prog_index=rand() % prog_limit;

  programt &father=parents.front()->programs[target_prog_index];
  programt &mother=parents[1u]->programs[target_prog_index];
  programt &son=children.front()->programs[target_prog_index];
  programt &daughter=children[1u]->programs[target_prog_index];

  const size_t f_sz=father.size();
  const size_t m_sz=mother.size();
  const size_t father_offset=rand() % (f_sz + 1);
  const size_t mo_lower=std::max(0u, father_offset + m_sz - max_prog_sz);
  const size_t mo_upper=std::min(m_sz, father_offset - f_sz + max_prog_sz);
  const size_t mo_range=mo_upper - mo_lower + 1;
  const size_t mother_offset=mo_range ? mo_lower + rand() % mo_range : 0u;

  son.resize(father_offset + m_sz - mother_offset);
  daughter.resize(father_offset + m_sz - mother_offset);
  std::copy(father.begin(), father.begin() + father_offset, son.begin());
  std::copy(mother.begin(), mother.begin() + mother_offset, daughter.begin());
  const size_t num_vars=random.get_num_vars();
  for (size_t f=father_offset, m=mother_offset; m < m_sz; ++f, ++m)
    fix_result_ops(son[f]=mother[m], m, f, num_vars);
  for (size_t m=mother_offset, f=father_offset; f < f_sz; ++m, ++f)
    fix_result_ops(daughter[m]=father[f], f, m, num_vars);
}