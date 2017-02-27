/* well_storage.cpp */
static const char rcsid[] = "$Id$";

#include <iostream>
#include <fstream>
#include <sstream> 
#include "well_storage.hpp"

WellStorage::WellStorage():
  m_well_num(0), m_date_num(0)
{
  // nothing to do
}
  
WellStorage::~WellStorage()
{
  // nothing to do
}
  
int WellStorage::init(const std::string &fname)
{
  // reset storage
  m_storage.clear();
  m_ids.clear();
  m_well_num = m_date_num = 0;

  std::ifstream fs(fname.c_str(), std::ifstream::in);
  if (fs.fail() || fs.bad())
    {
      std::cout << "WellStorage::init: error, failed to read \'";
      std::cout << fname << "\'\n";
      return -1;
    }
  
  int rc = 0;
  std::string str;
  std::map<std::string, size_t> well_name_map;
  std::map<double, size_t> date_map;
  while (std::getline(fs, str))
    {
      // skip comment
      if (!str.empty())
	if (str[0] == '#')
	  continue;
      
      // parse current line into data storage element
      Elem elem;
      std::stringstream ss(str);
      ss >> elem.date;
      ss >> elem.name_str;
      ss >> elem.dt;
      ss >> elem.q_oil;
      ss >> elem.q_water;
      
      // update well name and date indeces maps
      well_name_map[elem.name_str] = 0;
      date_map[elem.date] = 0;
      
      m_storage.push_back(elem);
      //std::cout << elem.toString() << std::endl;
    }
  
  // setup proper indeces for well name map
  size_t id = 0;
  for (std::map<std::string, size_t>::iterator it = well_name_map.begin();
       it != well_name_map.end(); ++it, ++id)
    {
      it->second = id;
      //std::cout << it->first << ": id=" << it->second << std::endl;
    }
  
  // setup proper indeces for date map
  id = 0;
  for (std::map<double, size_t>::iterator it = date_map.begin();
       it != date_map.end(); ++it, ++id)
    {
      it->second = id;
      //std::cout << it->first << ": id=" << it->second << std::endl;
    }
  
  // store number of wells and number of different dates
  m_well_num = well_name_map.size();
  m_date_num = date_map.size();
  
  // prepare indeces 
  for (id = 0; id<m_storage.size(); ++id)
    {
      const Elem &elem = m_storage[id];
      std::map<std::string, size_t>::const_iterator name_it = 
	well_name_map.find(elem.name_str);
      std::map<double, size_t>::const_iterator date_it = 
	date_map.find(elem.date);
      if (name_it == well_name_map.end() || date_it == date_map.end())
	{
	  std::cout << "WellStorage::init: error, indeces parsing failed.\n";
	  rc = -1;
	  break;
	}
      
      Key key(name_it->second, date_it->second);
      m_ids[key] = id;      
    }
  
  return rc;
}

const WellStorage::Elem& WellStorage::operator()(const size_t name_id, 
						 const size_t date_id) const
{
  if (name_id >= wellNum() || date_id >= dateNum())
    {
      std::cout << "WellStorage::(): error, out of range, ";
      std::cout << name_id << "/" << wellNum() << " wells, ";
      std::cout << date_id << "/" << dateNum() << " dates\n";
      static Elem dummy_elem;
      return dummy_elem;
    }
  
  Ids::const_iterator it = m_ids.find(Key(name_id, date_id));
  if (it == m_ids.end())
    {
      static Elem dummy_elem;
      return dummy_elem;
    }
  
  return m_storage[it->second];
}

//------------------------------------------------------------------------------------------
std::string WellStorage::Elem::toString() const
{
  std::stringstream ss;
  ss << name_str << "\t";
  ss << date << "\t";
  ss << dt << "\t";
  ss << q_oil << "\t";
  ss << q_water;
  return ss.str();
}
