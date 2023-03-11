#ifndef MODERN_CPP_INCLUDE_2023_03_09_LINKED_LIST_HPP
#define MODERN_CPP_INCLUDE_2023_03_09_LINKED_LIST_HPP

#include <algorithm>
#include <cstdint>
#include <iterator>

namespace class_of_2023_03_09 {

template <typename T> class LinkedList {
private:
  struct Node {
    T value;
    Node *next;
  };

public:
  class Iterator : public std::iterator<std::forward_iterator_tag, T> {
  public:
    Iterator() = default;
    explicit Iterator(Node *);
    Iterator(const Iterator &);

    Iterator &operator=(const Iterator &);
    Iterator operator++();
    Iterator operator++(int);
    T operator*() const;
    T &operator*();
    bool operator==(const Iterator &) const;
    bool operator!=(const Iterator &) const;

  private:
    void advanceIfNotNull_();

    Node *curr_{};
  };

  LinkedList();

  ~LinkedList();

  auto push(const T &);
  auto remove(const T &);
  [[maybe_unused]] auto contains(const T &) const;
  auto clear();
  [[maybe_unused]] auto size() const;

  auto begin();
  auto end();
  auto begin() const;
  auto end() const;

private:
  Node *head_, *tail_;
  std::size_t size_;
};

// iterator member functions

template <typename T>
LinkedList<T>::Iterator::Iterator(Node *node) : curr_(node) {}

template <typename T>
LinkedList<T>::Iterator::Iterator(const LinkedList::Iterator &other)
    : curr_(other.curr_) {}

template <typename T>
LinkedList<T>::Iterator &
LinkedList<T>::Iterator::operator=(const LinkedList::Iterator &other) {
  if (&other == this)
    goto iteratorCopyAssignmentOperatorReturn;
  curr_ = other.curr_;
iteratorCopyAssignmentOperatorReturn:
  return *this;
}

template <typename T>
LinkedList<T>::Iterator LinkedList<T>::Iterator::operator++() {
  advanceIfNotNull_();
  return *this;
}

template <typename T>
LinkedList<T>::Iterator LinkedList<T>::Iterator::operator++(int) {
  const auto result = Iterator(*this);
  advanceIfNotNull_();
  return result;
}

template <typename T> void LinkedList<T>::Iterator::advanceIfNotNull_() {
  if (curr_)
    curr_ = curr_->next;
}

template <typename T> T LinkedList<T>::Iterator::operator*() const {
  return curr_->value;
}

template <typename T> T &LinkedList<T>::Iterator::operator*() {
  return curr_->value;
}

template <typename T>
bool LinkedList<T>::Iterator::operator==(
    const LinkedList::Iterator &other) const {
  return curr_ == other.curr_;
}

template <typename T>
bool LinkedList<T>::Iterator::operator!=(
    const LinkedList::Iterator &other) const {
  return !operator==(other);
}

// member functions

template <typename T> LinkedList<T>::LinkedList() : head_(), tail_(), size_() {}

template <typename T> LinkedList<T>::~LinkedList() { clear(); }

template <typename T> auto LinkedList<T>::push(const T &value) {
  auto newNode = new Node{value, nullptr};
  if (head_)
    tail_ = tail_->next = newNode;
  else
    head_ = tail_ = newNode;
  ++size_;
}

template <typename T> auto LinkedList<T>::remove(const T &value) {
  for (auto it = head_; it && it->next; it = it->next) {
    if (it->next->value == value) {
      auto tmp = it->next;
      it->next = it->next->next;

      delete tmp;
      return true;
    }
  }
  return false;
}

template <typename T>
[[maybe_unused]] auto LinkedList<T>::contains(const T &value) const {
  auto tmp = end();
  return std::find(begin(), tmp, value) != tmp;
}

template <typename T> auto LinkedList<T>::clear() {
  Node *tmp;
  while (head_) {
    tmp = head_->next;
    delete head_;
    head_ = tmp;
  }
  size_ = 0;
}

template <typename T> [[maybe_unused]] auto LinkedList<T>::size() const { return size_; }

template <typename T> auto LinkedList<T>::begin() { return Iterator(head_); }

template <typename T> auto LinkedList<T>::end() { return Iterator(); }

template <typename T> auto LinkedList<T>::begin() const {
  return Iterator(head_);
}

template <typename T> auto LinkedList<T>::end() const { return Iterator(); }

} // namespace class_of_2023_03_09

#endif // MODERN_CPP_INCLUDE_2023_03_09_LINKED_LIST_HPP
