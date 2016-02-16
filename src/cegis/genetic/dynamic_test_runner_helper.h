#ifndef CEGIS_DYNAMIC_TEST_RUNNER_HELPER_H_
#define CEGIS_DYNAMIC_TEST_RUNNER_HELPER_H_

#include <functional>

typedef int (*fitness_testert)(const unsigned int[]);
typedef void *fitness_lib_handlet;

#define LIBRARY_PREFIX "fitness_test"
#ifndef _WIN32
#define LIBRARY_SUFFIX ".so"
#else
#define LIBRARY_SUFFIX ".dll"
#endif

/**
 * @brief
 *
 * @details
 *
 * @param handle
 * @param fitness_tester
 * @param source_code_provider
 * @param library_file_path
 */
void prepare_fitness_tester_library(fitness_lib_handlet &handle,
    fitness_testert &fitness_tester,
    const std::function<std::string(void)> &source_code_provider,
    const std::string &library_file_path);

/**
 * @brief
 *
 * @details
 *
 * @param handle
 * @param fitness_tester
 */
void close_fitness_tester_library(fitness_lib_handlet &handle,
    fitness_testert &fitness_tester);

#endif /* CEGIS_DYNAMIC_TEST_RUNNER_HELPER_H_ */
