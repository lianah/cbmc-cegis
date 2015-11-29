#!/bin/bash
export PATH=${PATH//cbmc-5190/cbmc-trunk-synth}
tool=./cbmc-accelerate-wrapper.sh
regression_dir=`pwd`
benchmark_dir=${regression_dir}/cegis-cbmc-acc
result_file=${benchmark_dir}/result.log
timeout_time=300

rm ${result_file} 1>/dev/null 2>&1
cd ${regression_dir}
for benchmark in ${benchmark_dir}/*; do
  if [ -d ${benchmark} ] && [[ ${benchmark} == *"benchmark"* ]]; then
    export file=${benchmark}/main.c
    export tool_file=${regression_dir}/main-tool.o
    export out_file=${benchmark}/main.out
    #goto-cc ${file} -o ${tool_file} >${out_file} 2>&1
    #goto-instrument --accelerate  ${tool_file} ${tool_file} >>${out_file} 2>&1
    start_time=$(date +%s.%N)
    timeout --kill-after=10 ${timeout_time} bash ${tool} --no-unwinding-assertions ${tool_file}
    end_time=$(date +%s.%N)
    duration=$(echo "${end_time} - ${start_time}" | bc)
    grep "VERIFICATION FAILED" ${out_file} >/dev/null
    error_found=$?
    grep "VERIFICATION SUCCESSFUL" ${out_file} >/dev/null
    no_bugs=$?
    if [ ${error_found} -eq 0 ]; then
      echo -e "${benchmark}\tFALSE\t${duration}" >>"${result_file}"
    elif [ ${no_bugs} -eq 0 ]; then
      echo -e "${benchmark}\tTRUE\t${duration}" >>"${result_file}"
    else
      echo -e "${benchmark}\tUNKNOWN\t${duration}" >>"${result_file}"
    fi
    rm ${tool_file} >/dev/null 2>&1
  fi
done
