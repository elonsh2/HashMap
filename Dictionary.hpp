#include "HashMap.hpp"

#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_

class InvalidKey : public std::invalid_argument
{
 public:
  InvalidKey () : std::invalid_argument (KEY_NOT_FOUND)
  {}
  InvalidKey (const string &str) : std::invalid_argument (str)
  {}
};

class Dictionary : public HashMap<string, string>
{
 public:
  // default ctr
  Dictionary () = default;
  // vectors ctr
  Dictionary (const vector<string> &keys, const vector<string> &values) :
      HashMap<string, string> (keys, values)
  {}
  // copy ctr
  Dictionary (const Dictionary &other_dict) : HashMap<string, string>
                                                  (other_dict)
  {}
  bool erase (const string &key) override;

  /**
 * gets an iterator and updates dict
 * @tparam Iterator iterator object
 * @param begin itertor start
 * @param end iterator end
 */
  template<typename Iterator>
  void update (Iterator begin, Iterator end);
};
bool Dictionary::erase (const string &key)
{
  // throws exception if key doesnt exist
  if (!HashMap::erase (key))
  {
    throw InvalidKey ();
  }
  return true;
}

template<typename Iterator>
void Dictionary::update (Iterator begin, Iterator end)
{
  // checks if iterator is empty
  if (begin == end)
  { return; }
  // same as vector constructor
  for (auto &it = begin; it != end; ++it)
  {

    if (contains_key (it->first))
    {
      at (it->first) = it->second;
    }
    else
    { insert (it->first, it->second); }
  }
}

#endif //_DICTIONARY_HPP_

