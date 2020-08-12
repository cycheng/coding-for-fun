#include "../leetcode/std.h"

/// Compute matrix multiplication: c[m, n] = a[m, k] x b[k, n]
template <class T> class tensor {
  vector<T> data;
  uint32_t row, col;

public:
  tensor(uint32_t row, uint32_t col) : row(row), col(col) {
    data.resize(row * col);
  }

  T &at(uint32_t r, uint32_t c) {
    assert(r < row && c < col && "invalid row, col index.");
    return data[r * col + c];
  }

  const T &at(uint32_t r, uint32_t c) const {
    assert(r < row && c < col && "invalid row, col index.");
    return data[r * col + c];
  }

  uint32_t row_size() const { return row; }
  uint32_t col_size() const { return col; }

  void zero() { std::fill(data.begin(), data.end(), T(0)); }
  void fill(T v) { std::fill(data.begin(), data.end(), v); }
};

template <class T> struct naive {
  static void matmul(tensor<T> &c, const tensor<T> &a, const tensor<T> &b) {
    if (!is_valid_size(c, a, b)) {
      cout << "Invalid matrix size\n";
      return;
    }

    for (uint32_t i = 0; i < c.row_size(); ++i)
      for (uint32_t j = 0; j < c.col_size(); ++j)
        for (uint32_t k = 0; k < b.row_size(); ++k)
          c.at(i, j) += a.at(i, k) * b.at(k, j);
  }

  static bool is_valid_size(const tensor<T> &c, const tensor<T> &a,
                            const tensor<T> &b) {
    if (c.row_size() != a.row_size() || c.col_size() != b.col_size() ||
        a.col_size() != b.row_size())
      return false;
    return true;
  }
};

template <class T, uint32_t TileRowSize, uint32_t TileColSize,
          uint32_t TileStepSize>
struct optimized {
  static void matmul(tensor<T> &c, const tensor<T> &a, const tensor<T> &b) {
    if (!is_valid_size(c, a, b)) {
      cout << "Invalid matrix size\n";
      return;
    }

    for (uint32_t i = 0; i < c.row_size(); i += TileRowSize)
      for (uint32_t j = 0; j < c.col_size(); j += TileColSize)
        for (uint32_t k = 0; k < b.row_size(); k += TileStepSize) {
          // T localB[TileStepSize][TileColSize];
          // for (uint32_t tk = k; tk < k + TileStepSize; ++tk)
          //   for (uint32_t tj = j; tj < j + TileColSize; ++tj)
          //     localB[tk - k][tj - j] = b.at(tk, tj);
          // T localA[TileRowSize][TileStepSize];
          // for (uint32_t ti = i; ti < i + TileRowSize; ++ti)
          //   for (uint32_t tk = k; tk < k + TileStepSize; ++tk)
          //     localA[ti - i][tk - k] = a.at(ti, tk);
          for (uint32_t tk = k; tk < k + TileStepSize; ++tk)
            for (uint32_t ti = i; ti < i + TileRowSize; ++ti)
              for (uint32_t tj = j; tj < j + TileColSize; ++tj)
                c.at(ti, tj) += a.at(ti, tk) * b.at(tk, tj);
        }
  }

  static bool is_valid_size(const tensor<T> &c, const tensor<T> &a,
                            const tensor<T> &b) {
    if (c.row_size() % TileRowSize || c.col_size() % TileColSize ||
        b.row_size() % TileStepSize)
      return false;
    return naive<T>::is_valid_size(c, a, b);
  }
};

// benchmark:
#include <chrono>
#include <random>
using namespace std;

namespace test {
struct ReportExecTime {
  std::chrono::high_resolution_clock::time_point start;
  const char *prefix;
  ReportExecTime(const char *prefix = nullptr) : prefix(prefix) {
    start = std::chrono::high_resolution_clock::now();
  }
  ~ReportExecTime() {
    std::chrono::duration<double, std::milli> elapsed =
        std::chrono::high_resolution_clock::now() - start;
    std::cout << prefix << "Exe time = " << elapsed.count() << " msec."
              << std::endl;
  }
};

tensor<float> gen_rand_matrix_f32(uint32_t row, uint32_t col, float a,
                                  float b) {
  default_random_engine gen;
  uniform_real_distribution<double> dist(a, b);

  tensor<float> mat(row, col);
  for (uint32_t i = 0; i < row; ++i)
    for (uint32_t j = 0; j < col; ++j)
      mat.at(i, j) = dist(gen);
  return mat;
}

tensor<float> gen_increase_matrix_f32(uint32_t row, uint32_t col) {
  tensor<float> mat(row, col);
  for (uint32_t i = 0; i < row; ++i)
    for (uint32_t j = 0; j < col; ++j)
      mat.at(i, j) = (float)i;
  return mat;
}

template <class T>
bool compare(const tensor<T> &c0, const tensor<T> &c1, uint32_t print_max = 16,
             T epsilon = T()) {
  uint32_t mismatch_cnt = 0;
  for (uint32_t i = 0; i < c0.row_size(); ++i)
    for (uint32_t j = 0; j < c0.col_size(); ++j)
      if (abs(c0.at(i, j) - c1.at(i, j)) > epsilon) {
        if (mismatch_cnt++ < print_max)
          cout << "mismatch at " << i << ", " << j << ". c0 = " << c0.at(i, j)
               << ", c1 = " << c1.at(i, j) << endl;
      }
  if (mismatch_cnt) {
    cout << "failed. total mismatch = " << mismatch_cnt << endl;
    return false;
  }
  return true;
}

void bench1024_different_tile() {
  tensor<float> a = gen_rand_matrix_f32(1024, 1024, -10.f, 10.f);
  tensor<float> b = gen_rand_matrix_f32(1024, 1024, -10.f, 10.f);
  tensor<float> c0(1024, 1024), c1(1024, 1024);
  float c_init = 0.2f;
  c0.fill(c_init);
  c1.fill(c_init);

  cout << "test 1024x1024x1024:\n";

  {
    ReportExecTime t("naive exe time: ");
    naive<float>::matmul(c0, a, b);
  }

  {
    ReportExecTime t("tile 32x32x32: ");
    optimized<float, 32, 32, 32>::matmul(c1, a, b);
  }
  compare(c0, c1);

  c1.fill(c_init);
  {
    ReportExecTime t("tile 32x32x16: ");
    optimized<float, 32, 32, 16>::matmul(c1, a, b);
  }
  compare(c0, c1);

  c1.fill(c_init);
  {
    ReportExecTime t("tile 32x32x8: ");
    optimized<float, 32, 32, 8>::matmul(c1, a, b);
  }
  compare(c0, c1);

  c1.fill(c_init);
  {
    // ReportExecTime t("tile 256x256x64: ");
    // optimized<float, 256, 256, 64>::matmul(c1, a, b);
    ReportExecTime t("tile 16x16x16: ");
    optimized<float, 16, 16, 16>::matmul(c1, a, b);
  }
  compare(c0, c1);

  c1.fill(c_init);
  {
    // ReportExecTime t("tile 128x128x64: ");
    // optimized<float, 128, 128, 64>::matmul(c1, a, b);
    ReportExecTime t("tile 8x8x8: ");
    optimized<float, 8, 8, 8>::matmul(c1, a, b);
  }
  compare(c0, c1);

  c1.fill(c_init);
  {
    // ReportExecTime t("tile 64x64x64: ");
    // optimized<float, 64, 64, 64>::matmul(c1, a, b);
    ReportExecTime t("tile 16x16x8: ");
    optimized<float, 16, 16, 8>::matmul(c1, a, b);
  }
  compare(c0, c1);
}

// void bench_different_size() {
//   tensor<float> a = gen_rand_matrix_f32(32, 64, -10.f, 10.f);
//   tensor<float> b = gen_rand_matrix_f32(64, 32, -10.f, 10.f);
//   tensor<float> c0(32, 32), c1(32, 32);
//   naive<float>::matmul(c0, a, b);
//   optimized<float, >::
// }

void correctness() {
  tensor<float> a = gen_increase_matrix_f32(256, 32);
  tensor<float> b = gen_increase_matrix_f32(32, 512);
  tensor<float> c0(256, 512), c1(256, 512);
  float c_init = 1.f;
  c0.fill(c_init);
  c1.fill(c_init);
  naive<float>::matmul(c0, a, b);
  optimized<float, 8, 8, 16>::matmul(c1, a, b);
  for (int i = 0; i < 256; ++i)
    for (int j = 0; j < 512; ++j)
      if (c0.at(i, j) != 1.f + 496.f * i) {
        cout << "row " << i << " failed\n";
        break;
      }
  if (compare(c0, c1))
    cout << "pass test.\n";
}

} // namespace test

int main(int argc, char *argv[]) {
  test::correctness();
  test::bench1024_different_tile();
  return 0;
}
