#include "iostream"
#include "vector"
#include "string"
#include <utility>
#include <algorithm>
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
  int capacity() const
  {return _capacity;}
  int size() const
  {return _entries;}
  bool empty() const
  {return _entries == 0;}
  bool insert (string key, string value);
  bool contains_key (const string &key);
  void print_all (); //TODO: delete
  ~HashMap ();

 private:
  int hash_function (const string &key) const;
  int _capacity;
  int _entries = 0;
  vector<std::pair<string, string>> *buckets;
};

HashMap::HashMap () : _capacity (INITIAL_SIZE)
{
  buckets = new vector<std::pair<string, string>>[_capacity];
}

bool HashMap::insert (string key, string value)
{
  int bucket = hash_function (key);
  auto my_pair = std::make_pair (key, value);
  buckets[bucket].push_back (my_pair);
  _entries += 1;
  return true;
}

void HashMap::print_all ()
{
  for (int i = 0; i < _capacity; i++)
  {
    if (buckets[i].empty())
    {
      continue;
    }
    cout <<  i << endl;
    for (const auto &pair: buckets[i])
    {
      cout << "(" << pair.first
           << ", " << pair.second << "), ";
    }
    endl(cout);
  }
}
HashMap::~HashMap ()
{
  delete[] buckets;
}
bool HashMap::contains_key (const string &key)
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


#endif //_HASHMAP_HPP_
