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
#include <umbrella/objc/Class.h>

#include "iterators.h"
#include "attributes.h"

PY_OBJC_NS_BEGIN

using Class = umbrella::objc::Class;

template <>
void create<Class>(nb::module_& _Module) {
    nb::class_<Class, umbrella::InProcess> objc_Class(_Module, "Class", nb::is_final());

    objc_Class.def_prop_ro("name", &Class::getName)
        .def_prop_ro("flags", &Class::getFlags)
        .def_prop_ro("super_class", &Class::getSuperClass, nb::rv_policy::reference_internal)
        .def_prop_ro("meta_class", &Class::getMetaClass, nb::rv_policy::reference_internal)
        .def("has_super_class", &Class::hasSuperClass)
        .def("has_meta_class", &Class::hasMetaClass)
        .def("get_decl", &Class::getDeclaration);

    iterator_<Class::it_ivars>(objc_Class, "it_ivars");

    objc_Class.def_prop_ro("methods", &Class::getMethods, nb::rv_policy::move)
        .def_prop_ro("ivars", &Class::getIVars, nb::rv_policy::move)
        .def_prop_ro("protocols", &Class::getProtocols, nb::rv_policy::move)
        .def_prop_ro("properties", &Class::getProperties, nb::rv_policy::move)
        PY_ATTR___STR__NAME(Class);


}

PY_OBJC_NS_END