#include <cassert>
#include "map"
#include "unordered_map"
#include "test_string.h"
#include "test_intstring.h"
#include "const_test.h"



void test_string();
void test_intstring();
void test_const();

int main ()
{
  test_string();
  test_intstring();
  test_const();



}

void test_string()
{
  HashMap<string, string> hash;
  test_insert(hash);
  find_test (hash);
  resize_up_test (hash);
  resize_down_test(hash);
  size_test (hash);
  ctr_test();
  test_operators();
  test_iterator();
}

void test_intstring()
{
  cout<< "SECOND TEST: "<<endl;
  HashMap<string, int> hash;
  test_insert1(hash);
  find_test1 (hash);
  resize_up_test1 (hash);
  resize_down_test1(hash);
  size_test1 (hash);
  ctr_test1();
  test_operators1();
  test_iterator1();
}

void test_const()
{
  cout<< "THIRD TEST: "<<endl;
  test_iterator_dixt ();
  test_dict_ctr();
  test_invalid_key();
  test_update();
}