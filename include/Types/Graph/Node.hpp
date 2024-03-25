#pragma once

#include <memory>
#include <unordered_set>

template <typename T>
class Node
{
private:
    std::unique_ptr<T> value;
    std::unordered_set<std::weak_ptr<Node<T>>> links;

    void addLinkInner(std::weak_ptr<Node<T>> link);

public:
    Node(const T &_value);

    void addLink(std::weak_ptr<Node<T>> link);
};

template <typename T>
inline void Node<T>::addLinkInner(std::weak_ptr<Node<T>> link)
{
    links.insert(link);
}

template <typename T>
inline Node<T>::Node(const T &_value)
    : value(std::make_unique(_value)) {}

template <typename T>
inline void Node<T>::addLink(std::weak_ptr<Node<T>> link)
{
    links.insert(link);
    link.lock()->addLinkInner(std::weak_ptr<Node<T>>(std::make_shared(this)));
}
