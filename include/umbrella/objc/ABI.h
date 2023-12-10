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
#if !defined(_UMBRELLA_OBJC_ABI_H__)
#define _UMBRELLA_OBJC_ABI_H__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "umbrella/visibility.h"

#include "umbrella/ObjC/Category.h"
#include "umbrella/ObjC/Class.h"
#include "umbrella/ObjC/Protocol.h"
#include "umbrella/iterators.h"
#include "umbrella/runtime.h"

namespace umbrella {
namespace objc {

class Class;
class Protocol;
class Category;

/**
 * @brief Class representing Objective-C ABI information.
 *
 * This class inherits from the ABIBase class.
 */
class ABIObjectiveC final : public ABIBase {
public:
  using TargetBinary = typename ABIBase::TargetBinary;
  using TargetBinaryStream = typename ABIBase::TargetBinaryStream;

  using ProtocolList = std::vector<std::shared_ptr<Protocol>>;
  using ClassList = std::vector<std::shared_ptr<Class>>;
  using CategoryList = std::vector<std::shared_ptr<Category>>;

  using ClassLookup = std::unordered_map<std::string, Class*>;
  using ProtocolLookup = std::unordered_map<std::string, Protocol*>;
  using CategoryLookup = std::unordered_map<std::string, Category*>;

  using it_classes = LIEF::const_ref_iterator<const ClassList&, Class*>;
  using it_protocols = LIEF::const_ref_iterator<const ProtocolList&, Protocol*>;
  using it_categories = LIEF::const_ref_iterator<const CategoryList&, Category*>;

private:
  ClassList classes;       /**< List of classes. */
  ProtocolList protocols;  /**< List of protocols. */
  CategoryList categories; /**< List of categories. */

  ClassLookup classLookup;       /**< Class name to class object lookup map. */
  ProtocolLookup protocolLookup; /**< Protocol name to protocol object lookup map. */
  CategoryLookup categoryLookup; /**< Category name to category object lookup map. */

public:
  /**
   * @brief Constructor for ABIObjectiveC.
   *
   * @param _Binary Pointer to the target binary.
   * @param _Stream Shared pointer to the target binary stream.
   */
  ABIObjectiveC(const TargetBinary* _Binary, std::shared_ptr<TargetBinaryStream> _Stream)
    : ABIBase(_Binary, _Stream){};

  /**
   * @brief Static function to parse Objective-C information.
   *
   * @param _Binary Reference to the target binary.
   * @param _Stream Shared pointer to the target binary stream.
   * @return std::unique_ptr<ABIObjectiveC> A unique pointer to the parsed data.
   */
  static std::unique_ptr<ABIObjectiveC> parse(const TargetBinary& _Binary,
                                              std::shared_ptr<TargetBinaryStream> _Stream);

  /**
   * @brief Get a pointer to a class by name.
   *
   * @param name The name of the class.
   * @return const Class* A pointer to the class. Returns nullptr if not found.
   */
  const Class* getClass(const std::string& name) { return lookup<Class>(name, classLookup); }

  /**
   * @brief Get a pointer to a category by name.
   *
   * @param name The name of the category.
   * @return const Category* A pointer to the category. Returns nullptr if not found.
   */
  const Category* getCategory(const std::string& name) {
    return lookup<Category>(name, categoryLookup);
  }

  /**
   * @brief Get a pointer to a protocol by name.
   *
   * @param name The name of the protocol.
   * @return const Protocol* A pointer to the protocol. Returns nullptr if not found.
   */
  const Protocol* getProtocol(const std::string& name) {
    return lookup<Protocol>(name, protocolLookup);
  }

  /**
   * @brief Get an iterator to the list of classes.
   *
   * @return it_classes An iterator to the classes.
   */
  it_classes getClasses() const { return classes; }

  /**
   * @brief Get an iterator to the list of protocols.
   *
   * @return it_protocols An iterator to the protocols.
   */
  it_protocols getProtocols() const { return protocols; }

  /**
   * @brief Get an iterator to the list of categories.
   *
   * @return it_categories An iterator to the categories.
   */
  it_categories getCategories() const { return categories; }

  /**
   * @brief Fix a pointer value based its representation.
   *
   * @param ptr The pointer value to fix.
   * @return uintptr_t The fixed pointer value.
   */
  uintptr_t fixPointer(uintptr_t ptr) const;

  /**
   * @brief Get the number of classes in the ABI.
   *
   * @return size_t The number of classes.
   */
  inline size_t getClassCount() const { return classes.size(); }

  /**
   * @brief Get the number of categories in the ABI.
   *
   * @return size_t The number of categories.
   */
  inline size_t getCategoryCount() const { return categories.size(); }

  /**
   * @brief Get the number of protocols in the ABI.
   *
   * @return size_t The number of protocols.
   */
  inline size_t getProtocolCount() const { return protocols.size(); }

private:
  /**
   * @brief Lookup a name in the specified map.
   *
   * @tparam T The type of the objects in the map.
   * @param name The name to look up.
   * @param map The map to search in.
   * @return const T* A pointer to the found object. Returns nullptr if not found.
   */
  template <typename T>
  const T* lookup(const std::string& name, std::unordered_map<std::string, T*>& map) {
    auto res = map.find(name);
    if (res != std::end(map)) {
      return res->second;
    }
    return nullptr;
  }
};

} // namespace objc
} // namespace umbrella

#endif  // _UMBRELLA_OBJC_ABI_H__