/* well_storage.hpp */
/* $Id$ */
#ifndef __well_storage_hpp
#define __well_storage_hpp

#include <map>
#include <vector>
#include <string>

/** Well data storage. Data sorted by wells and debit dates ascending. */
class WellStorage
{
public:
  
  /** Well debit element of the storage. */
  struct Elem
  {
    Elem():
      dt(0), q_oil(0), q_water(0)
    {/* nothing to do */}
    
    std::string name_str; /**< Well name. */
    double date;          /**< Debit date. */ 
    double dt;            /**< Production time, hours per month. */
    double q_oil;         /**< Oil debit. */
    double q_water;       /**< Water debit. */
    
    /** @return true if element is actual, false otherwise. */
    bool isActual() const {return (name_str.size() > 0);}
    
    /** @return element text representation. */
    std::string toString() const;
  };
  
  /** Default constructor for empty storage. */
  WellStorage();
  
  /** Destroy storage. */
  ~WellStorage();
  
  /** Initialize storage form file with povided name. */
  int init(const std::string &fname);
  
  /** @return current number of wells in the storage. */
  size_t wellNum() const {return m_well_num;}
  
  /** @return current number if production dates. */
  size_t dateNum() const {return m_date_num;}
  
  /** Access to well debit element.
      @param well_id  Id of the well required.
      @param date_id  Id of production date required. 
      @return well debit element. */
  const Elem& operator()(const size_t well_id, const size_t date_id) const;
  
private:
  
  /** Storage map key. */
  struct Key
  {
    Key(): name_id(0), date_id(0)
    {/* nothing to do */}
    
    Key(size_t i_name_id, size_t i_date_id):
      name_id(i_name_id), date_id(i_date_id)
    {/* nothing to do */}
    
    bool operator==(const Key &o) const 
    {return name_id == o.name_id && date_id == o.date_id;}

    bool operator<(const Key &o) const 
    {return name_id < o.name_id || (name_id == o.name_id && 
				    date_id < o.date_id);}
    
    size_t name_id; /**< Well id. */
    size_t date_id; /**< Production date id. */
  };
  
  /** Internal storage. */
  typedef std::vector<Elem> Storage;
  
  /** Internal storage keys. */
  typedef std::map<Key, size_t> Ids;
  
  /**< Current storage data */
  Storage m_storage;
  
  /**< Current storage indeces. */
  Ids m_ids;
  
  /**< Current number of wells. */
  size_t m_well_num;
  
  /**< Current number of production dates. */
  size_t m_date_num;
};

#endif /* well_storage.hpp */
