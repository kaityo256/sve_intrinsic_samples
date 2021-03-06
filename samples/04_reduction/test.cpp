#include <algorithm>
#include <arm_sve.h>
#include <cstdio>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

void svshow(svfloat64_t va) {
  int n = svcntd();
  std::vector<double> a(n);
  svbool_t tp = svptrue_b64();
  svst1_f64(tp, a.data(), va);
  for (int i = 0; i < n; i++) {
    printf("%+.7f ", a[n - i - 1]);
  }
  printf("\n");
}

void show_ppr(svbool_t tp) {
  int n = svcntb();
  std::vector<int8_t> a(n);
  std::vector<int8_t> b(n);
  std::fill(a.begin(), a.end(), 1);
  std::fill(b.begin(), b.end(), 0);
  svint8_t va = svld1_s8(tp, a.data());
  svst1_s8(tp, b.data(), va);
  for (int i = 0; i < n; i++) {
    std::cout << (int)b[n - i - 1];
  }
  std::cout << std::endl;
}

void add_vector() {
  const double a = 1e-8;
  const double b = 1e8;
  double d[8] = {a, b, a, b, a, b, a, b};
  svbool_t tp = svptrue_b64();
  svfloat64_t va = svld1_f64(tp, d);
  float64_t sum = svadda_f64(tp, 0.0, va);
  printf("adda = %.15f\n", sum);
  float64_t sum2 = svaddv_f64(tp, va);
  printf("addv = %.15f\n", sum2);
}

void add_scalar() {
  const double a = 1e-8;
  const double b = 1e8;
  double d[8] = {a, b, a, b, a, b, a, b};
  double sum = 0.0;
  for (int i = 0; i < 8; i++) {
    sum += d[i];
  }
  printf("adda = %.15f\n", sum);
  double s1 = d[0] + d[1];
  double s2 = d[2] + d[3];
  double s3 = d[4] + d[5];
  double s4 = d[6] + d[7];
  double s12 = s1 + s2;
  double s34 = s3 + s4;
  double sum2 = s12 + s34;
  printf("addv = %.15f\n", sum2);
}

void maxv() {
  const int n = svcntd();
  std::vector<double> a(n);
  for (int i = 0; i < n; i++) {
    a[i] = (i + 1);
  }
  std::shuffle(a.begin(), a.end(), std::mt19937());
  svbool_t tp = svptrue_b64();
  svfloat64_t va = svld1_f64(tp, a.data());
  std::cout << "va = " << std::endl;
  svshow(va);
  float64_t max = svmaxv(tp, va);
  std::cout << "max(va) = " << max << std::endl;
}

void maxnmv() {
  const int n = svcntd();
  std::vector<double> a(n);
  for (int i = 0; i < n; i++) {
    a[i] = (i + 1);
  }
  std::shuffle(a.begin(), a.end(), std::mt19937());
  a[0] = std::numeric_limits<double>::quiet_NaN();
  svbool_t tp = svptrue_b64();
  svfloat64_t va = svld1_f64(tp, a.data());
  std::cout << "va = " << std::endl;
  svshow(va);
  float64_t max = svmaxv(tp, va);
  std::cout << "maxv(va) = " << max << std::endl;
  float64_t maxnmv = svmaxnmv(tp, va);
  std::cout << "maxnmv(va) = " << maxnmv << std::endl;
}

int main() {
  std::cout << "add vector" << std::endl;
  add_vector();
  std::cout << std::endl;
  std::cout << "add scalar" << std::endl;
  add_scalar();
  std::cout << std::endl;
  std::cout << "maxv" << std::endl;
  maxv();
  std::cout << std::endl;
  std::cout << "maxnmv" << std::endl;
  maxnmv();
}
