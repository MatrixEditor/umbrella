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
#if !defined(__UMBRELLA_PY_ATTRS_H__)
#define __UMBRELLA_PY_ATTRS_H__

#include <sstream>
#include <string>

#define PY_ATTR___STR__(type, ...)                                                                 \
    .def("__str__", [](const type& _Value) -> std::string {                                        \
        std::ostringstream stream;                                                                 \
        __VA_ARGS__                                                                                \
        return stream.str();                                                                       \
    })

#define PY_ATTR___STR__NAME(type)                                                                  \
    PY_ATTR___STR__(type, stream << ("<" #type " name='") << _Value.getName() << "'>";)

#endif  // __UMBRELLA_PY_ATTRS_H__
