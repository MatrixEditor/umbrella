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

#if defined(NDEBUG)
  #define UMBRELLA_NAME "umbrella"
#else
  #define UMBRELLA_NAME "umbrella (Debug)"
#endif

#define UMBRELLA_VERSION "0.1.0-55aeee2"
#define UMBRELLA_TAGGED 0
#define UMBRELLA_TAG    "55aeee26e829041c603ade61d3b34b8351c5bab6"
#define UMBRELLA_COMMIT "55aeee2"

#define HUMAN_VERSION " v" UMBRELLA_VERSION
#define HUMAN_NAME UMBRELLA_NAME HUMAN_VERSION
