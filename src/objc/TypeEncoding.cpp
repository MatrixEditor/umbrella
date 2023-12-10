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
#include "umbrella/objc/TypeEncoding.h"
#include "umbrella/visibility.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace umbrella {
namespace objc {

using Iterator = std::string::const_iterator;

std::shared_ptr<TypeNode> parseType(Iterator& it, std::shared_ptr<TypeNode> parent,
                                    const std::string& encoded);
uint32_t parseNatural(Iterator& it, const std::string& encoded);
void parseBitfield(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded);
void parseBlock(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded);
void parsePointer(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded);
void parseArray(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded);
void parseStruct(Iterator& it, std::shared_ptr<TypeNode> node, bool isUnion,
                 const std::string& encoded);
void parseObject(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded);
void parseProperty(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded);
std::string decodeType(const TypeNode& _Node);
// public:
std::shared_ptr<TypeNode> typedesc(const std::string& _Encoded) {
    if (_Encoded.empty()) {
        return nullptr;
    }

    Iterator it = std::begin(_Encoded);
    const size_t _Length = _Encoded.size();
    std::shared_ptr<TypeNode> type = std::make_shared<TypeNode>();
    while (it != std::end(_Encoded)) {
        std::shared_ptr<TypeNode> child = parseType(it, type, _Encoded);
        type->children.push_back(std::move(child));
    }
    return type;
}

std::string decode(const TypeNode& _Node) {
    if (_Node.type != 0) {
        return decodeType(_Node);
    }

    std::ostringstream stream;
    size_t count = _Node.children.size();
    for (size_t i = 0; i < count; i++) {
        stream << decodeType(*_Node.children[i]);
        if (i != (count - 1)) {
            stream << " ";
        }
    }
    return stream.str();
}

std::string signature(const std::string& _Selector, const std::string& _Signature) {
    std::ostringstream stream;
    std::shared_ptr<TypeNode> node = typedesc(_Signature);
    const TypeNode& returnType = *node->children[0];
    const size_t count = node->children.size();

    stream << '(' << decode(returnType) << ")";
    // 0 => rtype
    // 1 => sel
    // 2 => id
    if (count <= 3) {
        // no arguments
        stream << _Selector;
        return stream.str();
    }

    size_t start = 0;
    size_t pos = 0;
    size_t index = 3;
    std::string token;

    // This way we sanitize labels and ignore anonymous parameters
    do {
        if (index >= count)
            break;

        pos = _Selector.find(':', start);
        token = _Selector.substr(start, pos - start);
        if (!token.empty()) {
            const TypeNode& child = *node->children[index];
            stream << token << ":(" << decode(child) << ")";

            if (index < (count - 1)) {
                stream << " ";
            }
        }
        start = pos + 1;
        index++;
    } while (pos != std::string::npos);

    return stream.str();
}

// private:
std::shared_ptr<TypeNode> parseType(Iterator& it, std::shared_ptr<TypeNode> parent,
                                    const std::string& encoded) {
    std::shared_ptr<TypeNode> node = std::make_shared<TypeNode>();
    node->parent = std::move(parent);

    char tok = *it++;
    auto search = OBJC_METHOD_TYPES.find(tok);
    while (search != std::end(OBJC_METHOD_TYPES)) {
        MethodType methodType = search->second;
        switch (methodType) {
#define METHOD_TYPE(id, name, value)                                                               \
    case MethodType::value: {                                                                      \
        std::string _name(name);                                                                   \
        node->attributes.push_back(std::move(_name));                                              \
        break;                                                                                     \
    }

#include "umbrella/objc/TypeEncoding.def"
#undef METHOD_TYPE
        }

        tok = *it++;
        search = OBJC_METHOD_TYPES.find(tok);
    }

    auto simpleTypeResult = OBJC_NODE_TYPES.find(tok);
    if (simpleTypeResult != std::end(OBJC_NODE_TYPES)) {
        Type type = simpleTypeResult->second;
        switch (type) {
#define OBJC_TYPE(id, name_, alignment_, value)                                                    \
    case Type::value: {                                                                            \
        node->name = std::move(std::string(name_));                                                \
        node->type = (uint32_t)Type::value;                                                        \
        node->alignment = alignment_;                                                              \
        node->size = alignment_;                                                                   \
        break;                                                                                     \
    }

#include "umbrella/objc/TypeEncoding.def"
#undef OBJC_TYPE
        }
    } else {
        switch (tok) {
        case '^':
            parsePointer(it, node, encoded);
            break;

        case '[':
            parseArray(it, node, encoded);
            break;

        case '{':
            parseStruct(it, node, false, encoded);
            break;

        case '(':
            parseStruct(it, node, true, encoded);
            break;

        case '@':
            parseObject(it, node, encoded);
            break;

        case 'b':
            parseBitfield(it, node, encoded);
            break;

        case '"': {
            // Special case: struct member definition starting with a name
            std::string memberName;
            while (*it != '"' && it != std::end(encoded)) {
                memberName.append({*it++});
            }
            it++;  // skip trailing "

            // REVISIT: This actually does not parse types in the way we
            // want them to be parsed. The returned TypeNode should be
            // stored as a child node with Type::STRUCT_MEMBER as its type.
            node = parseType(it, node, encoded);
            node->name = std::move(memberName);
            break;
        }

        case 'T':
            // Special case: the whole string defines property attributes
            parseProperty(it, node, encoded);
            break;
        }
    }

    node->stack_size = parseNatural(it, encoded);
    return node;
}

void parseBitfield(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded) {
    uint32_t count = parseNatural(it, encoded);
    node->size = count;
    node->alignment = count;
    node->type = (uint32_t)Type::BIT_FIELD;
}

void parseBlock(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded) {
    if (*it == '?') {
        ++it;  // Special case: ^? --> void*
        node->type = (uint32_t)Type::PVOID;
        return;
    }

    if (*it == '<')
        // Skip block start
        ++it;

    std::shared_ptr<TypeNode> return_type = parseType(it, node, encoded);
    node->children.push_back(std::move(return_type));

    std::shared_ptr<TypeNode> block_self = parseType(it, node, encoded);
    node->children.push_back(std::move(block_self));

    while (*it != '>') {
        std::shared_ptr<TypeNode> child = parseType(it, node, encoded);
        node->size += child->size;
        node->children.push_back(std::move(child));
    }

    if (*it == '>') {
        ++it;  // skip '>' at the end
    }
    node->type = (uint32_t)Type::BLOCK;
    node->alignment = 8;
}

void parsePointer(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded) {
    std::shared_ptr<TypeNode> child = parseType(it, node, encoded);
    node->children.push_back(std::move(child));
    node->size = 8;
    node->alignment = 8;
    node->type = (uint32_t)Type::POINTER;
}

void parseArray(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded) {
    uint32_t count = parseNatural(it, encoded);
    std::shared_ptr<TypeNode> child = parseType(it, node, encoded);

    node->type = (uint32_t)Type::ARRAY;
    node->size = count;
    node->dim = count;
    node->alignment = child->alignment;
    node->children.push_back(std::move(child));

    if (*(it) == ']') {
        // Make sure we skip the closing bracket
        ++it;
    }
}

uint32_t parseNatural(Iterator& it, const std::string& encoded) {
    if (it == std::end(encoded)) {
        return 0;
    }

    uint32_t value = 0;
    while ('0' <= *it && '9' >= *it) {
        char tok = *it++;
        int digit = tok - '0';
        value *= 10;
        value += digit;
    }
    return value;
}

void parseObject(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded) {
    if (*it == '?') {
        it++;  // skip blocks
        if (*it == '<') {
            // actual block definition, parse that and return
            parseBlock(it, node, encoded);
            return;
        }
    }

    if (*it == '"') {
        ++it;  // skip '"'
        std::string name;

        // parse additional name
        while (*it != '"' && it != std::end(encoded)) {
            name.append({*it++});
        }
        ++it;  // skip '"' at the end
        node->name = std::move(name);
    } else {
        node->name = "id";
    }

    node->type = (uint32_t)Type::OBJECT;
    node->size = 8;
    node->alignment = 8;
}

void parseStruct(Iterator& it, std::shared_ptr<TypeNode> node, bool isUnion,
                 const std::string& encoded) {
    // Structs are represented as "{name=??}", the cursor has moved pass '{'
    // when this function is called.
    std::string name;
    node->type = (uint32_t)(isUnion ? Type::UNION : Type::STRUCT);

    char close_tok = isUnion ? ')' : '}';
    while (it != std::end(encoded)) {
        char tok = *it++;
        if (tok == '=') {
            break;
        }

        if (tok == close_tok) {
            node->name = std::move(name);
            return;
        }
        name.append({tok});
    }

    node->name = std::move(name);
    while (it != std::end(encoded)) {
        char tok = *it;
        if (tok == close_tok) {
            it++;  // skip close token
            break;
        }

        std::shared_ptr<TypeNode> child = parseType(it, node, encoded);
        node->alignment = std::max(child->alignment, node->alignment);
        if (isUnion) {
            node->size += child->size;
        } else {
            node->size = std::max(child->size, node->size);
        }
        node->children.push_back(std::move(child));
    }
}

void parseProperty(Iterator& it, std::shared_ptr<TypeNode> node, const std::string& encoded) {
    size_t start = std::distance(std::begin(encoded), it);
    size_t pos = start;
    size_t index = 0;
    std::string token;

    size_t debug_index = start;

    node->type = (uint32_t)Type::ATTRIBUTES;
    do {
        pos = encoded.find(',', start);
        token = encoded.substr(start, pos - start);

        // Child 0 is always the typedesc
        if (index == 0) {
            std::shared_ptr<TypeNode> child = parseType(it, node, encoded);
            node->children.push_back(std::move(child));
        } else {
            std::shared_ptr<TypeNode> attrNode = std::make_shared<TypeNode>();
            switch (token[0]) {
#define ATTR_TYPE(id, name_, value)                                                                \
    case id:                                                                                       \
        attrNode->name = std::string(name_);                                                       \
        attrNode->type = (uint32_t)AttributeType::value;                                           \
        break;
#include "umbrella/objc/TypeEncoding.def"
#undef ATTR_TYPE

            case 'G':  // Getter
                attrNode->type = (uint32_t)AttributeType::GETTER;
                attrNode->name = token.substr(1);
                break;

            case 'S':  // Setter
                attrNode->type = (uint32_t)AttributeType::SETTER;
                attrNode->name = token.substr(1);
                break;

            default:
                // the last child is the name of the backing instance variable
                node->name = std::move(token);
                break;
            }

            node->children.push_back(std::move(attrNode));
        }

        index++;
        start = pos + 1;  // plus one due to delimiter
    } while (pos != std::string::npos);

    // We assume that only one property encoding per type encoding
    // is possible.
    it = std::end(encoded);
}

std::string decodeType(const TypeNode& _Node) {
    std::ostringstream stream;
    if (!_Node.attributes.empty()) {
        // append attributes first (if present). The whitespace at the end
        // can be aded safely as a node can not be of an unregistered type
        // but stores attributes.
        for (const auto& attribute : _Node.attributes) {
            stream << attribute << " ";
        }
    }

    // Search for any primitive types
    auto result = OBJC_NODE_TYPE_NAMES.find(_Node.type);
    if (result != std::end(OBJC_NODE_TYPE_NAMES) || _Node.type == (uint32_t)Type::OBJECT) {
        // name of simple types are set by default
        stream << _Node.name;
    } else {
        const size_t count = _Node.children.size();

        switch (_Node.type) {
        case (uint32_t)Type::BIT_FIELD: {
            stream << "BitField<" << _Node.size << ">";
            break;
        }

        case (uint32_t)Type::POINTER: {
            if (_Node.children.size() != 1) {
                // This error will be converted to 'ValueError' on Python side.
                throw std::range_error("Invalid children count on POINTER node!");
            }

            std::string ptrChild = decode(*_Node.children[0]);
            stream << ptrChild << (ptrChild[ptrChild.size() - 1] == '*' ? "*" : " *");
            break;
        }

        case (uint32_t)Type::ARRAY: {
            std::string arrChild = decode(*_Node.children[0]);
            if (_Node.dim != 0) {
                // FIXME: this results in swapped dimensions in case of
                // multidimensional arrays
                stream << arrChild << "[" << _Node.dim << "]";
            } else {
                stream << arrChild << "[]";
            }
            break;
        }

        case (uint32_t)Type::STRUCT: {
            // TODO: maybe add option to expand structs and unions
            stream << "struct " << _Node.name;
            break;
        }

        case (uint32_t)Type::UNION: {
            // TODO: maybe add option to expand structs and unions
            stream << "union " << _Node.name;
            break;
        }

        case (uint32_t)Type::PVOID: {
            stream << "void *";
            break;
        }

        case (uint32_t)Type::BLOCK: {
            //  void (^id)(NSError, ...)
            //    ^            ^     ^
            // This implementation ignores the 'Self' argument of each block.
            const TypeNode& returnType = *_Node.children[0];
            stream << decode(returnType) << " ";

            std::ostringstream attrStream;
            if (count >= 2) {  // additional check to prevent errors
                for (size_t i = 2; i < count; i++) {
                    attrStream << decode(*_Node.children[i]);
                    if (i != (count - 1)) {
                        attrStream << ", ";
                    }
                }
            }
            stream << "(^_)(" << attrStream.str() << ")";
            break;
        }

        case (uint32_t)Type::ATTRIBUTES: {
            // First child is type
            const TypeNode& typeSpec = *_Node.children[0];
            std::ostringstream attributes;

            for (size_t i = 1; i < count; i++) {
                const TypeNode& child = *_Node.children[i];
                switch (child.getAttributeType()) {
                case AttributeType::GETTER: {
                    attributes << "getter=" << child.name;
                    break;
                }
                case AttributeType::SETTER: {
                    attributes << "setter=" << child.name;
                    break;
                }
                case AttributeType::DYNAMIC: {
                    stream << "@dynamic ";
                    break;
                }
                default: {
                    if (!child.name.empty()) {
                        attributes << child.name;
                    }
                    break;
                }
                }  // end switch
                // FIXME: this results sometimes in @property (..., ) foo
                if (i < (count - 1)) {
                    attributes << ", ";
                }
            }

            stream << "@property";
            if (count > 1) {
                stream << " (" << attributes.str() << ")";
            }

            stream << " " << decode(typeSpec);
            if (!_Node.name.empty()) {
                stream << " ";
                if (_Node.name.size() > 2 && _Node.name[0] == 'V' && _Node.name[1] == '_') {
                    stream << _Node.name.substr(2);
                } else {
                    stream << _Node.name;
                }
            }
            break;
        }

        default:
            stream << "?";
            break;
        }
    }

    return stream.str();
}

} // namespace objc
} // namespace umbrella
