/* Copyright 2017 - 2023 R. Thomas
 * Copyright 2017 - 2023 Quarkslab
 * Copyright 2017 - 2021, NVIDIA CORPORATION. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LIEF_ITERATORS_H
#define LIEF_ITERATORS_H
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iterator>
#include <ostream>
#include <type_traits>
#include <vector>

namespace LIEF {

template <class T>
using decay_t = typename std::decay<T>::type;

template <class T>
using add_const_t = typename std::add_const<T>::type;

template <class T>
using remove_const_t = typename std::remove_const<T>::type;

template <class T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

//! Iterator which returns reference on container's values
template <class T, typename U = typename decay_t<T>::value_type,
          class ITERATOR_T = typename decay_t<T>::iterator>
class ref_iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = decay_t<U>;
    using difference_type = ptrdiff_t;
    using pointer = typename std::remove_pointer<U>::type*;
    using reference = typename std::remove_pointer<U>::type&;

    using container_type = T;  // e.g. std::vector<Section*>&
    using DT_VAL = U;          // e.g. Section*
    using DT = decay_t<T>;     // e.g. std::vector<Section>
    using ref_t = typename ref_iterator::reference;
    using pointer_t = typename ref_iterator::pointer;

    ref_iterator(T container) : container_{std::forward<T>(container)}, distance_{0} {
        it_ = std::begin(container_);
    }

    ref_iterator(const ref_iterator& copy)
        : container_{copy.container_}, it_{std::begin(container_)}, distance_{copy.distance_} {
        std::advance(it_, distance_);
    }

    ref_iterator operator=(ref_iterator other) {
        swap(other);
        return *this;
    }

    void swap(ref_iterator& other) {
        std::swap(const_cast<add_lvalue_reference_t<remove_const_t<DT>>>(container_),
                  const_cast<add_lvalue_reference_t<remove_const_t<DT>>>(other.container_));
        std::swap(it_, other.it_);
        std::swap(distance_, other.distance_);
    }

    ref_iterator& operator++() {
        it_ = std::next(it_);
        distance_++;
        return *this;
    }

    ref_iterator operator++(int) {
        ref_iterator retval = *this;
        ++(*this);
        return retval;
    }

    ref_iterator& operator--() {
        if (it_ != std::begin(container_)) {
            it_ = std::prev(it_);
            distance_--;
        }
        return *this;
    }

    ref_iterator operator--(int) {
        ref_iterator retval = *this;
        --(*this);
        return retval;
    }

    ref_iterator& operator+=(const typename ref_iterator::difference_type& movement) {
        std::advance(it_, movement);
        distance_ += movement;
        return *this;
    }

    ref_iterator& operator-=(const typename ref_iterator::difference_type& movement) {
        return (*this) += -movement;
    }

    typename std::enable_if<!std::is_const<ref_t>::value, remove_const_t<ref_t>>::type
    operator[](size_t n) {
        return const_cast<remove_const_t<ref_t>>(
            static_cast<const ref_iterator*>(this)->operator[](n));
    }

    add_const_t<ref_t> operator[](size_t n) const {
        assert(n < size() && "integrity error: out of bound");

        ref_iterator* no_const_this = const_cast<ref_iterator*>(this);

        typename ref_iterator::difference_type saved_dist =
            std::distance(std::begin(no_const_this->container_), no_const_this->it_);
        no_const_this->it_ = std::begin(no_const_this->container_);
        std::advance(no_const_this->it_, n);

        auto&& v = const_cast<add_const_t<ref_t>>(no_const_this->operator*());

        no_const_this->it_ = std::begin(no_const_this->container_);
        std::advance(no_const_this->it_, saved_dist);

        return v;
    }

    ref_iterator operator+(typename ref_iterator::difference_type n) const {
        ref_iterator tmp = *this;
        return tmp += n;
    }

    ref_iterator operator-(typename ref_iterator::difference_type n) const {
        ref_iterator tmp = *this;
        return tmp -= n;
    }

    typename ref_iterator::difference_type operator-(const ref_iterator& rhs) const {
        return distance_ - rhs.distance_;
    }

    bool operator<(const ref_iterator& rhs) const { return (rhs - *this) > 0; }

    bool operator>(const ref_iterator& rhs) const { return rhs < *this; }

    bool operator>=(const ref_iterator& rhs) const { return !(*this < rhs); }

    bool operator<=(const ref_iterator& rhs) const { return !(*this > rhs); }

    ref_iterator begin() const { return container_; }

    ref_iterator cbegin() const { return begin(); }

    ref_iterator end() const {
        ref_iterator it = ref_iterator{container_};
        it.it_ = std::end(it.container_);
        it.distance_ = it.size();
        return it;
    }

    ref_iterator cend() const { return end(); }

    bool operator==(const ref_iterator& other) const {
        return (size() == other.size() && distance_ == other.distance_);
    }

    bool operator!=(const ref_iterator& other) const { return !(*this == other); }

    size_t size() const { return container_.size(); }

    bool empty() const { return container_.empty(); }

    typename std::enable_if<!std::is_const<ref_t>::value, remove_const_t<ref_t>>::type operator*() {
        return const_cast<remove_const_t<ref_t>>(
            static_cast<const ref_iterator*>(this)->operator*());
    }

    template <typename V = DT_VAL>
    typename std::enable_if<std::is_pointer<V>::value, add_const_t<ref_t>>::type operator*() const {
        assert(*it_ && "integrity error: nullptr");
        return const_cast<add_const_t<ref_t>>(static_cast<ref_t>(**it_));
    }

    template <typename V = DT_VAL>
    typename std::enable_if<!std::is_pointer<V>::value, add_const_t<ref_t>>::type
    operator*() const {
        return const_cast<add_const_t<ref_t>>(*(it_));
    }

    typename std::enable_if<!std::is_const<pointer_t>::value, pointer_t>::type operator->() {
        return const_cast<remove_const_t<pointer_t>>(
            static_cast<const ref_iterator*>(this)->operator->());
    }

    add_const_t<pointer_t> operator->() const {
        return const_cast<add_const_t<pointer_t>>(&(operator*()));
    }

  protected:
    T container_;
    ITERATOR_T it_;
    typename ref_iterator::difference_type distance_;
};

//! Iterator which return const ref on container's values
template <class T, typename U = typename decay_t<T>::value_type,
          class CT = typename std::add_const<T>::type>
using const_ref_iterator = ref_iterator<CT, U, typename decay_t<CT>::const_iterator>;

}  // namespace LIEF
#endif
