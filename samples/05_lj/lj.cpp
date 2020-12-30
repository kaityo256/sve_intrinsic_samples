#include <chrono>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <random>
#include <sstream>
#include <arm_sve.h>

enum { X,
       Y,
       Z };

const int ND = 4;                                // FCCの格子数
const int N = ND * ND * ND * 4;                   //全粒指数
double __attribute__((aligned(32))) q[3][N] = {}; //座標
double __attribute__((aligned(32))) p[3][N] = {}; //運動量
const double dt = 0.01;

/*
　初期化をする関数
  運動量をすべて0クリア
  座標はFCCに組む
 */

void init(void) {
  for (int i = 0; i < N; i++) {
    p[X][i] = 0.0;
    p[Y][i] = 0.0;
    p[Z][i] = 0.0;
  }
  for (int iz = 0; iz < ND; iz++) {
    for (int iy = 0; iy < ND; iy++) {
      for (int ix = 0; ix < ND; ix++) {
        int i = (ix + iy * ND + iz * ND * ND) * 4;
        q[X][i] = ix;
        q[Y][i] = iy;
        q[Z][i] = iz;
        q[X][i + 1] = ix + 0.5;
        q[Y][i + 1] = iy;
        q[Z][i + 1] = iz;
        q[X][i + 2] = ix;
        q[Y][i + 2] = iy + 0.5;
        q[Z][i + 2] = iz;
        q[X][i + 3] = ix;
        q[Y][i + 3] = iy;
        q[Z][i + 3] = iz + 0.5;
      }
    }
  }
  //規則的な座標のままだとデバッグがしづらいので、乱数を使って場所をずらす
  std::mt19937 mt;
  std::uniform_real_distribution<> ud(-0.01, 0.01);
  for (int i = 0; i < N; i++) {
    q[X][i] += ud(mt);
    q[Y][i] += ud(mt);
    q[Z][i] += ud(mt);
  }
}

// SIMD化していないシンプルな関数
void calc_force_simple(void) {
  for (int i = 0; i < N - 1; i++) {
    // i粒子の座標と運動量を受け取っておく (内側のループでiは変化しないから)
    double qix = q[X][i];
    double qiy = q[Y][i];
    double qiz = q[Z][i];
    double pix = p[X][i];
    double piy = p[Y][i];
    double piz = p[Z][i];
    for (int j = i + 1; j < N; j++) {
      double dx = q[X][j] - qix;
      double dy = q[Y][j] - qiy;
      double dz = q[Z][j] - qiz;
      double r2 = dx * dx + dy * dy + dz * dz;
      double r6 = r2 * r2 * r2;
      double df = (24.0 * r6 - 48.0) / (r6 * r6 * r2) * dt;
      p[X][j] -= df * dx;
      p[Y][j] -= df * dy;
      p[Z][j] -= df * dz;
      pix += df * dx;
      piy += df * dy;
      piz += df * dz;
    }
    // 内側のループで積算したi粒子への力積を書き戻す
    p[X][i] = pix;
    p[Y][i] = piy;
    p[Z][i] = piz;
  }
}

// SIMD化
void calc_force_simd(void) {
  for (int i = 0; i < N - 1; i++) {
    // i粒子の座標と運動量を受け取っておく (内側のループでiは変化しないから)
    double qix = q[X][i];
    double qiy = q[Y][i];
    double qiz = q[Z][i];
    double pix = p[X][i];
    double piy = p[Y][i];
    double piz = p[Z][i];
    for (int j = i + 1; j < N; j++) {
      double dx = q[X][j] - qix;
      double dy = q[Y][j] - qiy;
      double dz = q[Z][j] - qiz;
      double r2 = dx * dx + dy * dy + dz * dz;
      double r6 = r2 * r2 * r2;
      double df = (24.0 * r6 - 48.0) / (r6 * r6 * r2) * dt;
      p[X][j] -= df * dx;
      p[Y][j] -= df * dy;
      p[Z][j] -= df * dz;
      pix += df * dx;
      piy += df * dy;
      piz += df * dz;
    }
    // 内側のループで積算したi粒子への力積を書き戻す
    p[X][i] = pix;
    p[Y][i] = piy;
    p[Z][i] = piz;
  }
}

// インテルコンパイラのループ交換最適化阻害のためのダミー変数
int sum = 0;

/*
 受け取った関数を100回繰り返し実行して、実行時間を計測する
 */
void measure(void (*pfunc)(), const char *name, int particle_number) {
  const auto s = std::chrono::system_clock::now();
  const int LOOP = 100;
  for (int i = 0; i < LOOP; i++) {
    sum++; // ループ交換阻害
    pfunc();
  }
  const auto e = std::chrono::system_clock::now();
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
  printf("N=%d, %s \t %ld [ms]\n", particle_number, name, elapsed);
}

/*
 運動量を文字列にして返す関数
 後で結果をチェックするのに使う
 */
std::string p_to_str(void) {
  std::stringstream ss;
  for (int i = 0; i < N; i++) {
    ss << i << " ";
    ss << p[X][i] << " ";
    ss << p[Y][i] << " ";
    ss << p[Z][i] << std::endl;
  }
  return ss.str();
}

int main(void) {
  // まずSIMD化していない関数の実行時間を測定し、結果を文字列として保存する
  init();
  measure(calc_force_simple, "simple", N);
  std::string simple = p_to_str();

  // 次にSIMD化した関数の実行時間を測定し、結果を文字列として保存する
  init();
  measure(calc_force_simd, "simd", N);
  std::string simd = p_to_str();

  if (simple == simd) {
    // 二つの結果が一致すればOK
    printf("Check OK\n");
  } else {
    // そうでなければ、なにかが間違っている。
    printf("Check Failed\n");
  }
}
