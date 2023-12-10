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
#if !defined(__UMBRELLA_OBJC_CATEGORY_H__)
#define __UMBRELLA_OBJC_CATEGORY_H__

#include <memory>
#include <string>
#include <vector>

#include "umbrella/visibility.h"

#include "umbrella/ObjC/Class.h"
#include "umbrella/ObjC/Method.h"
#include "umbrella/ObjC/Property.h"
#include "umbrella/ObjC/Protocol.h"
#include "umbrella/ObjC/Types.h"
#include "umbrella/iterators.h"
#include "umbrella/runtime.h"

namespace umbrella {
namespace objc {

class ABIObjectiveC;

/**
 * @brief Class representing an Objective-C category.
 */
class Category final : public InProcess {
public:
  using MethodList = std::vector<std::shared_ptr<Method>>;
  using PropertyList = std::vector<std::shared_ptr<Property>>;
  using ProtocolList = std::vector<std::shared_ptr<Protocol>>;

  using it_methods = LIEF::const_ref_iterator<const MethodList&, Method*>;
  using it_properties = LIEF::const_ref_iterator<const PropertyList&, Property*>;
  using it_protocols = LIEF::const_ref_iterator<const ProtocolList&, Protocol*>;

private:
  std::string name;                 /**< The name of the category. */
  std::shared_ptr<Class> baseClass; /**< Pointer to the base class associated with the category. */
  MethodList instanceMethods;       /**< List of instance methods defined in the category. */
  MethodList classMethods;          /**< List of class methods defined in the category. */
  PropertyList instanceProperties;  /**< List of properties defined in the category. */
  ProtocolList baseProtocols;       /**< List of protocols conformed to by the category. */

public:
  /**
   * @brief Static function to parse an Objective-C category.
   *
   * @param abi Reference to the ABIObjectiveC object.
   * @return std::shared_ptr<Category> A shared pointer to the parsed Category object.
   */
  static std::shared_ptr<Category> parse(ABIObjectiveC& abi);

  /**
   * @brief Default constructor for the Category class.
   * Initializes the address to 0.
   */
  Category() : InProcess(0) {}

  /**
   * @brief Get an iterator to the instance methods defined in the category.
   *
   * @return it_methods An iterator to the instance methods.
   */
  inline it_methods getInstanceMethods() const { return instanceMethods; }

  /**
   * @brief Get an iterator to the class methods defined in the category.
   *
   * @return it_methods An iterator to the class methods.
   */
  inline it_methods getClassMethods() const { return classMethods; }

  /**
   * @brief Get an iterator to the instance properties defined in the category.
   *
   * @return it_properties An iterator to the instance properties.
   */
  inline it_properties getInstanceProperties() const { return instanceProperties; }

  /**
   * @brief Get an iterator to the protocols conformed to by the category.
   *
   * @return it_protocols An iterator to the base protocols.
   */
  inline it_protocols getBaseProtocols() const { return baseProtocols; }

  /**
   * @brief Get a pointer to the base class associated with the category.
   *
   * @return const Class* A pointer to the base class. Returns nullptr if it's
   *         an extension.
   */
  inline const Class* getBaseClass() const { return baseClass.get(); }

  /**
   * @brief Check if the category is an extension (i.e., it does not have a base class).
   *
   * @return bool True if it's an extension; otherwise, false.
   */
  inline bool isExtension() const { return getBaseClass() == nullptr; }

  /**
   * @brief Get the name of the category.
   *
   * @return const std::string The name of the category.
   */
  inline const std::string getName() const { return name; }

  /**
   * @brief Get the declaration of this category.
   *
   * @return std::string An Objective-C source code snippet representing the
   *         declaration of the category.
   */
  std::string getDeclaration() const;
};

} // namespace objc
} // namespace umbrella

#endif  // __UMBRELLA_OBJC_CATEGORY_H__
