for bound in 2 6 12 17 21 40 100 500 10000 1000000 10000000 1000000000 ; do
  ${tool} --unroll ${bound} --loop-limit ${bound} "$@" >>${out_file} 2>&1
  grep "This assertion can fail" ${out_file} >/dev/null
  error_found=$?
  if [ ${error_found} -eq 0 ]; then
    break
  fi
done