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
#include "umbrella/objc/Class.h"
#include "umbrella/objc/IVar.h"
#include "umbrella/objc/Method.h"
#include "umbrella/objc/Property.h"
#include "umbrella/objc/Protocol.h"
#include "umbrella/visibility.h"

#include "objc/Declaration.h"  // private include
#include "objc/Parsing.h"      // private include

namespace umbrella {
namespace objc {

std::shared_ptr<Class> Class::parse(ABIObjectiveC& abi) {
  LIEF::BinaryStream& stream = abi.stream();
  PEEK(raw, umbrella::objc::class_t, stream)

  std::shared_ptr<Class> cls = std::make_shared<Class>();
  cls->setAddress(stream.pos());
  CLASS_FIXED(raw->super_class, cls->superClass)
  CLASS_FIXED(raw->isa, cls->metaClass)

  const uintptr_t address = raw->bits.class_ro();
  if (!address) {
    return nullptr;
  }

  stream.setpos(address);
  PEEK(raw_data, umbrella::objc::class_ro_t, stream);
  STRING_FIXED(cls->name, raw_data->name)

  PROTOCOLS(raw_data->base_protocols, cls->protocols)
  PROPERTIES(raw_data->base_properties, cls->properties)
  METHODS(raw_data->base_methods, cls->methods, false)

  cls->flags = raw_data->flags;
  if (raw_data->ivars) {
    // REVISIT: maybe put this into a macro
    LIST(raw_data->ivars, umbrella::objc::ivar_list_t) {
      const size_t size = sizeof(ivar_t);
      const size_t baseAddress = stream.pos();

      for (size_t i = 0; i < list->count; i++) {
        stream.setpos(baseAddress + i * size);
        if (auto ivar = IVar::parse(abi)) {
          cls->ivars.push_back(std::move(ivar));
        }
      }
    }
  }

  return cls;
}

std::string Class::getDeclaration() const {
  // Dumping a class is somewhat more comlicated, but can be broken
  // down into pieces. As classes may contain a metaclass (a class
  // that stores all 'static'/class methods, properties and ivars),
  // these attributes have to be dumped as well.
  std::ostringstream stream;
  std::string superClassName = "NSObject";

  if (hasSuperClass()) {
    superClassName = superClass->getName();
  }

  stream << "@interface " << name << ": " << superClassName << " ";
  if (protocols.size()) {
    // REVISIT: these names have to be demangled
    PROTO_CONFORM_DECL(protocols, "<", ">")
  }

#define META_LIST_DECL(var_name)                                                                   \
  if (hasMetaClass() && var_name.size()) {                                                         \
    LIST_DECL(var_name, "\n")                                                                      \
  }

  const bool hasIvars = ivars.size() || (hasMetaClass() && metaClass->ivars.size());
  if (hasIvars) {
    // ivars will be placed into a block
    stream << "\n{";
  }

  stream << "\n";
  LIST_DECL(ivars, "\n")
  META_LIST_DECL(metaClass->ivars)
  if (hasIvars) {
    stream << "}\n";
  }

  // Properties
  LIST_DECL(properties, "\n")
  META_LIST_DECL(metaClass->properties)

  // Methods
  LIST_DECL(methods, "\n")
  META_LIST_DECL(metaClass->methods)

  stream << "@end";
  return stream.str();
}

} // namespace objc
} // namespace umbrella
