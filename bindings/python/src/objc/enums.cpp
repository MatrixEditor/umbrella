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

#include <umbrella/objc/TypeEncoding.h>

#include "objc/pyObjC.h"

PY_OBJC_NS_BEGIN

void init_enums(nb::module_& _Module){

    nb::enum_<umbrella::objc::Type>(_Module, "TYPE")
#define OBJC_TYPE(id, name, alignment, v) .value(#v, umbrella::objc::Type::v)
#include <umbrella/ObjC/TypeEncoding.def>
#undef OBJC_TYPE

#define OBJC_TYPE(v) .value(#v, umbrella::objc::Type::v)
        OBJC_TYPE(PVOID)
        OBJC_TYPE(OBJECT)
        OBJC_TYPE(ARRAY)
        OBJC_TYPE(STRUCT)
        OBJC_TYPE(UNION)
        OBJC_TYPE(BIT_FIELD)
        OBJC_TYPE(POINTER)
        OBJC_TYPE(ATTRIBUTES)
        OBJC_TYPE(BLOCK)
#undef OBJC_TYPE
        .export_values();

    nb::enum_<umbrella::objc::AttributeType>(_Module, "ATTRIBUTE_TYPE")
#define ATTR_TYPE(id, name, v) .value(#v, umbrella::objc::AttributeType::v)
#include <umbrella/objc/TypeEncoding.def>
#undef ATTR_TYPE
        .value("GETTER", umbrella::objc::AttributeType::GETTER)
        .value("SETTER", umbrella::objc::AttributeType::SETTER)
        .export_values();

    nb::enum_<umbrella::objc::MethodType>(_Module, "METHOD_TYPE")
#define METHOD_TYPE(id, name, v) .value(#v, umbrella::objc::MethodType::v)
#include <umbrella/objc/TypeEncoding.def>
#undef METHOD_TYPE
        .export_values();

}

PY_OBJC_NS_END