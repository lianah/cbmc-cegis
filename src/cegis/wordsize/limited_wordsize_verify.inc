#include <goto-programs/safety_checker.h>

#include <cbmc/cbmc_solvers.h>
#include <cbmc/bmc.h>

#define MIN_WORD_WIDTH 2u

template<class verifyt>
limited_wordsize_verifyt<verifyt>::limited_wordsize_verifyt(optionst &options,
    verifyt &verifier, const std::function<void(size_t)> set_wordsize) :
    options(options), verifier(verifier), set_wordsize(set_wordsize), is_success(
        false), word_width(MIN_WORD_WIDTH)
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
void limited_wordsize_verifyt<verifyt>::verify_full(counterexamplest &ces,
    const candidatet &candidate)
{
  verifier.verify(candidate);
  if (verifier.has_counterexamples()) get_ces(verifier, ces);
  else is_success=verifier.success();
}

namespace
{
#define WIDTH_OPT "cegis-word-width"
}

template<class verifyt>
void limited_wordsize_verifyt<verifyt>::verify(const candidatet &candidate)
{
  ces.clear();
  counterexamplest full_ces;
  const size_t full_width=32u;
  for (; word_width < full_width; word_width*=2u)
  {
    set_wordsize(word_width);
    options.set_option(WIDTH_OPT, static_cast<unsigned int>(word_width));
    verifier.verify(candidate);
    if (verifier.has_counterexamples()) return get_ces(verifier, ces);
    if (full_ces.empty())
    {
      set_wordsize(full_width);
      verify_full(full_ces, candidate);
      if (!verifier.has_counterexamples()) return;
    }
  }
  if (full_ces.empty()) verify_full(ces, candidate);
  else std::copy(full_ces.begin(), full_ces.end(), std::back_inserter(ces));
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
