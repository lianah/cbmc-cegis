#include <cegis/genetic/instruction_set_info_factory.h>

instruction_set_info_factoryt::instruction_set_info_factoryt(
    const goto_programt &body) :
    body_provider([&body] () -> const goto_programt &
    { return body;})
{
}

instruction_set_info_factoryt::~instruction_set_info_factoryt()
{
}

const tournament_selectt::instruction_set_infot &instruction_set_info_factoryt::get_info() const
{
  if (!info.empty()) return info;
  const goto_programt &body=body_provider();
  // TODO: Extract instruction set info.
  return info;
}
