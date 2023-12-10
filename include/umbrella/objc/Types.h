/**
 * Copyright 2023 MatrixEditor
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(_UMBRELLA_OBJC_TYPES_H__)
#define _UMBRELLA_OBJC_TYPES_H__

#include <inttypes.h>
#include <type_traits>

#include "umbrella/visibility.h"

namespace umbrella {
namespace objc {

/**
 * @brief Base class for all Objective-C structs that store an isa pointer.
 */
struct object_t {
  uintptr_t isa; /**< Class storage or pointer. */
};

/**
 * @brief Template struct for generic lists in Objective-C.
 *
 * @tparam _Mask A bitmask for flags.
 */
template <uint32_t _Mask>
struct generic_list_t {
  static constexpr uint32_t FLAG_MASK = _Mask;
  uint32_t entsize_and_flags; /**< High bits used for fixup markers and flags. */
  uint32_t count;             /**< Number of elements in the list. */

  /**
   * @brief Get the element size.
   * @return uint32_t The element size.
   */
  inline uint32_t entsize() const { return entsize_and_flags & ~FLAG_MASK; }

  /**
   * @brief Get the flags.
   * @return uint32_t The flags.
   */
  inline uint32_t flags() const { return entsize_and_flags & FLAG_MASK; }
};

/**
 * @brief Struct for a method list in Objective-C.
 */
struct method_list_t : public generic_list_t<0xFFFF0003> {
  static constexpr uint32_t IS_SMALL = 0x80000000;
};

/**
 * @brief Alias for an ivar list in Objective-C.
 */
using ivar_list_t = generic_list_t<0>;

/**
 * @brief Alias for a property list in Objective-C.
 */
using property_list_t = generic_list_t<0>;

/**
 * @brief Struct for a protocol list in Objective-C.
 */
struct protocol_list_t {
  uintptr_t count; /**< The number of protocols in the list (pointer-sized). */
};

/**
 * @brief Struct for a protocol in Objective-C.
 */
struct protocol_t : public object_t {
  uintptr_t name;                      /**< Pointer to the protocol's name. */
  uintptr_t protocols;                 /**< Pointer to protocol_list_t. */
  uintptr_t required_instance_methods; /**< Pointer to required instance methods list. */
  uintptr_t required_class_methods;    /**< Pointer to required class methods list. */
  uintptr_t optional_instance_methods; /**< Pointer to optional instance methods list. */
  uintptr_t optional_class_methods;    /**< Pointer to optional class methods list. */
  uintptr_t instance_properties;       /**< Pointer to instance properties list. */
  uint32_t size;                       /**< Size of the protocol struct. */
  uint32_t flags;                      /**< Flags associated with the protocol. */
};

/**
 * @brief Struct for a property in Objective-C.
 */
struct property_t {
  uintptr_t name;       /**< Pointer to the property's name. */
  uintptr_t attributes; /**< Pointer to the property's attributes. */
};

/**
 * @brief Template struct for a method in Objective-C.
 *
 * @tparam _Ptr The type of pointers used in the method.
 */
template <typename _Ptr>
struct method_t {
  _Ptr name;      /**< Pointer to the method's name. */
  _Ptr signature; /**< Pointer to the method's signature. */
  _Ptr impl;      /**< Pointer to the method's implementation. */
};

/**
 * @brief Alias for a method using uintptr_t pointers in Objective-C.
 */
using big_method_t = method_t<uintptr_t>;

/**
 * @brief Alias for a method using int32_t pointers in Objective-C.
 */
using small_method_t = method_t<int32_t>;

/**
 * @brief Struct for an instance variable (ivar) in Objective-C.
 */
struct ivar_t {
  uintptr_t offset;    /**< Offset of the ivar within the instance. */
  uintptr_t name;      /**< Pointer to the ivar's name. */
  uintptr_t type;      /**< Pointer to the ivar's type. */
  uintptr_t alignment; /**< Alignment of the ivar. */
  uintptr_t size;      /**< Size of the ivar. */
};

/**
 * @brief Struct for class read-only data (class_ro_t) in Objective-C.
 */
struct class_ro_t {
  uint32_t flags;             /**< Flags associated with the class. */
  uint32_t instance_start;    /**< Offset of the start of instance variables. */
  uint32_t instance_end;      /**< Offset of the end of instance variables. */
  uint32_t reserved;          /**< Reserved for 64-bit binaries. */
  uintptr_t ivar_layout;      /**< Pointer to ivar layout information. */
  uintptr_t name;             /**< Pointer to the class's name. */
  uintptr_t base_methods;     /**< Pointer to the base methods list. */
  uintptr_t base_protocols;   /**< Pointer to the base protocols list. */
  uintptr_t ivars;            /**< Pointer to the instance variables list. */
  uintptr_t weak_ivar_layout; /**< Pointer to weak ivar layout information. */
  uintptr_t base_properties;  /**< Pointer to the base properties list. */
};

/**
 * @brief Struct for class data bits (class_data_bits_t) in Objective-C.
 */
struct class_data_bits_t {
private:
  uintptr_t bits;

public:
  static constexpr auto MASK = 0x00007FFFFFFFFFF8UL;

  /**
   * @brief Get the class_ro pointer from the bits.
   * @return uintptr_t The class_ro pointer.
   */
  uintptr_t class_ro() const { return bits & MASK; };
};

/**
 * @brief Struct for a class (class_t) in Objective-C.
 */
struct class_t : public object_t {
  uintptr_t super_class;  /**< Pointer to the superclass. */
  uintptr_t cache;        /**< Pointer to method cache. */
  uintptr_t vtable;       /**< Pointer to the vtable. */
  class_data_bits_t bits; /**< Class data bits. */
};

/**
 * @brief Struct for a category in Objective-C.
 */
struct category_t {
  uintptr_t name;                /**< Pointer to the category's name. */
  uintptr_t base_class;          /**< Pointer to the base class. */
  uintptr_t instance_methods;    /**< Pointer to instance methods list. */
  uintptr_t class_methods;       /**< Pointer to class methods list. */
  uintptr_t base_protocols;      /**< Pointer to base protocols list. */
  uintptr_t instance_properties; /**< Pointer to instance properties list. */
};

} // namespace objc
} // namespace umbrella

#endif  // _UMBRELLA_OBJC_TYPES_H__
