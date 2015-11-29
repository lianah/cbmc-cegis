#!/bin/bash
tool_path='/users/pkesseli/software/cpp/seahorn/seahorn-svcomp15'
tool=${tool_path}/bin/seahorn-svcomp-par.py
regression_dir=`pwd`
benchmark_dir=${regression_dir}/cegis-seahorn
result_file=${benchmark_dir}/result.log
timeout_time=300

rm ${result_file} 1>/dev/null 2>&1
cd ${tool_path}
for benchmark in ${benchmark_dir}/*; do
  if [ -d ${benchmark} ] && [[ ${benchmark} == *"benchmark"* ]]; then
    file=${benchmark}/main.c
    tool_file=${benchmark}/main-tool.c
    out_file=${benchmark}/main.out
    echo -e "#define __CPROVER_assert(cond, name) if(!(cond)) __VERIFIER_error()\n" >${tool_file}
    cat ${file} >>${tool_file}
    start_time=$(date +%s.%N)
    timeout --kill-after=10 ${timeout_time} ${tool} ${tool_file} >${out_file} 2>&1
    end_time=$(date +%s.%N)
    duration=$(echo "${end_time} - ${start_time}" | bc)
    rm ${tool_file}

    grep "Result TRUE" ${out_file} >/dev/null
    is_true=$?
    grep "Result FALSE" ${out_file} >/dev/null
    is_false=$?
    if [ ${is_true} -eq 0 ]; then
      echo -e "${benchmark}\tTRUE\t${duration}" >>"${result_file}"
    elif [ ${is_false} -eq 0 ]; then
      echo -e "${benchmark}\tFALSE\t${duration}" >>"${result_file}"
    else
      echo -e "${benchmark}\tUNKNOWN\t${duration}" >>"${result_file}"
    fi
  fi
done
cd ${regression_dir}
