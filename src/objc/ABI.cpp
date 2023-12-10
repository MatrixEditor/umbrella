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
#include <LIEF/Abstract.hpp>
#include <LIEF/MachO.hpp>
#include <LIEF/BinaryStream/BinaryStream.hpp>
#include <LIEF/BinaryStream/SpanStream.hpp>

#include "objc/Parsing.h"  // private include
#include "MachOStream.h"

#include "umbrella/objc/ABI.h"
#include "umbrella/objc/Category.h"
#include "umbrella/objc/Class.h"
#include "umbrella/objc/Protocol.h"
#include "umbrella/visibility.h"

namespace umbrella {
namespace objc {

uintptr_t ABIObjectiveC::fixPointer(uintptr_t ptr) const {
  uintptr_t patched = ptr & ((1LLU << 51) - 1);
  if (imagebase() > 0 && patched < imagebase()) {
    patched += imagebase();
  }
  return patched;
}

const LIEF::Section* __objc_section(const ABIObjectiveC::TargetBinary& _Binary,
                                    const std::string& name) {
  for (auto& section : _Binary.sections()) {
    if (section.name() == name) {
      return &section;
    }
  }
  return nullptr;
}

inline const LIEF::Section* __objc_classlist(const ABIObjectiveC::TargetBinary& _Binary) {
  return __objc_section(_Binary, "__objc_classlist");
}

inline const LIEF::Section* __objc_catlist(const ABIObjectiveC::TargetBinary& _Binary) {
  return __objc_section(_Binary, "__objc_catlist");
}

inline const LIEF::Section* __objc_protolist(const ABIObjectiveC::TargetBinary& _Binary) {
  return __objc_section(_Binary, "__objc_protolist");
}

std::unique_ptr<ABIObjectiveC> ABIObjectiveC::parse(const TargetBinary& _Binary,
                                                    std::shared_ptr<TargetBinaryStream> _Stream) {
  auto abi = std::make_unique<ABIObjectiveC>(&_Binary, _Stream);

#define SLIST(sectionName, type, attr, attrLookup, key)                                            \
  if (const LIEF::Section* section = __objc_section(abi->binary(), sectionName)) {                 \
    LIEF::SpanStream list(section->content());                                                     \
    const size_t numPtrs = list.size() / sizeof(uintptr_t);                                        \
    for (size_t i = 0; i < numPtrs; i++) {                                                         \
      uintptr_t location = 0;                                                                      \
      if (auto ptr = list.read<uintptr_t>()) {                                                     \
        location = abi->fixPointer(*ptr);                                                          \
      } else {                                                                                     \
        break;                                                                                     \
      }                                                                                            \
      if (location) {                                                                              \
        LIEF::ScopedStream scoped(abi->stream(), location);                                        \
        if (std::shared_ptr<type> cls = type::parse(*abi)) {                                       \
          attrLookup[cls->key()] = cls.get();                                                      \
          attr.push_back(std::move(cls));                                                          \
        }                                                                                          \
      }                                                                                            \
    }                                                                                              \
  }

  SLIST("__objc_classlist", umbrella::objc::Class, abi->classes, abi->classLookup, getName)
  SLIST("__objc_catlist", umbrella::objc::Category, abi->categories, abi->categoryLookup, getName)
  SLIST("__objc_protolist", umbrella::objc::Protocol, abi->protocols, abi->protocolLookup, getName)

  return abi;
}



std::unique_ptr<objc::ABIObjectiveC> parseObjC(const std::string& fileName) {
    auto fatBinary = LIEF::MachO::Parser::parse(fileName);
    if (!fatBinary || fatBinary->empty()) {
        return nullptr;
    }

    if (auto arm64 = fatBinary->take(LIEF::MachO::CPU_TYPES::CPU_TYPE_ARM64)) {
        std::shared_ptr<MachOStream> stream = std::make_shared<MachOStream>(*arm64);
        return objc::ABIObjectiveC::parse(*arm64, stream);
    }

    if (auto x86_64 = fatBinary->take(LIEF::MachO::CPU_TYPES::CPU_TYPE_X86_64)) {
        std::shared_ptr<MachOStream> stream = std::make_shared<MachOStream>(*x86_64);
        return objc::ABIObjectiveC::parse(*x86_64, stream);
    }

    return nullptr;
}

} // namespace objc
} // namespace umbrella
