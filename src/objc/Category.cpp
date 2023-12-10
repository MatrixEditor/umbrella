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
#include <sstream>

#include <LIEF/BinaryStream/BinaryStream.hpp>

#include "umbrella/objc/ABI.h"
#include "umbrella/objc/Category.h"
#include "umbrella/objc/Class.h"
#include "umbrella/objc/Method.h"
#include "umbrella/objc/Property.h"
#include "umbrella/objc/Protocol.h"
#include "umbrella/visibility.h"

#include "objc/Declaration.h"  // private include
#include "objc/Parsing.h"      // private include

namespace umbrella {
namespace objc {

std::shared_ptr<Category> Category::parse(ABIObjectiveC& abi) {
    LIEF::BinaryStream& stream = abi.stream();
    PEEK(raw, umbrella::objc::category_t, stream)

    std::shared_ptr<Category> category = std::make_shared<Category>();
    category->setAddress(stream.pos());
    STRING_FIXED(category->name, raw->name)
    METHODS(raw->class_methods, category->classMethods, true)
    METHODS(raw->instance_methods, category->instanceMethods, false)
    PROTOCOLS(raw->base_protocols, category->baseProtocols)
    CLASS_FIXED(raw->base_class, category->baseClass)
    PROPERTIES(raw->instance_properties, category->instanceProperties)
    return category;
}

std::string Category::getDeclaration() const {
    // Categories and extensions will be dumped using this method as
    // we doesn't differ between categories and extensions. Methods
    // will be devided into class and instance methods.
    std::ostringstream stream;
    stream << "@interface " << name << " ";
    if (isExtension()) {
        stream << "() ";
    } else {
        // base name only if this category is a 'real' category
        stream << "(" << baseClass->getName() << ") ";
    }

    if (baseProtocols.size()) {
        PROTO_CONFORM_DECL(baseProtocols, "(", ")")
    }

    stream << "\n";
    LIST_DECL(instanceProperties, "\n")
    LIST_DECL(instanceMethods, "\n")
    LIST_DECL(classMethods, "\n")

    stream << "@end";
    return stream.str();
}

} // namespace objc
} // namespace umbrella
