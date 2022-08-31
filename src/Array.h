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

#ifndef _ARRAY_H
#define _ARRAY_H


//#include <complex>
#include <valarray>
#include <vector>
#include <deque>
//#include <errno.h>
//#include <stdio.h>
//#include <cstdlib>
#include <typeinfo>
//#include <climits>
//#include <algorithm>
//#include <functional>
#include <numeric>
#include <iterator>
#include <cassert>
#include <cstdarg>
#include <iostream>

namespace hgz{


typedef std::valarray<double> valarray_t;



//! Container class for general (1-dim) Arrays
/*!
 * ...
*/
template <typename T>
class Array_base
{
   public:
      typedef T value_type;
      typedef T* iterator;
      typedef const T* const_iterator;
      typedef T& reference;
      typedef const T& const_reference;
      
      Array_base();
      Array_base(size_t a_size, const bool init);
      Array_base(size_t a_size, const T& def);
      ~Array_base();
      
      iterator begin() { return _mem; }
      const_iterator begin() const { return _mem; }
      iterator end() { return _mem+_size; }
      const_iterator end() const { return _mem+_size; }
      
      T * data() { return _mem; }
      
      reference operator[](size_t i)
      {
         assert(i<_size);
         return _mem[i];
      }
      
      const_reference operator[](size_t i) const
      {
         assert(i<_size);
         return _mem[i];
      }
      reference operator()(size_t i)
      {
         assert(i<_size);
         return _mem[i];
      }
      const_reference operator()(size_t i) const
      {
         assert(i<_size);
         return _mem[i];
      }
      
   public:
      size_t size() const { return _size; }
      size_t typesize() const { return sizeof(T); }

      void SetAt(T* address, size_t a_size);

      bool resize(size_t new_size)
      {
         if (_mem) { delete[] _mem; _mem=0; }
         _size=new_size;
         if (_size) _mem=new T[_size];
         else return true;
         assert (_mem!=0);
         return (_mem!=0);
      }
      
   protected:
      T *_mem;
      size_t _size;
      bool _isReference;
};





class ExtendsVector : public std::deque<int>
{
   public:
      ExtendsVector() {}
      ExtendsVector(int i0, const int i1=0, const int i2=0, const int i3=0,
                        const int i4=0, const int i5=0, const int i6=0,
                        const int i7=0, const int i8=0, const int i9=0,
                        const int i10=0, const int i11=0, const int i12=0);
      ~ExtendsVector() {}
      void fill(int N, int value);
};



template <int NrDims>
class DimVector : public std::deque<int>
{
   public:
      DimVector():std::deque<int>(NrDims,0) {}
      DimVector(int i0,...);
      template <int otherDim>
      DimVector(const DimVector<otherDim> &x);
      ~DimVector() {}

      DimVector& operator()(int i0,...);
      DimVector<1>& operator=(int i0);
      DimVector<NrDims+1>& operator,(int in);

      void fill(int value);
      inline int sum() const { return std::accumulate(this->begin(),this->end(),0); }
};



class Range
{
   public:
      Range(){ _begin=0; _end=0; }
      Range(int begin, int end){ _begin=begin; _end=end; }
      ~Range() {}
      
      inline int begin() const { return _begin; }
      inline int end() const { return _end; }
      inline int length() const { return _end-_begin+1; }

      inline bool operator== ( const Range& other ) const
         { return (_begin==other._begin && _end==other._end); }
      inline bool operator!= ( const Range& other ) const
         { return !(other==(*this)); }
   protected:
      int _begin;
      int _end;
};


static const Range rAll;


typedef ExtendsVector extends;


// Forward declarations
template <typename T, int N>
class ArrayIterator;

template <typename T, int N>
class ConstArrayIterator;








//! Container class for N-dim Arrays
/*!
 * ...
*/

template <typename T , int NrDims>
class Array : public Array_base<T>
{

public:
   typedef Array_base<T> T_base;

protected:   
   using T_base::_mem;
   using T_base::_size;
   using T_base::_isReference;

public:

   typedef DimVector<NrDims> bases;
   typedef DimVector<NrDims> strides;
   typedef DimVector<NrDims> indices;


   typedef ArrayIterator<T,NrDims> iterator;
   typedef const ConstArrayIterator<T,NrDims> const_iterator;


   /*! default constructor \n
    * creates empty Array
   */
   Array();

   Array(const extends &a_extends,
         const bases &a_bases=bases(),
         const bool init=true);

   Array(const extends &a_extends,
         const bases &a_bases,
         const strides &a_strides,
         T* a_mem,
         size_t a_size);

   /*! Copy Constructor \n
    *  make deep copy of \e src
    */
   Array(const Array& src);

   /*! default destructor
    */
   ~Array(){}


/* private Methods */

   private:

   /*! Calculate strides of all dimensions
    */
      void CalcStrides();

   public:

/* Iterator functions */

      /*! begin iterator \n
       * \return iterator to the first element
       */
      inline iterator begin()
      {
         iterator it((*this));
         return it;
      }

      /*! const begin iterator \n
       * \return const_iterator to the first element
       */
      inline const_iterator begin() const
      {
         const_iterator it((*this));
         return it;
      }
      
      /*! end iterator \n
       * \return iterator pointing after the last element
       */
      inline iterator end()
      {
         iterator it;
         return it;
      }
      
      /*! const end iterator \n
       * \return const_iterator pointing after the last element
       */
      inline const_iterator end() const
      {
         const_iterator it;
         return it;
      }


/* Operators */
      
      /*! General Assignment \n
       *\return deep copy of \e x
       */
      const Array<T,NrDims>& operator=(const Array<T,NrDims> &src);

      /*! Assignment to the entire array (subarray) from \e val
       */
      const Array<T,NrDims>& operator=(const T& val);

      /*! Assignment from std::vector<T> \n
       * Assign a vector. The result is a 1-dim. Array
       */
//      const Array<T,1>& operator=(const std::vector<T> &vec);

      /*! Select a subarray \n
       * this operation returns a collapsed subarray with the last (rightmost)\n
       * dimension fixed
       *\return N-1-dim subarray
       */
      Array<T,NrDims-1> operator[](int in) const;

      /*! Select a subarray \n
       * this operation returns a subarray with the selected Range in the last \n
       * (rightmost) dimension
       *\return N-dim subarray
       *\todo needs work!
       */
      Array<T,NrDims> operator[](Range r);

      /*! cast to generic type T */
//      operator T(){ return _mem[0]; }

      /*! cast to std::vector<T> */
//      operator std::vector<T>();
      

/* public Methods */

      /*! get number of Elements in array (subarray) (  != size() ) */
      size_t NrElements() const;

      inline const strides& GetStrides() const { return _strides; }
      inline const bases& GetBases() const { return _bases; }
      inline const extends& GetExtends() const { return _extends; }
      
      /*! returns pointer to the first element of datafield */
      inline T* data() const { return _mem; }

      /*! returns element with given indices */
      T& GetElement(indices a_indices) const;

      /*! print some info about actual instance to stdout */
      void Print() const;


/* Specializations */

/*
// friend fw declarations illegal since gcc 4.xx
      template <T>
      friend std::ostream& operator<<(std::ostream& o,const Array<T,1>& v);

      template <T>
      friend std::ostream& operator<<(std::ostream& o,const Array<T,2>& v);
*/

/* protected members */

   protected:
      extends _extends;
      bases _bases;
      strides _strides;
};





template <typename T>
class Array<T,1>;


/*! Specialization of Array-Class for 2 dimensions  */
template <class T>
class Array<T,2> : public Array_base<T>
{
public:
   typedef Array_base<T> T_base;

protected:   
   using Array_base<T>::_mem;
   using Array_base<T>::_size;
   using Array_base<T>::_isReference;
   
public:

   typedef DimVector<2> bases;
   typedef DimVector<2> strides;
   typedef DimVector<2> indices;


   typedef ArrayIterator<T,2> iterator;
   typedef const ConstArrayIterator<T,2> const_iterator;


   /*! default constructor \n
    * creates empty Array
   */
   Array():Array_base<T>()
   {
      _extends.fill(2,0);
      _bases.fill(0);
      _strides.fill(0);
   }

   Array(const extends &a_extends,
         const bases &a_bases=bases(),
         const bool init=true)
        :Array_base<T>(_size=a_extends[0]*a_extends[1],init)
   {
      _isReference=false;
      assert((a_extends.size()==2));
      _extends=a_extends;

      if (a_bases.empty()) _bases.fill(0);
      else
      {
         _bases=a_bases;
         assert((a_bases.size()==2));
      }
      _strides[0]=1;
      _strides[1]=_extends[0];
   }

   Array(const extends &a_extends,
         const bases &a_bases,
         const strides &a_strides,
         T* a_mem,
         size_t a_size)
   {
      assert((a_extends.size()==2));
      _extends=a_extends;
      assert((a_bases.size()==2));
      _bases=a_bases;
      assert((a_strides.size()==2));
      _strides=a_strides;
      _mem=a_mem;
      _size=a_size;
      _isReference=true;
   }

   /*! Copy Constructor \n
    *  make deep copy of \e src
    */
   Array(const Array& src)
   {
//      if (this == &src) return;
//      assert(!src._isReference);
      
      if (_size!=src._size)
      {
         assert(this->resize(src.NrElements()));
         _isReference=false;
      }

      _extends=src._extends;
      _bases=src._bases;

      _strides.front()=1;
      _strides.back()=_extends.front();

//      _isReference=false;
      std::copy(src.begin(),src.end(),this->begin());
//      memcpy(_mem,src._mem,sizeof(T)*_size);
   }

   /*! default destructor
    */
   ~Array(){}


   public:

/* Iterator functions */

      inline iterator begin() { return iterator(*this); }
      inline iterator end() { return iterator(); }
      inline const_iterator begin() const { return const_iterator(*this); }
      inline const_iterator end() const { return const_iterator(); }


/* Operators */
      
      /*! General Assignment \n
       *\return deep copy of \e x
       * @todo Iterator mu� vertauschte Strides handeln k�nnen (z.B.
       * Transponierte Matrix)
       */
      Array<T,2>& operator=(const Array<T,2> &src)
      {
//         std::cout<<"Array<"<<typeid(T).name()<<",2>::operator=(const Array&)"<<std::endl;

//         assert(!src._isReference);

         if (this == &src) return *this;
/*
         if (src._isReference)
         {
            std::cout<<"*** Array<T,2>& operator=(const Array<T,2> &src) ***"<<std::endl;
            std::cout<<" src:"<<std::endl;
            src.Print();
            std::cout<<" this:"<<std::endl;
            this->Print();
         }
         assert(!src._isReference);
*/         
         if (_isReference)
         {
            assert(this->NrElements()==src.NrElements());
            _extends=src._extends;
            _bases=src._bases;
         }
         else
         {
            if (_size!=src.NrElements())
            {
               assert(this->resize(src.NrElements()));
               _isReference=false;
            }
            _extends=src._extends;
            _bases=src._bases;
            
            _strides[0]=1;
            _strides[1]=_extends[0];
         }

         std::copy(src.begin(),src.end(),this->begin());
         return *this;
      }

      /*! Assignment to the entire array (subarray) from \e val
       */
      const Array<T,2>& operator=(const T& val)
      {
      //   std::cout<<"Array<"<<typeid(T).name()<<","<<NrDims
      //            <<">::operator=(T)(0)"<<std::endl;
//         for (T* i=_mem; i<_mem+_size; i++) *i=val;
//         for (size_t i=0; i<_size; i+=_strides[0]) _mem[i]=val;
//         for (size_t i=0; i<this->NrElements(); i++) _mem[i*_strides[0]]=val;
         std::fill(this->begin(),this->end(),val);
         return *this;
      }


      Array<T,2>& operator+=(const Array<T,2>& x)
      {
         assert(x.NrElements()==this->NrElements());
         transform(this->begin(), this->end(),
                   x.begin(),this->begin(),
                   std::plus<T>());
         return *this;
      }

      Array<T,2>& operator-=(const Array<T,2>& x)
      {
         assert(x.NrElements()==this->NrElements());
         transform(this->begin(), this->end(),
                   x.begin(),this->begin(),
                   std::minus<T>());
         return *this;
      }

      Array<T,2>& operator*=(const Array<T,2>& x)
      {
         assert(x.NrElements()==this->NrElements());
         transform(this->begin(), this->end(),
                   x.begin(),this->begin(),
                   std::multiplies<T>());
         return *this;
      }

      Array<T,2>& operator/=(const Array<T,2>& x)
      {
         assert(x.NrElements()==this->NrElements());
         for (int i=0; i<(int)this->NrElements(); i++)
         {
            if (x[i]==0) _mem[i*_strides[0]]=INFINITY;
            else _mem[i*_strides[0]]/=x[i];
         }
         return *this;
      }

      Array<T,2>& operator*=(const T& x)
      {
//         std::cout<<"Array<"<<typeid(T).name()<<",1>::operator*=(const T&)"<<std::endl;
         for (int i=0; i<(int)this->NrElements(); i++)
            _mem[i*_strides[0]]*=x;
         return *this;
      }

      Array<T,2>& operator/=(const T& x)
      {
         if (x==T()) fill(this->begin(),this->end(),T(INFINITY));
         else
         (*this)*=T(1)/x;
         return *this;
      }

      Array<T,1> operator[](int in) const
      {

         extends ext=_extends;
         bases bas =_bases;
         strides str =_strides;

/*
         ext.pop_front();
         bas.pop_front();
         str.pop_front();
*/
         ext.pop_back();
         bas.pop_back();
         str.pop_back();

//         int offs=_strides[0]*in;
         int offs=_strides.back()*in;

         Array<T,1> _x(ext,
                       DimVector<1>(bas),
                       DimVector<1>(str),
                       _mem+offs,
                       _size-offs);
         return _x;
      }

      Array<T,2> operator[](Range r)
      {

         extends ext=_extends;
         bases bas =_bases;
         strides str =_strides;

         std::swap(ext.front(),ext.back());
         std::swap(bas.front(),bas.back());
         std::swap(str.front(),str.back());

         Array<T,2> _x(ext,
                       DimVector<2>(bas),
                       DimVector<2>(str),
                       _mem,
                       _size);
         return _x;
      }



/* public Methods */

      /*! get number of Elements in array (subarray) (  != size() ) */
      size_t NrElements() const { return _extends[0]*_extends[1]; }

      inline strides GetStrides() const { return _strides; }
      inline bases GetBases() const { return _bases; }
      inline extends GetExtends() const { return _extends; }
      /*! returns number of rows */
      inline int rows() const{ return _extends[1]; }
      /*! returns number of columns */
      inline int cols() const{ return _extends[0]; }
      
      /*! returns pointer to the first element of datafield */
      inline T* data() const { return _mem; }

      /*! returns element with given indices */
      T& GetElement(indices a_indices) const{ return _mem[0]; }

      Array<T,1> Row(int row) const
      {
         assert(_isReference==false);
         extends ext=_extends;
         bases bas =_bases;
         strides str =_strides;

         ext.pop_back();
         bas.pop_back();
         str.pop_back();

         int offs=_strides[1]*row;

         Array<T,1> _x(ext,
                       DimVector<1>(bas),
                       DimVector<1>(str),
                       _mem+offs,
                       _size-offs);
         return _x;
      }

      Array<T,1> Col(int col) const
      {
         assert(_isReference==false);
         extends ext=_extends;
         bases bas =_bases;
         strides str =_strides;

         ext.pop_front();
         bas.pop_front();
         str.pop_front();

         int offs=_strides[0]*col;

         Array<T,1> _x(ext,
                       DimVector<1>(bas),
                       DimVector<1>(str),
                       _mem+offs,
                       _size-offs);
         return _x;
      }

      /*! print some info about actual instance to stdout */
      void Print() const
      {
         std::cout<<"Object: Array<2>"<<std::endl;
         std::cout<<"Field Address: "<<this<<std::endl;
         std::cout<<"Data Address: "<<_mem<<std::endl;
         if (_isReference)
            std::cout<<"(Field is a reference!)"<<std::endl;
         std::cout<<"Datatype: "<<typeid(T).name()<<std::endl;
         std::cout<<"size of Datatype: "<<sizeof(T)<<" bytes"<<std::endl;
         std::cout<<"Nr. of Elements: "<<NrElements()<<std::endl;
         std::cout<<"Extends: x="<<_extends[0]<<", y="<<_extends[1]<<std::endl;
         std::cout<<"Bases: x="<<_bases[0]<<", y="<<_bases[1]<<std::endl;
         std::cout<<"Strides: x="<<_strides[0]<<", y="<<_strides[1]<<std::endl;
         std::cout<<"size: "<<this->size()<<std::endl;
         std::cout<<"size of Array: "<<sizeof(T)*this->size()<<" bytes"<<std::endl;
      }


/* protected members */

   protected:
      extends _extends;
      bases _bases;
      strides _strides;
};










/*! Specialization of class Array which represents a vector */
template <typename T>
class Array<T,1> : public Array_base<T>
{
public:
   typedef Array_base<T> T_base;

protected:   
   using Array_base<T>::_mem;
   using Array_base<T>::_size;
   using Array_base<T>::_isReference;
   
public:

   typedef DimVector<1> bases;
   typedef DimVector<1> strides;
   typedef DimVector<1> indices;


   typedef ArrayIterator<T,1> iterator;
   typedef const ConstArrayIterator<T,1> const_iterator;


   /*! default constructor \n
    * creates empty Array
   */
   Array():Array_base<T>()
   {
      _extends.fill(1,0);
      _bases.fill(0);
      _strides.fill(0);
   }

   Array(const extends &a_extends,
         const bases &a_bases=bases(),
         const bool init=true)
        :Array_base<T>(_size=a_extends[0],init)
   {
      assert((a_extends.size()==1));
      _extends=a_extends;

      if (a_bases.empty()) _bases.fill(0);
      else
      {
         _bases=a_bases;
         assert((a_bases.size()==1));
      }
//      _strides.fill(0);
      _strides[0]=1;
//      this->CalcStrides();
   }

   Array(const extends &a_extends,
         const bases &a_bases,
         const strides &a_strides,
         T* a_mem,
         size_t a_size)
   {
      assert((a_extends.size()==1));
      _extends=a_extends;
      assert((a_bases.size()==1));
      _bases=a_bases;
      assert((a_strides.size()==1));
      _strides=a_strides;
      _mem=a_mem;
      _size=a_size;
      _isReference=true;
   }

   /*! Copy Constructor \n
    *  make deep copy of \e src
    */
   Array(const Array& src) : Array_base<T>()
   {
        assert(this->resize(src.NrElements()));
        _isReference=false;
        _extends=src._extends;
        _bases=src._bases;
        _strides=src._strides;

        std::copy(src.begin(),src.end(),this->begin());
   }

   /*! destructor
    */
   ~Array(){}


   public:

/* Iterator functions */

      inline iterator begin() { return iterator(*this); }
      inline iterator end() { return iterator(); }
      inline const_iterator begin() const { return const_iterator(*this); }
      inline const_iterator end() const { return const_iterator(); }


/* Operators */
      
      /*! General Assignment \n
       *\return deep copy of \e x
       */
      Array<T,1>& operator=(const Array<T,1> &x)
      {
      //   std::cout<<"Array<"<<typeid(T).name()<<","<<NrDims
      //            <<">::operator=(...&x)"<<std::endl;

         if (this == &x) return *this;

         if (_isReference) assert(NrElements()==x.NrElements());
         else
         {
            if (_size!=x.NrElements())
            {
/*
               if (_size) delete[] _mem;
               _mem=0;
               _size=x.NrElements();
               if (_size) _mem=new T[_size];
*/
               assert(this->resize(x.NrElements()));
               _isReference=false;
            }
            _extends=x._extends;
            _bases=x._bases;
            _strides[0]=1;
         }

         if (!_isReference && !x._isReference) 
         {
            memcpy(_mem,x._mem,sizeof(T)*x.NrElements());
            return *this;
         }
         else
         if (_isReference)
         {
            _extends=x._extends;
            _bases=x._bases;
//            _strides=x._strides;
            std::copy(x.begin(),x.end(),begin());
         }
         else
         {
//            memcpy(_mem,x._mem,sizeof(T)*x.NrElements());
            std::copy(x.begin(),x.end(),begin());
         }
         return *this;
      }

      /*! Assignment to the entire array (subarray) from \e val
       */
      const Array<T,1>& operator=(const T& val)
      {
      //   std::cout<<"Array<"<<typeid(T).name()<<","<<NrDims
      //            <<">::operator=(T)(0)"<<std::endl;
         std::fill(this->begin(),this->end(),val);
         return *this;
      }


      Array<T,1>& operator=(const std::vector<T> &vec)
      {
         if (_isReference)
         {
            assert(vec.size()==this->NrElements());
//            for (int i=0; i<(int)vec.size(); ++i)
//               _mem[i*_strides[0]]=vec[i];
         }
         else if (vec.size()!=this->NrElements())
         {
            this->resize(vec.size());
         }

         std::copy(vec.begin(),vec.end(),this->begin());
         return *this;
/*
         assert(vec.size()==NrElements());
         for (int i=0; i<(int)vec.size(); ++i)
            _mem[i*_strides[0]]=vec[i];
         return *this;
*/
      }

      Array<T,1>& operator+=(const Array<T,1>& x)
      {
         assert(x.NrElements()==this->NrElements());
         transform(this->begin(), this->end(),
                   x.begin(),this->begin(),
                   std::plus<T>());
         return *this;
      }

      Array<T,1>& operator-=(const Array<T,1>& x)
      {
         assert(x.NrElements()==this->NrElements());
         transform(this->begin(), this->end(),
                   x.begin(),this->begin(),
                   std::minus<T>());
         return *this;
      }

      Array<T,1>& operator*=(const Array<T,1>& x)
      {
         assert(x.NrElements()==this->NrElements());
         transform(this->begin(), this->end(),
                   x.begin(),this->begin(),
                   std::multiplies<T>());
         return *this;
      }

      Array<T,1>& operator/=(const Array<T,1>& x)
      {
         assert(x.NrElements()==this->NrElements());
         for (int i=0; i<(int)this->NrElements(); i++)
         {
            if (x[i]==0) _mem[i*_strides[0]]=INFINITY;
            else _mem[i*_strides[0]]/=x[i];
         }
         return *this;
      }

      Array<T,1>& operator*=(const T& x)
      {
//         std::cout<<"Array<"<<typeid(T).name()<<",1>::operator*=(const T&)"<<std::endl;
         for (int i=0; i<(int)this->NrElements(); i++)
            _mem[i*_strides[0]]*=x;
         return *this;
      }

      Array<T,1>& operator/=(const T& x)
      {
         if (x==T()) std::fill(this->begin(),this->end(),T(INFINITY));
         else
         (*this)*=T(1)/x;
         return *this;
      }

      operator std::vector<T>()
      {
         std::vector<T> x;
         for (int i=0; i<(int)NrElements(); ++i)
            x.push_back(_mem[i*_strides[0]]);
         return x;
      }

      const T& operator[](int in) const { return _mem[_strides[0]*in]; }
      T&       operator[](int in) { return _mem[_strides[0]*in]; }

      Array<T,1>& operator[](Range r) { return *this; }



/* public Methods */

      /*! get number of Elements in array (subarray) (  != size() ) */
      size_t NrElements() const { return _extends[0]; }

      bool resize(size_t new_size)
      {
         if (!T_base::resize(new_size)) return false;
         _extends.front()=new_size;
         _strides.front()=1;
         _bases.front()=0;
         return true;
      }

      inline strides GetStrides() const { return _strides; }
      inline bases GetBases() const { return _bases; }
      inline extends GetExtends() const { return _extends; }
      
      /*! returns pointer to the first element of datafield */
      inline T* data() const { return _mem; }

      /*! returns element with given indices */
      T& GetElement(indices a_indices) const{ return _mem[0]; }

      /*! print some info about actual instance to stdout */
      void Print() const
      {
         std::cout<<"Object: Array<1>"<<std::endl;
         std::cout<<"Field Address: "<<this<<std::endl;
         std::cout<<"Data Address: "<<_mem<<std::endl;
         if (_isReference)
            std::cout<<"(Field is a reference!)"<<std::endl;
         std::cout<<"Datatype: "<<typeid(T).name()<<std::endl;
         std::cout<<"size of Datatype: "<<sizeof(T)<<" bytes"<<std::endl;
         std::cout<<"Nr. of Elements: "<<NrElements()<<std::endl;
         std::cout<<"Extend: "<<_extends[0]<<std::endl;
         std::cout<<"Base: "<<_bases[0]<<std::endl;
         std::cout<<"Stride: "<<_strides[0]<<std::endl;
         std::cout<<"size: "<<this->size()<<std::endl;
         std::cout<<"size of Array: "<<sizeof(T)*this->size()<<" bytes"<<std::endl;
      }


/* protected members */

   protected:
      extends _extends;
      bases _bases;
      strides _strides;
};





/*
 * ConstArrayIterator
 */
 
//! Array Const-Iterator class
template <typename T, int N>
class ConstArrayIterator
{
   public:
      
      ConstArrayIterator()
       : _strides(), _extends(), _pos(), _data(0) {}

      ConstArrayIterator(const ConstArrayIterator& x)
      {
         _strides=x._strides;
         _extends=x._extends;
         _data=x._data;
         _pos=x._pos;
      }

      ConstArrayIterator(const Array<T,N> &x)
       : _strides(x.GetStrides()),
         _extends(x.GetExtends()),
         _pos(),
         _data(x.data())
      {
         if (!x.GetExtends().size()) { _data=0; return; }
      }

      ~ConstArrayIterator() {}
   
      bool operator==(const ConstArrayIterator<T,N> &x) const
      {
         return (_data==x._data);
      }

      bool operator!=(const ConstArrayIterator<T,N> &x) const
      {
         return (_data!=x._data);
      }

      const T& operator*() const
      {
         return *_data;
      }

      const T* operator->() const
      {
         return _data;
      }

      ConstArrayIterator& operator++()
      {
         _pos[0]++;
         _data+=_strides[0];
         for (unsigned short int i=0; i<_extends.size()-1; ++i)
         {
            if (_pos[i]==_extends[i]) 
            {
               _pos[i]=0;
               _pos[i+1]++;
               _data+=_strides[i+1]-_extends[i]*_strides[i];
            }
         }
         if (_pos.back()==_extends.back()) _data=0;
         return *this;
      }

      ConstArrayIterator operator++(int)
      {
         ConstArrayIterator<T,N> tmp=*this;
         ++(*this);
         return tmp;
      }

      void advance() const
      {
         ++(*this);
      }


   protected:
      DimVector<N> _strides;
      extends _extends;
      DimVector<N> _pos;
      T* _data;
};



//! Array Iterator class
template <typename T, int N>
class ArrayIterator : public ConstArrayIterator<T,N>
{
      typedef ConstArrayIterator<T,N> T_base;
      using T_base::_strides;
      using T_base::_extends;
      using T_base::_data;

   public:
      
      ArrayIterator(){}

      ArrayIterator(Array<T,N>& x) : T_base(x) { }
      
      T& operator*() const
      {
         return *_data;
      }

      T* operator->() const
      {
         return _data;
      }

      ArrayIterator<T,N>& operator++()
      {
         T_base::operator++();
         return *this;
      }

      ArrayIterator<T,N> operator++(int)
      {
         ArrayIterator<T,N> tmp = *this;
         ++(*this); 
         return tmp;
      }

      void advance()
      {
         ++(*this);
      }

/*
      void advance_n(int n)
      {
//         _data+=n;
         _data+=n*_stride;
      }
*/
};



template <typename T>
inline std::ostream& operator<<(std::ostream& o,const Array<T,1>& v)
{
//   std::cout<<"ostream<<(Array<T,1>)"<<std::endl;
   o<<"(";
//   v.Print();
   std::copy(v.begin(),v.end(),std::ostream_iterator<T>(o," "));
   o<<"\b)";
   return o;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& o,Array<T,2>& v)
{
/*
   std::cout<<"ostream<<(Array<T,2>)"<<std::endl;
   std::cout<<"dims="<<v.GetExtends().size()<<std::endl;
   std::cout<<"size="<<v.size()<<std::endl;
   std::cout<<"extends[0]="<<v.GetExtends()[0]<<std::endl;
   std::cout<<"strides[0]="<<v.GetStrides()[0]<<std::endl;
   v[0].Print();
*/
   for (int i=0;i<v.rows();i++)
   {
      o<<v.Row(i)<<std::endl;
      
   }
//   o<<"\b)";
//   std::copy(v.begin(),v.end(),std::ostream_iterator<Array<T,1> >(o," "));
   return o;
}





// Member definitions

// class Array_base
template <typename T>
Array_base<T>::Array_base():_mem(0)
{
   _size = 0;
   _isReference = false;
}

template <typename T>
Array_base<T>::Array_base(size_t a_size, const bool init):_size(a_size)
{
   _isReference = false;
   if (!_size) return;
   _mem=new T[_size];
   if (init) for (size_t i=0;i<_size;i++) _mem[i]=T();
}

template <typename T>
Array_base<T>::Array_base(size_t a_size, const T& def):_size(a_size)
{
   _isReference = false;
   if (!_size) return;
   _mem=new T[_size];
   for (size_t i=0;i<_size;i++) _mem[i]=T(def);
}

template <typename T>
Array_base<T>::~Array_base()
{
   if (!_isReference)
      if (_size)
         delete[] _mem;
   _mem=0;
   _size = 0;
}

template <typename T>
void Array_base<T>::SetAt(T* address, size_t a_size)
{
   if (!_isReference)
      if (_size)
         delete[] _mem;
   _size=a_size;
   _mem=address;
   _isReference=true;
}




// class Array

template <typename T , int NrDims>
Array<T,NrDims>::Array():Array_base<T>(0)
{
   _extends.fill(NrDims,0);
   _bases.fill(0);
   _strides.fill(0);
}

template <typename T , int NrDims>
Array<T,NrDims>::Array(const extends &a_extends,
                       const bases &a_bases, const bool init)
                :Array_base<T>(_size=std::accumulate(a_extends.begin(),
                                                     a_extends.end(),
                                                     1,
                                                     std::multiplies<int>()),
                                                     init)
{
   assert((a_extends.size()==NrDims));
   _extends=a_extends;

   if (a_bases.empty()) _bases.fill(0);
   else
   {
      _bases=a_bases;
      assert((a_bases.size()==NrDims));
   }
   _strides.fill(0);
   CalcStrides();
}


template <typename T , int NrDims>
Array<T,NrDims>::Array(const extends &a_extends,
                       const bases &a_bases,
                       const strides &a_strides,
                       T* a_mem,
                       size_t a_size)
{
   assert((a_extends.size()==NrDims));
   _extends=a_extends;
   assert((a_bases.size()==NrDims));
   _bases=a_bases;
   assert((a_strides.size()==NrDims));
   _strides=a_strides;
   _mem=a_mem;
   _size=a_size;
   _isReference=true;
}

template <typename T , int NrDims>
Array<T,NrDims>::Array(const Array& src)
{
      if (_size!=src._size)
      {
         assert(this->resize(src.NrElements()));
         _isReference=false;
      }

      _extends=src._extends;
      _bases=src._bases;

      CalcStrides();
//      _strides.front()=1;
//      _strides.back()=_extends.front();

//      _isReference=false;
      std::copy(src.begin(),src.end(),this->begin());

/*

   if (_size!=src._size)
   {
      delete[] _mem;
      _size=src._size;
      if (_size) _mem=new T[_size];
   }

   _extends=src._extends;
   _bases=src._bases;
   _strides=src._strides;

   std::copy(src.begin(),src.end(),this->begin());
//   memcpy(_mem,src._mem,sizeof(T)*_size);
*/
}

template <typename T , int NrDims>
void Array<T,NrDims>::CalcStrides()
{
   _strides[0]=1;
   for (int i=1; i<NrDims; ++i)
   {
    _strides[i]=std::accumulate(_extends.begin(),_extends.begin()+i,1,std::multiplies<int>());
//          std::cout<<"   Dim"<<i+1<<": "<<_strides[i]<<std::endl;
   }
}

/*! \todo assignment auf Referenz noch nicht korrekt.
 *  s. todo ArrayIterator
 */
template <typename T , int NrDims>
const Array<T,NrDims>& Array<T,NrDims>::operator=(const Array<T,NrDims> &src)
{
//   std::cout<<"Array<"<<typeid(T).name()<<","<<NrDims
//            <<">::operator=(...&x)"<<std::endl;

   if (this == &src) return *this;


   if (_isReference)
   {
      assert(this->NrElements()==src.NrElements());
      _extends=src._extends;
      _bases=src._bases;
   }
   else
   {
      if (_size!=src.NrElements())
      {
         assert(this->resize(src.NrElements()));
         _isReference=false;
      }
      _extends=src._extends;
      _bases=src._bases;

      CalcStrides();
   }

   std::copy(src.begin(),src.end(),this->begin());
   return *this;
}

template <typename T , int NrDims>
const Array<T,NrDims>& Array<T,NrDims>::operator=(const T& val)
{
//   std::cout<<"Array<"<<typeid(T).name()<<","<<NrDims
//            <<">::operator=(T)(0)"<<std::endl;
   if (NrDims==0)
   {
      _mem[0]=val;
      return *this;
   }

//   for (size_t i=0; i<_size; i+=_strides[0]) _mem[i]=val;
//   std::cout<<"val[1][2]="<<_mem[19]<<std::endl;
   
   std::fill(this->begin(),this->end(),val);
   return *this;
}

template <typename T , int NrDims>
Array<T,NrDims-1> Array<T,NrDims>::operator[](int in) const
{

   extends ext=_extends;
   bases bas =_bases;
   strides str =_strides;

   ext.pop_front();
   bas.pop_front();
   str.pop_front();

   int offs=_strides[0]*in;

   Array<T,NrDims-1> _x(ext,
                        DimVector<NrDims-1>(bas),
                        DimVector<NrDims-1>(str),
                        _mem+offs,
                        _size-offs);

   return _x;
}

template <typename T , int NrDims>
Array<T,NrDims> Array<T,NrDims>::operator[](Range r)
{

   extends ext=_extends;
   bases bas =_bases;
   strides str =_strides;

   ext.push_back(ext.front());
   ext.pop_front();

   bas.push_back(bas.front());
   bas.pop_front();

   str.push_back(str.front());
   str.pop_front();

   Array<T,NrDims> _x(ext,
                      DimVector<NrDims>(bas),
                      DimVector<NrDims>(str),
                      _mem,
                      _size);
   return _x;
}


template <typename T , int NrDims>
size_t Array<T,NrDims>::NrElements() const
{
    return std::accumulate(_extends.begin(),_extends.end(),1,std::multiplies<int>());
}

template <typename T , int NrDims>
T& Array<T,NrDims>::GetElement(indices a_indices) const
{
   long offset = 0;
   indices index=a_indices;
//   assert(index.size()==NrDims);
//   std::cout<<"index.size()="<<index.size()<<std::endl;
//   std::cout<<"index[0]="<<index[0]<<std::endl;
   // fill rest with zeroes
   for (int i=index.size(); i<NrDims; ++i) index.push_back(0);
   for (int i=0; i<NrDims;++i)
   {
      offset+=index[i]*_strides[i];
   }

//   std::cout<<"Offset="<<offset<<std::endl;

   return _mem[offset];
}

template <typename T , int NrDims>
void Array<T,NrDims>::Print() const
{
   std::cout<<"Object: Array"<<std::endl;
   std::cout<<"Field Address: "<<this<<std::endl;
   std::cout<<"Data Address: "<<_mem<<std::endl;
   if (_isReference)
      std::cout<<"(Field is a reference!)"<<std::endl;
   std::cout<<"Datatype: "<<typeid(T).name()<<std::endl;
   std::cout<<"size of Datatype: "<<sizeof(T)<<" bytes"<<std::endl;
   std::cout<<"Dimensions: "<<NrDims<<std::endl;
   std::cout<<"Nr. of Elements: "<<NrElements()<<std::endl;
   std::cout<<"Extends: "<<std::endl;
   for (int i=0;i<NrDims;i++)
      std::cout<<"   Dim"<<i+1<<": "<<_extends[i]<<std::endl;
   std::cout<<"Bases: "<<std::endl;
   for (int i=0;i<NrDims;i++)
      std::cout<<"   Dim"<<i+1<<": "<<_bases[i]<<std::endl;
   std::cout<<"Strides: "<<std::endl;
   for (int i=0;i<NrDims;i++)
      std::cout<<"   Dim"<<i+1<<": "<<_strides[i]<<std::endl;
   std::cout<<"size: "<<this->size()<<std::endl;
   std::cout<<"size of Array: "<<sizeof(T)*this->size()<<" bytes"<<std::endl;
}





// class DimVector

template <int NrDims>
DimVector<NrDims>::DimVector(int i0,...):std::deque<int>()
{
   va_list ap;
   va_start(ap,i0);
   this->push_back(i0);
   int in;
//   std::cout<<"i0="<<i0<<std::endl;
   for (int i=1;i<NrDims;++i)
   {
      in=va_arg(ap,int);
//      std::cout<<"i"<<i<<"="<<in<<std::endl;
      this->push_back(in);
   }
   va_end(ap);
}

template <int NrDims>
template <int otherDim>
DimVector<NrDims>::DimVector(const DimVector<otherDim> &x):std::deque<int>(NrDims,0)
{
   std::copy(x.begin(),x.begin()+std::min(x.size(),this->size()),this->begin());
/*
   for (int i=0;i<std::min(NrDims,otherDim);++i)
   {
      std::cout<<i<<"="<<x[i]<<std::endl;
   }
*/
}

template <int NrDims>
DimVector<NrDims>& DimVector<NrDims>::operator()(int i0,...)
{
   this->clear();
   va_list ap;
   va_start(ap,i0);
   this->push_back(i0);
   int in;
//   std::cout<<"i0="<<i0<<std::endl;
   for (int i=1;i<NrDims;++i)
   {
      in=va_arg(ap,const int);
//      std::cout<<"i"<<i<<"="<<in<<std::endl;
      this->push_back(in);
   }
   va_end(ap);
   return *this;
}

template <int NrDims>
DimVector<1>& DimVector<NrDims>::operator=(int i0)
{
   DimVector<1>* _x=(DimVector<1>*)(this);
   _x->clear();
   _x->push_back(i0);
//      std::cout<<"added index0="<<(*this)[0]<<std::endl;
   return *_x;
}

template <int NrDims>
DimVector<NrDims+1>& DimVector<NrDims>::operator,(int in)
{
   DimVector<NrDims+1>* _x=(DimVector<NrDims+1>*)(this);
   _x->push_back(in);
//      std::cout<<"added index"<<this->size()-1<<"="<<(*this)[this->size()-1]<<std::endl;
   return *_x;
}

template <int NrDims>
void DimVector<NrDims>::fill(int value)
{
   this->clear();
   for (int i=0;i<NrDims; ++i) this->push_back(value);
}


} // namespace hgz


namespace std{

template <typename T, int N>
struct iterator_traits< hgz::ArrayIterator<T,N> > 
{
    typedef forward_iterator_tag               iterator_category;
    typedef T                                  value_type;
    typedef ptrdiff_t                          difference_type;
    typedef T*                                 pointer;
    typedef T&                                 reference;
};

template <typename T, int N>
struct iterator_traits< hgz::ConstArrayIterator<T,N> > 
{
    typedef forward_iterator_tag               iterator_category;
    typedef T                                  value_type;
    typedef ptrdiff_t                          difference_type;
    typedef const T*                           pointer;
    typedef const T&                           reference;
};


} // namespace std



#endif //_ARRAY_H

