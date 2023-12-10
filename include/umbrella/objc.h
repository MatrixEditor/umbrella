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
#if !defined(__UMBRELLA_OBJC_H__)
#define __UMBRELLA_OBJC_H__

#include "umbrella/objc/ABI.h"
#include "umbrella/objc/Category.h"
#include "umbrella/objc/Class.h"
#include "umbrella/objc/IVar.h"
#include "umbrella/objc/Method.h"
#include "umbrella/objc/Property.h"
#include "umbrella/objc/Protocol.h"
#include "umbrella/objc/TypeEncoding.h"
#include "umbrella/objc/Types.h"

namespace umbrella {
namespace objc {

/**
 * @brief Parse Objective-C ABI information from a file.
 *
 * This function parses Objective-C ABI information from a file with the given
 * fileName. It returns a unique pointer to an ABIObjectiveC object, representing
 * the parsed information.
 *
 * @param fileName The name of the file to parse Objective-C ABI information
 *                 from.
 * @return std::unique_ptr<objc::ABIObjectiveC> A unique pointer to the parsed
 *         ABIObjectiveC object.
 *
 * @note You should call this function to obtain Objective-C ABI information
 * from a MachO file only. The returned unique pointer manages the ownership
 * of the ABIObjectiveC object. Make sure to handle the unique pointer properly
 * to prevent resource leaks.
 */
std::unique_ptr<objc::ABIObjectiveC> parseObjC(const std::string& fileName);

} // namespace objc
} // namespace umbrella

#endif  // __UMBRELLA_OBJC_H__
