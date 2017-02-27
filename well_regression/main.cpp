/* main.cpp */
static const char rcsid[] = "$Id$";

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "well_storage.hpp"
#include "well_math.hpp"

// just for storage test
void test_storage(const WellStorage &ws, const std::string &out_fname)
{
  std::ofstream fs(out_fname.c_str(), std::ifstream::out);
  for (size_t i=0; i<ws.wellNum(); ++i)
    for (size_t j=0; j<ws.dateNum(); ++j)
      {
	const WellStorage::Elem &elem = ws(i, j);
	if (elem.isActual() && (i == 20))
	  {
	    fs << elem.toString(); 
	    fs << "\t" << elem.q_oil / elem.dt; 
	    fs << "\n";
	  }
      }
}

/** Application parameter. */
struct AppParameters
{
  AppParameters(): pt(PT_OIL), func_type(1) 
  {/* nothing to do. */}
  
  /** Initialize parameters from cmd line arguments provided. 
      @return true on success, false on incorrect initialization. */
  bool init(int argc, char **argv)
  {
    if (argc < 5)
      {
	show_usage();
	return false;
      }
    
    in_fname_str = argv[1];
    if (strcmp(argv[2], "o") == 0)
      pt = PT_OIL;
    else
      pt = PT_WATER;
    func_type = atoi(argv[3]);
    out_fname_str = argv[4];
        
    return true;
  }
  
  void show_usage()
  {
    std::cout << "usage: well_regression IFILE product_type func_type OFILE\n";
    std::cout << "IFILE - input file with wells data;\n";
    std::cout << "product_type - type of the product (o - for oil, w - for water);\n";
    std::cout << "func_type - type of model time function (1 or 2);\n";
    std::cout << "OFILE - output file with results.\n";
  }
  
  std::string in_fname_str;
  std::string out_fname_str;
  ProductType pt;
  int func_type;
};

int main(int argc, char **argv)
{
  AppParameters app_params;
  if (!app_params.init(argc, argv))
    return 0;
  
  WellStorage ws;
  std::cout << "Initialization from \'" << argv[1] << "\'...\n";
  if (0 != ws.init(app_params.in_fname_str))
    return -1;
  std::cout << "Initialized, " << ws.wellNum() << " wells, ";
  std::cout << ws.dateNum() << " dates, product type is " << app_params.pt;
  std::cout << "\n";
  
  //test_storage(ws, "test.txt");
  
  double D = 0;
  WellResultsMap well_res_map;
  if (app_params.func_type == 1)
    {
      regression(ws, app_params.pt, g1, D, well_res_map);
      analyze(ws, app_params.pt, D, well_res_map, f1, app_params.out_fname_str);
    }
  else
    {
      regression(ws, app_params.pt, g2, D, well_res_map);
      analyze(ws, app_params.pt, D, well_res_map, f2, app_params.out_fname_str);
    }
  
  return 0;
}
