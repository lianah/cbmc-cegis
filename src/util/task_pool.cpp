#ifndef _WIN32
#include <sys/wait.h>
#endif
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <sstream>

#include <util/irep.h>
#include <util/task_pool.h>

task_poolt::task_poolt()
{
}

task_poolt::~task_poolt()
{
}

#ifdef _WIN32
#define NOT_SUPPORTED() assert(!"task_poolt not supported on Windows.")
#endif

namespace
{
void cleanup(task_poolt::task_idst &tasks, task_poolt::handlerst &handlers)
{
#ifndef _WIN32
  std::map<task_poolt::task_idt, int> joined;
  int status;
  for (pid_t child_pid=waitpid(-1, &status, WNOHANG); child_pid > 0; child_pid=
      waitpid(-1, &status, WNOHANG))
    joined.insert(std::make_pair(child_pid, status));
  for (const std::pair<task_poolt::task_idt, int> &task : joined)
  {
    const task_poolt::task_idt id=task.first;
    const task_poolt::handlerst::iterator it=handlers.find(id);
    if (handlers.end() != it)
    {
      it->second(task.second);
      handlers.erase(it);
    }
    tasks.erase(id);
  }
#else
  NOT_SUPPORTED();
#endif
}
}

task_poolt::task_idt task_poolt::schedule(const taskt &task)
{
  cleanup(task_ids, handlers);
#ifndef _WIN32
  const pid_t child_pid=fork();
  if (child_pid)
  {
    task_ids.insert(child_pid);
    return child_pid;
  }
  try
  {
    exit(task());
  } catch (...)
  {
    exit(EXIT_FAILURE);
  }
#else
  NOT_SUPPORTED();
#endif
}

task_poolt::task_idt task_poolt::schedule(const taskt &task,
    const on_completet &on_complete)
{
  cleanup(task_ids, handlers);
  const task_poolt::task_idt id=schedule(task);
  handlers.insert(std::make_pair(id, on_complete));
  return id;
}

void task_poolt::join(const task_idt id)
{
  const task_idst::iterator task_id=task_ids.find(id);
  if (task_ids.end() == task_id) return;
#ifndef _WIN32
  int status;
  waitpid(id, &status, 0);
  const handlerst::iterator it=handlers.find(id);
  if (handlers.end() != it)
  {
    it->second(status);
    handlers.erase(it);
  }
  task_ids.erase(task_id);
#endif
}

void task_poolt::join_all()
{
  for (const task_idt id : task_ids)
    join(id);
}
