#include "HashMap.hpp"
#include <iterator>

#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_



 class invalid_key : public std::invalid_argument
 {
  public:
    invalid_key(): std::invalid_argument (KEY_NOT_FOUND) {}
 };

class Dictionary : public HashMap<string ,string>
{
 public:
  Dictionary():HashMap<string, string>() {}
  Dictionary(const vector<string>& keys, const vector<string>& values):
      HashMap<string, string>(keys, values) {}
  Dictionary(Dictionary& other_dict) : HashMap<string, string>(other_dict) {}

  bool erase (const string &key);
  template<typename Iterator>
  void update(Iterator begin, Iterator end);
};
bool Dictionary::erase (const string &key)
{
  if (!HashMap::erase (key))
  {
    throw invalid_key ();
  }
  return true;
}

template<typename Iterator>
void Dictionary::update (Iterator begin, Iterator end)
{
  if (begin == end)
  {return;}
  for (auto& it = begin ; it!= end; ++it)
  {
    if (contains_key (it->first))
    {
      erase (it->first); // TODO: check, erase or just change;
    }
    insert (it->first, it->second);
  }

}

#endif //_DICTIONARY_HPP_

