#include <cegis/value/program_individual.h>

program_individualt::instructiont::opst::value_type &program_individualt::instructiont::operator [](
    const opst::size_type n)
{
  return ops[n];
}

const program_individualt::instructiont::opst::value_type &program_individualt::instructiont::operator [](
    const opst::size_type n) const
{
  return ops[n];
}
