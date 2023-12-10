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
#if !defined(__UMBRELLA_OBJC_PROTOCOL_H__)
#define __UMBRELLA_OBJC_PROTOCOL_H__

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "umbrella/visibility.h"

#include "umbrella/ObjC/Method.h"
#include "umbrella/ObjC/Property.h"
#include "umbrella/iterators.h"

namespace umbrella {
namespace objc {

class ABIObjectiveC;

/**
 * @brief Class representing an Objective-C protocol.
 */
class Protocol final : public InProcess {
public:
  using MethodList = std::vector<std::shared_ptr<objc::Method>>;
  using PropertyList = std::vector<std::shared_ptr<Property>>;
  using ProtocolList = std::vector<std::shared_ptr<Protocol>>;

  using it_methods = LIEF::const_ref_iterator<const MethodList&, Method*>;
  using it_properties = LIEF::const_ref_iterator<const PropertyList&, Property*>;
  using it_protocols = LIEF::const_ref_iterator<const ProtocolList&, Protocol*>;

private:
  std::string name; /**< The name of the protocol. */
  uint32_t flags;   /**< Flags associated with the protocol. */

  MethodList requiredInstanceMethods; /**< List of required instance methods for the protocol. */
  MethodList requiredClassMethods;    /**< List of required class methods for the protocol. */
  MethodList optionalInstanceMethods; /**< List of optional instance methods for the protocol. */
  MethodList optionalClassMethods;    /**< List of optional class methods for the protocol. */
  PropertyList instanceProperties;    /**< List of properties associated with the protocol. */
  ProtocolList protocols;             /**< List of protocols this protocol conforms to. */

public:
  /**
   * @brief Static function to parse an Objective-C protocol.
   *
   * @param abi Reference to the ABIObjectiveC object.
   * @return std::shared_ptr<Protocol> A shared pointer to the parsed Protocol object.
   */
  static std::shared_ptr<Protocol> parse(ABIObjectiveC& abi);

  /**
   * @brief Default constructor for the Protocol class.
   * Initializes the address to 0.
   */
  Protocol() : InProcess(0) {}

  /**
   * @brief Get the name of the protocol.
   *
   * @return const std::string& The name of the protocol.
   */
  inline const std::string& getName() const { return name; }

  /**
   * @brief Get an iterator to the required instance methods of the protocol.
   *
   * @return it_methods An iterator to the required instance methods.
   */
  inline it_methods getRequiredInstanceMethods() const { return requiredInstanceMethods; }

  /**
   * @brief Get an iterator to the optional instance methods of the protocol.
   *
   * @return it_methods An iterator to the optional instance methods.
   */
  inline it_methods getOptionalInstanceMethods() const { return optionalInstanceMethods; }

  /**
   * @brief Get an iterator to the required class methods of the protocol.
   *
   * @return it_methods An iterator to the required class methods.
   */
  inline it_methods getRequiredClassMethods() const { return requiredClassMethods; }

  /**
   * @brief Get an iterator to the optional class methods of the protocol.
   *
   * @return it_methods An iterator to the optional class methods.
   */
  inline it_methods getOptionalClassMethods() const { return optionalClassMethods; }

  /**
   * @brief Get an iterator to the instance properties of the protocol.
   *
   * @return it_properties An iterator to the instance properties.
   */
  inline it_properties getInstanceProperties() const { return instanceProperties; }

  /**
   * @brief Get an iterator to the protocols this protocol conforms to.
   *
   * @return it_protocols An iterator to the conforming protocols.
   */
  inline it_protocols getProtocols() const { return protocols; }

  /**
   * @brief Get the flags associated with the protocol.
   *
   * @return uint32_t The protocol flags.
   */
  inline uint32_t getFlags() const { return flags; }

  /**
   * @brief Get the declaration of this protocol.
   *
   * @return std::string An Objective-C source code snippet representing the
   *         declaration of the protocol.
   */
  std::string getDeclaration() const;

  // TODO: maybe support that:
  // friend std::ostream &operator<<(std::ostream &stream, Protocol &protocol) {
  //     stream << protocol.getDeclaration();
  //     return stream;
  // }
};

} // namespace objc
} // namespace umbrella

#endif  // __UMBRELLA_OBJC_PROTOCOL_H__
