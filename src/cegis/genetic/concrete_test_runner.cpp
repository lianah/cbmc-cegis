#include <cstdlib>
#include <fstream>

#include <cegis/genetic/concrete_test_runner.h>

#define EXECUTABLE_PREFIX "test_runner"
#define EXECUTABLE_SUFFIX ".exe"
#define SOURCE_FILE_PREFIX "concrete_test"
#define SOURCE_FILE_SUFFIX ".c"

concrete_test_runnert::concrete_test_runnert(
    const std::function<std::string(void)> &source_code_provider) :
    source_code_provider(source_code_provider), executable(EXECUTABLE_PREFIX,
    EXECUTABLE_SUFFIX), executable_compiled(false)
{
}

concrete_test_runnert::~concrete_test_runnert()
{
}

namespace
{
void write_file(const char * const path, const std::string &content)
{
  std::ofstream ofs(path);
  ofs << content;
}

#define COMPILE_COMMAND "gcc "
#define EXECUTABLE_SEPARATOR " -o "
#define COMPLING_FAILED "Compiling test runner failed."

void prepare_executable(bool &executable_compiled,
    const std::function<std::string(void)> &source_code_provider,
    const std::string &executable)
{
  if (executable_compiled) return;
  const temporary_filet source_file(SOURCE_FILE_PREFIX, SOURCE_FILE_SUFFIX);
  const std::string source_file_name(source_file());
  const std::string source(source_code_provider());
  write_file(source_file_name.c_str(), source);
  std::string compile_command(COMPILE_COMMAND);
  compile_command+=source_file_name;
  compile_command+=EXECUTABLE_SEPARATOR;
  compile_command+=executable;
  const int result=system(compile_command.c_str());
  if (result) throw std::runtime_error(COMPLING_FAILED);
  executable_compiled=true;
}
}

void concrete_test_runnert::run_test(individualt &ind,
    const counterexamplet &ce)
{
  const std::string exe(executable());
  prepare_executable(executable_compiled, source_code_provider, exe);
}

void concrete_test_runnert::join()
{
}
