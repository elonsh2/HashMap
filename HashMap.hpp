#include "iostream"
#include "vector"
#include "string"
#include <utility>
#include <algorithm>
#include <stdexcept>

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
  int bucket_size(const string key) const;
  int capacity () const
  { return _capacity; }
  int bucket_index(const string key) const;
  void clear();
  int size () const
  { return _entries; }
  bool empty () const
  { return _entries == 0; }
  double get_load_factor () const
  { return load_factor; }
  string at (string key) const;
  bool insert (string key, string value);
  bool contains_key (const string &key) const;
  bool erase (const string key);
  void print_all (); //TODO: delete
  ~HashMap ();

 private:
  void update_load_factor ()
  { load_factor = _entries / double (_capacity); }
  void resize_array (size_t size);
  int hash_function (const string &key) const;
  int _capacity;
  int _entries = 0;
  double load_factor = _entries / _capacity;
  vector<std::pair<string, string>> *buckets;
};

HashMap::HashMap () : _capacity (INITIAL_SIZE)
{
  buckets = new vector<std::pair<string, string>>[_capacity];
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

string HashMap::at (string key) const
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
  int old_capacity =_capacity;
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
  for (const auto &pair: buckets[bucket])
  {
    if (pair.first == key)
    {
      buckets[bucket].erase (buckets[bucket].begin()+index);
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
  {throw std::out_of_range ("Key doesn't exist");}
  int bucket = hash_function (key);
  return buckets[bucket].size();
}
int HashMap::bucket_index (const string key) const
{
  if (!contains_key (key))
  {throw std::out_of_range ("Key doesn't exist");}
  return hash_function (key);

}
void HashMap::clear ()
{
  for (int i=0;i<_capacity;i++)
  {
    buckets[i].clear();
  }
  _entries = 0;
  update_load_factor();

}

#endif //_HASHMAP_HPP_
