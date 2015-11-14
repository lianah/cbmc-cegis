#include <util/constant_width.h>
#include <util/mp_arith.h>

#include <cegis/danger/options/danger_program.h>
#include <cegis/danger/instrument/meta_variables.h>
#include <cegis/danger/constant/literals_constant_strategy.h>
#include <cegis/genetic/genetic_constant_strategy.h>
#include <cegis/genetic/genetic_constant_strategy.h>

namespace
{
const char NONDET_CONSTANT_PREFIX[]="DANGER_CONSTANT_NONDET_";
std::string get_ndt_name(size_t index)
{
  std::string name(NONDET_CONSTANT_PREFIX);
  return name+=integer2string(index);
}

const char CONSTANT_PREFIX[]="DANGER_CONSTANT_";
std::string get_name(size_t index)
{
  std::string name(CONSTANT_PREFIX);
  return name+=integer2string(index);
}
}

// XXX: Debug
#include <iostream>
// XXX: Debug

size_t genetic_constant_strategy(danger_programt &prog, const size_t max_length)
{
  symbol_tablet &st=prog.st;
  goto_functionst &gf=prog.gf;
  goto_programt::targett pos=prog.danger_range.begin;
  const std::vector<constant_exprt> literals(collect_literal_constants(prog));
  size_t max_word_width=0u;
  size_t const_index=0u;
  // XXX: Benchmark performance
  for (const constant_exprt &expr : literals)
  {
    // XXX: Debug
    /*std::cout << "<id>" << const_index << "</id>" << std::endl;
    std::cout << "<id>" << expr.to_string() << "</id>" << std::endl;*/
    // XXX: Debug
    const std::string base_name(get_name(const_index++));
    pos=declare_danger_variable(st, gf, pos, base_name, expr.type());
    pos=assign_danger_variable(st, gf, pos, base_name, expr);
    max_word_width=std::max(max_word_width, get_min_word_width(expr));
  }
  return max_word_width;
  /*typet type=danger_meta_type(); // XXX: Currently single data type
   type.set(ID_C_constant, true);
   // TODO: Multiply by number of programs and loops?
   for (size_t i=0; i < max_length; ++i)
   pos=declare_danger_variable(st, gf, pos, get_ndt_name(const_index++), type);*/
}
