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
#if !defined(__UMBRELLA_VISIBILITY_H__)
#define __UMBRELLA_VISIBILITY_H__

#define DECL_NAMESPACE(NAME) namespace NAME {
#define END_NAMESPACE(NAME) }

#define UMBRELLA_NAMESPACE_BEGIN { DECL_NAMESPACE(umbrella)
#define UMBRELLA_NAMESPACE_END END_NAMESPACE(umbrella)

#define UMBRELLA_NAMESPACE_OBJC_BEGIN namespace objc {
#define UMBRELLA_NAMESPACE_OBJC_END END_NAMESPACE(objc)

#define UMBRELLA_SWIFT_NAMESPACE_BEGIN namespace swift {
#define UMBRELLA_SWIFT_NAMESPACE_END } // namespace umbrella

#endif  // __UMBRELLA_VISIBILITY_H__
