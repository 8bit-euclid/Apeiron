#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"

#include <memory>

namespace aprn::graph {

/***************************************************************************************************************************************************************
* Generic Tree Node Class Definitions
***************************************************************************************************************************************************************/

/** Node abstract base class */
template<class T>
struct Node
{
   T Data;
   size_t Depth;
   std::shared_ptr<Node<T>> Parent;

 protected:
   Node() = default;
};

/** Static node class (known number of children) */
template<class T, size_t N>
struct StaticNode final : public Node<T> { StaticArray<std::shared_ptr<Node<T>>, N> Children; };

/** Dynamic node class (unknown number of children) */
template<class T>
struct DynamicNode final : public Node<T> { DynamicArray<std::shared_ptr<Node<T>>> Children; };

/***************************************************************************************************************************************************************
* Generic Tree Class Definition
***************************************************************************************************************************************************************/
template<class T>
class Tree
{
 public:
   Tree() = default;

 private:
   std::shared_ptr<Node<T>> Root;
   DynamicArray<std::shared_ptr<Node<T>>> Nodes;
};

}
