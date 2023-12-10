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
#include <LIEF/Abstract.hpp>

#include "umbrella/runtime.h"

namespace umbrella {

ABIBase::ABIBase(const TargetBinary* _Binary, std::shared_ptr<TargetBinaryStream> _Stream)
    : Binary{_Binary}, ImageBase{_Binary->imagebase()}, Stream{std::move(_Stream)} {}

} // namespace umbrella
