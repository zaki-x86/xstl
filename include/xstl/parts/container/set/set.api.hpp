#ifndef _SET_API_H_

#define _SET_API_H_

#include <iostream>
#include <memory>
#include <set>

#include "xstl/config/config.h"

_BEGIN_XSTL

template <class Key, class Compare = std::less<Key>,
          class Allocator = std::allocator<Key>>
class set {
public:
  // types:
  typedef Key* iterator;					// temp
  typedef const Key* const_iterator;		// temp
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  
  typedef Key key_type;
  typedef Key value_type;
  typedef Compare key_compare;
  typedef Compare value_compare;
  typedef Allocator allocator_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;
  typedef typename Allocator::size_type size_type;
  typedef typename Allocator::difference_type difference_type;

  // construct/copy/destroy:
  explicit set(const Compare &comp = Compare(),
               const Allocator & = Allocator());

  template <class InputIterator>
  set(InputIterator first, InputIterator last, const Compare &comp = Compare(),
      const Allocator & = Allocator());

  set(const set &x);
  set(set &&x);
  set(std::initializer_list<value_type> il, const Compare &comp = Compare(),
      const Allocator & = Allocator());

  ~set();

  set &operator=(const set &x);
  set &operator=(set &&x);
  set &operator=(std::initializer_list<value_type> il);

  // iterators:
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;

  const_iterator cbegin() const;
  const_iterator cend() const;
  const_reverse_iterator crbegin() const;
  const_reverse_iterator crend() const;

  // capacity:
  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  // modifiers:
  template <class... Args> std::pair<iterator, bool> emplace(Args &&... args);

  template <class... Args>
  iterator emplace_hint(const_iterator position, Args &&... args);

  std::pair<iterator, bool> insert(const value_type &x);
  std::pair<iterator, bool> insert(value_type &&x);
  iterator insert(const_iterator position, const value_type &x);
  iterator insert(const_iterator position, value_type &&x);
  template <class InputIterator>
  void insert(InputIterator first, InputIterator last);
  void insert(std::initializer_list<value_type> il);

  iterator erase(const_iterator position);
  size_type erase(const key_type &x);
  iterator erase(const_iterator first, const_iterator last);
  void clear();

  // observers:
  key_compare key_comp() const;
  value_compare value_comp() const;
  allocator_type get_allocator() const;

  // set operations:
  iterator find(const key_type &x);
  const_iterator find(const key_type &x) const;
  size_type count(const key_type &x) const;
  iterator lower_bound(const key_type &x);
  const_iterator lower_bound(const key_type &x) const;
  iterator upper_bound(const key_type &x);
  const_iterator upper_bound(const key_type &x) const;
  std::pair<iterator, iterator> equal_range(const key_type &x);
  std::pair<const_iterator, const_iterator>
  equal_range(const key_type &x) const;
};

template <class Key, class Compare, class Allocator>
void swap(set<Key, Compare, Allocator>& x,
          set<Key, Compare, Allocator>& y);

template <class Key, class Compare, class Allocator>
bool operator!=(const set<Key, Compare, Allocator>& x,
                const set<Key, Compare, Allocator>& y);

template <class Key, class Compare, class Allocator>
bool operator> (const set<Key, Compare, Allocator>& x,
                const set<Key, Compare, Allocator>& y);

template <class Key, class Compare, class Allocator>
bool operator>=(const set<Key, Compare, Allocator>& x,
                const set<Key, Compare, Allocator>& y);

template <class Key, class Compare, class Allocator>
bool operator<=(const set<Key, Compare, Allocator>& x,
                const set<Key, Compare, Allocator>& y);

#if __cplusplus >= 202003L
    template <class Key, class Compare, class Allocator>
    inline auto operator<=>(const set<Key, Compare, Allocator>& x,
                    const set<Key, Compare, Allocator>& y) ->std::strong_ordering;
#endif

_END_XSTL

#endif // !_SET_API_H_