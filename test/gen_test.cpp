#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <utility>

#include "gen.h"

using namespace ::testing;
using namespace std;
using namespace generators;

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

TEST(test_gen, test_reset)
{
    gen fib(fib_factory);
    EXPECT_EQ(fib.get(), 0);
    EXPECT_EQ(fib.index(), 0);
    EXPECT_EQ(fib.reset(), 0);
    EXPECT_EQ(fib.get(), 0);
    EXPECT_EQ(fib.index(), 0);
}

TEST(test_gen, test_fwd_rew)
{
    gen fib(fib_factory);
    EXPECT_EQ(fib.fwd(3), 2);
    EXPECT_EQ(fib.rew(3), 0);
    EXPECT_EQ(fib.index(), 0);
}

TEST(test_gen, test_fixed_point)
{
    gen fib(fib_factory);
    auto first = fib[fib.fwd(5)];
    EXPECT_EQ(first, fib[5]);
}

TEST(test_gen, test_seek_rew)
{
    gen fib(fib_factory);
    EXPECT_EQ(fib[6], 8);
    EXPECT_EQ(fib[7], 13);
    auto first = fib.rew(2);
    EXPECT_EQ(first, fib[5]);
}
