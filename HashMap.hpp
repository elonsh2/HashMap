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
  ~HashMap (); // destructor

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
  int bucket_size (const KeyT &key) const;
  int bucket_index (const KeyT &key) const;
  void clear ();
  const ValueT &at (const KeyT &key) const; //TODO: implement const
  bool insert (KeyT key, ValueT value);
  bool contains_key (const KeyT &key) const;
  bool erase (const KeyT &key);

  /*    operators      */
  ValueT &operator[] (KeyT key);
  const ValueT operator[] (KeyT key) const;  //TODO: implement const
  HashMap &operator= (const HashMap &sec_hash_map);
  bool operator== (const HashMap &other_map) const;
  bool operator!= (const HashMap &other_map) const
  { return !(*this == other_map); }
  void print_all (); //TODO: delete

  class ConstIterator
  {
   private:
    HashMap &table;
    vector<std::pair<KeyT, ValueT>> *buckets;
    int cur_bucket;
    int cur_index;

   public:
    typedef std::pair<KeyT, ValueT> value_type;
    typedef std::pair<KeyT, ValueT> &reference;
    typedef std::pair<KeyT, ValueT> *pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    ConstIterator (const HashMap &table, int cur_bucket_in, int cur_index_in) :
        buckets (table.buckets), cur_bucket (cur_bucket_in), cur_index (cur_index_in),
        table (const_cast<HashMap &>(table))
    {
      while (buckets[cur_bucket].empty ()
             && cur_bucket < (table._capacity - 1))
      {
        cur_bucket++;
      }
    }

    ConstIterator &operator++ ()
    {
      if (buckets[cur_bucket].size () > (cur_index + 1))
      {
        cur_index++;
        return *this;
      }
      cur_index = 0;
      cur_bucket++;
      while (buckets[cur_bucket].empty ()
             && cur_bucket < (table._capacity - 1))
      {
        cur_bucket++;
      }
      return *this;

    }

    ConstIterator operator++ (int)
    {
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
  const_iterator end () const
  {
    return ConstIterator (*this,
                          _capacity - 1, buckets[_capacity - 1].size ());
  }
  const_iterator cend () const
  {
    return ConstIterator (*this,
                          _capacity - 1, buckets[_capacity - 1].size ());
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
  void resize_array (size_t size);

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

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap ()
{
  buckets = new vector<std::pair<KeyT, ValueT>>[_capacity];
}
template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap (const vector<KeyT> &keys, const vector<ValueT>
&values)
{
  if (keys.size () != values.size ())
  {
    throw std::length_error (WRONG_SIZE_VECTORS);
  }
  buckets = new vector<std::pair<KeyT, ValueT>>[_capacity];
  for (int i = 0; i < keys.size (); i++)
  {
    if (contains_key (keys[i]))
    {
      erase (keys[i]); // TODO: check, erase or just change;
    }
    insert (keys[i], values[i]);
  }
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap (HashMap<KeyT, ValueT> &other_map)
{
  _capacity = other_map._capacity;
  buckets = new vector<std::pair<KeyT, ValueT>>[_capacity];
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
  if (contains_key (key))
  { return false; }
  int bucket = hash_function (key);
  auto my_pair = std::make_pair (key, value);
  buckets[bucket].push_back (my_pair);
  _entries += 1;
  update_load_factor ();
  if (load_factor > DEFAULT_UPPER_LOAD)
  {
    resize_array (_capacity * 2);
  }
  return true;
}
template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::print_all ()
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
  delete[] buckets;
}
template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::contains_key (const KeyT &key) const
{
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
  std::hash<KeyT> hasher;
  unsigned long place = hasher (key) & (_capacity - 1);
  return place;
}
template<typename KeyT, typename ValueT>
const ValueT &HashMap<KeyT, ValueT>::at (const KeyT &key) const
{
  int bucket = hash_function (key);
  for (const auto &pair: buckets[bucket])
  {
    if (pair.first == key)
    {
      return pair.second;
    }
  }
  throw std::out_of_range ("Key doesn't exist");
}
template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::resize_array (const size_t size)
{
  auto old_array = buckets;
  int old_capacity = _capacity;
  buckets = new vector<std::pair<KeyT, ValueT>>[size];
  _capacity = size;
  _entries = 0;
  for (int bucket = 0; bucket < old_capacity; bucket++)
  {
    for (auto &pair: old_array[bucket])
    {
      insert (pair.first, pair.second);
    }
  }
  delete[] old_array;
}
template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::erase (const KeyT &key)
{
  if (!contains_key (key))
  { return false; }
  int bucket = hash_function (key);
  int index = 0;
  for (auto &pair: buckets[bucket])
  {
    if (pair.first == key)
    {
      buckets[bucket].erase (buckets[bucket].begin () + index);
      _entries -= 1;
      break;
    }
    index++;
  }
  update_load_factor ();
  if (load_factor < DEFAULT_LOWER_LOAD)
  {
    resize_array (_capacity / 2);
  }
  return true;
}
template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_size (const KeyT &key) const
{
  if (!contains_key (key))
  { throw std::out_of_range ("Key doesn't exist"); }
  int bucket = hash_function (key);
  return buckets[bucket].size ();
}
template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_index (const KeyT &key) const
{
  if (!contains_key (key))
  { throw std::out_of_range ("Key doesn't exist"); }
  return hash_function (key);

}
template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::clear ()
{
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
  int bucket = hash_function (key);
  for (auto &pair: buckets[bucket])
  {
    if (pair.first == key)
    {
      return pair.second;
    }
  }
}
template<typename KeyT, typename ValueT>
const ValueT HashMap<KeyT, ValueT>::operator[] (KeyT key) const
{
  if (!contains_key (key))
  {
    return ValueT ();
  }
  int bucket = hash_function (key);
  for (auto &pair: buckets[bucket])
  {
    if (pair.first == key)
    {
      return pair.second;
    }
  }
}
template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT> &
HashMap<KeyT, ValueT>::operator= (const HashMap &sec_hash_map)
{
  if (&sec_hash_map == this)
  {
    return *this;
  }
  delete[] buckets;
  _entries = 0;
  _capacity = sec_hash_map._capacity;
  buckets = new vector<std::pair<KeyT, ValueT>>[_capacity];
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
bool
HashMap<KeyT, ValueT>::operator== (const HashMap<KeyT, ValueT> &other_map) const
{
  if (_entries != other_map.size ())
  { return false; }
  for (int i = 0; i < _capacity; i++)
  {
    for (const auto &pair: buckets[i])
    {
      if (other_map[pair.first] != pair.second)
      {
        return false;
      }
    }
  }
  return true;
}

#endif //_HASHMAP_HPP_
