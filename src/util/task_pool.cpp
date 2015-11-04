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
void execute_and_remove(task_poolt::handlerst &handlers, const pid_t pid,
    const int status)
{
  const task_poolt::handlerst::iterator it=handlers.find(pid);
  if (handlers.end() != it)
  {
    it->second(status);
    handlers.erase(it);
  }
}

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
    execute_and_remove(handlers, id, task.second);
    tasks.erase(id);
  }
#else
  NOT_SUPPORTED();
#endif
}
}

#define FORK_ERROR "Fork system call failed."

task_poolt::task_idt task_poolt::schedule(const taskt &task)
{
  cleanup(task_ids, handlers);
#ifndef _WIN32
  const pid_t child_pid=fork();
  if (child_pid == -1)
  {
    perror(FORK_ERROR);
    throw std::runtime_error(FORK_ERROR);
  }
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
  execute_and_remove(handlers, id, status);
  task_ids.erase(task_id);
#else
  NOT_SUPPORTED();
#endif
}

void task_poolt::join_all()
{
#ifndef _WIN32
  const size_t num_children=task_ids.size();
  int status;
  for (size_t i=0; i < num_children; ++i)
  {
    const pid_t pid=waitpid(-1, &status, 0);
    assert(pid > 0);
    execute_and_remove(handlers, pid, status);
  }
  task_ids.clear();
  assert(handlers.empty());
#else
  NOT_SUPPORTED();
#endif
}
