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

#include <umbrella/version.h>
#include <umbrella/visibility.h>

#include "abstract.h"
#include "pyUmbrella.h"

// Submodules
#include "objc/init.h"

namespace umbrella {
UMBRELLA_PY_NAMESPACE_BEGIN

#ifdef UMBRELLA_USE_PYBIND11
PYBIND11_MODULE(_umbrella, m) {
#else
NB_MODULE(_umbrella, m) {
#endif

    // Basic attributes
    m.attr("__version__") = UMBRELLA_VERSION;
    m.attr("__commit__") = UMBRELLA_COMMIT;
    m.attr("__tag__") = UMBRELLA_TAG;
    m.attr("__full_version__") = HUMAN_NAME;

    m.doc() = "Umbrella Python API";

    umbrella::py::init_abstract(m);
    umbrella::py::objc::init(m);
}

UMBRELLA_PY_NAMESPACE_END
} // namespace umbrella
