for bound in 2 6 12 17 21 40 100 500 10000 1000100 ; do
  cbmc --unwind ${bound} "$@" >>${out_file} 2>&1
  grep "VERIFICATION FAILED" ${out_file} >/dev/null
  error_found=$?
  if [ ${error_found} -eq 0 ]; then
    break
  fi
done