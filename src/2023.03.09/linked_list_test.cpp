#include "linked_list_test.hpp"
#include "linked_list.hpp"
#include <iostream>

void testLinkedList() {
  using class_of_2023_03_09::LinkedList;

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