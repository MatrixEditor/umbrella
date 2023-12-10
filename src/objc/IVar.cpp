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
#include <cctype>

#include <LIEF/BinaryStream/BinaryStream.hpp>

#include "umbrella/objc/ABI.h"
#include "umbrella/objc/IVar.h"
#include "umbrella/visibility.h"

#include "objc/Parsing.h"

namespace umbrella {
namespace objc {

std::shared_ptr<IVar> IVar::parse(ABIObjectiveC& abi) {
  LIEF::BinaryStream& stream = abi.stream();
  PEEK(raw, umbrella::objc::ivar_t, stream)

  std::shared_ptr<IVar> ivar = std::make_shared<IVar>();
  ivar->setAddress(stream.pos());
  ivar->alignment = raw->alignment;
  ivar->size = raw->size;

  STRING_FIXED(ivar->name, raw->name);
  STRING_FIXED(ivar->typeName, raw->type);

  // Somtimes these names must be swapped???
  if (!ivar->typeName.empty() && !ivar->name.empty()) {
    if (ivar->typeName[0] == '_' || ivar->name[0] == 'T' || ivar->name.size() <= 2) {
      std::swap(ivar->name, ivar->typeName);
    }
  }
  return ivar;
}

std::string IVar::getDeclaration() const {
  // As Ivars are not that complicated, the structure of a dumped Ivar
  // is rather simple:
  //   - <ivar> := <type> <name>';'
  std::ostringstream stream;
  if (name.empty() || typeName.empty()) {
    // we are dealing with a remapped type (address is relocated - NOT IMPLEMENTED)
    stream << "// 0x" << std::hex << getAddress() << " <remapped>";
  }

  else {
    try {
      std::shared_ptr<TypeNode> typeDesc = objc::typedesc(typeName);
      if (!typeDesc) {
        stream << "// 0x" << std::hex << getAddress() << " <invalid type> '" << typeName << "'";
      } else {
        stream << objc::decode(*typeDesc);
        // NOTE: In some cases, especially when dumping basic ivars with a
        // protocol, the name may be hardcoded into the property's attributes.
        // Therefore, we have to check whether the name is already present in
        // the encoded type description.
        auto result = typeName.find(name);
        if (result == std::string::npos) {
            if (std::isprint(name[0])) {
              stream << " " << name;
            } else {
              stream << " _$remapped_name";
            }
        }
        stream << ";";
      }
    } catch (const std::out_of_range& err) {
       // ignore and print remapped
       stream << "// 0x" << std::hex << getAddress() << " <remapped, invalid type>";
    }
  }

  return stream.str();
}

} // namespace objc
} // namespace umbrella
