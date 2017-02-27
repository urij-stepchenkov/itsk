/* well_math.hpp */
/* $Id$ */
#ifndef __well_math_hpp
#define __well_math_hpp

#include <utility>
#include <map>

class WellStorage;

/** Type of well production fluid. */
enum ProductType
  {
    PT_OIL = 0, 
    PT_WATER
  };
std::string toString(ProductType pt);

/** Well time function ptr type. */
typedef double (*f_ptr)(const double &t, const double &p);

/** f(t) = exp(-pt) */
double f1(const double &t, const double &p);

/** f(t) = (1+t)^(-p) */
double f2(const double &t, const double &p);


/** Regression time function ptr type. */
typedef double (*g_ptr)(const double &t);

/** g(t) = t */
double g1(const double &t);

/** g(t) = ln(1+t) */
double g2(const double &t);


/** Regression result data. < well_id, <date0, q0> >, 
    date0 - well start month, q0 - initial debit. */
typedef std::map<size_t, std::pair<double, double> > WellResultsMap;

/** Regression procedure for f(t) = exp(-Dt) 
    @param ws  Well data storage.
    @param pt  Type of product.
    @param g   Ptr to regression function.
    @param D   Resulted regression coefficient
    @param well_res_map  Regression results.
    @return 0 on success, error code otherwise.
*/
int regression(const WellStorage &ws, 
	       const ProductType pt,
	       g_ptr g,
	       double &D, 
	       WellResultsMap &well_res_map);


/** Analyze results of regression. 
    @param ws  Well data storage.
    @param pt  Type of product.
    @param D   Regression coefficient
    @param well_res_map  Regression results.
    @param f Well time function.  
    @param out_fname  Output file name for results.
    @return 0 on success, error code otherwise. */
int analyze(const WellStorage &ws, 
	    const ProductType pt, 
	    const double D, 
	    const WellResultsMap &well_res_map,
	    f_ptr f,
	    const std::string &out_fname);

#endif /* well_math.hpp */
