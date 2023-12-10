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

#ifdef UMBRELLA_USE_PYBIND11
#include <pybind11/stl.h>
#else
#include <nanobind/stl/string.h>
#endif

#include <umbrella/objc/Category.h>

#include "iterators.h"
#include "attributes.h"

PY_OBJC_NS_BEGIN

using Category = umbrella::objc::Category;

template <>
void create<Category>(nb::module_& _Module) {
    nb::class_<Category, umbrella::InProcess> objc_Category(_Module, "Category", nb::is_final());

    objc_Category.def_prop_ro("name", &Category::getName)
        .def("is_extension", &Category::isExtension)
        .def("get_decl", &Category::getDeclaration)
        .def_prop_ro("base_class", &Category::getBaseClass, nb::rv_policy::reference_internal);

    iterator_<Category::it_methods>(objc_Category, "it_methods");
    iterator_<Category::it_properties>(objc_Category, "it_properties");

    objc_Category
        .def_prop_ro("instance_methods", &Category::getInstanceMethods, nb::rv_policy::move)
        .def_prop_ro("class_methods", &Category::getClassMethods, nb::rv_policy::move)
        .def_prop_ro("instance_properties", &Category::getInstanceProperties, nb::rv_policy::move)
        .def_prop_ro("base_protocols", &Category::getBaseProtocols, nb::rv_policy::move);

    objc_Category PY_ATTR___STR__NAME(Category);
}

PY_OBJC_NS_END
