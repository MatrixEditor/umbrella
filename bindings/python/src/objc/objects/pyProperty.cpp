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
#include <umbrella/objc/Property.h>

#include "attributes.h"

PY_OBJC_NS_BEGIN

using Property = umbrella::objc::Property;

template <>
void create<Property>(nb::module_& _Module) {
    nb::class_<Property, umbrella::InProcess>(_Module, "Property", nb::is_final())
        .def_prop_ro("name", &Property::getName)
        .def_prop_ro("mangled_attributes", &Property::getAttributes)
        .def("get_decoded_attributes", &Property::decodeAttributes)
        .def("get_decl", &Property::getDeclaration)
        PY_ATTR___STR__NAME(Property);
}

PY_OBJC_NS_END
