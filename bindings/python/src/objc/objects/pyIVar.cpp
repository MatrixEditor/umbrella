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
#include <umbrella/objc/IVar.h>

#include "attributes.h"

PY_OBJC_NS_BEGIN

using IVar = umbrella::objc::IVar;

template <>
void create<IVar>(nb::module_& _Module) {
  nb::class_<IVar, umbrella::InProcess>(_Module, "IVar", nb::is_final())
    .def_prop_ro("name", [](const IVar& ivar) {
      const std::string& fullname = ivar.getName();
      return nb::bytes(fullname.data(), fullname.size());
    })
    .def_prop_ro("mangled_type_name",
    [](const IVar& ivar) {
      const std::string& fullname = ivar.getMangledTypeName();
      return nb::bytes(fullname.data(), fullname.size());
    })
    .def_prop_ro("alignment", &IVar::getAlignment)
    .def_prop_ro("size", &IVar::getSize)
    .def("get_type_name", &IVar::getTypeName)
    .def("get_decl", &IVar::getDeclaration) PY_ATTR___STR__NAME(IVar);
}

PY_OBJC_NS_END
