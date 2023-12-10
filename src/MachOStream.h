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

#if !defined(__UMBRELLA_PRIVATE_MACHO_STREAM_H__)
#define __UMBRELLA_PRIVATE_MACHO_STREAM_H__

#include <LIEF/BinaryStream/BinaryStream.hpp>

#include "umbrella/visibility.h"

namespace LIEF {

namespace MachO {
class Binary;
}  // namespace MachO

}  // namespace LIEF

namespace umbrella {

class MachOStream : public LIEF::BinaryStream {
  private:
    const LIEF::MachO::Binary* Binary = nullptr;

  public:
    MachOStream(const LIEF::MachO::Binary& _Binary) : Binary{&_Binary} {};
    // must be overriden
    uint64_t size() const override;

    LIEF::result<const void*> read_at(uint64_t offset, uint64_t size) const override;

    inline const LIEF::MachO::Binary& binary() const { return *Binary; }
};

} // namespace umbrella

#endif  // __UMBRELLA_PRIVATE_MACHO_STREAM_H__
