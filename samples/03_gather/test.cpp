#include <cstdio>
#include <vector>
#include <arm_sve.h>
#include <random>
#include <algorithm>

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

void svshow(svint64_t va){
  int n = svcntd();
  std::vector<int64_t> a(n);
  svbool_t tp = svptrue_b8();
  svst1_s64(tp, a.data(), va);
  for(int i=0;i<n;i++){
    printf("%d ", a[n-i-1]);
  }
  printf("\n");
}

void gather(){
  int n = svcntd();
  const int N = 128;
  std::vector<double> a(N);
  std::vector<int64_t> indices(n);
  for(int i=0;i<N;i++){
    a[i] = i;
  }
  for(int i=0;i<n;i++){
    indices[i] = i;
  }
  std::shuffle(indices.begin(), indices.end(), std::mt19937());
  svbool_t tp = svptrue_b8();
  svint64_t vindices = svld1_s64(tp, indices.data());
  svshow(vindices);
  svfloat64_t va = svldff1_gather_s64index_f64(tp, a.data(), vindices);
  svshow(va);
}

int main(){
  gather();
}
