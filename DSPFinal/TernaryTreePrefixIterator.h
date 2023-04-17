#pragma once
#include "TernaryTree.h"
#include <stack>
using namespace std;
template<typename T>
class TernaryTreePrefixIterator
{
private:
    using TTree = TernaryTree<T>;
    using TTreeNode = TTree*;
    using TTreeStack = std::stack<const TTree*>;

    const TTree* fTTree;                // ternary tree
    TTreeStack fStack;                  // traversal stack
public:
    using Iterator = TernaryTreePrefixIterator<T>;

    Iterator operator++(int)
    {
        Iterator old = *this;
        ++(*this);

        return old;
    }

    bool operator!=(const Iterator& aOtherIter) const
    {
        return !(*this == aOtherIter);
    }
    /////////////////////////////////////////////////////////////////////////
    // Problem 4: TernaryTree Prefix Iterator
private:

    // push subtree of aNode [30]
    void push_subtrees(const TTree* aNode);
public:

    // iterator constructor [12]
    TernaryTreePrefixIterator(const TTree* aTTree)
    {
        fTTree = aTTree;
        if (!fTTree->empty())
        {
            fStack.push(fTTree);
            while (!fStack.top().getLeft().empty())
            {
                fStack.push(fStack.top().getLeft());
                fTTree = fTTree.getLeft();
            }
            fStack.pop();
        }
    }
    // iterator dereference [8]
    const T& operator*() const
    {
        return fTTree->fKey;
    }
    // prefix increment [12]
    Iterator& operator++()
    {
        if (fStack.empty())
        {
            fTTree = &TTree::NIL;
        }
        else
        {
            fTTree = fStack.top();
            fStack.pop();
            if (!fTTree->getRight().empty())
            {
                fStack.push(fTTree->getRight());
                while (!fStack.top().getLeft().empty())
                {
                    fStack.push(fStack.top().getLeft());
                }
            }
        }
        return *this;
    }
    // iterator equivalence [12]
    bool operator==(const Iterator& aOtherIter) const
    {
        return fTTree->fKey == aOtherIter.fTTree->fKey;
    }
    // auxiliaries [4,10]
    Iterator begin() const
    {
        return Iterator(*this);
    }

    Iterator end() const
    {
        return begin().end();
    }
};
