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
#include "pyUmbrella.h"

#include <umbrella/objc.h>

#include "objc/init.h"
#include "objc/pyObjC.h"

#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/unique_ptr.h>

PY_OBJC_NS_BEGIN

using namespace nb::literals;

void init(nb::module_& _Module) {
    nb::module_ _objc = _Module.def_submodule("objc", "Objective-C ABI");
    init_enums(_objc);

    create<umbrella::objc::TypeNode>(_objc);

    _objc.def("typedesc", &umbrella::objc::typedesc, nb::rv_policy::move);
    _objc.def("decode", &umbrella::objc::decode);
    _objc.def("signature", &umbrella::objc::signature, R"doc(
        Generates a fully qualified method signature.

        Example:
        >>> umbrella.objc.signature("foo:bar:", "q32@0:8@16q24")
        '(long long)foo:(id) bar:(long long)'

        :param __selector: the method's selector string
        :type __selector: str
        :param __signature: the encoded signature
        :type __signature: str
        :raises SyntaxError: if the signature is malformed
        :return: the qualified signature as a string
        :rtype: str
    )doc");

    create<umbrella::objc::Method>(_objc);
    create<umbrella::objc::Property>(_objc);
    create<umbrella::objc::IVar>(_objc);
    create<umbrella::objc::Protocol>(_objc);
    create<umbrella::objc::Class>(_objc);
    create<umbrella::objc::Category>(_objc);
    create<umbrella::objc::ABIObjectiveC>(_objc);

    _objc.def("parse", &umbrella::objc::parseObjC, "file_name"_a);
}

PY_OBJC_NS_END