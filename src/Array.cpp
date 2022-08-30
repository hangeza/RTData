/***************************************************************************
 * Copyright (C) 2010-2022  HG Zaunick, A. Großmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ***************************************************************************/

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
