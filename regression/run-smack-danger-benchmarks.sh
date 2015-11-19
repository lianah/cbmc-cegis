#!/bin/bash
export PATH=/users/pkesseli/software/cpp/mono/mono-3.8.0/target/i686-linux/bin:${PATH}:/users/pkesseli/software/cpp/llvm/llvm-3.5.0.src/target/i686-linux/bin:/users/pkesseli/software/cpp/smack/smack-trunk/target/i686-linux/bin
export BOOGIE="mono /users/pkesseli/software/cpp/boogie/boogie-535d0b56285a436251bd955f3da9fb550c1a2e79/Binaries/Boogie.exe"
export CORRAL="mono /users/pkesseli/software/cpp/corral/corral-trunk/bin/Release/corral.exe"
tool_path='/users/pkesseli/software/cpp/smack/smack-trunk/target/i686-linux/bin'
export tool=${tool_path}/smackverify.py
regression_dir=`pwd`
benchmark_dir=${regression_dir}/cegis-smack
result_file=${benchmark_dir}/result.log
timeout_time=300

rm ${result_file} 1>/dev/null 2>&1
cd ${tool_path}
for benchmark in ${benchmark_dir}/*; do
  if [ -d ${benchmark} ] && [[ ${benchmark} == *"benchmark"* ]]; then
    file=${benchmark}/main.c
    tool_file=${benchmark}/main-tool.c
    export out_file=${benchmark}/main.out
    echo -e "#define __CPROVER_assert(cond, name) assert(cond)\n" >${tool_file}
    cat ${file} >>${tool_file}
    rm ${out_file} >/dev/null 2>&1
    error_found=1
    start_time=$(date +%s.%N)
    timeout --kill-after=10 ${timeout_time} bash ${regression_dir}/smack-wrapper.sh ${tool_file}
    end_time=$(date +%s.%N)
    duration=$(echo "${end_time} - ${start_time}" | bc)
    grep "This assertion can fail" ${out_file} >/dev/null
    error_found=$?
    grep "Program has no bugs" ${out_file} >/dev/null
    no_bugs=$?
    if [ ${error_found} -eq 0 ]; then
      echo -e "${benchmark}\tFALSE\t${duration}" >>"${result_file}"
    elif [ ${no_bugs} -eq 0 ]; then
      echo -e "${benchmark}\tTRUE\t${duration}" >>"${result_file}"
    else
      echo -e "${benchmark}\tUNKNOWN\t${duration}" >>"${result_file}"
    fi
    rm ${tool_file}
  fi
done
cd ${regression_dir}
