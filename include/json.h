#pragma once

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

using namespace std::literals;

namespace json
{
class Node;

using Dict = std::map<std::string, Node>;
using Array = std::vector<Node>;

class ParsingError : public std::runtime_error
{
  public:
    using runtime_error::runtime_error;
};

class Node final : private std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>
{
  public:
    using variant::variant;
    using Value = variant;

    /*
      Node() = default;
      Node(bool value);
      Node(Array array);
      Node(Dict map);
      Node(int value);
      Node(std::string value);
      Node(std::nullptr_t);
      Node(double value);
    */

    Node(Value value) : variant(std::move(value))
    {
    }

    const Array &AsArray() const;
    const Dict &AsDict() const;
    int AsInt() const;
    double AsDouble() const;
    bool AsBool() const;
    const std::string &AsString() const;

    bool IsNull() const;
    bool IsInt() const;
    bool IsDouble() const;
    bool IsPureDouble() const;
    bool IsBool() const;
    bool IsString() const;
    bool IsArray() const;
    bool IsDict() const;

    const Value &GetValue() const
    {
        return *this;
    }

    Value &GetValue()
    {
        return *this;
    }

    bool operator==(const Node &rhs) const
    {
        return GetValue() == rhs.GetValue();
    }

    bool operator!=(const Node &rhs) const
    {
        return !(GetValue() == rhs.GetValue());
    }

  private:
    // Value value_;
};

class Document
{
  public:
    Document() = default;

    explicit Document(Node root) : root_(std::move(root))
    {
    }

    const Node &GetRoot() const
    {
        return root_;
    }

    bool operator==(const Document &rhs) const
    {
        return root_ == rhs.root_;
    }

    bool operator!=(const Document &rhs) const
    {
        return !(root_ == rhs.root_);
    }

  private:
    Node root_;
};

Document Load(std::istream &input);

void Print(const Document &doc, std::ostream &output);
} // end namespace json