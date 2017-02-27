/* well_math.cpp */
static const char rcsid[] = "$Id$";

#include <iostream>
#include <fstream>
#include <math.h>
#include <limits>
#include "well_storage.hpp"
#include "well_math.hpp"

#define Fortran(x) x##_

// lapack dgesv prototype 
extern "C" void Fortran(dgesv)(int *n, int *nrhs, double *a, int *lda, int *ipiv,
			       double *b, int *ldb, int *info);

// blas dgemm prototype
extern "C" void Fortran(dgemm)(char* TRANSA, char* TRANSB, const int* M,
			       const int* N, const int* K, double* alpha, double* A,
			       const int* LDA, double* B, const int* LDB, double* beta,
			       double* C, const int* LDC);

// minimal positive value (precison) for q
static const double s_q_eps = std::numeric_limits<double>::min();

double f1(const double &t, const double &p)
{
  return exp(-p*t);
}

double f2(const double &t, const double &p)
{
  return pow(1 + t, -p);
}

double g1(const double &t)
{
  return t;
}

double g2(const double &t)
{
  return log(1+t);
}

int regression(const WellStorage &ws, 
	       const ProductType pt,
	       g_ptr g,
	       double &D, 
	       WellResultsMap &well_res_map)
{
  well_res_map.clear();
  
  double A = 0;
  double B = 0;
  double T = 0;
  // for all wells 
  for (size_t well_id = 0; well_id<ws.wellNum(); ++well_id)
    {
      // for all months  
      double q0 = 0;
      double date0 = 0;
      bool init_debit_flag = false;
      for (size_t date_id = 0; date_id<ws.dateNum(); ++date_id)
	{
	  const WellStorage::Elem &elem = ws(well_id, date_id);
	  if (!elem.isActual())
	    continue;
	  
	  // current debit, skip zero debits
	  double q = (pt == PT_OIL)?(elem.q_oil):(elem.q_water);
	  if (q <= s_q_eps)
	    continue;
	  q /= elem.dt;
	  
	  // initial debit is the first data for current well
	  if (!init_debit_flag)
	    {
	      q0 = q;
	      date0 = elem.date;
	      init_debit_flag = true;
	      
	      std::cout << "well=\'" << elem.name_str;
	      std::cout << "\'\n"; 
	    }
	  // update regression parameters
	  else
	    {
	      A += log(q)*g(elem.date);
	      B += log(q0)*g(elem.date);
	      T += g(elem.date)*g(elem.date);
	    }
	  
	  //std::cout << "date=" << elem.date; 
	  //std::cout << ", dt=" << elem.dt;
	  //std::cout << ", q=" << q;
	  //std::cout << ", A=" << A; 
	  //std::cout << ", B=" << B; 
	  //std::cout << "\n";
	}
      
      well_res_map[well_id] = std::make_pair(date0, q0);
    }
    
  D = (B - A)/T;
  
  //std::cout << "D=" << D; 
  //std::cout << "\n";
  
  return 0;
 }
      
int analyze(const WellStorage &ws, 
	    const ProductType pt, 
	    const double D, 
	    const WellResultsMap &well_res_map,
	    f_ptr f,
	    const std::string &out_fname)
{
  std::ofstream fs(out_fname.c_str(), std::ifstream::out);
  
  WellResultsMap::const_iterator well_it = well_res_map.begin();
     
  fs << "# Well regression results\n";
  fs << "# product: " << toString(pt) << "\n"; 
  fs << "# D=" << D << "\n"; 
  fs << std::endl;
  
  std::vector< std::pair<double, double> > q_vec;
  
  for (WellResultsMap::const_iterator well_it = well_res_map.begin();
       well_it != well_res_map.end(); ++well_it)
    {
      size_t well_id = well_it->first;
      double date0 = well_it->second.first;
      double q0 = well_it->second.second;
           
      fs << "# well: #" << well_it->first;
      fs << ", date0=" << date0;
      fs << ", q0=" << q0 << "\n";
      fs << "# well_name, time_stamp, q_real, q_model, eps^2\n";
      
      for (size_t date_id=0; date_id<ws.dateNum(); ++date_id)
	{
	  const WellStorage::Elem &elem = ws(well_id, date_id);
	  if (!elem.isActual())
	    continue;
	  	  
	  // current debit
	  double q = (pt == PT_OIL)?(elem.q_oil):(elem.q_water);
	  if (q <= s_q_eps)
	    continue;
	  q /= elem.dt;
	  	  
	  double q_model = q0*f(elem.date - date0, D);
	  double eps2 = (q - q_model)*(q - q_model);
	  q_vec.push_back(std::make_pair(q, q_model));

	  fs << elem.name_str << "\t";
	  fs << elem.date << "\t"; 
	  fs << q << "\t";
	  fs << q_model << "\t";
	  fs << eps2 << "\n";
	}
      fs << "\n";
    }
  
  // coefficient of determination
  double D1 = 0;
  double D2 = 0;
  double q_avg = 0;
  size_t n = q_vec.size();
  for (size_t i=0; i<q_vec.size(); ++i)
    {
      q_avg += q_vec[i].first/n;
      D1 += (q_vec[i].first - q_vec[i].second)*(q_vec[i].first - q_vec[i].second)/n;
    }
  for (size_t i=0; i<q_vec.size(); ++i)
    D2 += (q_vec[i].first - q_avg)*(q_vec[i].first - q_avg)/n;
  double R = fabs(1 - D1/D2);
  
  fs << "# D1=" << D1 << "\n";
  fs << "# D2=" << D2 << "\n";
  fs << "# R=" << R << "\n";
  
  return 0;
}

std::string toString(ProductType pt)
{
  return (pt == PT_OIL)?("oil"):("water");
}
