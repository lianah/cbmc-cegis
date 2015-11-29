#!/bin/bash
tool_path='/users/pkesseli/software/cpp/cpachecker/CPAchecker-1.4-svcomp16c-unix'
tool=${tool_path}/scripts/cpa.sh
regression_dir=`pwd`
benchmark_dir=${regression_dir}/cegis-cpa
result_file=${benchmark_dir}/result.log
timeout_time=300

rm ${result_file} 1>/dev/null 2>&1
cd ${tool_path}
for benchmark in ${benchmark_dir}/*; do
  if [ -d ${benchmark} ] && [[ ${benchmark} == *"benchmark"* ]]; then
    file=${benchmark}/main.c
    tool_file=${benchmark}/main-tool.c
    out_file=${benchmark}/main.out
    echo -e "#define __CPROVER_assert(cond, name) if(!(cond)) ERROR: return (-1)\n" >${tool_file}
    cat ${file} >>${tool_file}
    start_time=$(date +%s.%N)
    timeout --kill-after=10 ${timeout_time} ${tool} -preprocess -sv-comp15 -timelimit ${timeout_time} -disable-java-assertions -heap 10000m -setprop spec.singlePathMatching=false -setprop parser.transformTokensToLines=true ${tool_file} >${out_file} 2>&1
    end_time=$(date +%s.%N)
    duration=$(echo "${end_time} - ${start_time}" | bc)
    rm ${tool_file}

    grep "Verification result: TRUE" ${out_file} >/dev/null
    is_true=$?
    grep "Verification result: FALSE" ${out_file} >/dev/null
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
