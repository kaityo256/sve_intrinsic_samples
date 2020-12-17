#include <iostream>
#include <cstdio>
#include <arm_sve.h>
#include <vector>

void svshow(svfloat64_t va){
  int n = svcntd();
  std::vector<double> a(n);
  svbool_t tp = svptrue_b64();
  svst1_f64(tp, a.data(), va);
  for(int i=0;i<n;i++){
    printf("%.8f ", a[n-i-1]);
  }
  printf("\n");
}

void load(){
  int n = svcntd();
  std::vector<double> a(n);
  for(int i=0;i<n;i++){
    a[i] = (i+1);
  }
  svbool_t tp = svptrue_b64();
  svfloat64_t va = svld1_f64(tp, a.data());
  svshow(va);
}

int main(){
  load();
}
