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
#include "objc/pyObjC.h"

#include <nanobind/stl/string.h>
#include <umbrella/objc/Method.h>

#include "attributes.h"

PY_OBJC_NS_BEGIN

using Method = umbrella::objc::Method;

template <>
void create<Method>(nb::module_& _Module) {
    nb::class_<Method, umbrella::InProcess>(_Module, "Method", nb::is_final())
        .def("is_small", &Method::isSmallMethod)
        .def_prop_ro("name", &Method::getName)
        .def_prop_ro("signature", &Method::getSignature)
        .def_prop_ro("impl", &Method::getImplementation)
        .def_prop_ro("relative_impl", &Method::getRelativeImplementation)
        .def("get_typedesc", &Method::getTypeDesc)
        .def("decode_signature", &Method::decodeSignature)
        .def("get_decl", &Method::getDeclaration)
        PY_ATTR___STR__NAME(Method);
}

PY_OBJC_NS_END
