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

#if !defined(__UMBRELLA_PY_ITERATOR_H__)
#define __UMBRELLA_PY_ITERATOR_H__

#include <iterator>
#include <stdexcept>

#ifdef UMBRELLA_USE_PYBIND11
#include <pybind11/pybind11.h>
namespace nb = pybind11;
#else
#include <nanobind/nanobind.h>
namespace nb = nanobind;
#endif

template <typename T>
void iterator_(nb::handle& _Module, const char* _Name) {
    nb::class_<T>(_Module, _Name)
        .def("__getitem__",
             [](T& self, size_t i) -> typename T::reference {
                 if (i >= self.size()) {
                     throw nb::index_error("Index out of range!");
                 }
                 return self[i];
             }, nb::rv_policy::reference_internal)

        .def("__len__", [](T& self) { return self.size(); })

        .def(
            "__iter__", [](const T& self) { return std::begin(self); }, nb::rv_policy::move)

        .def("__next__", [](T& self) -> typename T::reference {
            if (self == std::end(self)) {
                throw nb::stop_iteration();
            }
            return *(self++);
        }, nb::rv_policy::reference_internal);
}

#endif  // __UMBRELLA_PY_ITERATOR_H__
