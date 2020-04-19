#include <cstdint>

// Reference:
// [1] https://en.wikipedia.org/wiki/Montgomery_modular_multiplication
// [2] https://cryptography.fandom.com/wiki/Montgomery_reduction
// [3] https://cp-algorithms.com/algebra/montgomery_multiplication.html
// [4] https://github.com/Fattouche/RSA
typedef __uint128_t uint128_t;

struct uint256_t {
  /// result (256-bit) = a * b
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
    uint128_t highLow = (uint128_t)ah64 * bl64 + (uint128_t)al64 * bh64;
    uint128_t high = (uint128_t)ah64 * bh64;

    highLow += low >> 64;
    high += highLow >> 64;

    uint128_t low128 = (uint128_t)(uint64_t)low | (highLow << 64);
    uint128_t high128 = high;
    return {low128, high128};
  }

  uint256_t(uint128_t l, uint128_t h) : low(l), high(h) {}
  uint256_t() : low(0), high(0) {}

  uint128_t low, high;
};

struct Montgomery {
  /// Inputs: x, e = 128-bit integer, x is in Normal Space
  uint128_t montModExp(uint128_t x, uint128_t e) {
    // Result x in Montgomery Space
    uint128_t mX = montMul(1, R2);

    // Transform x to Montgomery Space as base
    // mBase = x * r^2 * r^-1 (mod N)
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
    uint128_t a = x.high + uint256_t::mul(q, N).high;
    if (a >= N)
      a -= N;
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
      Ninv *= 2 - N * Ninv;

    for (int i = 0; i < 4; i++) {
      R2 <<= 1;
      if (R2 >= N)
        R2 -= N;
    }
    for (int i = 0; i < 5; i++)
      R2 = ((R2 % N) * R2) % N;
  }

  uint128_t N, Ninv, R2;
};
