/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_ANSI_C_CPROVER_LIBRARY_H
#define CPROVER_ANSI_C_CPROVER_LIBRARY_H

#include <set>

#include <util/symbol_table.h>
#include <util/message.h>

unsigned get_cprover_library_text(std::string &result,
    const std::set<irep_idt> &functions, const symbol_tablet &symbol_table,
    const std::string library_text_prefix="");

void add_cprover_library(
  const std::set<irep_idt> &functions,
  symbol_tablet &symbol_table,
  message_handlert &message_handler,
  const std::string library_text_prefix="");

#endif
