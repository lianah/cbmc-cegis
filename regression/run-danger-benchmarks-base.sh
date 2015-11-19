#!/bin/bash
export PATH=${PATH//cbmc-5190/cbmc-trunk-synth}
tool=cbmc
regression_dir=`pwd`
benchmark_dir=${regression_dir}/cegis-danger-base
result_file=${benchmark_dir}/result.log
timeout_time=300

rm ${result_file} 1>/dev/null 2>&1
cd ${regression_dir}
for benchmark in ${benchmark_dir}/*; do
  if [ -d ${benchmark} ] && [[ ${benchmark} == *"benchmark"* ]]; then
    file=${benchmark}/main.c
    tool_file=${regression_dir}/main-tool.c
    out_file=${benchmark}/main.out
    cp ${file} ${tool_file}
    error_found=1
    start_time=$(date +%s.%N)
    timeout --kill-after=10 ${timeout_time} ${tool} --danger --cegis-genetic ${tool_file} >>${out_file} 2>&1
    end_time=$(date +%s.%N)
    duration=$(echo "${end_time} - ${start_time}" | bc)
    grep "Programs:" ${out_file} >/dev/null
    error_found=$?
    if [ ${error_found} -eq 0 ]; then
      echo -e "${benchmark}\tFALSE\t${duration}" >>"${result_file}"
    else
      echo -e "${benchmark}\tUNKNOWN\t${duration}" >>"${result_file}"
    fi
    rm ${tool_file} >/dev/null 2>&1
  fi
done
