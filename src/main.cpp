#include <iostream>

#include "gen.h"

using namespace generators;
using namespace std;

// Factorial generator
constexpr auto fact_factory() {
  return [a = 1, i = 0](gen_ctl ctl = gen_ctl::FWD_N, int n = 1) mutable {
    const auto get = [&a]() { return a; };
    const auto fwd = [&a, &i, &get]() {
      a *= ++i;
      return get();
    };
    const auto rew = [&a, &i, &get]() {
      a /= i--;
      return get();
    };
    const auto reset = [&a, &i, &get]() {
      a = 1;
      i = 0;
      return get();
    };
    GEN_GENERIC(ctl);
  };
}

// Fibonacci generator
constexpr auto fib_factory() {
  return
      [a = 0, b = 1, i = 0](gen_ctl ctl = gen_ctl::FWD_N, int n = 1) mutable {
        const auto get = [&a]() { return a; };
        const auto fwd = [&a, &b, &i, &get]() {
          const auto a1 = exchange(a, b);
          b = a1 + b;
          ++i;
          return get();
        };
        const auto rew = [&a, &b, &i, &get]() {
          const auto a1 = exchange(a, b - a);
          b = a1;
          --i;
          return get();
        };
        const auto reset = [&a, &b, &i, &get]() {
          a = 0;
          b = 1;
          i = 0;
          return get();
        };
        GEN_GENERIC(ctl);
      };
}

template <typename T> int gen_test(gen<T> &g) {
  constexpr auto it = 5;
  for (auto i = 0; i < it; ++i) {
    cout << g() << '\n';
  }
  g.reset();
  cout << "Generator reset.\n";
  cout << g;
  for (auto i = 0; i < it; ++i) {
    cout << g() << '\n';
  }
  constexpr auto n = 2;
  cout << g;
  cout << "Forward " << n << " steps.\n";
  cout << g.fwd(n) << '\n';
  cout << g;
  cout << "Rewind 1 step.\n";
  cout << g.rew(1) << '\n';
  cout << g;
  cout << "Rewind " << n << " steps.\n";
  cout << g.rew(n) << '\n';
  cout << g;
  constexpr auto index = 6;
  cout << "Seek " << index << ".\n";
  cout << g[index] << '\n';
  cout << g;
  return g.reset();
}

int main() {
  cout << "Factorial generator test...\n";
  gen fact(fact_factory);
  gen_test(fact);
  cout << "\nFibonacci generator test...\n";
  gen fib(fib_factory);
  return gen_test(fib);
}
