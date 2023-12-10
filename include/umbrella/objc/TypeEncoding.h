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
#if !defined(__UMBRELLA_OBJC_TYPE_ENCODING_H__)
#define __UMBRELLA_OBJC_TYPE_ENCODING_H__

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "umbrella/iterators.h"
#include "umbrella/visibility.h"

namespace umbrella {
namespace objc {

/**
 * @brief Enum representing Objective-C types.
 */
enum class Type {
  PVOID = 1, /**< Special case: void * */
#define OBJC_TYPE(id, name, alignment, value_) value_,
#include "umbrella/objc/TypeEncoding.def"
#undef OBJC_TYPE
  OBJECT,
  ARRAY,
  STRUCT,
  UNION,
  BIT_FIELD,
  POINTER,
  ATTRIBUTES,
  BLOCK,
};

/**
 * @brief Enum representing Objective-C attribute types.
 */
enum class AttributeType {
  GETTER = 1,
  SETTER,
  READONLY_,
  COPY,
  RETAIN,
  NONATOMIC,
  DYNAMIC,
  WEAK,
  GARBAGE,
};

/**
 * @brief Enum representing Objective-C method argument types.
 */
enum class MethodType {
  CONST,
  IN,
  INOUT,
  OUT,
  BYCOPY,
  BYREF,
  ONEWAY,
  ATOMIC,
  COMPLEX,
};

/**
 * @brief Struct to store mangled type information.
 */
struct TypeNode {
  using Children = std::vector<std::shared_ptr<TypeNode>>;
  using it_children = LIEF::const_ref_iterator<const Children&, TypeNode*>;

  uint32_t type{0};                          /**< The internal Objective-C type. */
  uint32_t size{0};                          /**< The size in bytes. */
  uint32_t alignment{0};                     /**< Alignment in bytes. */
  uint32_t dim{0};                           /**< Array dimensions. */
  uint32_t stack_size{0};                    /**< Stack size. */
  std::vector<std::string> attributes;       /**< Attributes used for properties. */
  std::string name;                          /**< The node's name. */
  std::shared_ptr<TypeNode> parent{nullptr}; /**< The parent node. */
  Children children;                         /**< All children stored in a separate list. */

  const TypeNode& operator[](size_t _Index) const {
    // assert _Index
    return *children[_Index];
  }

  const Type getType() const { return (Type)type; }
  const AttributeType getAttributeType() const { return (AttributeType)type; }
  it_children getChildren() const { return children; }
};

/**
 * @brief Map of character representations to Type enum values.
 */
static const std::unordered_map<char, Type> OBJC_NODE_TYPES{
#define OBJC_TYPE(id, name, alignment, value) {id, Type::value},
#include "umbrella/objc/TypeEncoding.def"
#undef OBJC_TYPE
};

/**
 * @brief Map of Type enum values to type names.
 */
static const std::unordered_map<uint32_t, std::string> OBJC_NODE_TYPE_NAMES{
#define OBJC_TYPE(id, name, alignment, value) {(uint32_t)Type::value, name},
#include "umbrella/objc/TypeEncoding.def"
#undef OBJC_TYPE
};

/**
 * @brief Map of character representations to MethodType enum values.
 */
static const std::unordered_map<char, MethodType> OBJC_METHOD_TYPES{
#define METHOD_TYPE(id, name, value) {id, MethodType::value},
#include "umbrella/objc/TypeEncoding.def"
#undef METHOD_TYPE
};

/**
 * @brief Creates a type description based on the given signature.
 *
 * This function works with plain type encodings as well as method and property
 * encodings. It is recommended to NOT use any other functions to create a type
 * description besides this one.
 *
 * @param _Encoded the raw type encoding.
 * @return std::shared_ptr<TypeNode> A shared pointer to the generated TypeNode.
 */
std::shared_ptr<TypeNode> typedesc(const std::string& _Encoded);

/**
 * @brief Decodes a type description.
 *
 * @param _Node the type description node.
 * @return std::string The decoded type (or property string).
 */
std::string decode(const TypeNode& _Node);

/**
 * @brief Generates a fully qualified method signature.
 *
 * @param _Selector The method's selector string.
 * @param _Signature The encoded signature.
 * @return std::string The qualified signature as a string.
 */
std::string signature(const std::string& _Selector, const std::string& _Signature);

// TODO:
// void dumpTree(const TypeNode& _Node);

} // namespace objc
} // namespace umbrella

#endif  // __UMBRELLA_OBJC_TYPE_ENCODING_H__
