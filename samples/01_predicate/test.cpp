#include <arm_sve.h>
#include <iostream>
#include <vector>

void show_ppr(svbool_t tp) {
  std::vector<int8_t> a(64);
  std::vector<int8_t> b(64);
  std::fill(a.begin(), a.end(), 1);
  std::fill(b.begin(), b.end(), 0);
  svint8_t va = svld1_s8(tp, a.data());
  svst1_s8(tp, b.data(), va);
  for (int i = 0; i < 64; i++) {
    std::cout << (int)b[63 - i];
  }
  std::cout << std::endl;
}

void ptrue() {
  std::cout << "svptrue_b8" << std::endl;
  show_ppr(svptrue_b8());
  std::cout << "svptrue_b16" << std::endl;
  show_ppr(svptrue_b16());
  std::cout << "svptrue_b32" << std::endl;
  show_ppr(svptrue_b32());
  std::cout << "svptrue_b64" << std::endl;
  show_ppr(svptrue_b64());
}

void ptrue_pat() {
  std::cout << "svptrue_pat_b8(SV_VL2)" << std::endl;
  show_ppr(svptrue_pat_b8(SV_VL2));
  std::cout << "svptrue_pat_b16(SV_VL2)" << std::endl;
  show_ppr(svptrue_pat_b16(SV_VL2));
  std::cout << "svptrue_pat_b32(SV_VL2)" << std::endl;
  show_ppr(svptrue_pat_b32(SV_VL2));
  std::cout << "svptrue_pat_b64(SV_VL2)" << std::endl;
  show_ppr(svptrue_pat_b64(SV_VL2));
}

int main() {
  ptrue();
  std::cout << "----" << std::endl;
  ptrue_pat();
}
