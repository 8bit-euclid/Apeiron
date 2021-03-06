#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "Tree.h"

namespace aprn::graph {

/***************************************************************************************************************************************************************
* Alternating Digital Tree Node Data
***************************************************************************************************************************************************************/
struct ADTreeNode
{

};

/***************************************************************************************************************************************************************
* Alternating Digital Tree Class Definition
***************************************************************************************************************************************************************/
class ADTree : public Tree<ADTreeNode>
{
 public:
   ADTree() = default;

 private:
};

}
