#include <goto-programs/safety_checker.h>

#include <cbmc/cbmc_solvers.h>
#include <cbmc/bmc.h>

template<class verifyt>
limited_wordsize_verifyt<verifyt>::limited_wordsize_verifyt(verifyt &verifier,
    const std::function<void(size_t)> set_wordsize) :
    verifier(verifier), set_wordsize(set_wordsize), is_success(false)
{
}

template<class verifyt>
limited_wordsize_verifyt<verifyt>::~limited_wordsize_verifyt()
{
}

namespace
{
template<class verifyt>
void get_ces(verifyt &verifier,
    typename limited_wordsize_verifyt<verifyt>::counterexamplest &ces)
{
  std::copy(verifier.counterexamples_begin(), verifier.counterexamples_end(),
      std::back_inserter(ces));
}
}

template<class verifyt>
void limited_wordsize_verifyt<verifyt>::verify(const candidatet &candidate)
{
  ces.clear();
  counterexamplest full_width_ces;
  const size_t full_width=32u;
  for (size_t width=2u; width < full_width; width*=2u)
  {
    set_wordsize(width);
    verifier.verify(candidate);
    if (verifier.has_counterexamples()) return get_ces(verifier, ces);
    if (full_width_ces.empty())
    {
      set_wordsize(full_width);
      verifier.verify(candidate);
      if (verifier.has_counterexamples()) get_ces(verifier, full_width_ces);
      else
      {
        is_success=verifier.success();
        return;
      };
    }
  }
  assert(!full_width_ces.empty());
  std::copy(full_width_ces.begin(), full_width_ces.end(),
      std::back_inserter(ces));
}

template<class verifyt>
typename limited_wordsize_verifyt<verifyt>::const_iterator limited_wordsize_verifyt<
    verifyt>::counterexamples_begin() const
{
  return ces.begin();
}

template<class verifyt>
typename limited_wordsize_verifyt<verifyt>::const_iterator limited_wordsize_verifyt<
    verifyt>::counterexamples_end() const
{
  return ces.end();
}

template<class verifyt>
bool limited_wordsize_verifyt<verifyt>::has_counterexamples() const
{
  return !ces.empty();
}

template<class verifyt>
bool limited_wordsize_verifyt<verifyt>::success() const
{
  return is_success;
}
