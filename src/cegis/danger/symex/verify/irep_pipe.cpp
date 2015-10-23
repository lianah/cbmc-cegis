#include <exception>
#include <sstream>

#ifndef _WIN32
#include <unistd.h>
#endif

#include <xmllang/xml_parser.h>
#include <util/xml_irep.h>

#include <cegis/danger/symex/verify/irep_pipe.h>

namespace
{
const char IREP_PIPE[]="irep_pipe";
}

irep_pipet::irep_pipet() :
    read_closed(false), write_closed(false), close_on_destuction(false)
{
#ifndef _WIN32
  if (pipe(fd))
  {
    perror(IREP_PIPE);
    throw std::runtime_error("Error creating pipe.");
  }
#endif
}

irep_pipet::~irep_pipet()
{
  if (close_on_destuction)
  {
    if (!read_closed) close_read();
    if (!write_closed) close_write();
  }
}

void irep_pipet::close_read()
{
  assert(!read_closed);
#ifndef _WIN32
  close(fd[0u]);
#endif
  read_closed=true;
}

void irep_pipet::close_write()
{
  assert(!write_closed);
#ifndef _WIN32
  close(fd[1u]);
#endif
  write_closed=true;
}

namespace
{
const char SDU[]="sdu";
}

void irep_pipet::send(const irept &sdu) const
{
#ifndef _WIN32
  assert(!write_closed);
  xmlt xml(SDU);
  convert(sdu, xml);
  std::ostringstream os;
  xml.output(os);
  const std::string data(os.str());
  const size_t size=data.size() + 1;
  const char * offset=data.c_str();
  for (size_t written=0; written < size;)
  {
    const ssize_t result=write(fd[1u], offset + written, size - written);
    if (result == -1)
    {
      perror(IREP_PIPE);
      throw std::runtime_error("Error writing pipe.");
    }
    written+=result;
    offset+=result;
  }
#else
  assert(!"Pipes currently not supported on Windows.");
#endif
}

namespace
{
const size_t READ_BUFFER_SIZE=1024;
}

void irep_pipet::receive(irept &sdu) const
{
#ifndef _WIN32
  assert(!read_closed);
  std::string data;
  char buffer[READ_BUFFER_SIZE];
  do
  {
    const ssize_t result=read(fd[0u], buffer, sizeof(buffer));
    if (result == -1)
    {
      perror(IREP_PIPE);
      throw std::runtime_error("Error reading pipe.");
    }
    if (result > 0) data.append(buffer, result);
    else break;
  } while (true);
  data.erase(data.end() - 1);
  std::istringstream is(data);
  xmlt xml;
  null_message_handlert msg;
  parse_xml(is, "", msg, xml);
  convert(xml, sdu);
#else
  assert(!"Pipes currently not supported on Windows.");
#endif
}

void irep_pipet::auto_close()
{
  close_on_destuction=true;
}