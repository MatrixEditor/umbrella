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
#if !defined(__UMBRELLA_OBJC_PROPERTY_H__)
#define __UMBRELLA_OBJC_PROPERTY_H__

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
 * @brief Class representing an Objective-C property.
 */
class Property final : public InProcess {
private:
  std::string name;       /**< The name of the property. */
  std::string attributes; /**< The attributes of the property. */

public:
  /**
   * @brief Static function to parse an Objective-C property.
   *
   * @param abi Reference to the ABIObjectiveC object.
   * @return std::shared_ptr<Property> A shared pointer to the parsed Property object.
   */
  static std::shared_ptr<Property> parse(ABIObjectiveC& abi);

public:
  /**
   * @brief Default constructor for the Property class.
   * Initializes the address to 0.
   */
  Property() : InProcess(0) {}

  /**
   * @brief Get the name of the property.
   *
   * @return const std::string& The name of the property.
   */
  inline const std::string& getName() const { return name; }

  /**
   * @brief Get the attributes of the property.
   *
   * @return const std::string& The attributes of the property.
   */
  inline const std::string& getAttributes() const { return attributes; }

  /**
   * @brief Decode the attributes of this property into a fully representative
   *        string.
   *
   * @return std::string The decoded attributes as a string.
   */
  inline std::string decodeAttributes() const {
    const TypeNode& node = *umbrella::objc::typedesc(getAttributes());
    return umbrella::objc::decode(node);
  }

  /**
   * @brief Get the declaration of this property.
   *
   * @return std::string An Objective-C source code snippet representing the
   *         declaration of the property.
   */
  std::string getDeclaration() const;
};

} // namespace objc
} // namespace umbrella

#endif  // __UMBRELLA_OBJC_PROPERTY_H__
