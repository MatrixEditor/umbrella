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
#include <nanobind/stl/vector.h>

#include <sstream>
#include <string>

#include <umbrella/objc/ABI.h>

#include "iterators.h"
#include "attributes.h"

PY_OBJC_NS_BEGIN

using ABIObjectiveC = umbrella::objc::ABIObjectiveC;

template <>
void create<ABIObjectiveC>(nb::module_& _Module) {
    nb::class_<ABIObjectiveC, umbrella::ABIBase> objc_ABI(_Module, "ABIObjectiveC", nb::is_final());

    iterator_<ABIObjectiveC::it_classes>(objc_ABI, "it_classes");
    iterator_<ABIObjectiveC::it_protocols>(objc_ABI, "it_protocols");
    iterator_<ABIObjectiveC::it_categories>(objc_ABI, "it_categories");

    objc_ABI.def_prop_ro("classes", &ABIObjectiveC::getClasses, nb::rv_policy::move)
        .def_prop_ro("protocols", &ABIObjectiveC::getProtocols, nb::rv_policy::move)
        .def_prop_ro("categories", &ABIObjectiveC::getCategories, nb::rv_policy::move)
        .def_prop_ro("get_class", &ABIObjectiveC::getClass, nb::rv_policy::reference_internal)
        .def_prop_ro("get_category", &ABIObjectiveC::getCategory, nb::rv_policy::reference_internal)
        .def_prop_ro("get_protocol", &ABIObjectiveC::getProtocol, nb::rv_policy::reference_internal)
        PY_ATTR___STR__(ABIObjectiveC,
            stream << "<ABIObjectiveC ";
            stream << "classes=" << _Value.getClassCount() << ", ";
            stream << "protocols=" << _Value.getProtocolCount() << ", ";
            stream << "categories=" << _Value.getCategoryCount() << ">";
        );
}

PY_OBJC_NS_END
