#include "iostream"
#include "vector"
#include "string"
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <forward_list>

#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

using std::string;
using std::vector;
using std::cout;

using std::endl;
#define INITIAL_SIZE 16
#define DEFAULT_LOWER_LOAD 0.25
#define DEFAULT_UPPER_LOAD 0.75

class HashMap
{
 public:

  HashMap ();
  HashMap (const vector<string> &keys, const vector<string> &values);
  HashMap (HashMap &other_map);
  int bucket_size (const string key) const;
  int capacity () const
  { return _capacity; }
  int bucket_index (const string key) const;
  void clear ();
  int size () const
  { return _entries; }
  bool empty () const
  { return _entries == 0; }
  double get_load_factor () const
  { return load_factor; }
  const string &at (const string key) const; //TODO: implement const
  bool insert (string key, string value);
  bool contains_key (const string &key) const;
  bool erase (const string key);
  string &operator[] (string key);
  const string operator[] (string key) const;  //TODO: implement const
  HashMap &operator= (const HashMap &sec_hash_map);
  bool operator== (const HashMap &other_map) const;
  bool operator!= (const HashMap &other_map) const
  { return !(*this == other_map); }
  void print_all (); //TODO: delete


  ~HashMap ();

  class ConstIterator
  {
   private:
    vector<std::pair<string, string>> *buckets;
    std::pair<string, string> pair;

   public:
    typedef std::pair<string, string> value_type;
    typedef std::pair<string, string> &reference;
    typedef std::pair<string, string> *pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    ConstIterator (std::pair<string, string> &pair) : pair (pair)
    {}

    ConstIterator &operator++ ()
    {

      return *this;
    }

  };

  using const_iterator = ConstIterator;
  const_iterator begin () const
  { return ConstIterator (buckets[0][0]); }
 private:

  void update_load_factor ()
  { load_factor = _entries / double (_capacity); }
  void resize_array (size_t size);
  int hash_function (const string &key) const;
  int _capacity = INITIAL_SIZE;
  int _entries = 0;
  double load_factor = _entries / _capacity;
  vector<std::pair<string, string>> *buckets;
};

HashMap::HashMap ()
{
  buckets = new vector<std::pair<string, string>>[_capacity];
}

HashMap::HashMap (const vector<string> &keys, const vector<string> &values)
{
  if (keys.size () != values.size ())
  {
    throw std::length_error ("Vectors are not in the same size");
  }
  buckets = new vector<std::pair<string, string>>[_capacity];
  for (int i = 0; i < keys.size (); i++)
  {
    if (contains_key (keys[i]))
    {
      erase (keys[i]); // TODO: check, erase or just change;
    }
    insert (keys[i], values[i]);
  }
}

HashMap::HashMap (HashMap &other_map)
{
  _capacity = other_map._capacity;
  buckets = new vector<std::pair<string, string>>[_capacity];
  for (int bucket = 0; bucket < _capacity; bucket++)
  {
    for (auto &pair: other_map.buckets[bucket])
    {
      insert (pair.first, pair.second);
    }
  }
}

bool HashMap::insert (string key, string value)
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

void HashMap::print_all ()
{
  for (int i = 0; i < _capacity; i++)
  {
    if (buckets[i].empty ())
    {
      continue;
    }
    cout << i << endl;
    for (const auto &pair: buckets[i])
    {
      cout << "(" << pair.first
           << ", " << pair.second << "), ";
    }
    endl (cout);
  }
}

HashMap::~HashMap ()
{
  delete[] buckets;
}

bool HashMap::contains_key (const string &key) const
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

int HashMap::hash_function (const string &key) const
{
  std::hash<string> hasher;
  unsigned long place = hasher (key) & (_capacity - 1);
  return place;
}

const string &HashMap::at (const string key) const
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

void HashMap::resize_array (const size_t size)
{
  auto old_array = buckets;
  int old_capacity = _capacity;
  buckets = new vector<std::pair<string, string>>[size];
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

bool HashMap::erase (const string key)
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

int HashMap::bucket_size (const string key) const
{
  if (!contains_key (key))
  { throw std::out_of_range ("Key doesn't exist"); }
  int bucket = hash_function (key);
  return buckets[bucket].size ();
}

int HashMap::bucket_index (const string key) const
{
  if (!contains_key (key))
  { throw std::out_of_range ("Key doesn't exist"); }
  return hash_function (key);

}

void HashMap::clear ()
{
  for (int i = 0; i < _capacity; i++)
  {
    buckets[i].clear ();
  }
  _entries = 0;
  update_load_factor ();

}

string &HashMap::operator[] (string key)
{
  if (!contains_key (key))
  {
    insert (key, string ());
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

const string HashMap::operator[] (string key) const
{
  if (!contains_key (key))
  {
    return string ();
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

HashMap &HashMap::operator= (const HashMap &sec_hash_map)
{
  if (&sec_hash_map == this)
  {
    return *this;
  }
  delete[] buckets;
  _entries = 0;
  _capacity = sec_hash_map._capacity;
  buckets = new vector<std::pair<string, string>>[_capacity];
  for (int bucket = 0; bucket < _capacity; bucket++)
  {
    for (auto &pair: sec_hash_map.buckets[bucket])
    {
      insert (pair.first, pair.second);
    }
  }
  return *this;
}

bool HashMap::operator== (const HashMap &other_map) const
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
