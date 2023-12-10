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
#include <sstream>

#include "umbrella/objc/ABI.h"
#include "umbrella/objc/Method.h"
#include "umbrella/visibility.h"

#include "objc/Parsing.h"

namespace umbrella {
namespace objc {

std::shared_ptr<Method> Method::parse(ABIObjectiveC& abi, bool isSmall, bool isClassMethod) {
    LIEF::BinaryStream& stream = abi.stream();
    std::shared_ptr<Method> method = std::make_shared<Method>();

    method->setAddress(stream.pos());
    method->classMethod = isClassMethod;
    method->relativeMethod = isSmall;
    if (isSmall) {
        PEEK(raw, umbrella::objc::small_method_t, stream)

        if (auto result = stream.peek<uintptr_t>(method->applyRelativeOffset(raw->name))) {
            const uintptr_t ptr = abi.fixPointer(*result);
            if (auto res = stream.peek_string_at(ptr)) {
                method->name = std::move(*res);
            }
        }

        const size_t offset = offsetof(umbrella::objc::small_method_t, signature);
        if (auto result =
                stream.peek_string_at(method->applyRelativeOffset(offset + raw->signature))) {
            method->signature = std::move(*result);
        }
        method->relImpl = raw->impl;
    } else {
        PEEK(raw, umbrella::objc::big_method_t, stream)

        STRING_FIXED(method->name, raw->name)
        STRING_FIXED(method->signature, raw->signature)
        method->absImpl = raw->impl;
    }

    return method;
}

std::string Method::getDeclaration() const {
    // Methods will be dumped with their signature and selector combined.
    //   - <method> := [ '+' | '-' ] <signature>
    std::ostringstream stream;

    std::string signature = std::move(decodeSignature());
    if (isClassMethod()) {
        // Class methods will be annotated with a '+'
        stream << "+ ";
    } else {
        stream << "- ";
    }

    uintptr_t impl = getImplementation();
    if (isSmallMethod()) {
        impl = getRelativeImplementation();
    }
    stream << signature << " // 0x" << std::hex << impl;
    return stream.str();
}

} // namespace objc
} // namespace umbrella
