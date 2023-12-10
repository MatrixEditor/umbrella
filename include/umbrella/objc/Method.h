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
#if !defined(__UMBRELLA_OBJC_METHOD_H__)
#define __UMBRELLA_OBJC_METHOD_H__

#include <memory>
#include <string>

#include "umbrella/ObjC/TypeEncoding.h"
#include "umbrella/ObjC/Types.h"
#include "umbrella/runtime.h"
#include "umbrella/visibility.h"

namespace umbrella {
namespace objc {

class ABIObjectiveC;

/**
 * @brief Class representing an Objective-C method.
 */
class Method final : public InProcess {
private:
  /// parsed raw implementation
  union {
    uintptr_t absImpl; /**< The absolute implementation address. */
    int32_t relImpl;   /**< The relative implementation address for small methods. */
  };

  std::string name;      /**< The name of the method (selector string). */
  std::string signature; /**< The encoded signature of the method. */
  bool classMethod;      /**< Indicates whether this method is a class method. */
  bool relativeMethod;   /**< Indicates whether the raw method is a small method. */

public:
  /**
   * @brief Static function to parse an Objective-C method.
   *
   * @param abi Reference to the ABIObjectiveC object.
   * @param isSmall Flag indicating whether the method is a small method.
   * @param isClassMethod Flag indicating whether the method is a class method.
   * @return std::shared_ptr<Method> A shared pointer to the parsed Method object.
   */
  static std::shared_ptr<Method> parse(ABIObjectiveC& abi, bool isSmall, bool isClassMethod);

public:
  /**
   * @brief Get the name of the method.
   *
   * @return const std::string& The name of the method (selector string).
   */
  const std::string& getName() const { return name; }

  /**
   * @brief Get the encoded signature of the method.
   *
   * @return const std::string& The encoded signature of the method.
   */
  const std::string& getSignature() const { return signature; }

  /**
   * @brief Get the absolute implementation address of the method.
   *
   * @return uintptr_t The absolute implementation address.
   */
  uintptr_t getImplementation() const { return absImpl; }

  /**
   * @brief Get the relative implementation address of the method for small methods.
   *
   * @return int32_t The relative implementation address for small methods.
   */
  int32_t getRelativeImplementation() const { return relImpl; }

  /**
   * @brief Check if the method is a small method.
   *
   * @return bool True if the method is a small method; otherwise, false.
   */
  bool isSmallMethod() const { return relativeMethod; }

  /**
   * @brief Check if the method is a class method.
   *
   * @return bool True if the method is a class method; otherwise, false.
   */
  bool isClassMethod() const { return classMethod; }

  /**
   * @brief Get the type description of the method's signature.
   *
   * @return std::shared_ptr<TypeNode> A shared pointer to the type description.
   */
  inline std::shared_ptr<TypeNode> getTypeDesc() const { return typedesc(getSignature()); }

  /**
   * @brief Decode the signature of this method to get the human-readable signature.
   *
   * @return std::string The decoded method signature.
   */
  inline std::string decodeSignature() const {
    return umbrella::objc::signature(getName(), getSignature());
  }

  /**
   * @brief Get the declaration of this method.
   *
   * @return std::string An Objective-C source code snippet representing the
   *         declaration of the method.
   */
  std::string getDeclaration() const;

  /**
   * @brief Default constructor for the Method class.
   * Initializes the address to 0.
   */
  Method() : InProcess(0) {}
};

} // namespace objc
} // namespace umbrella

#endif  // __UMBRELLA_OBJC_METHOD_H__
