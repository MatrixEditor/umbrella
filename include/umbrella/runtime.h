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

#if !defined(_UMBRELLA_RUNTIME_H__)
#define _UMBRELLA_RUNTIME_H__

#include <memory>

#include "umbrella/visibility.h"

namespace LIEF {
class Binary;
class BinaryStream;
}  // namespace LIEF

namespace umbrella {

/**
 * @brief Class representing an object in the process.
 */
class InProcess {
private:
  uintptr_t address; /**< The address of the object in the process memory. */

public:
  /**
   * @brief Constructor for InProcess class.
   *
   * @param _Address The address of the object in the process memory.
   */
  InProcess(uintptr_t _Address) : address(_Address) {}

  /**
   * @brief Get the address of the object in the process memory.
   *
   * @return uintptr_t The address of the object.
   */
  uintptr_t getAddress() const { return address; }

  /**
   * @brief Set the address of the object in the process memory.
   *
   * @param _Address The new address to set.
   */
  void setAddress(uintptr_t _Address) { address = _Address; }

  /**
   * @brief Apply a relative offset to a base pointer.
   *
   * The offset is applied to the base pointer using sign-extended,
   * wrapping arithmetic.
   *
   * @param offset The offset to apply.
   * @return uintptr_t The calculated address after applying the offset.
   */
  uintptr_t applyRelativeOffset(int32_t offset) const {
    uintptr_t base = getAddress();

    uintptr_t extendOffset = (uintptr_t)(intptr_t)offset;
    return base + extendOffset;
  }
};

/**
 * @brief Class representing the default ABI implementation.
 */
class ABIBase {
public:
  using TargetBinary = LIEF::Binary;
  using TargetBinaryStream = LIEF::BinaryStream;

private:
  const TargetBinary* Binary;                 /**< Pointer to the target binary. */
  uintptr_t ImageBase;                        /**< The image base address. */
  std::shared_ptr<LIEF::BinaryStream> Stream; /**< Shared pointer to the binary stream. */

public:
  /**
   * @brief Constructor for ABIBase class.
   *
   * @param _Binary Pointer to the target binary.
   * @param _Stream Shared pointer to the binary stream.
   */
  ABIBase(const TargetBinary* _Binary, std::shared_ptr<TargetBinaryStream> _Stream);

  /**
   * @brief Destructor for ABIBase class.
   */
  ~ABIBase() = default;

  // Disable copy creation.
  ABIBase(const ABIBase&) = delete;
  ABIBase& operator=(const ABIBase&) = delete;

  /**
   * @brief Get a reference to the target binary.
   *
   * @return const TargetBinary& Reference to the target binary.
   */
  const TargetBinary& binary() const { return *Binary; }

  /**
   * @brief Get a reference to the binary stream.
   *
   * @return TargetBinaryStream& Reference to the binary stream.
   */
  TargetBinaryStream& stream() { return *Stream; };

  /**
   * @brief Get the image base address.
   *
   * @return uintptr_t The image base address.
   */
  uintptr_t imagebase() const { return ImageBase; }
};

} // namespace umbrella

#endif  // _UMBRELLA_RUNTIME_H__
