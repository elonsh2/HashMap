#include "iostream"
#include "vector"
#include "string"
#include "set"
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <forward_list>
#include <list>

#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

using std::string;
using std::vector;
using std::cout;

using std::endl;
#define INITIAL_SIZE 16
#define DEFAULT_LOWER_LOAD 0.25
#define DEFAULT_UPPER_LOAD 0.75
#define WRONG_SIZE_VECTORS "Vectors are not in the same size"
#define KEY_NOT_FOUND "Key doesn't exist"
template<typename KeyT, typename ValueT>
class HashMap
{
 public:
  /*  Constructors  */
  HashMap ();  // default ctr
  // vectors ctr
  HashMap (const vector<KeyT> &keys, const vector<ValueT> &values);
  // copy ctr
  HashMap (HashMap &other_map);
  virtual ~HashMap (); // destructor

  /*    Short Methods  */
  /**
   * returns capacity
   * @return int
   */
  int capacity () const
  { return _capacity; }

  /**
   * return number of elements in map
   * @return int
   */
  int size () const
  { return _entries; }

  /**
   * checks if map is empty
   * @return true if map is empty, false otherwise
   */
  bool empty () const
  { return _entries == 0; }

  /**
   * returns load factor of map
   * @return double
   */
  double get_load_factor () const
  { return load_factor; }

  /*     Methods      */
  /**
   * returns number of objects in bucket
   * @param key key to check
   * @return int
   */
  int bucket_size (const KeyT &key) const;

  /**
   * returns index of bucket in array
   * @param key KetT
   * @return index as int
   */
  int bucket_index (const KeyT &key) const;

  /**
   * clears map, deletes all
   */
  void clear ();

  /**
   * returns value of given key
   * @param key KeyT
   * @return ValueT
   */
  const ValueT &at (const KeyT &key) const; //TODO: implement const
  ValueT &at (const KeyT &key) ; //TODO: implement const

  /**
   * insert key and his value to map
   * @param key KetT object
   * @param value ValueT object
   * @return true if inserted successfully, false if key exist
   */
  bool insert (KeyT key, ValueT value);

  /**
   * checks if map contains specific key
   * @param key KeyT
   * @return true if key in map
   */
  bool contains_key (const KeyT &key) const;

   /**
   * deletes specific key and his value
   * @param key KeyT
   * @return true if erased, false if key not in map
   */
  virtual bool erase (const KeyT &key);

  /*    operators      */

  ValueT &operator[] (KeyT key);
  const ValueT &operator[] (KeyT key) const;  //TODO: implement const
  HashMap &operator= (const HashMap &sec_hash_map);
  bool operator== ( const HashMap &other_map) const;
  bool operator!= ( const HashMap &other_map) const
  { return !(*this == other_map); }
  void print_all (); //TODO: delete

  /*      Iterator class & methods      */
  class ConstIterator
  {
   private:
    vector<std::pair<KeyT, ValueT>> *buckets;
    int cur_bucket;
    int cur_index;
    HashMap &table;

   public:
    typedef std::pair<KeyT, ValueT> value_type;
    typedef std::pair<KeyT, ValueT> &reference;
    typedef std::pair<KeyT, ValueT> *pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    ConstIterator (const HashMap &table, int cur_bucket_in, int cur_index_in):
        buckets (table.buckets), cur_bucket (cur_bucket_in),
        cur_index (cur_index_in), table (const_cast<HashMap &>(table))
    {
      // finds first element in map
      while (buckets[cur_bucket].empty ()
             && cur_bucket < (table._capacity - 1))
      {
        cur_bucket++;
      }
    }

    ConstIterator &operator++ ()
    {
      if (cur_bucket == table._capacity-1 && cur_index== buckets[cur_bucket]
                                                             .size()-1)
      {cur_index++;
        return *this;}
      // if there's more elements in bucket, updates index
      if (buckets[cur_bucket].size () > (cur_index+1))
      {
        cur_index++;
        return *this;
      }
      // resets index and searches for first element in next buckets
      if (cur_bucket < table._capacity -1)
      {cur_bucket++;}
      cur_index = 0;
      while (buckets[cur_bucket].empty ()
             && cur_bucket < (table._capacity - 1))
      {
        cur_bucket++;
      }

      return *this;
    }

    ConstIterator operator++ (int)
    {
      // same as pre increment
      if (cur_bucket == table._capacity-1 && cur_index== buckets[cur_bucket]
                                                             .size()-1)
      {cur_index++;}
      ConstIterator it (table, cur_bucket, cur_index);
      if (buckets[cur_bucket].size () > (cur_index + 1))
      {
        cur_index++;
        return it;
      }
      cur_index = 0;
      cur_bucket++;
      while (buckets[cur_bucket].empty ()
             && cur_bucket < (table._capacity - 1))
      {
        cur_bucket++;
      }
      return it;
    }

    bool operator== (const ConstIterator &rhs) const
    {
      return (cur_bucket == rhs.cur_bucket && cur_index == rhs.cur_index);
    }

    bool operator!= (const ConstIterator &rhs) const
    {
      return (cur_bucket != rhs.cur_bucket || cur_index != rhs.cur_index);
    }

    reference operator* () const
    { return buckets[cur_bucket][cur_index]; }

    pointer operator-> () const
    { return &(operator* ()); }
  };

  using const_iterator = ConstIterator;
  const_iterator begin () const
  { return ConstIterator (*this, 0, 0); }
  const_iterator cbegin () const
  { return ConstIterator (*this, 0, 0); }
  // end points to  one past last element in last bucket
  const_iterator end () const
  {
    int last_bucket = _capacity - 1;
    return ConstIterator (*this,last_bucket, buckets[last_bucket].size ());
  }
  const_iterator cend () const
  {
    int last_bucket = _capacity - 1;
    return ConstIterator (*this,last_bucket, buckets[last_bucket].size ());
  }

 private:
  /**
   * updates load factor after each insertion or erase
   */
  void update_load_factor ()
  { load_factor = _entries / double (_capacity); }

  /**
   * resize array if load_factor passes lower or upper load
   * @param size new capacity
   */
  void resize_array (size_t size, bool flag=false);

  /**
   * calculates hash function for key
   * @param key given key
   * @return bucket to place into
   */
  int hash_function (const KeyT &key) const;
  int _capacity = INITIAL_SIZE;
  int _entries = 0;
  double load_factor = _entries / _capacity;
  vector<std::pair<KeyT, ValueT>> *buckets;  // array of vectors
};

/*        Long Methods         */

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap ()
{
  buckets = new vector<std::pair<KeyT, ValueT>>[_capacity];
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap (const vector<KeyT> &keys, const vector<ValueT>
&values)
{
  // checks that vectors are in the same size
  if (keys.size () != values.size ())
  {
    throw std::length_error (WRONG_SIZE_VECTORS);
  }
  buckets = new vector<std::pair<KeyT, ValueT>>[_capacity];
  // updates map with vectors
  for (size_t i = 0; i < keys.size (); i++)
  {
    if (contains_key (keys[i]))
    {
      at(keys[i]) = values[i]; // TODO: check, erase or just change;
    }
    else
    { insert (keys[i], values[i]); }
  }
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap (HashMap<KeyT, ValueT> &other_map)
{
  _capacity = other_map._capacity;
  buckets = new vector<std::pair<KeyT, ValueT>>[_capacity];
  // copies all elements
  for (int bucket = 0; bucket < _capacity; bucket++)
  {
    for (auto &pair: other_map.buckets[bucket])
    {
      insert (pair.first, pair.second);
    }
  }
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::insert (KeyT key, ValueT value)
{
  // checks if key already in map
  if (contains_key (key))
  { return false; }
  int bucket = hash_function (key);
  auto my_pair = std::make_pair (key, value);
  buckets[bucket].push_back (my_pair);
  _entries += 1;
  update_load_factor ();
  // if map load factor is bigger than upper load, updates size
  if (load_factor > DEFAULT_UPPER_LOAD)
  {
    resize_array (_capacity * 2);
  }
  return true;
}

template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::print_all () //TODO: delete
{
  for (int i = 0; i < _capacity; i++)
  {
    if (buckets[i].empty ())
    {
      continue;
    }
    for (const auto &pair: buckets[i])
    {
      cout << "(" << pair.first
           << ", " << pair.second << "), ";
    }
    endl (cout);
  }
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::~HashMap ()
{
  // deletes dynamic array, vectors will destruct themselves
  delete[] buckets;
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::contains_key (const KeyT &key) const
{
  // calculates hash and checks if key is in bucket
  int bucket = hash_function (key);
  for (const auto &pair: buckets[bucket])
  {
    if (pair.first == key)
    {
      return true;
    }
  }
  return false;
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::hash_function (const KeyT &key) const
{
  // calculates hash function and bucket number
  std::hash<KeyT> hasher;
  unsigned long place = hasher (key) & (_capacity - 1);
  return place;
}

template<typename KeyT, typename ValueT>
const ValueT &HashMap<KeyT, ValueT>::at (const KeyT &key) const
{
  // calculates hash and search in bucket
  int bucket = hash_function (key);
  for (const auto &pair: buckets[bucket])
  {
    if (pair.first == key)
    {
      return pair.second;
    }
  }
  throw std::out_of_range (KEY_NOT_FOUND);
}

template<typename KeyT, typename ValueT>
ValueT &HashMap<KeyT, ValueT>::at (const KeyT &key)
{
  // calculates hash and search in bucket
  int bucket = hash_function (key);
  for (auto &pair: buckets[bucket])
  {
    if (pair.first == key)
    {
      return pair.second;
    }
  }
  throw std::out_of_range (KEY_NOT_FOUND);
}

template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::resize_array (const size_t size, bool flag)
{
  auto old_array = buckets;
  int old_capacity = _capacity;
  // creates new array in given size

  _capacity = size;
  update_load_factor();
  if (flag)
  {
    while (load_factor < DEFAULT_LOWER_LOAD)
    {
      _capacity = _capacity / 2;
      update_load_factor();
      if (_capacity==1)
      { break;}
    }
  }
  buckets = new vector<std::pair<KeyT, ValueT>>[_capacity];
  _entries = 0;
  // insert all items from old array to new array
  for (int bucket = 0; bucket < old_capacity; bucket++)
  {
    for (auto &pair: old_array[bucket])
    {
      insert (pair.first, pair.second);
    }
  }
  // free old array
  delete[] old_array;
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::erase (const KeyT &key)
{
  // if key not in map return false
  if (!contains_key (key))
  { return false; }
  int bucket = hash_function (key);
  int index = 0;
  for (auto &pair: buckets[bucket])
  {
    if (pair.first == key)
    {
      //keeps track on vector index to delete the item
      buckets[bucket].erase (buckets[bucket].begin () + index);
      _entries -= 1;
      break;
    }
    index++;
  }
  update_load_factor ();
  // checks if map needs to change size
  if (load_factor < DEFAULT_LOWER_LOAD)
  {
    resize_array (_capacity / 2, true);
  }
  return true;
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_size (const KeyT &key) const
{
  if (!contains_key (key))
  { throw std::out_of_range (KEY_NOT_FOUND); }
  // calculates hash function to find the bucket
  int bucket = hash_function (key);
  return buckets[bucket].size ();
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_index (const KeyT &key) const
{
  if (!contains_key (key))
  { throw std::out_of_range (KEY_NOT_FOUND); }
  // hash function calculates bucket number
  return hash_function (key);
}

template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::clear ()
{
  // clears all vectors
  for (int i = 0; i < _capacity; i++)
  {
    buckets[i].clear ();
  }
  _entries = 0;
  update_load_factor ();
}

template<typename KeyT, typename ValueT>
ValueT &HashMap<KeyT, ValueT>::operator[] (KeyT key)
{
  if (!contains_key (key))
  {
    insert (key, ValueT ());
  }
  // uses non const at
  return at(key);
}

template<typename KeyT, typename ValueT>
const ValueT &HashMap<KeyT, ValueT>::operator[] (KeyT key) const
{
  if (!contains_key (key))
  {
    auto last_bucket = buckets[_capacity-1];
    return last_bucket[last_bucket.size()].second; // will throw seg fault
  }
  // uses const at
  return at(key);
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT> &HashMap<KeyT, ValueT>::operator=
    (const HashMap &sec_hash_map)
{
  // self assigment
  if (&sec_hash_map == this)
  {
    return *this;
  }
  delete[] buckets;
  _entries = 0;
  _capacity = sec_hash_map._capacity;
  buckets = new vector<std::pair<KeyT, ValueT>>[_capacity];
  // copies all items
  for (int bucket = 0; bucket < _capacity; bucket++)
  {
    for (auto &pair: sec_hash_map.buckets[bucket])
    {
      insert (pair.first, pair.second);
    }
  }
  return *this;
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::operator==
    (const HashMap<KeyT, ValueT> &other_map) const
{
  // returns false if maps are diffrent sizes
  if (_entries != other_map.size ())
  { return false; }
  // checks that each entry is in second map
  for (int i = 0; i < _capacity; i++)
  {
    for (auto &pair: buckets[i])
    {
      if (!other_map.contains_key (pair.first))
      {
        return false;
      }
      if (other_map[pair.first] != pair.second)
      {
        return false;
      }
    }
  }
  return true;
}


#endif //_HASHMAP_HPP_
