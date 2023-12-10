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
#include <LIEF/BinaryStream/MemoryStream.hpp>
#include <LIEF/MachO.hpp>

#include "MachOStream.h"

using namespace LIEF::MachO;

namespace umbrella {

uint64_t MachOStream::size() const {
    // What size do we need here?
    return static_cast<uint64_t>(-1);
}

LIEF::result<const void*> MachOStream::read_at(uint64_t offset, uint64_t size) const {
    uint64_t address = offset;
    if (Binary->memory_base_address() > 0 && offset > Binary->memory_base_address()) {
        address -= Binary->memory_base_address();
        address += Binary->imagebase();
    }

    const SegmentCommand* cmd = Binary->segment_from_virtual_address(address);
    if (cmd == nullptr) {
        return make_error_code(lief_errors::read_error);
    }

    LIEF::span<const uint8_t> content = cmd->content();
    uintptr_t rel_offset = address - cmd->virtual_address();
    return content.data() + rel_offset;
}

} // namespace umbrella
