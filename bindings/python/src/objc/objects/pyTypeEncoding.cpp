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
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#endif

#include <memory>
#include <sstream>
#include <umbrella/objc/TypeEncoding.h>

#include "iterators.h"



PY_OBJC_NS_BEGIN

using TypeNode = umbrella::objc::TypeNode;

template <>
void create<TypeNode>(nb::module_& _Module) {
    auto cls_TypeNode = nb::class_<TypeNode>(_Module, "TypeNode");
    iterator_<TypeNode::it_children>(cls_TypeNode, "it_children");

    cls_TypeNode.def("get_type", &TypeNode::getType)
        .def("get_attr_type", &TypeNode::getAttributeType)
        .def_prop_ro("children", &TypeNode::getChildren, nb::rv_policy::move)
        .def_ro("type", &TypeNode::type)
        .def_ro("size", &TypeNode::size)
        .def_ro("alignment", &TypeNode::alignment)
        .def_ro("stack_size", &TypeNode::stack_size)
        .def_ro("attributes", &TypeNode::attributes)
        .def_ro("name", &TypeNode::name)
        .def_ro("parent", &TypeNode::parent)
        .def("__str__", [](const TypeNode& Self) {
            std::ostringstream stream;
            stream << "TypeNode[type=" << Self.type << "]";
            return stream.str();
        });
}

PY_OBJC_NS_END