#include <complex>
#include <valarray>
#include <vector>
#include <deque>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <climits>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <cassert>
#include <cstdarg>
#include <iostream>
#include "Array.h"


namespace hgz{


// member definitions

// class ExtendsVector

ExtendsVector::ExtendsVector(int i0, const int i1, const int i2, const int i3,
                  const int i4, const int i5, const int i6,
                  const int i7, const int i8, const int i9,
                  const int i10, const int i11, const int i12)
         :std::deque<int>()
{
   this->push_back(i0);
   if (i1==0) return;
   this->push_back(i1);
   if (i2==0) return;
   this->push_back(i2);
   if (i3==0) return;
   this->push_back(i3);
   if (i4==0) return;
   this->push_back(i4);
   if (i5==0) return;
   this->push_back(i5);
   if (i6==0) return;
   this->push_back(i6);
   if (i7==0) return;
   this->push_back(i7);
   if (i8==0) return;
   this->push_back(i8);
   if (i9==0) return;
   this->push_back(i9);
   if (i10==0) return;
   this->push_back(i10);
   if (i11==0) return;
   this->push_back(i11);
   if (i12==0) return;
   this->push_back(i12);
}

void ExtendsVector::fill(int N, int value)
{
   if (!(this->empty())) this->clear();
   for (int i=0;i<N; ++i) this->push_back(value);
}




} // namespace hgz
