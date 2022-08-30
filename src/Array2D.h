#ifndef _ARRAY2D_H
#define _ARRAY2D_H

#include <complex>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <climits>
#include <cstring>

//#include <CCfits>


#include <QtGui>

//#include <QT.h>
//#include <QColor>
//#include <QImage>
#include "basic.h"

class QColor;
class QImage;

//using namespace Qt;
//using namespace CCfits;

namespace hgz{


template <typename T>
class Array2D;

//template <>
double max(const Array2D<double> &x, int *x_max, int *y_max);

template <class T>
T max(const Array2D<T> &x);
template <class T>
T min(const Array2D<T> &x);

template <typename T=real_t>
class Array2D : public AbstractArray<T>
{
//   using AbstractArray<T>::_size;
//   using AbstractArray<T>::mem;
public:

   /** Default Constructor
   */
   Array2D() : AbstractArray<T>(),_xsize(0),_ysize(0)
   { }

   /** overloaded Constructor,
       create array with dimensions <i>xsize , ysize</i>
   */
   Array2D(size_t a_xsize, size_t a_ysize)
      :AbstractArray<T>(a_xsize*a_ysize),_xsize(a_xsize),_ysize(a_ysize)
   {}

/*
   Array2D(int a_xsize, int a_ysize):ysize(a_ysize),xsize(a_xsize)
   {
      size=0;
      size=(long)a_xsize*(long)a_ysize;
      if (size!=0)
         mem=new T[size];
      for (long i=0;i<size;i++) mem[i]=T();
   }
*/
   /** overloaded Constructor,
       create array with dimensions <i>xsize , ysize</i> filled with
       default value <i>def</i>
   */
   Array2D(size_t a_xsize, size_t a_ysize, T def)
      :AbstractArray<T>(a_xsize*a_ysize,def),_xsize(a_xsize),_ysize(a_ysize)
   {}

/*
   Array2D(int a_xsize, int a_ysize, T def):ysize(a_ysize),xsize(a_xsize)
//   Array2D(int a_xsize, int a_ysize, T def)
   {
//      Array2D(a_xsize,a_ysize);

      size=(long)a_xsize*(long)a_ysize;
      if (size)
         mem=new T[size];

      for (long i=0;i<size;i++) mem[i]=def;
//      (*this)+=def;
   }
*/
   /** Copy Constructor
   */
   Array2D<T>(const Array2D<T> &src)
   {
      if (this == &src) return;

      _xsize=src._xsize;
      _ysize=src._ysize;
      this->_size=_xsize*_ysize;

//      AbstractArray(_xsize*y_size);
      if (this->mem) { delete[] this->mem; this->mem=0; }
      if (this->_size)
         this->mem=new T[this->_size];
/*
      int i=src.xsize;
      int j=src.ysize;
      cout<<"x= "<<i<<"y= "<<j<<endl;
      Array2D(i,j);
*/
//      Array2D(int i=1,int j=2);
      memcpy(this->mem,src.mem,sizeof(T)*this->_size);
   }

   /** overloaded Constructor,
       read array from file <i>name</i>
   */
   Array2D(const std::string& name)
   {
//      _size=0;
      _xsize=0;
      _ysize=0;
      if (this->mem) { delete[] this->mem; this->mem=0; }
      this->Read(name.c_str());
   }

   /** Default Destructor
   */
   ~Array2D()
   {
//      if (size)
//         delete[] mem;
      _xsize = 0;
      _ysize = 0;
//      size = 0;
   }

   inline size_t xsize() const { return _xsize; }
   inline size_t ysize() const { return _ysize; }

   void SetSize(size_t a_xsize, size_t a_ysize)
   {
      if (this->_size)
         delete[] this->mem;
      this->_size=a_xsize*a_ysize;
      _xsize = a_xsize;
      _ysize = a_ysize;
      this->mem=new T[this->_size];
   }

private:
   struct Proxy
   {
      Proxy(long a_offset, signed int a_ysize, T*& a_array):
         offset(a_offset), ysize(a_ysize), array(a_array)
      {}
      Proxy(long a_offset, signed int a_ysize, T* const& a_array):
            offset(a_offset), ysize(a_ysize), array(const_cast<T*&>(a_array))
      {}
      inline T & operator[](signed int j)
      {
         int i=j;
         if (j<0) i+=ysize;
         return array[offset + i];
      }
      inline const T & operator[](signed int j) const
      {
         int i=j;
         if (j<0) i+=ysize;
         return array[offset + i];
      }

      private:
         long offset;
         signed int ysize;
         T* &array;
   };

public:

   inline T* At(int x)
   {
      if (x<0) x+=_ysize;
      return this->mem+x*_ysize;
   }

   inline signed int xindex_lo(void) const
   {
      if (!_xsize) return 0;
      return -((_xsize)/2);
   }

   inline signed int yindex_lo(void) const
   {
      if (!_ysize) return 0;
      return -((_ysize)/2);
   }

   inline signed int xindex_hi(void) const
   {
      if (!_xsize) return 0;
      return xindex_lo()+_xsize-1;
   }

   inline signed int yindex_hi(void) const
   {
      if (!_ysize) return 0;
      return yindex_lo()+_ysize-1;
   }

   inline T GetElement(size_t i) const
   {
      return this->mem[i];
   }

/*
	T GetElement(unsigned int i,unsigned int j)
	{
		return (*this)[i][j];
	}
*/
   inline T GetElement(signed int i,signed int j)
   {
      signed int x,y;
      x=i;
      y=j;
      if (x<0) x+=_xsize;
      if (y<0) y+=_ysize;
      return (*this)[x][y];
   }

   inline void PutElement(size_t i, T value)
   {
      this->mem[i]=value;
   }


   void ShiftXY(int xshift, int yshift)
   {
      if ((!xshift) && (!yshift)) return;
      Array2D<T> temp(_xsize,_ysize);
      for (int i=0;i<(int)_xsize;i++)
         for (int j=0;j<(int)_ysize;j++)
         {
            int x=i+xshift;
            int y=j+yshift;
            if ((x>=0) && (x<(int)_xsize) &&
                (y>=0) && (y<(int)_ysize))
            {
               temp[x][y]=(*this)[i][j];
            }
         }
      (*this)=temp;
   }

   double SumSquares()
   {
      double sum = 0;
      for (size_t i=0;i<this->_size;i++)
			sum+=std::abs(this->mem[i])*std::abs(this->mem[i]);
//            sum+=norm(mem[i]);
      return sum;
   }

   T Sum()
   {
      T sum = 0;
      for (size_t i=0;i<this->_size;i++)
         sum+=this->mem[i];
      return sum;
   }

   void AbsSquare()
   {
      for (size_t i=0;i<this->_size;i++)
      {
         complex_t v=this->mem[i];
         this->mem[i]=std::conj(v)*v;
//            mem[i]=norm(mem[i]);
      }
   }

   inline void Abs()
   {
      for (size_t i=0;i<this->_size;i++)
         this->mem[i]=std::abs(this->mem[i]);
   }

   double MaxAmplitude() const
   {
      if (!this->_size) return 0;
      double max = 0;
      double v;
      for (size_t i=0;i<this->_size;i++)
      {
         v=std::abs(this->mem[i]);
         if (v>max) max=v;
      }
      return max;
   }

   double MinAmplitude() const
   {
      if (!this->_size) return 0;
      double min = INFINITY;
      double v;
      for (size_t i=0;i<this->_size;i++)
      {
         v=std::abs(this->mem[i]);
         if (v<min) min=v;
      }
      return min;
   }

   double StdDev() const
   {
      return 0.;
   }

   double Mean() const
   {
      return 0.;
   }

   friend T min<>(const Array2D<T> &x);
   friend T max<>(const Array2D<T> &x);

   friend T max(const Array2D<T> &x, int *x_max, int *y_max)
   {
      Array2D<T> _z=x;
      if (!_z.size()) return 0;
      T max = -INFINITY;
      T v;
      size_t ix_max=0;
      for (size_t i=0;i<_z.size();i++)
      {
         v=_z.mem[i];
         if (v>max) {max=v; ix_max=i;}
      }
      if (x_max && y_max) {
         *x_max=ix_max/_z._ysize;
         *y_max=ix_max-(*x_max)*_z._ysize;
         if (*x_max>_z.xindex_hi()) *x_max-=_z._xsize;
         if (*y_max>_z.yindex_hi()) *y_max-=_z._ysize;
      }
      return max;
   }

   friend T min(const Array2D<T> &x, int *x_min, int *y_min)
   {
      Array2D<T> _z=x;
      if (!_z.size()) return 0;
      T min = INFINITY;
      T v;
      size_t ix_min=0;
      for (size_t i=0;i<_z.size();i++)
      {
         v=_z.mem[i];
         if (v<min) {min=v; ix_min=i;}
      }
      *x_min=ix_min/_z._ysize;
      *y_min=ix_min-(*x_min)*_z._ysize;
      if (*x_min>_z.xindex_hi()) *x_min-=_z._xsize;
      if (*y_min>_z.yindex_hi()) *y_min-=_z._ysize;
      return min;
   }

/*
   friend Array2D<T> abs(const Array2D<T> &x)
   {
      Array2D<T> _z=x;
      _z.Abs();
      return _z;
   }
*/

   friend Array2D<T> norm(const Array2D &x)
   {
      Array2D<T> _z=x;
      for (size_t i=0;i<_z.size();i++)
         _z.mem[i]=std::norm(_z.mem[i]);
      return _z;
   }

   friend Array2D<T> abs_square(const Array2D &x)
   {
      Array2D<T> _z=x;
      _z.AbsSquare();
      return _z;
   }

   friend Array2D<T> conj(const Array2D<T> &x)
   {
      Array2D<complex_t> _z=x;
      for (size_t i=0;i<_z.size();i++)
         _z.mem[i]=std::conj(_z.mem[i]);
      return _z;
   }

   Array2D<T> GetRect(int x, int y, int width, int height)
   {
      Array2D<T> _z(width,height);
      for (int i=0;i<width;i++)
         for (int j=0;j<height;j++)
           if ((i+x)>=0 && (i+x)<this->xsize() && (j+y)>=0 && (j+y)<this->ysize()) _z[i][j]=(*this)[i+x][j+y];
	   else _z[i][j]=T();
      return _z;
   }

   void Insert(Array2D<T> &src, int posx, int posy)
   {
      if (!src._xsize || !src._ysize) return;

      for (int i=posx; i<posx+(int)src._xsize; i++)
         for (int j=posy; j<posy+(int)src._ysize; j++)
         {
            if (!( i<0 || i>=(int)_xsize || j<0 || j>=(int)_ysize))
               (*this)[i][j]=src[i-posx][j-posy];
         }

   }

   void Print()
   {
//         signed int i,j;
//         std::cout<<"Object: "<<typeid(Array2D<T>).name()<<std::endl;
      std::cout<<"Object: Array2D"<<std::endl;
      std::cout<<"Address: "<<this<<std::endl;
      std::cout<<"Datatype: "<<typeid(T).name()<<std::endl;
//       cout<<"size of Object: "<<sizeof((Array2D<T>)*this)<<" bytes"<<endl;
      std::cout<<"size: "<<this->size()<<std::endl;
      std::cout<<"x-size: "<<xsize()<<std::endl;
      std::cout<<"y-size: "<<ysize()<<std::endl;
      std::cout<<"first x-index: "<<xindex_lo()<<std::endl;
      std::cout<<"first y-index: "<<yindex_lo()<<std::endl;
      std::cout<<"last x-index: "<<xindex_hi()<<std::endl;
      std::cout<<"last y-index: "<<yindex_hi()<<std::endl;
      std::cout<<"size of Datatype: "<<sizeof(T)<<" bytes"<<std::endl;
      std::cout<<"size of Array: "<<sizeof(T)*this->size()<<" bytes"<<std::endl;
//       cout<<"size of Array: "<<sizeof(mem)<<" bytes"<<endl;
      std::cout<<"Sum: "<<Sum()<<std::endl;
      std::cout<<"Sum of Squares: "<<SumSquares()<<std::endl;
      std::cout<<"Maximum Magnitude: "<<MaxAmplitude()<<std::endl;
      std::cout<<"Minimum Magnitude: "<<MinAmplitude()<<std::endl;
/*
      cout<<"Contents (raw): "<<endl;
      for (j=yindex_start();j<yindex_start()+ysize;j++)
      {
         cout<<"(";
         for (i=xindex_start();i<xindex_start()+xsize;i++)
         {
            cout<<(*this)[i][j]<<" ";
         }
         cout<<")"<<endl;
      }

      cout<<endl;

      cout<<"Contents (absolutes): "<<endl;
      for (j=yindex_start();j<yindex_start()+ysize;j++)
      {
         cout<<"(";
         for (i=xindex_start();i<xindex_start()+xsize;i++)
         {
//	            cout<<abs(mem[i*ysize+j])<<" ";
            cout<<abs((*this)[i][j])<<" ";
         }
         cout<<")"<<endl;
      }
*/
   }

   void Write(const char *name)
   {
      FILE *stream;

      errno = 0;
      stream = fopen (name, "wb");
      if (stream == NULL)
      {
         fprintf (stderr, "%s: Couldn't open file %s; %s\n",
            program_invocation_short_name, name, strerror (errno));
         exit (EXIT_FAILURE);
         return;
      }
//         fwrite(&size,sizeof(T),1,stream);
      fwrite(&this->_size,sizeof(this->_size),1,stream);
      fwrite(&_xsize,sizeof(_xsize),1,stream);
      fwrite(&_ysize,sizeof(_ysize),1,stream);
      fwrite(this->mem,sizeof(T),this->_size,stream);
      fclose(stream);
   }

   void Read(const char *name)
   {
      size_t bytes_read=0;
      FILE *stream;

      errno = 0;
      stream = fopen (name, "rb");
      if (stream == NULL)
      {
         fprintf (stderr, "%s: Couldn't open file %s; %s\n",
            program_invocation_short_name, name, strerror (errno));
         exit (EXIT_FAILURE);
         return;
      }
      if (this->_size) delete[] this->mem;

      int typesize = 1;
//         fread(&typesize,sizeof(typesize),1,stream);
      fread(&this->_size,sizeof(this->_size),1,stream);
      fread(&_xsize,sizeof(_xsize),1,stream);
      fread(&_ysize,sizeof(_ysize),1,stream);
      if (this->_size!=_xsize*_ysize)
      {
         fprintf(stderr,"File %s corrupted\n",name);
         fclose(stream);
         exit (-1);
      }

      this->mem=new T[this->_size];
//       mem=(T*)realloc(mem,size*sizeof(T));

      if (fread(this->mem,sizeof(T),this->_size,stream)!=this->_size)
         fprintf(stderr,"File %s corrupted\n",name);
//          std::cout<<"File "<<name<<" corrupted"<<std::endl;
      fclose(stream);
   }


   bool ReadImage(const char *name, const QColor ColorChannel = Qt::white)
   // TODO: �berpr�fung, ob Bild richtig geladen wurde
   {
      FILE *stream;

      errno = 0;
      stream = fopen (name, "rb");
      if (stream == NULL)
      {
         fprintf (stderr, "%s: Couldn't open file %s; %s\n",
            program_invocation_short_name, name, strerror (errno));
//         exit (EXIT_FAILURE);
         return false;
      }
      fclose(stream);

//      std::cout<<"reading image "<<name<<std::endl;

      QImage img(name);

//      std::cout<<"dimensions :"<<img.width()<<"x"<<img.height()<<"x"<<img.depth()<<"bpp"<<std::endl;

      if (this->_size) delete[] this->mem;

      _xsize=img.width();
      _ysize=img.height();
      this->_size=_xsize*_ysize;
      this->mem=new T[this->_size];

      if (img.depth()==8)
      {
         for (int j=0; j<img.height(); j++)
         {                 // set image pixels
            uchar *p = (uchar*)img.scanLine(j);
            for (int i=0; i<img.width(); i++ )
            {
               double v=(double)((uchar)(*p) & 0xFF)/255.0;
               p++;
               (*this)[i][j]=v;
            }
         }
         return true;
      }

      if (img.depth()==16)
      {
         for (int j=0; j<img.height(); j++)
         {                 // set image pixels
            unsigned short *p = (unsigned short*)img.scanLine(j);
            for (int i=0; i<img.width(); i++ )
            {
               double v=(double)((unsigned short)(*p) & 0xFFFF)/65535.0;
               p++;
               (*this)[i][j]=v;
            }
         }
         return true;
      }

      if ((ColorChannel==Qt::red)
       || (ColorChannel==Qt::green)
       || (ColorChannel==Qt::blue))
      {
         const double norm = ColorChannel.red()+ColorChannel.green()+ColorChannel.blue();
         /*
         std::cout<<"color mask : r="<<ColorChannel.red()
             <<" g="<<ColorChannel.green()
             <<" b="<<ColorChannel.blue()
             <<" alpha="<<qAlpha(ColorChannel.rgb())<<std::endl;
         */
         for (int j=0; j<img.height(); j++)
         {
            // set image pixels
            QRgb *p = (QRgb*)img.scanLine(j);
            for (int i=0; i<img.width(); i++ )
            {
               double v=(double)((qRed((QRgb)(*p)) & ColorChannel.red())
                              +(qGreen((QRgb)(*p)) & ColorChannel.green())
                              + (qBlue((QRgb)(*p)) & ColorChannel.blue())
                              )/norm;
               p++;
               (*this)[i][j]=v;
            }
         }
         return true;
      }

      if (ColorChannel==Qt::white)
      {
         /*
         std::cout<<"color mask : r="<<ColorChannel.red()
             <<" g="<<ColorChannel.green()
             <<" b="<<ColorChannel.blue()
             <<" alpha="<<qAlpha(ColorChannel.rgb())<<std::endl;
         */
         for (int j=0; j<img.height(); j++)
         {
            // set image pixels
            QRgb *p = (QRgb*)img.scanLine(j);
            for (int i=0; i<img.width(); i++ )
            {
               double v=(double)((qRed((QRgb)(*p)) & ColorChannel.red())
                              +(qGreen((QRgb)(*p)) & ColorChannel.green())
                              + (qBlue((QRgb)(*p)) & ColorChannel.blue())
                              )/765.0;
               p++;
               (*this)[i][j]=v;
            }
         }
         return true;
      }

      if (ColorChannel==Qt::gray)
      {
         for (int j=0; j<img.height(); j++)
         {                 // set image pixels
            QRgb *p = (QRgb*)img.scanLine(j);
            for (int i=0; i<img.width(); i++ )
            {
               double v=(double)( qGray((QRgb)(*p++)) )/255.0;
               (*this)[i][j]=v;
            }
         }
         return true;
      }


      for (int j=0; j<img.height(); j++)
      {
         // set image pixels
         QRgb *p = (QRgb*)img.scanLine(j);
         for (int i=0; i<img.width(); i++ )
         {
            double v=(double)( ((QRgb)(*p++)) );
            (*this)[i][j]=v;
         }
      }

      return true;
   }


   void WriteImage(const char *name,
                   const QColor ColorChannel = Qt::white,
       		       const bool symmetric = false,
                   const bool normalize = false)
   {
      FILE *stream;

      errno = 0;
      stream = fopen (name, "wb");
      if (stream == NULL)
      {
         fprintf (stderr, "%s: Couldn't create file %s; %s\n",
            program_invocation_short_name, name, strerror (errno));
         exit (EXIT_FAILURE);
         return;
      }
      fclose(stream);

      QImage img(_xsize,_ysize,QImage::Format_RGB32);

//         if (size) delete[] mem;

//         xsize=img.width();
//         ysize=img.height();
//         size=(long)xsize*(long)ysize;
//         mem=new T[size];

      double normfact = 1.0;
      if (normalize) normfact=this->MaxAmplitude();


      if ((ColorChannel==Qt::red)
       || (ColorChannel==Qt::green)
       || (ColorChannel==Qt::blue))
      {
         /*
         std::cout<<"color mask : r="<<ColorChannel.red()
             <<" g="<<ColorChannel.green()
             <<" b="<<ColorChannel.blue()
             <<" alpha="<<qAlpha(ColorChannel.rgb())<<std::endl;
         */
         int jj=yindex_lo();
         for (int j=0; j<img.height(); j++,jj++)
         {                 // set image pixels
            QRgb *p = (QRgb*)img.scanLine(j);
            int ii=xindex_lo();
            for (int i=0; i<img.width(); i++,ii++ )
            {
               T value;
               if (symmetric)
                  value=(*this)[ii][jj]/(real_t)normfact*(real_t)255.0;
               else
                  value=(*this)[i][j]/(real_t)normfact*(real_t)255.0;
               int v = (int)(std::abs(value)) & 0xff;

               (*p++) = (QRgb)qRgb(v & ColorChannel.red(),
                                   v & ColorChannel.green(),
                                   v & ColorChannel.blue());

            }
         }
         img.save(name,"BMP");
         return;
      }

      if (ColorChannel==Qt::white)
      {
         /*
         std::cout<<"color mask : r="<<ColorChannel.red()
             <<" g="<<ColorChannel.green()
             <<" b="<<ColorChannel.blue()
             <<" alpha="<<qAlpha(ColorChannel.rgb())<<std::endl;
	      */
         int jj=yindex_lo();
         for (int j=0; j<img.height(); j++,jj++)
         {                 // set image pixels
            QRgb *p = (QRgb*)img.scanLine(j);
	         int ii=xindex_lo();
            for (int i=0; i<img.width(); i++,ii++)
            {
		         T value;
		         if (symmetric)
		            value=(*this)[ii][jj]/(real_t)normfact*(real_t)255.0;
		         else
		            value=(*this)[i][j]/(real_t)normfact*(real_t)255.0;
               int v = (int)(std::abs(value)) & 0xff;

               (*p++) = (QRgb)qRgb(v,v,v);
            }
         }
         img.save(name,"BMP");
         return;
      }

      if (ColorChannel==Qt::gray)
      {
			int jj=yindex_lo();
         for (int j=0; j<img.height(); j++,jj++)
         {                 // set image pixels
            QRgb *p = (QRgb*)img.scanLine(j);
	      	int ii=xindex_lo();
            for (int i=0; i<img.width(); i++,ii++)
            {
			      T value;
               if (symmetric)
                  value=(*this)[ii][jj]/(real_t)normfact*(real_t)255.0;
               else
                  value=(*this)[i][j]/(real_t)normfact*(real_t)255.0;
               int v = (int)(std::abs(value)) & 0xff;
               (*p++) = (QRgb)qRgb(v,v,v);
            }
         }
         img.save(name,"BMP");
         return;
      }


      int jj=yindex_lo();
      for (int j=0; j<img.height(); j++,jj++)
      {                 // set image pixels
         QRgb *p = (QRgb*)img.scanLine(j);
         int ii=xindex_lo();
         for (int i=0; i<img.width(); i++,ii++ )
         {
            T value;
      	    if (symmetric)
      	       value=(*this)[ii][jj];
      	    else
      	       value=(*this)[i][j];
            double v = std::abs(value)*double(0xffffffff);

            (*p++) = (QRgb)(v);
         }
      }

      img.save(name,"BMP");
   }

   inline const Proxy operator[](signed int i) const
   {
      int j=i;
      if (i<0) j+=_xsize;
      size_t temp=_ysize*j;
      return Proxy(temp,_ysize,this->mem);
   }

   inline Proxy operator[](signed int i)
   {
      int j=i;
      if (i<0) j+=_xsize;
      size_t temp=_ysize*j;
      return Proxy(temp,_ysize,this->mem);
   }

   inline Array2D<T>& operator=(T x)
   {
      for (size_t i=0;i<this->_size;i++)
         this->mem[i]=T(x);
      return *this;
   }

   inline Array2D<T>& operator+=(T x)
   {
      for (size_t i=0;i<this->_size;i++)
         this->mem[i]+=x;
      return *this;
   }

   inline Array2D<T>& operator-=(T x)
   {
      for (size_t i=0;i<this->_size;i++)
         this->mem[i]-=x;
      return *this;
   }

   inline Array2D<T>& operator*=(T x)
   {
      for (size_t i=0;i<this->_size;i++)
         this->mem[i]*=x;
      return *this;
   }

   inline Array2D<T>& operator/=(T x)
   {
      for (size_t i=0;i<this->_size;i++)
      {
         if (std::abs(x)==0) this->mem[i]=INFINITY;
         else
            this->mem[i]/=x;
      }
      return *this;
   }

   Array2D<T>& operator=(const Array2D<T> &x)
   {

      if (this == &x) return *this;
      if (this->_size!=x._size)
      {
         if (this->_size)
            delete[] this->mem;
         this->_size=x._size;
         _xsize=x._xsize;
         _ysize=x._ysize;
         this->mem=new T[this->_size];
      }
      memcpy(this->mem,x.mem,sizeof(T)*this->_size);
/*
      long i;
      for (i=0;i<size;i++)
         mem[i]=x.mem[i];
*/
//       cout<<"M1: "<<&x<<endl;
//         std::cout<<"Array2D::operator= called"<<std::endl;
      return *this;
   }

   Array2D<T>& operator+=(const Array2D<T> &x)
   {
//         std::cout<<"Array2D::operator+= called"<<std::endl;
      if (this->_size!=x._size)
      {
         *this=x;
         return *this;
      }

      for (size_t i=0;i<this->_size;i++)
         this->mem[i]+=x.mem[i];

      return *this;
   }


   Array2D<T>& operator-=(const Array2D<T> &x)
   {
//         std::cout<<"Array2D::operator-= called"<<std::endl;
      if (this->_size!=x._size)
      {
         *this=x;
         return *this;
      }

      for (size_t i=0;i<this->_size;i++)
         this->mem[i]-=x.mem[i];

      return *this;
   }

   Array2D<T>& operator*=(const Array2D<T> &x)
   {
//         std::cout<<"Array2D::operator*= called"<<std::endl;
      if (this->_size!=x._size)
      {
         *this=x;
         return *this;
      }

      for (size_t i=0;i<this->_size;i++)
         this->mem[i]*=x.mem[i];

      return *this;
   }

   Array2D<T>& operator/=(const Array2D<T> &x)
   {
//         std::cout<<"Array2D::operator*= called"<<std::endl;
      if (this->_size!=x._size)
      {
         *this=x;
         return *this;
      }

      for (size_t i=0;i<this->_size;i++)
         if (std::abs(x.mem[i])==0) this->mem[i]=INFINITY;
         else
            this->mem[i]/=x.mem[i];
         return *this;
   }


 template <class U>
   Array2D<T>& operator=(const Array2D<U> &x)
   {

//      if (this == &x) return *this;
      if (this->_size!=x.size())
      {
         if (this->_size)
            delete[] this->mem;
         this->_size=x.size();
         _xsize=x.xsize();
         _ysize=x.ysize();
         this->mem=new T[this->_size];
      }

      for (size_t i=0;i<this->_size;i++)
         this->mem[i]=static_cast<T>(x.GetElement(i));
//      memcpy(mem,x.mem,sizeof(T)*_size);

//         std::cout<<"Array2D::operator= called"<<std::endl;
      return *this;
   }

 template <class U>
   Array2D<T>& operator*=(const Array2D<U> &x)
   {
//         std::cout<<"Array2D::operator*= called"<<std::endl;
      if (this->_size!=x.size())
      {
         *this=x;
         return *this;
      }

      for (size_t i=0;i<this->_size;i++)
         this->mem[i]*=static_cast<T>(x.GetElement(i));

      return *this;
   }

   inline friend Array2D<T> operator*(const Array2D<T> &x,const Array2D<T> &y)
   {
      Array2D<T> _z=x;
      _z*=y;
      return _z;
//         return Array2D<T>(x) *= y;
   }


private:
protected:
      size_t _xsize;
      size_t _ysize;
};


template <class T>
T max(const Array2D<T> &x)
{
   Array2D<T> _z=x;
   if (!_z.size()) return 0;
   T max = -1e+38;
   T v;
   for (size_t i=0;i<_z.size();i++)
   {
      v=_z.mem[i];
      if (v>max) max=v;
   }
   return max;
}

template <class T>
T min(const Array2D<T> &x)
{
//   Array2D<T> _z=x;
   if (!x.size()) return 0;
   T min = 1e+38;
   T v;
   for (size_t i=0;i<x.size();i++)
   {
      if (isnan(x.mem[i])) continue;
      v=x.mem[i];
      if (v<min) min=v;
   }
   return min;
}


inline Array2D<double> real(const Array2D<complex_t> &x)
{
   Array2D<double> _z(x.xsize(),x.ysize());

   for (size_t i=0;i<_z.size();i++)
      _z.PutElement(i,std::real(x.GetElement(i)));
//         std::cout<<"Array2D::operator= called"<<std::endl;
   return _z;
}

inline Array2D<double> abs(const Array2D<complex_t> &x)
{

//      if (this == &x) return *this;
   Array2D<double> _z(x.xsize(),x.ysize());

   for (size_t i=0;i<_z.size();i++)
      _z.PutElement(i,std::abs(x.GetElement(i)));
//      memcpy(mem,x.mem,sizeof(T)*_size);

//         std::cout<<"Array2D::operator= called"<<std::endl;
   return _z;
}

inline Array2D<double> arg(const Array2D<complex_t> &x)
{

//      if (this == &x) return *this;
   Array2D<double> _z(x.xsize(),x.ysize());

   for (size_t i=0;i<_z.size();i++)
      _z.PutElement(i,std::arg(x.GetElement(i)));
//      memcpy(mem,x.mem,sizeof(T)*_size);

//         std::cout<<"Array2D::operator= called"<<std::endl;
   return _z;
}

/*
inline Array2D<complex_t> abs(const Array2D<complex_t> &x)
{
   Array2D<complex_t> _z=x;
   _z.Abs();
   return _z;
}
*/

} // namespace hgz



#endif //_ARRAY2D_H

