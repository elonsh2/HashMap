#include "iostream"
#include "vector"
#include "string"
#include <utility>
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
  bool insert (string key, string value);
  void print_all (); //TODO: delete

 private:
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
  auto my_pair = std::make_pair (key, value);
  buckets[0].push_back (my_pair);
  return true;
}

void HashMap::print_all ()
{
  for (int i = 0; i < _capacity; i++)
  {
    for (const auto &pair: buckets[i])
    {
      cout << "Bucket num " << i << endl << "KEY: " << pair.first
           << " VALUE: " << pair.second << endl;
    }
  }

}

#endif //_HASHMAP_HPP_
