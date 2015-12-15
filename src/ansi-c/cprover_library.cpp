/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <sstream>

#include <util/config.h>
#include <util/replace_symbol.h>

#include "cprover_library.h"
#include "ansi_c_language.h"

/*******************************************************************\

Function: add_cprover_library

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

struct cprover_library_entryt
{
  const char *function;
  const char *model;
} cprover_library[]=
#include "cprover_library.inc"

unsigned get_cprover_library_text(std::string &result,
  const std::set<irep_idt> &functions,
  const symbol_tablet &symbol_table,
  const std::string library_text_prefix)
{
  std::ostringstream library_text;

  library_text <<
    "#line 1 \"<builtin-library>\"\n"
    "#undef inline\n";

  if(config.ansi_c.string_abstraction)
    library_text << "#define __CPROVER_STRING_ABSTRACTION\n";

  library_text << library_text_prefix;

  unsigned count=0;

  for(cprover_library_entryt *e=cprover_library;
      e->function!=NULL;
      e++)
  {
    irep_idt id=e->function;

    if(functions.find(id)!=functions.end())
    {
      symbol_tablet::symbolst::const_iterator old=
        symbol_table.symbols.find(id);

      if(old!=symbol_table.symbols.end() &&
         old->second.value.is_nil())
      {
        count++;
        library_text << e->model << '\n';
      }
    }
  }
  result=library_text.str();
  return count;
}

void add_cprover_library(
  const std::set<irep_idt> &functions,
  symbol_tablet &symbol_table,
  message_handlert &message_handler,
  const std::string library_text_prefix)
{
  if(config.ansi_c.lib==configt::ansi_ct::libt::LIB_NONE)
    return;

  std::string library_text;
  unsigned count=get_cprover_library_text(library_text, functions,
      symbol_table, library_text_prefix);

  if(count>0)
  {
    std::istringstream in(library_text);
    
    // switch mode temporarily from gcc C++ to gcc C flavour
    configt::ansi_ct::flavourt old_mode=config.ansi_c.mode;
    
    if(config.ansi_c.mode==configt::ansi_ct::flavourt::MODE_GCC_CPP)
      config.ansi_c.mode=configt::ansi_ct::flavourt::MODE_GCC_C;
    
    ansi_c_languaget ansi_c_language;
    ansi_c_language.set_message_handler(message_handler);
    ansi_c_language.parse(in, "");
    
    ansi_c_language.typecheck(symbol_table, "<built-in-library>");

    config.ansi_c.mode=old_mode;
  }
}

