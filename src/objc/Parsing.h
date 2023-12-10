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
#if !defined(__UMBRELLA_PRIVATE_PARSING_H__)
#define __UMBRELLA_PRIVATE_PARSING_H__

#define STRING_FIXED(attr, raw_attr)                                                               \
    if (auto result = stream.peek_string_at(abi.fixPointer(raw_attr))) {                           \
        attr = std::move(*result);                                                                 \
    }

#define STRING(attr, raw_attr)                                                                     \
    if (auto result = stream.peek_string_at(raw_attr)) {                                           \
        attr = std::move(*result);                                                                 \
    }

#define PEEK(var_name, type, stream_var)                                                           \
    const auto var_name = stream_var.peek<type>();                                                 \
    if (!var_name) {                                                                               \
        return nullptr;                                                                            \
    }

#define LIST(var, type)                                                                            \
    LIEF::ScopedStream scoped(stream, var);                                                        \
    if (const auto list = stream.read<type>())

#define METHODS(var, attr, isClass)                                                                \
    if (var) {                                                                                     \
        LIST(var, umbrella::objc::method_list_t) {                                                 \
            const bool isSmall = list->flags() & method_list_t::IS_SMALL;                          \
            const size_t size = isSmall ? sizeof(small_method_t) : sizeof(big_method_t);           \
            const size_t baseAddress = stream.pos();                                               \
            for (size_t i = 0; i < list->count; i++) {                                             \
                stream.setpos(baseAddress + i * size);                                             \
                if (std::shared_ptr<Method> method = Method::parse(abi, isSmall, isClass)) {       \
                    attr.push_back(std::move(method));                                             \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
    }

#define PROTOCOLS(var, attr)                                                                       \
    if (var) {                                                                                     \
        LIEF::ScopedStream scoped(stream, var);                                                    \
        if (const auto list = stream.read<umbrella::objc::protocol_list_t>()) {                    \
            const size_t size = sizeof(uintptr_t);                                                 \
            const size_t baseAddress = stream.pos();                                               \
            for (size_t i = 0; i < list->count; i++) {                                             \
                stream.setpos(baseAddress + i * size);                                             \
                if (auto result_ptr = stream.peek<uintptr_t>()) {                                  \
                    stream.setpos(abi.fixPointer(*result_ptr));                                    \
                    if (std::shared_ptr<Protocol> proto = Protocol::parse(abi)) {                  \
                        attr.push_back(std::move(proto));                                          \
                    }                                                                              \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
    }

#define PROPERTIES(var, attr)                                                                      \
    if (var) {                                                                                     \
        LIST(var, umbrella::objc::property_list_t) {                                               \
            const size_t size = sizeof(property_t);                                                \
            const size_t baseAddress = stream.pos();                                               \
            for (size_t i = 0; i < list->count; i++) {                                             \
                stream.setpos(baseAddress + i * size);                                             \
                if (std::shared_ptr<Property> prop = Property::parse(abi)) {                       \
                    attr.push_back(std::move(prop));                                               \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
    }

#define CLASS_FIXED(var, attr)                                                                     \
    if (var) {                                                                                     \
        stream.setpos(abi.fixPointer(var));                                                        \
        if (auto cls_ = Class::parse(abi)) {                                                       \
            attr = std::move(cls_);                                                                \
        }                                                                                          \
    }

#endif  // __UMBRELLA_PRIVATE_PARSING_H__
