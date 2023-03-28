#include "tests/linked_list_test.hpp"
#include "data_structures/linked_list.hpp"
#include <iostream>

void testLinkedList() {
  using data_structures::LinkedList;

  LinkedList<int> l;

  for (auto i = 0; i < 10; ++i)
    l.push(i);

  for (auto i : l)
    std::cout << i << ' ';
  std::cout << std::endl;

  l.remove(4);

  for (auto i : l)
    std::cout << i << ' ';
  std::cout << std::endl;
}