#include <cegis/danger/constant/add_constant.h>

#include <cegis/genetic/genetic_constant_strategy.h>

size_t genetic_constant_strategy(danger_programt &program,
    const size_t max_length)
{
  for (size_t i=0; i < max_length; ++i)
    add_danger_constant(program);
  return max_length;
}
