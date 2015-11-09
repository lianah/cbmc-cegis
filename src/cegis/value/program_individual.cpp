#include <cegis/value/program_individual.h>

/*program_individualt::instructiont::instructiont() :
    opcode(0u)
{
}

program_individualt::instructiont::instructiont(const instructiont &other) :
    opcode(other.opcode), ops(other.ops)
{
}

program_individualt::instructiont::~instructiont()
{
}

program_individualt::instructiont &program_individualt::instructiont::operator=(
    const instructiont &other)
{
  opcode=other.opcode;
  ops.resize(other.ops.size());
  std::copy(other.ops.begin(), other.ops.end(), ops.begin());
  return *this;
}*/

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
