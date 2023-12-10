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
#if !defined(__UMBRELLA_PRIVATE_DECLARATION_H__)
#define __UMBRELLA_PRIVATE_DECLARATION_H__

#define LIST_DECL(var_name, END)                                                                   \
  if (var_name.size()) {                                                                           \
    stream << ("// " #var_name) << "\n";                                                           \
    for (auto _elem : var_name) {                                                                  \
      stream << _elem->getDeclaration() << "\n";                                                   \
    }                                                                                              \
    stream << END;                                                                                \
  }

#define PROTO_CONFORM_DECL(var_name, start, end)                                                   \
  stream << start;                                                                                 \
  const auto count = var_name.size();                                                              \
  for (size_t i = 0; i < count; i++) {                                                             \
    stream << var_name[i]->getName();                                                              \
    if (i < (count - 1)) {                                                                         \
      stream << ", ";                                                                              \
    }                                                                                              \
  }                                                                                                \
  stream << (end " ");

#endif  // __UMBRELLA_PRIVATE_DECLARATION_H__
