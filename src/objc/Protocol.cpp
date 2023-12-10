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
#include <LIEF/BinaryStream/BinaryStream.hpp>

#include "umbrella/objc/ABI.h"
#include "umbrella/objc/Method.h"
#include "umbrella/objc/Property.h"
#include "umbrella/objc/Protocol.h"
#include "umbrella/visibility.h"

#include "objc/Declaration.h"  // private include
#include "objc/Parsing.h"      // private include

#include <sstream>

namespace umbrella {
namespace objc {

std::shared_ptr<Protocol> Protocol::parse(ABIObjectiveC& abi) {
    LIEF::BinaryStream& stream = abi.stream();
    PEEK(raw, umbrella::objc::protocol_t, stream)

    std::shared_ptr<Protocol> protocol = std::make_shared<Protocol>();
    protocol->setAddress(stream.pos());
    protocol->flags = raw->flags;
    STRING_FIXED(protocol->name, raw->name)

    PROTOCOLS(raw->protocols, protocol->protocols)
    METHODS(raw->required_class_methods, protocol->requiredClassMethods, true)
    METHODS(raw->optional_class_methods, protocol->optionalClassMethods, true)
    METHODS(raw->required_instance_methods, protocol->requiredInstanceMethods, false)
    METHODS(raw->optional_instance_methods, protocol->optionalInstanceMethods, false)
    PROPERTIES(raw->instance_properties, protocol->instanceProperties)
    return protocol;
}

std::string Protocol::getDeclaration() const {
    // Protocol dumps will store a detailed overview of stored methods
    // and properties. Especially methods will be devided into class and
    // instance methods, which then will be devided into optional and
    // required methods.
    std::ostringstream stream;
    stream << "@protocol " << name << " ";
    if (protocols.size() != 0) {
        // Add protocol conformance
        PROTO_CONFORM_DECL(protocols, "<", ">")
    }

    // TODO: parent
    stream << "\n";
    LIST_DECL(instanceProperties, "\n")

    if (optionalClassMethods.size() || optionalInstanceMethods.size()) {
        stream << "@optional\n";
    }
    LIST_DECL(optionalInstanceMethods, "\n")
    LIST_DECL(optionalClassMethods, "\n")

    if (requiredClassMethods.size() || requiredInstanceMethods.size()) {
        stream << "@required\n";
    }
    LIST_DECL(requiredInstanceMethods, "\n")
    LIST_DECL(requiredClassMethods, "\n")

    stream << "@end";
    return stream.str();
}

} // namespace objc
} // namespace objc
