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
#if !defined(_UMBRELLA_OBJC_CLASS_H__)
#define _UMBRELLA_OBJC_CLASS_H__

#include <memory>
#include <string>
#include <vector>

#include "umbrella/visibility.h"

#include "umbrella/ObjC/IVar.h"
#include "umbrella/ObjC/Method.h"
#include "umbrella/ObjC/Property.h"
#include "umbrella/ObjC/Protocol.h"
#include "umbrella/iterators.h"

namespace umbrella {
namespace objc {

class ABIObjectiveC;

/**
 * @brief Class representing an Objective-C class.
 */
class Class final : public InProcess {
public:
  using MethodList = std::vector<std::shared_ptr<Method>>;
  using PropertyList = std::vector<std::shared_ptr<Property>>;
  using ProtocolList = std::vector<std::shared_ptr<Protocol>>;
  using IVarList = std::vector<std::shared_ptr<IVar>>;

  using it_methods = LIEF::const_ref_iterator<const MethodList&, Method*>;
  using it_properties = LIEF::const_ref_iterator<const PropertyList&, Property*>;
  using it_protocols = LIEF::const_ref_iterator<const ProtocolList&, Protocol*>;
  using it_ivars = LIEF::const_ref_iterator<const IVarList&, IVar*>;

private:
  std::string name; /**< The name of the class. */
  uint32_t flags;   /**< Flags associated with the class. */

  std::shared_ptr<Class> superClass; /**< Pointer to the superclass. */
  std::shared_ptr<Class> metaClass;  /**< Pointer to the metaclass. */
  MethodList methods;                /**< List of methods defined in the class. */
  IVarList ivars;                    /**< List of instance variables defined in the class. */
  ProtocolList protocols;            /**< List of protocols conformed to by the class. */
  PropertyList properties;           /**< List of properties defined in the class. */

public:
  /**
   * @brief Static function to parse an Objective-C class.
   *
   * @param abi Reference to the ABIObjectiveC object.
   * @return std::shared_ptr<Class> A shared pointer to the parsed Class object.
   */
  static std::shared_ptr<Class> parse(ABIObjectiveC& abi);

  /**
   * @brief Default constructor for the Class class.
   * Initializes the address to 0.
   */
  Class() : InProcess(0) {}

  /**
   * @brief Get the name of the class.
   *
   * @return const std::string& The name of the class.
   */
  inline const std::string& getName() const { return name; }

  /**
   * @brief Get the flags associated with the class.
   *
   * @return uint32_t The class flags.
   */
  inline uint32_t getFlags() const { return flags; }

  /**
   * @brief Get an iterator to the methods defined in the class.
   *
   * @return it_methods An iterator to the methods.
   */
  inline it_methods getMethods() const { return methods; }

  /**
   * @brief Get an iterator to the instance variables defined in the class.
   *
   * @return it_ivars An iterator to the instance variables.
   */
  inline it_ivars getIVars() const { return ivars; }

  /**
   * @brief Get an iterator to the protocols conformed to by the class.
   *
   * @return it_protocols An iterator to the conformed protocols.
   */
  inline it_protocols getProtocols() const { return protocols; }

  /**
   * @brief Get an iterator to the properties defined in the class.
   *
   * @return it_properties An iterator to the properties.
   */
  inline it_properties getProperties() const { return properties; }

  /**
   * @brief Check if the class has a superclass.
   *
   * @return bool True if the class has a superclass; otherwise, false.
   */
  inline bool hasSuperClass() const { return getSuperClass() != nullptr; }

  /**
   * @brief Check if the class has a metaclass.
   *
   * @return bool True if the class has a metaclass; otherwise, false.
   */
  inline bool hasMetaClass() const { return getMetaClass() != nullptr; }

  /**
   * @brief Get a pointer to the superclass.
   *
   * @return const Class* A pointer to the superclass. Returns nullptr if there
   *         is no superclass.
   */
  inline const Class* getSuperClass() const { return superClass.get(); }

  /**
   * @brief Get a pointer to the metaclass.
   *
   * @return const Class* A pointer to the metaclass. Returns nullptr if there
   *         is no metaclass.
   */
  inline const Class* getMetaClass() const { return metaClass.get(); }

  /**
   * @brief Get the declaration of this class.
   *
   * @return std::string An Objective-C source code snippet representing the
   *         declaration of the class.
   */
  std::string getDeclaration() const;
};

} // namespace objc
} // namespace umbrella

#endif  // _UMBRELLA_OBJC_CLASS_H__
