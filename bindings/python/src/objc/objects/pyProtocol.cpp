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
#include <umbrella/objc/Protocol.h>

#include "iterators.h"
#include "attributes.h"

PY_OBJC_NS_BEGIN

using Protocol = umbrella::objc::Protocol;

template <>
void create<Protocol>(nb::module_& _Module) {
    nb::class_<Protocol, umbrella::InProcess> (_Module, "Protocol", nb::is_final())
        .def_prop_ro("name", &Protocol::getName)
        .def_prop_ro("flags", &Protocol::getFlags)
        .def_prop_ro("required_instance_methods", &Protocol::getRequiredInstanceMethods, nb::rv_policy::move)
        .def_prop_ro("optional_instance_methods", &Protocol::getOptionalInstanceMethods, nb::rv_policy::move)
        .def_prop_ro("required_class_methods", &Protocol::getRequiredClassMethods, nb::rv_policy::move)
        .def_prop_ro("optional_class_methods", &Protocol::getOptionalClassMethods, nb::rv_policy::move)
        .def_prop_ro("instance_properties", &Protocol::getInstanceProperties, nb::rv_policy::move)
        .def_prop_ro("protocols", &Protocol::getProtocols, nb::rv_policy::move)
        .def("get_decl", &Protocol::getDeclaration)
        PY_ATTR___STR__NAME(Protocol);
}

PY_OBJC_NS_END
