#ifndef _HGZFUNCTION_H
#define _HGZFUNCTION_H

#include <functional>
#include <vector>
#include <exception>
#include <cmath>
#include <cassert>

#include "basic.h"


class math_error : public std::exception
{};
class divide_by_zero_error : public std::exception
{};


namespace hgz
{


/**
 @class Operation
 generic base class for algebraic operations
*/
template <typename Targ, typename Tret>
class Operation
{
   public:
      typedef Targ arg_type;
      typedef Tret ret_type;

      Operation() {}
      virtual ~Operation() {}
      virtual Tret operator()() { return Tret(0); }
      virtual Tret operator()(Targ) { return Tret(1000); }
      virtual Tret operator()(Targ,Targ) { return Tret(2000); }
      virtual void Print() const{ std::cout<<" *** Operation ***"<<std::endl; }
};

/**
 @class generator
 algebraic generating operation
*/
template <typename T>
class generator : public Operation<T,T>
{
   public:
      void Print() const
      {
         std::cout<<" *** generating Operation ***"<<std::endl;
      }
};


/**
 @class generator
 algebraic unary operation
*/
template <typename Targ,typename Tret>
class unary_operation : public Operation<Targ,Tret>
{
   public:
      void Print() const
      {
         std::cout<<" *** unary Operation ***"<<std::endl;
      }
};


/**
 @class generator
 algebraic binary operation
*/
template <typename Targ,typename Tret>
class binary_operation : public Operation<Targ,Tret>
{
   public:
      void Print() const
      {
         std::cout<<" *** binary Operation ***"<<std::endl;
      }
};


/**
 @class plus
 algebraic addition operation
*/
template <typename Targ,typename Tret>
class plus : public binary_operation<Targ,Tret>
{
   public:
      Tret operator()(Targ x,Targ y){ return x+y; }
};

/**
 @class minus
 algebraic substraction operation
*/
template <typename Targ,typename Tret>
class minus : public binary_operation<Targ,Tret>
{
   public:
      Tret operator()(Targ x,Targ y){ return x-y; }
};

/**
 @class multiplies
 algebraic multiplying operation
*/
template <typename Targ,typename Tret>
class multiplies : public binary_operation<Targ,Tret>
{
   public:
      Tret operator()(Targ x,Targ y){ return x*y; }
};

/**
 @class divides
 algebraic dividing operation
*/
template <typename Targ,typename Tret>
class divides : public binary_operation<Targ,Tret>
{
   public:
    Tret operator()(Targ x,Targ y) noexcept(false)
      {
//         if (y==Targ(0)) throw divide_by_zero_error();
         if (y==Targ(0)) return INFINITY;
         return x/y;
      }
};

/**
 @class negate
 algebraic inverting operation
*/
template <typename Targ,typename Tret>
class negate : public unary_operation<Targ,Tret>
{
   public:
      Tret operator()(Targ x){ return -x; }
};

/**
 @class unity
 algebraic unity generating operation
*/
template <typename T>
class unity : public generator<T>
{
   public:
      T operator()(){ return T(1); }
};

template <typename Targ,typename Tret>
Tret neutral(Targ x) { return x; }



// forward decl of class Function
template <class Targ, class Tret>
class Function;

// operator template forward declarations
// needed to declare these operators as friend inside Function class
template <class Targ, class Tret>
Function<Targ,Tret> operator+ (const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2);
template <class Targ, class Tret>
Function<Targ,Tret> operator- (const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2);
template <class Targ, class Tret>
Function<Targ,Tret> operator* (const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2);
template <class Targ, class Tret>
Function<Targ,Tret> operator/ (const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2);
template <class Targ, class Tret>
Function<Targ,Tret> operator+ (Targ x, const Function<Targ,Tret>& f2);
template <class Targ, class Tret>
Function<Targ,Tret> operator- (Targ x, const Function<Targ,Tret>& f2);
template <class Targ, class Tret>
Function<Targ,Tret> operator* (Targ x, const Function<Targ,Tret>& f2);
template <class Targ, class Tret>
Function<Targ,Tret> operator/ (Targ x, const Function<Targ,Tret>& f2);
template <class Targ, class Tret>
Function<Targ,Tret> operator+ (const Function<Targ,Tret>& f1, Targ x);
template <class Targ, class Tret>
Function<Targ,Tret> operator- (const Function<Targ,Tret>& f1, Targ x);
template <class Targ, class Tret>
Function<Targ,Tret> operator* (const Function<Targ,Tret>& f1, Targ x);
template <class Targ, class Tret>
Function<Targ,Tret> operator/ (const Function<Targ,Tret>& f1, Targ x);
template <class Targ, class Tret>
Function<Targ,Tret> operator+ (const Function<Targ,Tret>& f1);
template <class Targ, class Tret>
Function<Targ,Tret> operator- (const Function<Targ,Tret>& f1);


/**
 @class domain
 mathematical domain helper class
*/
template <class T>
class domain
{
   private:
      T value;
      bool _fixed;
   public:
      domain()
      {
         _fixed=false;
         value=T(0);
      }
      domain(T a_val)
      {
         _fixed=true;
         value=a_val;
      }

      operator Function<T,T>()
      {
         Function<T,T> f;
         f=*this;
         return f;
      }
};



/**
 @class Function
 @brief class for operating with analytical functions

 @author HG Zaunick
 @date 30.11.2008
*/
template <class Targ, class Tret>
class Function
{
   public:
      typedef Tret (*func1_type)(Targ);
      typedef Tret (*func1p_type)(Targ,const std::vector<Targ>&);
      typedef Targ arg_type;
      typedef Tret ret_type;

   private:
      domain<Targ> _domain;
      func1_type _funcptr;
      func1p_type _funcptr_p;
      int NrFunc;
      Function<Targ,Tret>* _extfunc1;
      Function<Targ,Tret>* _extfunc2;
      Operation<Targ,Tret>* op;
      Tret _const_val;
      std::vector<Targ> _param;

   public:
      Function();
      Function(const Function& f);
      /**
      @brief create function from function reference

      this constructor instantiates Function from a C++ function reference with the signature:
      @code Tret (*)(Targ) @endcode
      @par example
      @code
         double linear(double x)
         {
            return 2.0*x;
         }

         // ... //

         // create linear function
         Function<double,double> f(linear);
      @endcode
      */
      Function(const func1_type& f);
      /**
      @brief create function from function reference with parameters

      this constructor instantiates Function from a C++ function reference with the signature:
      @code Tret (*)(Targ,const std::vector<double>&) @endcode
      @par example
      @code
         double gauss(double x, const std::vector<double>& params)
         {
            // params[0] = mean
            // params[1] = sigma
            // ... //
         }

         // ... //

         // create Gauss function with mean 0.5 and sigma 2
         Function<double,double> f(gauss);
         f.Params.push_back(0.5);
         f.Params.push_back(2);
      @endcode
      */
      Function(const func1p_type& f);
      /**
      @brief create function from function reference with parameters

      this constructor instantiates Function from a C++ function reference with the signature:
      @code Tret (*)(Targ,const std::vector<double>&) @endcode
      and the associated vector of parameters
      @par example
      @code
         double gauss(double x, const std::vector<double>& params)
         {
            // params[0] = mean
            // params[1] = sigma
            // ... //
         }

         // ... //

         // create Gauss function with mean 0.5 and sigma 2
         std::vector<double> par(2);
         par[0] = 0.5;
         par[1] = 2;
         Function<double,double> f(gauss, par);
      @endcode
      */
      Function(const func1p_type& f, const std::vector<Targ>& params);
      /**
      @brief create function from constant

      this constructor instantiates a constant Function
      @par example
      @code
         // create constant function with value pi
         Function<double,double> f(pi);
      @endcode
      */
      Function(Tret x);
      ~Function();

      /**
      @brief assignment from object of type Function
      @par example
      @code f = g; @endcode
      */
      Function& operator= (const Function& f);
      /**
      @brief assignment from function reference
      @par example
      @code f = sin; @endcode
      */
      Function& operator= (const func1_type& f);

      /**
      @brief assignment from function reference with parameters
      @par example
      @code f = gauss(double x, std::vector<double> params); @endcode
      */
      Function& operator= (const func1p_type& f);

      /**
      @brief assignment from domain
      @par example
      @code f = x @endcode
      */
      Function& operator= (const domain<Targ>& x);

      /**
      @brief get domain of function object
      @return domain in which the function is defined
      */
      inline domain<Targ>& Domain() { return _domain; }

      /**
      @brief get domain of function object
      @return reference to function in domain \e a_domain
      @par example
      @code f(x) = ... , ... = f(x) @endcode
      */
      Function& operator()(const domain<Targ>& a_domain);

      /**
      @brief evaluate function
      @return function value at \e x
      @par example
      @code
         Function<double,double> f = cos;
         double y = f(pi);
         // y == 1
      @endcode
      */
      Tret operator()(Targ x);


      /**
      @brief get parameters of function
      @return vector of function parameters
      */
      inline std::vector<Targ>& Params() { return _param; }


      friend Function<Targ,Tret> operator+ <>(const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2);
      friend Function<Targ,Tret> operator- <>(const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2);
      friend Function<Targ,Tret> operator* <>(const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2);
      friend Function<Targ,Tret> operator/ <>(const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2);
      friend Function<Targ,Tret> operator+ <>(Targ x, const Function<Targ,Tret>& f2);
      friend Function<Targ,Tret> operator- <>(Targ x, const Function<Targ,Tret>& f2);
      friend Function<Targ,Tret> operator* <>(Targ x, const Function<Targ,Tret>& f2);
      friend Function<Targ,Tret> operator/ <>(Targ x, const Function<Targ,Tret>& f2);
      friend Function<Targ,Tret> operator+ <>(const Function<Targ,Tret>& f1, Targ x);
      friend Function<Targ,Tret> operator- <>(const Function<Targ,Tret>& f1, Targ x);
      friend Function<Targ,Tret> operator* <>(const Function<Targ,Tret>& f1, Targ x);
      friend Function<Targ,Tret> operator/ <>(const Function<Targ,Tret>& f1, Targ x);
      friend Function<Targ,Tret> operator+ <>(const Function<Targ,Tret>& f1);
      friend Function<Targ,Tret> operator- <>(const Function<Targ,Tret>& f1);

      void Print() const;

      static Function<double,double> Gauss()
      {
         Function<double,double> f((func1_type)hgz::Gauss);
//         Function<double,double> f;
         return f;
      }

      static Function<double,double> Gauss(double mean, double sigma)
      {
         std::vector<double> params(2);
         params[0] = mean;
         params[1] = sigma;
         Function<double,double> f((func1p_type)hgz::Gauss,params);
         std::cout<<" *** static Function::Gauss(double, double) called ***"<<std::endl;
//         f.Params().push_back(mean);
//         f.Params().push_back(sigma);
         std::cout<<"     p[0]="<<f._param[0]<<" , p[1]="<<f._param[1]<<std::endl;
         return f;
      }
};






/*-----------------------------------------------------------------------*/
/* Implementation */
/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>::Function()
{
   _funcptr=NULL;
   _funcptr_p=NULL;
//   _domain=NULL;
   NrFunc=0;
   _extfunc1=NULL;
   _extfunc2=NULL;
   op=NULL;
   _const_val=Tret(0);
}

/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>::Function(const Function& f)
{
   NrFunc=0;
//   _domain=NULL;
   _funcptr=NULL;
   _funcptr_p=NULL;
   _extfunc1=NULL;
   _extfunc2=NULL;
   op=NULL;
   _param.clear();
   _const_val=Tret(0);
//   std::cout<<"+++ Function(const Function&) +++"<<std::endl;
//   std::cout<<"     f.size="<<f.NrFunc<<std::endl;

   _const_val=f._const_val;
   NrFunc=f.NrFunc;
   if (NrFunc==1)
   {
      _extfunc1=new Function(*(f._extfunc1));
//            if (f.op) op=new unary_operation<double>
//                        (dynamic_cast<unary_operation<double>& >
//                        (*(f.op)));
   }
   if (NrFunc==2)
   {
      _extfunc1=new Function(*(f._extfunc1));
      _extfunc2=new Function(*(f._extfunc2));
//            if (f.op) op=new binary_operation<double>
//                        (dynamic_cast<binary_operation<double>& >
//                        (*(f.op)));
   }

   if (f.op) op=f.op;

   if (f.NrFunc==0 && f._funcptr)
   {
//       _funcptr=(const_cast<Function&>(f)._funcptr);
      _funcptr=f._funcptr;
//      std::cout<<"     fptr :"<<(func1_type*)_funcptr<<std::endl;
   }
   if (f.NrFunc==0 && f._funcptr_p)
   {
//       _funcptr=(const_cast<Function&>(f)._funcptr);
      _funcptr_p=f._funcptr_p;
/*
      for (std::vector<arg_type>::iterator it=f._param.begin();
           it!=f._param.end();
           it++)
*/
      for (unsigned int i=0; i<f._param.size(); i++)
         _param.push_back(f._param[i]);

//      std::cout<<"     fptr_p :"<<(func1p_type*)_funcptr_p<<std::endl;
   }

//         if (_funcptr) std::cout<<"     fptr :"<<&_funcptr<<std::endl;
   if (op)
   {
//            std::cout<<"     op   :"<<op<<std::endl;
//            op->Print();
   }
//   std::cout<<"--- Function(const Function&) ---"<<std::endl;
}


/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>::Function(const func1_type& f)
{
//   _domain=NULL;
   _funcptr=f;
   _funcptr_p=NULL;
   _param.clear();
   NrFunc=0;
   _extfunc1=NULL;
   _extfunc2=NULL;
}


/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>::Function(const func1p_type& f)
{
//   _domain=NULL;
   _funcptr_p=f;
   _funcptr=NULL;
   _param.clear();
   NrFunc=0;
   _extfunc1=NULL;
   _extfunc2=NULL;
}

/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>::Function(const func1p_type& f, const std::vector<Targ>& params)
{
//   _domain=NULL;
   _funcptr_p=f;
   _funcptr=NULL;
   _param.clear();
   _param = params;
   NrFunc=0;
   _extfunc1=NULL;
   _extfunc2=NULL;
}

/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>::Function(Tret x)
{
//         std::cout<<"+++ Function(T x) +++"<<std::endl;
   _funcptr=NULL;
   _funcptr_p=NULL;
   _param.clear();
   NrFunc=0;
   _const_val=x;
//   _domain=NULL;
   _extfunc1=NULL;
   _extfunc2=NULL;
   op=NULL;
//         std::cout<<"--- Function(T x) ---"<<std::endl;
}

/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>::~Function()
{
//   std::cout<<"+++ dtor Function +++"<<std::endl;
   if (_extfunc2) { delete _extfunc2; _extfunc2=NULL;}
   if (_extfunc1) { delete _extfunc1; _extfunc1=NULL;}
   _param.clear();
//   if (_domain) { delete _domain; _domain=NULL;}

//   if (op) { delete op; op=NULL;}

//   std::cout<<"--- dtor Function ---"<<std::endl;
}


/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>& Function<Targ,Tret>::operator()(const domain<Targ>& a_domain)
{
//   if (!_domain) _domain = new domain<T>();
   _domain=a_domain;
   return *this;
}

/*-----------------------------------------------------------------------*/


template <class Targ, class Tret>
Function<Targ,Tret>& Function<Targ,Tret>::operator= (const Function& f)
{
   if (this==&f) return *this;
//   std::cout<<"+++ operator=(const Function&) +++"<<std::endl;
   if (_extfunc1) delete _extfunc1;
   if (_extfunc2) delete _extfunc2;
//   if (op) delete op;

   NrFunc=0;
//   _domain=NULL;
   _funcptr=NULL;
   _funcptr_p=NULL;
   _extfunc1=NULL;
   _extfunc2=NULL;
   op=NULL;
   _param.clear();

   _const_val=Tret(0);

   _const_val=f._const_val;

   NrFunc=f.NrFunc;
   _domain=f._domain;

   if (NrFunc==1)
   {
      _extfunc1=new Function(*(f._extfunc1));
   }
   if (NrFunc==2)
   {
      _extfunc1=new Function(*(f._extfunc1));
      _extfunc2=new Function(*(f._extfunc2));
   }

   if (f.op) op=f.op;

   if (f.NrFunc==0 && f._funcptr)
   {
//      _funcptr=(const_cast<Function&>(f)._funcptr);
      _funcptr=f._funcptr;
//       if (_funcptr) std::cout<<"     fptr :"<<(func1_type*)&_funcptr<<std::endl;
   }
   if (f.NrFunc==0 && f._funcptr_p)
   {
//       _funcptr=(const_cast<Function&>(f)._funcptr);
      _funcptr_p=f._funcptr_p;
      for (unsigned int i=0; i<f._param.size(); i++)
         _param.push_back(f._param[i]);
//      std::cout<<"     fptr_p :"<<(func1p_type*)_funcptr_p<<std::endl;
   }

//   std::cout<<"--- operator=(const Function&) ---"<<std::endl;
   return *this;
}


/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>& Function<Targ,Tret>::operator= (const func1_type& f)
{
   if (_extfunc1) delete _extfunc1;
   if (_extfunc2) delete _extfunc2;
//   if (op) delete op;

   _funcptr=f;
   _funcptr_p=NULL;
   NrFunc=0;
   _param.clear();
   return *this;
}

/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>& Function<Targ,Tret>::operator= (const func1p_type& f)
{
   if (_extfunc1) delete _extfunc1;
   if (_extfunc2) delete _extfunc2;
   _extfunc1=NULL;
   _extfunc2=NULL;
//   if (op) delete op;

//   _domain=NULL;
   _funcptr_p=f;
   _funcptr=NULL;
   _param.clear();
   NrFunc=0;
   return *this;
}

/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Function<Targ,Tret>& Function<Targ,Tret>::operator= (const domain<Targ>& x)
{
   if (_extfunc1) delete _extfunc1;
   if (_extfunc2) delete _extfunc2;
//   if (op) delete op;

   _funcptr=neutral<Targ>;
   _funcptr_p=NULL;
   _param.clear();
   NrFunc=0;
//   _domain=new domain<T>(x);

   return *this;
}

/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
Tret Function<Targ,Tret>::operator()(Targ x)
{
   Tret y;
   if (_funcptr && !NrFunc)
   {
//      std::cout<<"  ret _f(x) = "<<y<<std::endl;
      y=_funcptr(x);
   }
   else if (_funcptr_p && !NrFunc)
   {
      assert (_param.size());
      y=_funcptr_p(x,_param);
/*      std::cout<<"  ret _f(x,param) = "<<y<<std::endl;
*/
   }
   else if (NrFunc==1)
   {
//      std::cout<<"  ret op1(f(x))"<<std::endl;
      y=op->operator()((*_extfunc1)(x));
//      std::cout<<" y1="<<y<<std::endl;
   }
   else if (NrFunc==2)
   {
//      std::cout<<"  ret op2(f1(x),f2(x))"<<std::endl;
      y=op->operator()((*_extfunc1)(x),(*_extfunc2)(x));
//            y=(*op)(1.,1.);

//      std::cout<<" y2="<<y<<std::endl;
   }
   else
   {
      y=_const_val;
//      std::cout<<" const function = "<<y<<std::endl;
   }
   return y;
}


/*-----------------------------------------------------------------------*/

template <class Targ, class Tret>
void Function<Targ,Tret>::Print() const
{
   std::cout<<"*** Function ("<<this<<") ***"<<std::endl;
   if (_funcptr)
      std::cout<<"     int. function : "<<&_funcptr<<std::endl;
   else if (_funcptr_p)
   {
      std::cout<<"     int. function (w. parameters): "<<&_funcptr_p<<std::endl;
      std::cout<<"      parameters: "<<std::endl;
      for (int i=0; i<_param.size(); ++i)
         std::cout<<"       p["<<i<<"]="<<_param[i]<<std::endl;
   }
   else if (NrFunc==1)
   {
      std::cout<<"     ext. function   : "<<_extfunc1<<std::endl;
      std::cout<<"     unary operation : "<<op<<std::endl;

   }
   else if (NrFunc==2)
   {
      std::cout<<"     ext. function1   : "<<_extfunc1<<std::endl;
      std::cout<<"     ext. function2   : "<<_extfunc2<<std::endl;
      std::cout<<"     binary operation : "<<op<<std::endl;
   }
   else std::cout<<"     empty"<<std::endl;
}

/*-----------------------------------------------------------------------*/

// friends

template <class Targ, class Tret>
Function<Targ,Tret> operator+ (const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2)
{
//         std::cout<<"+++ operator+(const Function&, const Function&) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr_p=NULL;
   f._funcptr=NULL;
   f._domain=f1._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(f1);
/*
                    <f1.arg_type,
                     f1.ret_type,
                     f1.param_type>(f1);
*/
   f._extfunc2=new Function<Targ,Tret>(f2);
   f.op=new plus<Targ,Tret>();
//         std::cout<<"     size="<<f.NrFunc<<std::endl;
//         std::cout<<"--- operator+(const Function&, const Function&) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator- (const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2)
{
//         std::cout<<"+++ operator-(const Function&, const Function&) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr_p=NULL;
   f._funcptr=NULL;
   f._domain=f1._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(f1);
   f._extfunc2=new Function<Targ,Tret>(f2);
   f.op=new minus<Targ,Tret>();
//         std::cout<<"     size="<<f.NrFunc<<std::endl;
//         std::cout<<"--- operator-(const Function&, const Function&) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator* (const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2)
{
//         std::cout<<"+++ operator*(const Function&, const Function&) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr_p=NULL;
   f._funcptr=NULL;
   f._domain=f1._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(f1);
   f._extfunc2=new Function<Targ,Tret>(f2);
   f.op=new multiplies<Targ,Tret>();
//         std::cout<<"     size="<<f.NrFunc<<std::endl;
//         std::cout<<"--- operator*(const Function&, const Function&) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator/ (const Function<Targ,Tret>& f1, const Function<Targ,Tret>& f2)
{
//         std::cout<<"+++ operator/(const Function&, const Function&) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr_p=NULL;
   f._funcptr=NULL;
   f._domain=f1._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(f1);
   f._extfunc2=new Function<Targ,Tret>(f2);
   f.op=new divides<Targ,Tret>();
//         std::cout<<"     size="<<f.NrFunc<<std::endl;
//         std::cout<<"--- operator/(const Function&, const Function&) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator+ (Targ x, const Function<Targ,Tret>& f2)
{
//         std::cout<<"+++ operator+(T x, const Function&) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr_p=NULL;
   f._funcptr=NULL;
   f._domain=f2._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(x);
   f._extfunc2=new Function<Targ,Tret>(f2);
   f.op=new plus<Targ,Tret>();
//         std::cout<<"--- operator+(T x, const Function&) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator- (Targ x, const Function<Targ,Tret>& f2)
{
//         std::cout<<"+++ operator-(T x, const Function&) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr=NULL;
   f._funcptr_p=NULL;
   f._domain=f2._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(x);
   f._extfunc2=new Function<Targ,Tret>(f2);
   f.op=new minus<Targ,Tret>();
//         std::cout<<"--- operator-(T x, const Function&) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator* (Targ x, const Function<Targ,Tret>& f2)
{
//         std::cout<<"+++ operator*(T x, const Function&) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr=NULL;
   f._funcptr_p=NULL;
   f._domain=f2._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(x);
   f._extfunc2=new Function<Targ,Tret>(f2);
   f.op=new multiplies<Targ,Tret>();
//         std::cout<<"--- operator*(T x, const Function&) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator/ (Targ x, const Function<Targ,Tret>& f2)
{
//         std::cout<<"+++ operator/(T x, const Function&) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr=NULL;
   f._funcptr_p=NULL;
   f._domain=f2._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(x);
   f._extfunc2=new Function<Targ,Tret>(f2);
   f.op=new divides<Targ,Tret>();
//         std::cout<<"--- operator/(T x, const Function&) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator+ (const Function<Targ,Tret>& f1, Targ x)
{
//         std::cout<<"+++ operator+(const Function&, T x) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr=NULL;
   f._funcptr_p=NULL;
   f._domain=f1._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(f1);
   f._extfunc2=new Function<Targ,Tret>(x);
   f.op=new plus<Targ,Tret>();
//         std::cout<<"--- operator+(const Function&,T x) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator- (const Function<Targ,Tret>& f1, Targ x)
{
//         std::cout<<"+++ operator+(const Function&, T x) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr=NULL;
   f._funcptr_p=NULL;
   f._domain=f1._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(f1);
   f._extfunc2=new Function<Targ,Tret>(x);
   f.op=new minus<Targ,Tret>();
//         std::cout<<"--- operator+(const Function&,T x) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator* (const Function<Targ,Tret>& f1, Targ x)
{
//         std::cout<<"+++ operator+(const Function&, T x) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr=NULL;
   f._funcptr_p=NULL;
   f._domain=f1._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(f1);
   f._extfunc2=new Function<Targ,Tret>(x);
   f.op=new multiplies<Targ,Tret>();
//         std::cout<<"--- operator+(const Function&,T x) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator/ (const Function<Targ,Tret>& f1, Targ x)
{
//         std::cout<<"+++ operator+(const Function&, T x) +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr=NULL;
   f._funcptr_p=NULL;
   f._domain=f1._domain;
   f._const_val=Tret(0);
   f.NrFunc=2;
   f._extfunc1=new Function<Targ,Tret>(f1);
   f._extfunc2=new Function<Targ,Tret>(x);
   f.op=new divides<Targ,Tret>();
//         std::cout<<"--- operator+(const Function&,T x) ---"<<std::endl;
   return f;
}

template <class Targ, class Tret>
Function<Targ,Tret> operator+ (const Function<Targ,Tret>& f1)
{
//         std::cout<<"+++ operator+() +++"<<std::endl;
//         std::cout<<"--- operator-() ---"<<std::endl;
   return Function<Targ,Tret>(f1);
}

template <class Targ, class Tret>
Function<Targ,Tret> operator- (const Function<Targ,Tret>& f1)
{
//         std::cout<<"+++ operator-() +++"<<std::endl;
   Function<Targ,Tret> f;
   f._funcptr=NULL;
   f._domain=f1._domain;
   f._const_val=Tret(0);
   f.NrFunc=1;
   f._extfunc1=new Function<Targ,Tret>(f1);
   f.op=new negate<Targ,Tret>();
//         std::cout<<"--- operator-() ---"<<std::endl;
   return f;
}



/*-----------------------------------------------------------------------*/


} // namespace hgz

#endif // # #ifndef _HGZFUNCTION_H
