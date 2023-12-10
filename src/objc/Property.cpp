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
#include "umbrella/objc/Property.h"
#include "umbrella/visibility.h"

#include "objc/Parsing.h"

namespace umbrella {
namespace objc {

std::shared_ptr<Property> Property::parse(ABIObjectiveC& abi) {
    LIEF::BinaryStream& stream = abi.stream();
    PEEK(raw, umbrella::objc::property_t, stream)

    std::shared_ptr<Property> property = std::make_shared<Property>();
    property->setAddress(stream.pos());
    STRING_FIXED(property->name, raw->name)
    STRING_FIXED(property->attributes, raw->attributes)
    return property;
}

std::string Property::getDeclaration() const {
    // Properties will be treated like Ivars. Note that we don't assume any
    // remapped type names.
    //   - <property> := '@property' [ <attributes> ] <type> <name>
    std::ostringstream stream;
    std::string attributes = std::move(decodeAttributes());

    stream << attributes;
    auto result = attributes.find(name);
    if (result == std::string::npos) {
        // NOTE: In some cases, especially when dumping root properties,
        // the name may be hardcoded into the property's attributes. Therefore,
        // we have to check whether the name is already present in the encoded
        // type description.
        stream << " " << name;
    }
    return stream.str();
}

} // namespace objc
} // namespace umbrella
