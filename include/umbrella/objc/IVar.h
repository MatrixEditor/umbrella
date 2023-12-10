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
#if !defined(__UMBRELLA_IVAR_H__)
#define __UMBRELLA_IVAR_H__

#include <iostream>
#include <memory>
#include <string>

#include "umbrella/visibility.h"

#include "umbrella/ObjC/TypeEncoding.h"
#include "umbrella/ObjC/Types.h"
#include "umbrella/runtime.h"

namespace umbrella {
namespace objc {

class ABIObjectiveC;

/**
 * @brief Class representing an instance variable (IVar) in Objective-C.
 */
class IVar final : public InProcess {
public:
  friend class ABIObjectiveC; /**< Allowing ABIObjectiveC class to access private members. */

private:
  std::string name;     /**< The name of the instance variable. */
  std::string typeName; /**< The mangled type name of the instance variable. */

  uintptr_t alignment;  /**< The alignment of the instance variable. */
  uintptr_t size;       /**< The size of the instance variable. */

public:
  /**
   * @brief Static function to parse an instance variable from ABIObjectiveC.
   *
   * @param abi Reference to the ABIObjectiveC object.
   * @return std::shared_ptr<IVar> A shared pointer to the parsed IVar object.
   */
  static std::shared_ptr<IVar> parse(ABIObjectiveC& abi);

public:
  /**
   * @brief Default constructor for the IVar class.
   * Initializes the address to 0.
   */
  IVar() : InProcess(0) {}

  /**
   * @brief Get the name of the instance variable.
   *
   * @return const std::string& The name of the instance variable.
   */
  const std::string& getName() const { return name; }

  /**
   * @brief Get the mangled type name of the instance variable.
   *
   * @return const std::string& The mangled type name.
   */
  const std::string& getMangledTypeName() const { return typeName; }

  /**
   * @brief Get the alignment requirement of the instance variable.
   *
   * @return uintptr_t The alignment requirement.
   */
  uintptr_t getAlignment() const { return alignment; }

  /**
   * @brief Get the size of the instance variable.
   *
   * @return uintptr_t The size of the instance variable.
   */
  uintptr_t getSize() const { return size; }

  /**
   * @brief Decode the mangled type name of this IVar to get the human-readable type name.
   *
   * @return std::string The decoded type name.
   */
  std::string getTypeName() const {
    const TypeNode& node = *umbrella::objc::typedesc(getMangledTypeName());
    return umbrella::objc::decode(node);
  }

  /**
   * @brief Get the declaration of this IVar.
   *
   * @return std::string An Objective-C source code snippet representing the declaration.
   */
  std::string getDeclaration() const;
};

} // namespace objc
} // namespace umbrella

#endif  // __UMBRELLA_IVAR_H__
