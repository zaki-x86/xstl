#ifndef _ARRAY_DEBUG_H_

#define _ARRAY_DEBUG_H_

#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>
#include <array>
#include <vector>
#include <typeinfo>
#include <helpers/generators.h>
#include <xstl/config/config.h>
#include <xstl/debug/xstl_crt.h>
#include <xstl/debug/assertions.h>
#include <xstl/array.hpp>

// ************************************************
// * Assertion Macros
// ************************************************

/**
 * @brief Assertion macros for xstl::array
 * 
 * `_xstl_assert*` macros are used to assert the correctness of xstl::array
 * These assertions are based on the `gtest` framework, they generate non_fatal failures, and continue the test execution
 * 
 * _xstl_assert(cond, fail_msg) asserts that `cond` is true, and if not, prints `fail_msg`
 * _xstl_assert_death(exp, err_msg, fail_msg) asserts that `exp` causes the process to termninate with an error message `err_msg`, and if not, prints `fail_msg`
 * _xstl_assert_throws(exp, exception, fail_msg) asserts that `exp` throws an exception of type `exception`, and if not, prints `fail_msg`
*/


_BEGIN_XSTL_TEST

// ************************************************
// * Utility Functions
// ************************************************

    /**
     * ArrayTestUtils
     * @brief A utility class for testing xstl::array
     * `clone_c_array` clones a c-style array into an xstl::array
     * `init_xstl_array` initializes an xstl::array with random values
     * `init_cstyle_array` initializes a c-style array with random values
     * `assert_out_of_range` asserts that an out of range exception is thrown
     * `assert_indexing_error` asserts that an indexing error is thrown
    */
    struct ArrayTesterUtils
    {
        template<typename _Ty, size_t _Size>
        static void clone_c_array(xstl::array<_Ty, _Size>& _arr, _Ty* _c_arr) {
            size_t i = 0;
            for (auto& elem : _arr)
            {
                elem = _c_arr[i];
                ++i;
            }
        }
    
        template<typename _Ty, size_t _Size>
        static void init_xstl_array(xstl::array<_Ty, _Size>& _arr)
        {
            for (auto& _e : _arr)
                set_random_test_value(_e);
        }
        
        template<typename _Ty, size_t _Size>
        static void init_cstyle_array(_Ty* _arr)   
        {
            for (size_t i = 0; i < _Size; i++)
            {
                set_random_test_value(_arr[i]);
            }
        }

    };

// ************************************************
// * ArrayTester Base
// ************************************************
    
    /**
     * _Array_tester
     * @brief A testing base for ArrayTester
     * @tparam _Ty is stored type within the array
     * @tparam _Size is the size of the array
     * @tparam _arr is the array to debug
     * @tparam _ref is a c-style array to use as a reference to debug xstl::array
     * @tparam _arr is a clone of _ref, and we used _ref as a "reference container" to debug _arr
     * 
     * `_test_forward_iteration()` tests the forward iteration of the array
     * `_test_reverse_iteration()` tests the backward iteration of the array
     * `_test_const_forward_iteration()` tests the forward const iteration of the array
     * `_test_const_reverse_iteration()` tests the backward const iteration of the array
     * `_test_size()` tests the size of the array
     * `_test_max_size()` tests the max size of the array
     * `_test_empty()` tests the emptyness of the array
     * `_test_at()` tests the `at()` method of the array
     * `_test_front()` tests the `front()` method of the array
     * `_test_back()` tests the `back()` method of the array
     * `_test_generated_c_array()` tests the `data()` method of the array
     * `_test_fill()` tests the `fill()` method of the array
     * `_test_swap()` tests the `swap()` method of the array
     * `_test_relational_operators()` tests the relational operators of the array
     * `_test_print()` tests the printing of the array
    */
    template<typename _Ty, std::size_t _Size>
    struct _Array_tester
    {
    protected:
        _Array_tester() = default;
        _Array_tester(const xstl::array<_Ty, _Size>& arr, _Ty* ref)
        {
            for (std::size_t i = 0; i < _Size; ++i)
            {
                *(_ref + i) = *(ref + i);
                _arr[i] = arr[i];
            }
        }
       
        void _test_forward_iteration() {
            // test: begin() and end()

            for (struct { size_t i; typename xstl::array<_Ty, _Size>::iterator _it; } loop = { 0, _arr.begin() }; loop._it != _arr.end(); ++loop._it, ++loop.i)
            {
                _xstl_assert(*(loop._it) == _ref[loop.i], "iterator functions failed!");
            }

            xstl::array<_Ty, _Size> _carr{};
            std::copy(_ref, _ref + _Size, _carr.begin());
            _carr = static_cast<const xstl::array<_Ty, _Size>>(_carr);

            // test const overloads of: begin() const and end() const
            
            for (struct{ size_t i; typename xstl::array<_Ty, _Size>::const_iterator _it; } loop = { 0, _carr.begin()}; loop._it != _carr.end(); ++loop._it, ++loop.i)
            {
                _xstl_assert(*(loop._it) == _ref[loop.i], "constant overloads of iterator functions failed!");
            }
        }

        void _test_const_forward_iteration() {
            xstl::array<_Ty, _Size> _carr{};
            std::copy(_ref, _ref + _Size, _carr.begin());
            _carr = static_cast<const xstl::array<_Ty, _Size>>(_carr);

            // test of: cbegin() const and cend() const
            for (struct{ size_t i; typename xstl::array<_Ty, _Size>::const_iterator _it; } loop = { 0, _carr.begin() }; loop._it != _carr.end(); ++loop._it, ++loop.i)
            {
                _xstl_assert(*(loop._it) == _ref[loop.i], "constant overloads of iterator functions failed!");
            }
        }

        void _test_reverse_iteration() {
            // Test rbegin and rend

            for (struct{ size_t i; typename xstl::array<_Ty, _Size>::reverse_iterator _it; } loop = { _Size - 1, _arr.rbegin() }; loop._it != _arr.rend(); ++loop._it, --loop.i)
            {
                _xstl_assert(*(loop._it) == _ref[loop.i], "reverse iterator functions failed!");
            }

            xstl::array<_Ty, _Size> _carr{};
            std::copy(_ref, _ref + _Size, _carr.begin());
            _carr = static_cast<const xstl::array<_Ty, _Size>>(_carr);

            // test const overloads of: rbegin() const and rend() const

            for (struct { size_t i; typename xstl::array<_Ty, _Size>::const_reverse_iterator _it; } loop = { _Size - 1, _carr.crbegin() }; loop._it != _carr.rend(); ++loop._it, --loop.i)
            {
                _xstl_assert(*(loop._it) == _ref[loop.i], "constant overloads of reverse iterator functions failed!");
            }
        }

        void _test_const_reverse_iteration() {
            xstl::array<_Ty, _Size> _carr{};
            std::copy(_ref, _ref + _Size, _carr.begin());
            _carr = static_cast<const xstl::array<_Ty, _Size>>(_carr);

            // Test: crbegin() and crend()

            for (struct{ size_t i ; typename xstl::array<_Ty, _Size>::const_reverse_iterator _it; } loop = { _Size - 1, _carr.crbegin() }; loop._it != _carr.crend(); ++loop._it, --loop.i)
            {
                _xstl_assert(*(loop._it) == _ref[loop.i], "constant overloads of reverse iterator functions failed!");
            }
        }

        bool _test_size() {
            auto _ref_size = sizeof(_ref) / sizeof(_Ty);

            _xstl_assert(_arr.size() == _ref_size, "size() failed");
            return _arr.size() == _ref_size;
        }

        bool _test_max_size() {
            auto _ref_size = sizeof(_ref) / sizeof(_Ty);

            _xstl_assert(_arr.max_size() == _ref_size, "max_size() failed");
            return _arr.max_size() == _ref_size;
        }

        bool _test_empty() {
            _xstl_assert(!_arr.empty(), "empty() failed");
            return !_arr.empty();
        }

        void _test_access_operator() {
            xstl::array<_Ty, _Size> _carr{};
            std::copy(_ref, _ref + _Size, _carr.begin());
            _carr = static_cast<const xstl::array<_Ty, _Size>>(_carr);

            for (size_t i = 0; i < _Size; i++)
            {
                _xstl_assert(_arr[i] == _ref[i], "operator[](size_t) failed");
                _xstl_assert(_carr[i] == _ref[i], "operator[](size_t) const overload failed");
            }
        }

        void _test_at() {
            xstl::array<_Ty, _Size> _carr{};
            std::copy(_ref, _ref + _Size, _carr.begin());
            _carr = static_cast<const xstl::array<_Ty, _Size>>(_carr);

            for (size_t i = 0; i < _Size; i++)
            {
                _xstl_assert(_arr.at(i) == _ref[i],  "at(size_t) failed");
                _xstl_assert(_carr.at(i) == _ref[i], "at(size_t) const overload failed");
            }
        }

        void _test_front() {
            xstl::array<_Ty, _Size> _carr{};
            std::copy(_ref, _ref + _Size, _carr.begin());
            _carr = static_cast<const xstl::array<_Ty, _Size>>(_carr);

            // Test: front() and front() const overload
            _xstl_assert(_arr.front() == _ref[0], "front() failed");
            _xstl_assert(_carr.front() == _ref[0], "front() const overload failed");
        }

        void _test_back() {
            xstl::array<_Ty, _Size> _carr{};
            std::copy(_ref, _ref + _Size, _carr.begin());
            _carr = static_cast<const xstl::array<_Ty, _Size>>(_carr);

            // Test: back() and back() const overload
            _xstl_assert(_arr.back() == _ref[_Size - 1], "back() failed");
            _xstl_assert(_carr.back() == _ref[_Size - 1], "back() const overload failed");
        }

        void _test_generated_c_array() {
            xstl::array<_Ty, _Size> _carr;
            std::copy(_ref, _ref + _Size, _carr.begin());
            _carr = static_cast<const xstl::array<_Ty, _Size>> (_carr);

            // Test: data() and data() const overload
            _Ty* _arr_data = _arr.data();
            const _Ty* _carr_data = _carr.data();

            _array_assert(_arr_data, _ref, _Size, [](_Ty x, _Ty y) {
                _xstl_assert(x == y, "data() failed");
                });
            _array_assert(_carr_data, _ref, _Size, [](_Ty x, _Ty y) {
                _xstl_assert(x == y, "data() const overload failed");
                });
        }

        void _test_fill() {
            // fill _new_arr with value 5
            _Ty _new_arr[_Size];
            _Ty val;
            set_random_test_value(val);
            std::fill(_new_arr, _new_arr + _Size, val);
            // fill _arr with same value 
            _arr.fill(val);

            // compare: _arr, _new_arr
            _array_assert(_arr, _new_arr, _Size, [](_Ty x, _Ty y){
                _xstl_assert(x == y, "fill(const_reference) failed");
            });
        }

        void _test_swap() {

            // init arr1 and arr2 with random values
            xstl::array<_Ty, _Size> arr1;
            xstl::array<_Ty, _Size> arr2;
            _Ty val1, val2;
            set_random_test_value(val1);
            set_random_test_value(val2);
            std::fill(arr1.begin(), arr1.end(), val1);
            std::fill(arr2.begin(), arr2.end(), val2);
            arr1.swap(arr2);
            // verify x in arr1 is equal to y in arr2  

            for (auto &i : arr1)
            {
                _xstl_assert(i == val2, "swap() failed");
            }            
        }

        void _test_equality() {
            xstl::array<_Ty, _Size> arr3;
            xstl::array<_Ty, _Size> arr4;
            xstl::array<_Ty, _Size> arr5;
            xstl::array<_Ty, _Size> arr6;
            _Ty val;
            _Ty val2;
            set_random_test_value(val);
            set_random_test_value(val2);

            arr3.fill(val);
            arr4.fill(val);
            arr5.fill(val2);

            for (auto &i : arr6)
            {
                set_random_test_value(i); 
            }

            _xstl_assert(arr3 == arr4, "Arrays of equal elements should be equal");
            _xstl_assert(arr3 != arr5, "Arrays of unequal elements should be unequal");
            _xstl_assert(arr5 != arr6, "Arrays of unequal elements should be unequal");
        }

        void _test_print() {
            xstl::array<_Ty, 5> _dummy;
            _Ty _val;
            set_random_test_value(_val);
            _dummy.fill(_val);
            

            std::string _expected = "";
            if constexpr(std::is_same<_Ty, std::string>::value)
            {
                _expected = "{" + _val + ", " + _val + ", " + _val + ", " + _val + ", " + _val + "}";
            }
            else if constexpr(std::is_same<_Ty, char>::value)
            {
                _expected.push_back('{');
                _expected.push_back(_val);
                _expected = _expected + ", ";
                _expected.push_back(_val);
                _expected = _expected + ", ";
                _expected.push_back(_val);
                _expected = _expected + ", ";
                _expected.push_back(_val);
                _expected = _expected + ", ";
                _expected.push_back(_val);
                _expected.push_back('}');                
            }
            else
            {
                _expected = "{" + std::to_string(_val) + ", " + std::to_string(_val) + ", " + std::to_string(_val) + ", " + std::to_string(_val) + ", " + std::to_string(_val) + "}";
            }

        
            std::ostringstream output;

            // Redirect std::cout to the output string stream
            std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
            std::cout.rdbuf(output.rdbuf());

            // Output something to std::cout
            std::cout << _dummy;

            // Restore std::cout
            std::cout.rdbuf(oldCoutStreamBuf);

            // Check the contents of the output string stream
            _xstl_assert(output.str() == _expected, "print() failed");
        }

    private:
        xstl::array<_Ty, _Size> _arr;
        _Ty _ref[_Size];
        
        // Declared two template params: _Arr1 and _Arr2 to support any array type comparison whether it's 
        // a c-style array or std::array or xstl::array or any custom array type that supports access via [].
        template<typename _Arr1, typename _Arr2, typename Assertion> 
        void _array_assert(_Arr1 _arr1, _Arr2 _arr2, std::size_t _N, Assertion assert_function) const {
            for (size_t i = 0; i< _N; ++i)
            {
                assert_function(_arr1[i], _arr2[i]);
            }
        }
    };

    template<typename _Ty>
    struct _Array_tester<_Ty, 0>
    {
        _Array_tester() : _carr(static_cast<const xstl::array<_Ty, 0>>(_carr)) {}

        void _test_forward_iteration() {
            // Test: begin() and begin() const overload
            _xstl_assert(_arr.begin() == _arr.end(), "begin() failed");
            _xstl_assert(_carr.begin() == _carr.end(), "begin() const overload failed"); 
        }
        void _test_const_forward_iteration() {
            // Test: cbegin() and cend() const overload
            _xstl_assert(_carr.cbegin() == _carr.cend(), "cbegin() const overload failed");
        }
        void _test_reverse_iteration() {
            // Test: rbegin() and rbegin() const overload
            _xstl_assert(_arr.rbegin() == _arr.rend(), "rbegin() failed");
            _xstl_assert(_carr.rbegin() == _carr.rend(), "rbegin() const overload failed");
        }
        void _test_const_reverse_iteration() {
            // Test: crbegin() and crend() const overload
            _xstl_assert(_carr.crbegin() == _carr.crend(), "crbegin() const overload failed");
        }

        void _test_size() {
            // Test: size() and size() const overload
            _xstl_assert(_arr.size() == 0, "size() failed");
            _xstl_assert(_carr.size() == 0, "size() const overload failed");
        }

        void _test_max_size() {
            // Test: max_size() and max_size() const overload
            _xstl_assert(_arr.max_size() == 0, "max_size() failed");
            _xstl_assert(_carr.max_size() == 0, "max_size() const overload failed");
        }

        void _test_empty() {
            // Test: empty() and empty() const overload
            _xstl_assert(_arr.empty() == true, "empty() failed");
            _xstl_assert(_carr.empty() == true, "empty() const overload failed");
        }

        void _test_access_operator() const {
            // Test: operator[] and operator[] const overload
            // TODO implement a proper death test
            return;
        } 

        void _test_at() const {
            // Test: at() and at() const overload
            #ifdef XSTL_EXCEPTIONS_ENABLED
            _xstl_assert_throws(_arr.at(1), std::out_of_range);
            #else
            _xstl_assert_death(_arr.at(1));
            #endif
        }
        
        void _test_generated_c_array() {
            // Test: data() and data() const overload
            _xstl_assert(_arr.data() == nullptr, "data() failed");
            _xstl_assert(_carr.data() == nullptr, "data() const overload failed");
        }

        void _test_front() const {
            // Test: front() and front() const overload
            #ifdef XSTL_EXCEPTIONS_ENABLED
            _xstl_assert_throws(_arr.front(), std::out_of_range);
            #else
            _xstl_assert_death(_arr.front());
            #endif
        }



        void _test_back(){
            // Test: back() and back() const overload
            #ifdef XSTL_EXCEPTIONS_ENABLED
            _xstl_assert_throws(_arr.back(), std::out_of_range);
            #else
            _xstl_assert_death(_arr.back());
            #endif
        }
        
        void _test_fill() {
            // Test: fill()
            _Ty val;
            set_random_test_value(val);
            _arr.fill(val);
            _carr.fill(val);

            _xstl_assert(_arr.empty(), "fill() failed");
            _xstl_assert(_carr.empty(), "fill() const overload failed");
        }
        
        void _test_swap() {
            // Test: swap()
            _arr.swap(_carr);
        }

        void _test_equality() {
            xstl::array<_Ty, 0> arr3;
            xstl::array<_Ty, 0> arr4;

            _xstl_assert(arr3 == arr4, "Empty arrays should be equal"); 
        }

        void _test_print(){
            // Test: operator<<        
            std::ostringstream output;

            // Redirect std::cout to the output string stream
            std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
            std::cout.rdbuf(output.rdbuf());

            // Output something to std::cout
            std::cout << _arr;

            // Restore std::cout
            std::cout.rdbuf(oldCoutStreamBuf);

            // Check the contents of the output string stream
            _xstl_assert(output.str() == "{}", "print() failed");
        }

        // add private members
    private:
        xstl::array<_Ty, 0> _arr{};
        xstl::array<_Ty, 0> _carr{};
    };
    
// *************************************************************
// ArrayTester
// *************************************************************

    /**
     * ArrayTester
     * @brief A testing class for xstl::array
     * @tparam _Ty is stored type within the array
     * @tparam _Size is the size of the array
     * @tparam _arr is the array to debug
     * @tparam _ref is a c-style array to use as a reference to debug xstl::array
     * 
     * `perform_iterator_checks()` performs all the iterator checks
     * `perform_storage_checks()` performs all the storage checks
     * `perform_accessors_checks()` performs all the accessors checks
     * `perform_modifiers_checks()` performs all the modifiers checks
     * `test_print()` performs all the stream checks
     * `perform_relational_ops_checks()` performs all the relational ops checks
    */
    template<typename _Ty, std::size_t _Size>
    struct ArrayTester : protected _Array_tester<_Ty, _Size>
    {
        using _Base = _Array_tester<_Ty, _Size>;

        ArrayTester() : _Base() {}
        ArrayTester(const xstl::array<_Ty, _Size>& arr, _Ty* ref) : _Base(arr, ref) {}

        void test_size() {
            this->_test_size();
        }

        void test_max_size() {
            this->_test_max_size();
        }

        void test_empty() {
            this->_test_empty();
        }

        void test_access_operator() {
            this->_test_access_operator();
        }

        void test_at() {
            this->_test_at();
        }

        void test_front() {
            this->_test_front();
        }

        void test_back() {
            this->_test_back();
        }

        void test_generated_c_array() {
            this->_test_generated_c_array();
        }

        void test_fill() {
            this->_test_fill();
        }

        void test_swap() {
            this->_test_swap();
        }

        void perform_iterator_checks();
        void perform_storage_checks();
        void perform_accessors_checks();
        void perform_modifiers_checks();
        void perform_relational_ops_checks();
        void test_print();
        void perform_all_initial_checks();
    };

    template<typename _Ty, std::size_t _Size>
    void ArrayTester<_Ty, _Size>::perform_storage_checks()
    {
        this->_test_size();
        this->_test_max_size();
        this->_test_empty();
    }

    template<typename _Ty, std::size_t _Size>
    void ArrayTester<_Ty, _Size>::perform_accessors_checks()
    {
        this->_test_access_operator();
        this->_test_at();
        this->_test_front();
        this->_test_back();
        this->_test_generated_c_array();
    }

    template<typename _Ty, std::size_t _Size>
    void ArrayTester<_Ty, _Size>::perform_iterator_checks()
    {
        this->_test_forward_iteration();
        this->_test_reverse_iteration();
        this->_test_const_forward_iteration();
        this->_test_const_reverse_iteration();
    }

    template<typename _Ty, std::size_t _Size>
    void ArrayTester<_Ty, _Size>::perform_modifiers_checks()
    {
        this->_test_fill();
        this->_test_swap();
    }

    template<typename _Ty, std::size_t _Size>
    void ArrayTester<_Ty, _Size>::perform_relational_ops_checks() {
        this->_test_equality();
    }

    template<typename _Ty, std::size_t _Size>
    void ArrayTester<_Ty, _Size>::test_print() {
        this->_test_print();
    }

    template<typename _Ty, std::size_t _Size>
    void ArrayTester<_Ty, _Size>::perform_all_initial_checks()
    {
        perform_iterator_checks();
        perform_storage_checks();
        perform_accessors_checks();
        perform_modifiers_checks();
        perform_relational_ops_checks();
        test_print();
    }

    template<typename _Ty>
    struct ArrayTester<_Ty, 0> : protected _Array_tester<_Ty, 0>
    {
        using _Base = _Array_tester<_Ty, 0>;

        ArrayTester() : _Base() {}
        ArrayTester(const xstl::array<_Ty, 0>& arr, _Ty* ref) : _Base(arr, ref) {}

        void test_size() {
            this->_test_size();
        }

        void test_max_size() {
            this->_test_max_size();
        }

        void test_empty() {
            this->_test_empty();
        }

        void test_access_operator() {
            this->_test_access_operator();
        }

        void test_at() {
            this->_test_at();
        }

        void test_front() {
            this->_test_front();
        }

        void test_back() {
            this->_test_back();
        }

        void test_generated_c_array() {
            this->_test_generated_c_array();
        }

        void test_fill() {
            this->_test_fill();
        }

        void test_swap() {
            this->_test_swap();
        }

        void test_print() {
            this->_test_print();
        }

        void perform_all_checks() {
            perform_iterator_checks();
            perform_storage_checks();
            perform_accessors_checks();
            perform_modifiers_checks();
            perform_relational_ops_checks();
            test_print();
        }

        void perform_iterator_checks() {
            this->_test_forward_iteration();
            this->_test_const_forward_iteration();
            this->_test_reverse_iteration();
            this->_test_const_reverse_iteration();
        }
        void perform_storage_checks() {
            this->_test_size();
            this->_test_max_size();
            this->_test_empty();
        }

        void perform_accessors_checks() {
            this->_test_access_operator();
            this->_test_at();
            this->_test_generated_c_array();
            this->_test_front();
            this->_test_back();
        }
        void perform_modifiers_checks() {
            this->_test_fill();
            this->_test_swap();
        }

        void perform_relational_ops_checks() {
            this->_test_equality();
        }
    };


_END_XSTL_TEST

#endif // !_ARRAY_DEBUG_H_