#pragma once

// Toy generator framework showing how to add different
// control-flows to generators implemented as a lambdas
// and handle them in a generic way.

#include <cassert>
#include <iostream>
#include <utility>

namespace generators {

enum class gen_ctl : int { FWD_N, REW_N, INDEX, SEEK_INDEX, RESET, GET };

#define GEN_DISPATCH(ctl)                                                      \
  {                                                                            \
    switch ((ctl)) {                                                           \
    case gen_ctl::RESET: {                                                     \
      return reset();                                                          \
    }                                                                          \
    case gen_ctl::FWD_N: {                                                     \
      return fwd_n(n);                                                         \
    }                                                                          \
    case gen_ctl::REW_N: {                                                     \
      return rew_n(n);                                                         \
    }                                                                          \
    case gen_ctl::INDEX: {                                                     \
      return index();                                                          \
    }                                                                          \
    case gen_ctl::SEEK_INDEX: {                                                \
      return seek_index(n);                                                    \
    }                                                                          \
    case gen_ctl::GET: {                                                       \
      return get();                                                            \
    }                                                                          \
    default: {                                                                 \
      assert(false);                                                           \
      return -1;                                                               \
    }                                                                          \
    }                                                                          \
  }

#define GEN_GENERIC(ctl)                                                       \
  {                                                                            \
    const auto index = [&i] { return i; };                                     \
    const auto fwd_n = [&fwd, &get](int n) {                                   \
      for (auto _j = 0; _j < n; ++_j) {                                        \
        fwd();                                                                 \
      }                                                                        \
      return get();                                                            \
    };                                                                         \
    const auto rew_n = [&rew, &get](int n) {                                   \
      for (auto _j = 0; _j < n; ++_j) {                                        \
        rew();                                                                 \
      }                                                                        \
      return get();                                                            \
    };                                                                         \
    const auto seek_index = [&fwd_n, &rew_n, &reset, &index](int n) {          \
      const auto i = index();                                                  \
      if (n >= i) {                                                            \
        return fwd_n(n - i);                                                   \
      }                                                                        \
      if (i - n > n) {                                                         \
        reset();                                                               \
        return fwd_n(n);                                                       \
      }                                                                        \
      return rew_n(i - n);                                                     \
    };                                                                         \
    GEN_DISPATCH((ctl));                                                       \
  }

template <typename Factory> struct gen {
  using Lambda = decltype(std::declval<Factory>()());
  using Value = decltype(std::declval<Lambda>()());
  gen(Factory factory) : m_lambda(factory()) {}
  Value reset() { return m_lambda(gen_ctl::RESET); }
  Value fwd(int n = 1) { return m_lambda(gen_ctl::FWD_N, n); }
  Value rew(int n = 1) { return m_lambda(gen_ctl::REW_N, n); }
  Value get() { return m_lambda(gen_ctl::GET); }
  Value seek(int n) { return m_lambda(gen_ctl::SEEK_INDEX, n); }
  Value operator[](int n) { return seek(n); }
  Value operator()() { return m_lambda(); }
  int index() { return m_lambda(gen_ctl::INDEX); }
  void print(std::ostream &stream) {
    stream << "Current index: " << index() << " value: " << get() << ".\n";
  }

private:
  Lambda m_lambda;
};

template <typename T> std::ostream &operator<<(std::ostream &stream, gen<T> &g) {
  g.print(stream);
  return stream;
}

} // namespace gen