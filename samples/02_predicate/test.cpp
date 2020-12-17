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
    printf("%+.7f ", a[n-i-1]);
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
  std::cout << "va = " << std::endl;
  svshow(va);
}

void load_pat(){
  int n = svcntd();
  std::vector<double> a(n);
  for(int i=0;i<n;i++){
    a[i] = (i+1);
  }
  svbool_t tp = svptrue_b64();
  svfloat64_t va = svld1_f64(tp, a.data());
  svbool_t tp2 = svptrue_pat_b64(SV_VL2);
  svfloat64_t vb = svld1_f64(tp2, a.data());
  std::cout << "va = " << std::endl;
  svshow(va);
  std::cout << "vb = " << std::endl;
  svshow(vb);
}

void load_whilelt(){
  int n = svcntd();
  std::vector<double> a(n);
  for(int i=0;i<n;i++){
    a[i] = (i+1);
  }
  svbool_t tp = svptrue_b64();
  svfloat64_t va = svld1_f64(tp, a.data());
  const uint64_t j = n;
  const uint64_t N = n+5;
  svbool_t tp2 = svwhilelt_b64(j, N);
  svfloat64_t vb = svld1_f64(tp2, a.data());
  std::cout << "va = " << std::endl;
  svshow(va);
  std::cout << "vb = " << std::endl;
  svshow(vb);
}



int main(){
  std::cout << "load" << std::endl;
  load();
  std::cout << std::endl;
  std::cout << "load_pat" << std::endl;
  load_pat();
  std::cout << std::endl;
  std::cout << "load_whilelt" << std::endl;
  load_whilelt();
}
