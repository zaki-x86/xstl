#ifndef _VECTOR_BASE_H_

#define _VECTOR_BASE_H_

#include "xstl/config/config.h"
#include <memory>

_BEGIN_XSTL

/**
 * @brief A class template that provides some of the implementation details for
 * a dynamic array-based container.
 *
 * The `vector_base` class template is intended to be used as a base class for a
 * dynamic array-based container. It provides a nested struct, `_vector_impl`,
 * to store the data for the container, as well as member functions to access
 * and modify the `_vector_impl` object.
 *
 * One of the key benefits of using vector_base as a base class is that it
 * allows us to reuse code and reduce the amount of code you need to write and
 * maintain. For example, you can use `vector_base` to provide the underlying
 * dynamic array storage and allocator functionality for your container, and
 * then focus on implementing the specific interface and functionality that you
 * want for your container.
 *
 * @tparam T The element type of the container.
 * @tparam Allocator The allocator type to use for allocating memory for the
 * container.
 */

template <typename T, typename Allocator>
struct vector_base {

  /**
   * @brief createS a new allocator type `_Tp_alloc_type` that is parameterized with the same type as `vector_base`.
   * 
   * This can be used to create a new allocator object that is compatible with `vector_base`, regardless of the original type of the allocator.
   * 
   * @note In C++11 and earlier, the `std::allocator` class template defined a nested type called `rebind`, which could be used to create a new allocator type that was parameterized with a different type. For example, you could use `std::allocator<int>::rebind<char>::other` to create an allocator type that could be used to allocate char objects, using an allocator that was originally designed to allocate int objects.
   * In C++17, the rebind type was deprecated and replaced with a rebind_alloc type trait, which is defined in the <memory> header file. To create a new allocator type that is parameterized with a different type.
   */
  using _Tp_alloc_type = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;
  using pointer = typename std::allocator_traits<_Tp_alloc_type>::pointer;

  /**
   * @brief Nested struct to store the data for the vector.
   *
   */
  struct _vector_impl {
    T *_m_start;
    T *_m_finish;
    T *_m_end_of_storage;
    Allocator _m_allocator;

    _vector_impl()
        : _m_start(nullptr), _m_finish(nullptr), _m_end_of_storage(nullptr) {}
    _vector_impl(const Allocator &a)
        : _m_start(nullptr), _m_finish(nullptr), _m_end_of_storage(nullptr),
          _m_allocator(a) {}
  };

  // Default constructor creates an empty vector_impl object with _m_start,
  // _m_finish, and _m_end_of_storage all set to nullptr.
  vector_base() {}

  // Constructor that takes an allocator object creates a vector_impl object
  // with _m_start, _m_finish, and _m_end_of_storage all set to nullptr and the
  // allocator object stored as a member variable.
  vector_base(const Allocator &a) : _m_impl(a) {}

  // Destructor calls the allocator's deallocate() function to free the memory
  // used by the vector's data.
  ~vector_base() {
    _m_deallocate(_m_impl._m_start,
                  static_cast<std::size_t>(_m_impl._m_end_of_storage - _m_impl._m_start));
  }

  // Returns a reference to the allocator object.
  Allocator &_m_get_Tp_allocator() { return _m_impl._m_allocator; }
  const Allocator &_m_get_Tp_allocator() const { return _m_impl._m_allocator; }

  // Returns a reference to the vector_impl object.
  _vector_impl &_m_get_impl() { return _m_impl; }
  const _vector_impl &_m_get_impl() const { return _m_impl; }

  _vector_impl _m_impl;

  // Helper function to deallocate memory using
  // Helper function to deallocate memory using the allocator's deallocate()
  // function.
  void _m_deallocate(T *p, std::size_t n) {
    if (p)
      _m_impl._m_allocator.deallocate(p, n);
  }

  void _m_allocate(std::size_t n) {
      _m_impl._m_allocator.allocate(n);
  }
  
};

_END_XSTL

#endif // !_vector_BASE_H
