// COS30008, Final Exam, 2022
#pragma once

#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class TernaryTreePrefixIterator;

template<typename T>
class TernaryTree
{
public:

    using TTree = TernaryTree<T>;
    using TSubTree = TTree*;
private:

    T fKey;
    TSubTree fSubTrees[3];
    // private default constructor used for declaration of NIL
    TernaryTree() : fKey(T())
    {
        for (size_t i = 0; i < 3; i++)
        {
            fSubTrees[i] = &NIL;
        }
    }
public:
    using Iterator = TernaryTreePrefixIterator<T>;

    static TTree NIL;           // sentinel
    // getters for subtrees
    const TTree& getLeft() const { return *fSubTrees[0]; }
    const TTree& getMiddle() const { return *fSubTrees[1]; }
    const TTree& getRight() const { return *fSubTrees[2]; }
    // add a subtree
    void addLeft(const TTree& aTTree) { addSubTree(0, aTTree); }
    void addMiddle(const TTree& aTTree) { addSubTree(1, aTTree); }
    void addRight(const TTree& aTTree) { addSubTree(2, aTTree); }

    // remove a subtree, may through a domain error
    const TTree& removeLeft() { return removeSubTree(0); }
    const TTree& removeMiddle() { return removeSubTree(1); }
    const TTree& removeRight() { return removeSubTree(2); }
    /////////////////////////////////////////////////////////////////////////
    // Problem 1: TernaryTree Basic Infrastructure
private:
    // remove a subtree, may throw a domain error [22]
    const TTree& removeSubTree(size_t aSubtreeIndex)
    {
        if (!(fSubTrees[aSubtreeIndex]->empty()))
        {
            TTree* Result = fSubTrees[aSubtreeIndex];
            fSubTrees[aSubtreeIndex] = &NIL;
            return *Result;
        }
        else 
        {
            throw domain_error("Subtree is NIL");
        }

    }

    // add a subtree; must avoid memory leaks; may throw domain error [18]
    void addSubTree(size_t aSubtreeIndex, const TTree& aTTree)
    {
        if (!empty())
        {
            if (aSubtreeIndex <= 2) 
            {
                if (fSubTrees[aSubtreeIndex]->empty())
                {
                    fSubTrees[aSubtreeIndex] = const_cast<TSubTree>(&aTTree);
                }
                else 
                {
                    throw domain_error("Subtree is not NIL");
                }
            }
            else 
            {
                throw domain_error("Illegal subtree index!");
            }
        }
        else 
        {
            throw domain_error("Empty Tree!");
        }
       
    }

public:

    // TernaryTree l-value constructor [10]
    TernaryTree(const T& aKey)
    {
        fKey = aKey;
        for (size_t i = 0; i < 3; i++)
        {
            fSubTrees[i] = &NIL;
        }
    }

    // destructor (free sub-trees, must not free empty trees) [14]
    ~TernaryTree()
    {
        if (!fSubTrees[0]->empty())
        {
            removeSubTree(0);
        }
        if (!fSubTrees[1]->empty())
        {
            removeSubTree(1);
        }
        if (!fSubTrees[2]->empty())
        {
            removeSubTree(2);
        }
    }

    // return key value, may throw domain_error if empty [2]
    const T& operator*() const
    {
        if (!fKey.empty())
        {
            return fKey;
        }
        else
        {
            throw domain_error("Operation not supported.");
        }
    }
    // returns true if this ternary tree is empty [4]
    bool empty() const
    {
        return this == &NIL;
    }
    // returns true if this ternary tree is a leaf [10]
    bool leaf() const
    {
        return fSubTrees[0]->empty() && fSubTrees[1]->empty() && fSubTrees[2]->empty();
    }
    // return height of ternary tree, may throw domain_error if empty [48]
    size_t height() const
    {
        if (empty())
        {
            throw domain_error("Operation not supported");
        }

        if (leaf())
        {
            return 0;
        }
        else 
        {
            size_t leftHeight = 0;
            size_t middleHeight = 0;
            size_t rightHeight = 0;

            if (!(fSubTrees[0])->empty())
            {
                leftHeight = fSubTrees[0]->height();
            }
            if (!(fSubTrees[1])->empty())
            {
                middleHeight = fSubTrees[1]->height();
            }
            if (!(fSubTrees[2])->empty())
            {
                rightHeight = fSubTrees[2]->height();
            }

            return max(max(leftHeight, rightHeight), middleHeight) + 1;
        }
    }

    /////////////////////////////////////////////////////////////////////////
    // Problem 2: TernaryTree Copy Semantics

    // copy constructor, must not copy empty ternary tree
    TernaryTree(const TTree& aOtherTTree)
    {
        *this = aOtherTTree;
    }
    // copy assignment operator, must not copy empty ternary tree
    // may throw a domain error on attempts to copy NIL
    TTree& operator=(const TTree& aOtherTTree)
    {
        if (!aOtherTTree.empty())
        {
            if (this != &aOtherTTree)
            {
                this->~TernaryTree();

                fKey = aOtherTTree.fKey;

                for (size_t i = 0; i <= 2; i++)
                {
                    if (!aOtherTTree.fSubTrees[i]->empty())
                    {
                        fSubTrees[i] = aOtherTTree.fSubTrees[i]->clone();
                    }
                    else 
                    {
                        fSubTrees[i] = &NIL;
                    }
                }
            }
            return *this;
        }
        else 
        {
            throw domain_error("Copying of NIL!");
        }
    }

    // clone ternary tree, must not copy empty trees
    TSubTree clone() const
    {
        if (!empty())
        {
            return new TTree(*this);
        }
        else 
        {
            throw domain_error("Cloning of NIL!");
        }
    }
    /////////////////////////////////////////////////////////////////////////
    // Problem 3: TernaryTree Move Semantics
    // TTree r-value constructor
    TernaryTree(T&& aKey) : fKey(move(aKey))
    {
        for (size_t i = 0; i < 3; i++)
        {
            fSubTrees[i] = &NIL;
        }
    }
    // move constructor, must not copy empty ternary tree
    TernaryTree(TTree&& aOtherTTree)
    {
        if (!empty())
        {
            *this = move(aOtherTTree);
        }
    }
    // move assignment operator, must not copy empty ternary tree
    TTree& operator=(TTree&& aOtherTTree)
    {
        if (!aOtherTTree.empty())
        {
            if (this != &aOtherTTree)
            {
                this->~TernaryTree();

                fKey = move(aOtherTTree.fKey);

                for (size_t i = 0; i <= 2; i++)
                {
                    if (!aOtherTTree.fSubTrees[i]->empty())
                    {
                        fSubTrees[i] = const_cast<TSubTree>(&aOtherTTree.removeSubTree(i));
                    }
                    else 
                    {
                        fSubTrees[i] = &NIL;
                    }
                }
            }
            return *this;
        }
        else 
        {
            throw domain_error("Moving NIL!");
        }
    }
    /////////////////////////////////////////////////////////////////////////
    // Problem 4: TernaryTree Prefix Iterator
    // return ternary tree prefix iterator positioned at start
    Iterator begin() const
    {
        return Iterator(const_cast<const TTree*>(fKey));
    }
    // return ternary prefix iterator positioned at end
    Iterator end() const
    {
        return Iterator(&TernaryTree<T>::NIL);
    }
};
template<typename T>
TernaryTree<T> TernaryTree<T>::NIL;
