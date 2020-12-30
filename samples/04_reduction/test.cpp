#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <random>
#include <arm_sve.h>

void svshow(svfloat64_t va){
  int n = svcntd();
  std::vector<double> a(n);
  svbool_t tp = svptrue_b64();
  svst1_f64(tp, a.data(), va);
  for(int i=0;i<n;i++){
    printf("%+.7f ", a[n-i-1]);
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

void adda(){
  int n = svcntd();
  std::vector<double> a(n);
  for(int i=0;i<n;i++){
    a[i] = (i+1)*1e-14;
  }
	//std::shuffle(a.begin(), a.end(), std::mt19937());
  svbool_t tp = svptrue_b64();
  svfloat64_t va = svld1_f64(tp, a.data());
  std::cout << "va = " << std::endl;
  svshow(va);
	float64_t sum = svadda_f64(tp, 0.0, va);
	printf("sum = %.15f\n",sum);
}


int main(){
  std::cout << "adda" << std::endl;
  adda();
  std::cout << std::endl;
}
