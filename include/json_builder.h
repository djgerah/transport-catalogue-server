#pragma once

#include "json.h"
#include <string>
#include <vector>

namespace json
{
class Builder
{
    class BaseContext;
    class DictValueContext;
    class DictItemContext;
    class ArrayItemContext;

  public:
    Builder() : root_(), nodes_stack_{&root_}
    {
    }

    DictValueContext Key(std::string key);
    BaseContext Value(Node::Value value);
    DictItemContext StartDict();
    ArrayItemContext StartArray();
    BaseContext EndDict();
    BaseContext EndArray();
    Node Build();

  private:
    void AddObject(Node::Value value, bool one_shot);
    Node *GetTopNode();
    Node::Value &GetCurrentValue();
    const Node::Value &GetCurrentValue() const;

    Node root_;
    std::vector<Node *> nodes_stack_;

    class BaseContext
    {
      public:
        BaseContext(Builder &builder) : builder_(builder)
        {
        }

        DictValueContext Key(std::string key);
        BaseContext Value(Node::Value value);
        DictItemContext StartDict();
        ArrayItemContext StartArray();
        BaseContext EndDict();
        BaseContext EndArray();
        Node Build();

      private:
        Builder &builder_;
    };

    class DictItemContext : public BaseContext
    {
      public:
        DictItemContext(BaseContext base) : BaseContext(base)
        {
        }

        BaseContext Value(Node::Value value) = delete;
        DictItemContext StartDict() = delete;
        ArrayItemContext StartArray() = delete;
        BaseContext EndArray() = delete;
        Node Build() = delete;
    };

    class ArrayItemContext : public BaseContext
    {
      public:
        ArrayItemContext(BaseContext base) : BaseContext(base)
        {
        }

        ArrayItemContext Value(Node::Value value)
        {
            return BaseContext::Value(std::move(value));
        }

        DictValueContext Key(std::string key) = delete;
        BaseContext EndDict() = delete;
        Node Build() = delete;
    };

    class DictValueContext : public BaseContext
    {
      public:
        DictValueContext(BaseContext base) : BaseContext(base)
        {
        }

        DictItemContext Value(Node::Value value)
        {
            return BaseContext::Value(std::move(value));
        }

        DictValueContext Key(std::string key) = delete;
        BaseContext EndDict() = delete;
        BaseContext EndArray() = delete;
        Node Build() = delete;
    };
};
} // namespace json