#include <dlfcn.h> // TODO: Windows MinGW/VS equivalent?

#include <cstdlib>
#include <fstream>

#include <util/substitute.h>
#include <util/tempfile.h>
#include <cegis/invariant/meta/literals.h>
#include <cegis/genetic/dynamic_test_runner_helper.h>

void close_fitness_tester_library(fitness_lib_handlet &handle,
    fitness_testert &fitness_tester)
{
  if (fitness_tester && handle)
  {
    dlclose(handle);
    handle=0;
    fitness_tester=0;
  }
}

namespace
{
void implement_deserialise(std::string &source)
{
  source+=
      "#include <string.h>\n\n"
          "#define " CEGIS_PREFIX "next_arg() argv[" CEGIS_PREFIX "deserialise_index++]\n"
      "#define " CEGIS_PREFIX "deserialise_init() unsigned int " CEGIS_PREFIX "deserialise_index=" CEGIS_PREFIX "first_prog_offset\n"
      "#define " CEGIS_PREFIX "declare_prog(var_name, sz) const size_t sz=" CEGIS_PREFIX "next_arg(); \\\n"
      "  struct " CEGIS_PREFIX "instructiont var_name[sz]; \\\n"
      "for (unsigned int i=0; i < sizeof(var_name) / sizeof(struct " CEGIS_PREFIX "instructiont); ++i) \\\n"
      "{ \\\n"
      "  var_name[i].opcode=" CEGIS_PREFIX "next_arg(); \\\n"
      "  var_name[i].op0=" CEGIS_PREFIX "next_arg(); \\\n"
      "  var_name[i].op1=" CEGIS_PREFIX "next_arg(); \\\n"
      "  var_name[i].op2=" CEGIS_PREFIX "next_arg(); \\\n"
      "}\n"
      "#define " CEGIS_PREFIX "deserialise_x0(var_name) var_name=" CEGIS_PREFIX "next_arg()\n"
      "#define " CEGIS_PREFIX "ce_value_init() unsigned int " CEGIS_PREFIX "ce_index=0u\n"
      "#define " CEGIS_PREFIX "ce_value() argv[" CEGIS_PREFIX "ce_index++]\n";

}

void add_default_return(std::string &source)
{
  source.replace(source.rfind('}'), 1, "return 0;}");
}

void write_file(const char * const path, const std::string &content)
{
  std::ofstream ofs(path);
  ofs << content;
}

#define SOURCE_FILE_PREFIX "concrete_test"
#define SOURCE_FILE_SUFFIX ".c"
#ifndef _WIN32
#define COMPILE_COMMAND "gcc -std=c99 -g0 -O2 -shared -rdynamic -fPIC "
//#define COMPILE_COMMAND "gcc -std=c99 -g3 -O0 -shared -rdynamic -fPIC "
#else
#define COMPILE_COMMAND "gcc -std=c99 -g0 -O2 -shared "
#endif
#define ARTIFACT_SEPARATOR " -o "
#define FUNC "__CPROVER_cegis_test_fitness"
#define COMPLING_FAILED "Compiling test runner failed."
#define OPEN_LIB_FAILED "Opening fitness test library failed."
#define LOAD_FUNC_FAILED "Loading fitness test function failed."
}

void prepare_fitness_tester_library(fitness_lib_handlet &handle,
    fitness_testert &fitness_tester,
    const std::function<std::string(void)> &source_code_provider,
    const std::string &library_file_path)
{
  if (fitness_tester) return;
  const temporary_filet source_file(SOURCE_FILE_PREFIX, SOURCE_FILE_SUFFIX);
  const std::string source_file_name(source_file());
  std::string source;
  implement_deserialise(source);
  source+=source_code_provider();
  substitute(source, "int main(const int argc, const char * const argv[])\n"
      "{\n", "int " FUNC "(const unsigned int argv[])\n"
  "{\n"
  "memset(" CEGIS_OPS ", 0, sizeof(" CEGIS_OPS "));\n"
  "memset(" CEGIS_RESULT_OPS ", 0, sizeof(" CEGIS_RESULT_OPS "));\n");
  add_default_return(source);
  write_file(source_file_name.c_str(), source);
  std::string compile_command(COMPILE_COMMAND);
  compile_command+=source_file_name;
  compile_command+=ARTIFACT_SEPARATOR;
  compile_command+=library_file_path;
  const int result=system(compile_command.c_str());
  if (result) throw std::runtime_error(COMPLING_FAILED);
  handle=dlopen(library_file_path.c_str(), RTLD_NOW);
  if (!handle)
  {
    perror(OPEN_LIB_FAILED);
    throw std::runtime_error(OPEN_LIB_FAILED);
  }
  fitness_tester=(fitness_testert) dlsym(handle, FUNC);
  char *error=0;
  if ((error=dlerror()))
  {
    perror(error);
    throw std::runtime_error(LOAD_FUNC_FAILED);
  }
}
