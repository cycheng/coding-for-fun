#include <cinttypes>
#include <cstdint>
#include <cstdio>

// Reference:
// [1] https://en.wikipedia.org/wiki/Montgomery_modular_multiplication
// [2] https://cryptography.fandom.com/wiki/Montgomery_reduction
// [3] https://cp-algorithms.com/algebra/montgomery_multiplication.html
// [4] https://github.com/Fattouche/RSA
// [5] https://github.com/calccrypto/uint128_t/blob/master/uint128_t.cpp
typedef __uint128_t uint128_t;
typedef __int128_t int128_t;
struct uint256_t;

static void print128(const char *prefix, const uint128_t &a);
static void print256(const char *prefix, const uint256_t &a);
uint128_t modexp(uint128_t x, uint128_t e, uint128_t n);

#define UINT128_C(high, low)                                                   \
  (((uint128_t)UINT64_C(high) << 64) | (UINT64_C(low)))

struct uint256_t {
  /// result (256-bit) = a * b
  ///
  /// test:
  ///   a = b = fbeab553608bdf65b2ab09bb910317f9
  ///   r = f7e616c17c2dc9735b47e453f14f580e322a313f6f357991aeff08f7a414b031
  static uint256_t mul(uint128_t a, uint128_t b) {
    //    a =   [ah64,     al64]
    // x) b =   [bh64,     bl64]
    // -----------------------------
    //                     al64*bl64
    //           ah64*bl64
    //           al64*bh64
    // ah64*bh64
    uint64_t ah64 = (uint64_t)(a >> 64), al64 = (uint64_t)a;
    uint64_t bh64 = (uint64_t)(b >> 64), bl64 = (uint64_t)b;
    // low = [0, 127] bit
    // highLow = [64, 191] bit
    // high = [128, 255] bit
    uint128_t low = (uint128_t)al64 * bl64;
    uint128_t highLow1 = (uint128_t)ah64 * bl64;
    uint128_t highLow2 = (uint128_t)al64 * bh64;
    uint128_t high = (uint128_t)ah64 * bh64;

    high += (highLow1 >> 64) + (highLow2 >> 64);
    highLow1 &= UINT128_C(0x0, 0xffffffffffffffff);
    highLow2 &= UINT128_C(0x0, 0xffffffffffffffff);

    highLow1 += (low >> 64) + highLow2;
    high += highLow1 >> 64;

    uint128_t low128 = (uint128_t)(uint64_t)low | (highLow1 << 64);
    uint128_t high128 = high;
    return {high128, low128};
  }

  uint16_t bits() const {
    uint16_t out = 0;
    if (uint128_t up = high) {
      out = 128;
      while (up) {
        up >>= 1;
        out++;
      }
    } else {
      uint128_t low = this->low;
      while (low) {
        low >>= 1;
        out++;
      }
    }
    return out;
  }

  uint128_t mod(const uint128_t b) const {
    uint128_t r = 0;
    for (uint16_t x = bits(); x > 0; x--) {
      r <<= 1;
      if (x > 128) {
        if ((high >> (x - 1U - 128)) & 1)
          ++r;
      } else {
        if ((low >> (x - 1)) & 1)
          ++r;
      }

      if (r >= b)
        r -= b;
    }
    return r;
  }

  uint256_t(uint128_t h, uint128_t l) : low(l), high(h) {}
  uint256_t() : low(0), high(0) {}

  uint128_t low, high;
};

struct Montgomery {
  /// Inputs: x = 128-bit integer in Normal Space
  uint128_t toMontSpace(uint128_t x) {
    x %= N;
    for (int i = 0; i < 128; i++) {
      x <<= 1;
      if (x >= N)
        x -= N;
    }
    return x;
  }

  /// Inputs: x, e = 128-bit integer, x is in Normal Space
  uint128_t montModExp(uint128_t x, uint128_t e) {
    // Result x in Montgomery Space
    // mX = toMontSpace(1)
    uint128_t mX = montMul(1, R2);

    // Transform x to Montgomery Space as base
    // mBase = x * r^2 * r^-1 (mod N)
    // mBase = toMontSpace(x);
    uint128_t mBase = montMul(x, R2);

    while (e) {
      // mX = mX * mBase * r^-1 (mod n)
      if (e & 0x1)
        mX = montMul(mX, mBase);
      // mBase = mBase * mBase * r^-1 (mod n)
      mBase = montMul(mBase, mBase);
      e >>= 1;
    }

    // Transform x from Montgomery Space to Normal Space.
    return montMul(mX, 1);
  }

  // result = mA * mB * r^-1 (mod n)
  // mA, mB are 128-bit integer in Montgomery space.
  uint128_t montMul(uint128_t mA, uint128_t mB) {
    return reduce(uint256_t::mul(mA, mB));
  }

  uint128_t reduce(uint256_t x) {
    // q = (x mod r) * n' mod r
    // a = (x + q * n) / r
    // if a >= n:
    //     a -= n
    // return a

    // (1) x.low = x mod r
    // (2) (uint128_t)(x.low * Ninv) = (x.low * Ninv) mod r
    // => q = (x mod r) * n' mod r
    //      = (uint128_t)x.low * n';
    uint128_t q = x.low * Ninv;
    // (1) x / r = x.high
    // (2) q * n / r = (q * n).high
    // => a = (x + q * n) / r
    //      = x.high + (q * n).high
    int128_t a = x.high - uint256_t::mul(q, N).high;
    if (a < 0)
      a += N;
    return a;
  }

  Montgomery(uint128_t N) : N(N), Ninv(1), R2(-N % N) {
    // Compute N^-1 s.t. N * N^-1 ≡ 1 (mod R)
    // Fast inverse trick:
    //    a * x ≡ 1 (mod 2^k) => a * x * (2 - a * x) ≡ 1 (mod 2^(2k))
    //
    // Ninv = 1,
    // (1) N * Ninv ≡ 1 (mod 2^1) => N * Ninv * (2 - N * Ninv) (mod 2^2)
    //     Update Ninv = Ninv * (2 - N * Ninv)
    // (2) N * Ninv ≡ 1 (mod 2^2) => N * Ninv * (2 - N * Ninv) (mod 2^4)
    //     Update Ninv = Ninv * (2 - N * Ninv)
    // (3) ...
    // (7) N * Ninv ≡ 1 (mod 2^64) => N * Ninv * (2 - N * Ninv) (mod 2^128)
    for (int i = 0; i < 7; ++i)
      Ninv = Ninv * (2 - N * Ninv);

    print128("Ninv", Ninv);
    // print128("R2", R2);
    for (int i = 0; i < 4; i++) {
      R2 <<= 1;
      if (R2 >= N)
        R2 -= N;
    }
    //  R2 = modexp(R2, 4, N);
    // print128("R2^4 mod N", R2);
    for (int i = 0; i < 5; i++) {
      R2 = montMul(R2, R2);
    }
    // print128("R2", R2);
    // print128("2^256 % n", modexp(2, 256, N));
  }

  uint128_t N, Ninv, R2;
};

/// Modular Exponentiation
/// Inputs: x, e, n 128-bit integer
/// Output: x^e mod n (128-bit integer)
uint128_t modexp(uint128_t x, uint128_t e, uint128_t n) {
  uint128_t base = x;
  x = 1;
  while (e) {
    if (e & 0x1) {
      uint256_t xb = uint256_t::mul(x, base);
      x = xb.mod(n);
    }

    uint256_t xb = uint256_t::mul(base, base);
    base = xb.mod(n);
    e >>= 1;
  }
  return x;
}

static void print128(const char *prefix, const uint128_t &a) {
  printf("%s 0x %" PRIx64 "%016" PRIx64 "\n", prefix, (uint64_t)(a >> 64),
         (uint64_t)a);
}

static void print256(const char *prefix, const uint256_t &a) {
  printf("%s 0x %" PRIx64 "%016" PRIx64 "%016" PRIx64 "%016" PRIx64 "\n",
         prefix, (uint64_t)(a.high >> 64), (uint64_t)a.high,
         (uint64_t)(a.low >> 64), (uint64_t)a.low);
}

int main(int argc, char *argv[]) {
  // 128-bit prime number
  uint128_t N = UINT128_C(0x9E40FD675571E0A, 0xF74D65DA4EA541CF);
  // uint128_t N = 19;
  uint128_t X = UINT128_C(0xFBEAB553608BDF65, 0xB2AB09BB910317F9);
  uint128_t E = UINT128_C(0x172A202E867B1177, 0x9604827082342863);
  // uint128_t E = UINT128_C(0, 1);
  print128("N", N);
  print128("X", X);
  print128("E", E);
  print128("general modexp =", modexp(X, E, N));
  // r = 1eac00fd9081a9b5b8a5d31a7b9f92f

  // print128("5^2 % 7", modexp(10, 1, 7));
  // divmod(10, 7);
  // uint256_t test0(UINT128_C(0xf7e616c17c2dc973, 0x5b47e453f14f580e),
  //                 UINT128_C(0x322a313f6f357991, 0xaeff08f7a414b031));
  // print128("test0", test0.mod(N));

  Montgomery Mont(N);
  print128("montgomery exponential =", Mont.montModExp(X, E));
  // print128("montgomery exponential =", Mont.montModExp(N, E));
  return 0;
}
