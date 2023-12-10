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

#include <umbrella/runtime.h>

#include "abstract.h"

namespace umbrella {
UMBRELLA_PY_NAMESPACE_BEGIN

void init_abstract(nb::module_& _Module) {
    nb::class_<umbrella::InProcess>(_Module, "InProcess")
        .def(nb::init<uintptr_t>())

        .def_prop_ro("address", &InProcess::getAddress, R"doc(
        Returns the assigned virtual address.
      )doc")

        .def("apply_relative_offset", &InProcess::applyRelativeOffset,
             "Apply a relative offset to a base pointer.", nb::arg("offset"));

    // ABIBase will be private within the C++ API
    nb::class_<umbrella::ABIBase>(_Module, "ABIBase")
        .def_prop_ro("image_base", &umbrella::ABIBase::imagebase);
}

UMBRELLA_PY_NAMESPACE_END
} // namespace umbrella