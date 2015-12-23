template<class loop_itert>
void restrict_bv_size(invariant_programt &prog, loop_itert first_loop,
    const loop_itert &last_loop, const size_t width_in_bits)
{
  restrict_bv_size(prog, width_in_bits);
  for (; first_loop != last_loop; ++first_loop)
    restrict_bv_size(first_loop->guard, width_in_bits);
}
