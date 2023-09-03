/**
 * @file imgtree.h
 * @description declaration of ImgTree class used for storing image data
 *              CPSC 221 2023S PA3
 * 
 *              THIS FILE WILL NOT BE SUBMITTED
 */

#ifndef _IMGTREE_H_
#define _IMGTREE_H_

#include <utility>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "stats.h"

using namespace std;
using namespace cs221util;

/**
 * Like we had for PA1, the Node class *should be* private to the tree
 * class via the principle of encapsulation -- the end user does not
 * need to know our node-based implementation details.
 * Given for PA3, and made as a public class for convenience of testing and debugging.
 */
class Node {
    public:
        Node(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr, HSLAPixel a); // Node constructor

        pair<unsigned int, unsigned int> upLeft;
        pair<unsigned int, unsigned int> lowRight;
        HSLAPixel avg;
        Node* LT; // left or top child rectangle
        Node* RB; // right or bottom child rectangle
};

/**
 * ImgTree: This is a structure used in decomposing an image
 * into rectangles of similarly colored pixels.
 *
 * You should not remove anything from this class definition, but
 * you will find it helpful to add your own private helper functions
 * to imgtree_private.h
 */

class ImgTree {
    public:

        /* =============== start of given functions ====================*/

        /**
         * ImgTree destructor.
         * Destroys all of the memory associated with the
         * current ImgTree. This function should ensure that
         * memory does not leak on destruction of a ImgTree.
         */
        ~ImgTree();

        /**
         * Copy constructor for a ImgTree. GIVEN
         * Since ImgTrees allocate dynamic memory (i.e., they use "new", we
         * must define the Big Three). This depends on your implementation
         * of the copy funtion.
         *
         * @param other The ImgTree  we are copying.
         */
        ImgTree(const ImgTree& other);

        /**
         * Counts the number of nodes in the tree
         */
        unsigned int CountNodes();

        /**
         * Counts the number of leaves in the tree
         */
        unsigned int CountLeaves();

        /* =============== end of given functions ====================*/

        /* =============== public PA3 FUNCTIONS =========================*/

        /**
         * Constructor that builds an ImgTree out of the given PNG.
         * Every leaf in the tree corresponds to a pixel in the PNG.
         * Every non-leaf node corresponds to a rectangle of pixels 
         * in the original PNG, represented by an (x,y) pair for the 
         * upper left corner of the rectangle and an (x,y) pair for 
         * lower right corner of the rectangle. In addition, the Node
         * stores a pixel representing the average color over the 
         * rectangle. 
         *
         * Every node's left and right children correspond to a partition
         * of the node's rectangle into two smaller rectangles. The node's
         * rectangle is split by the horizontal or vertical line that 
         * results in the two smaller rectangles whose Information Gain
         * is as large as possible. (or correspondingly, whose weighted
         * sum of entropies is as small as possible. See online spec.)
         *
         * Note that splits will occur over the long side of the rectangle;
         * That is, wide rectangles will be split horizontally (over a vertical axis)
         * and tall rectangles will be split vertically (over a horizontal axis).
         * Square rectangles will be split horizontally.
         *
         * The LT child of the node will contain the upper left corner
         * of the node's rectangle, and the RB child will contain the
         * lower right corner. (see illustrations within the spec.)
         *
         * This function will build the stats object used to score the 
         * splitting lines. It will also call helper function BuildTree.
         */
        ImgTree(const PNG& imIn);

        /**
         * Overloaded assignment operator for ImgTrees. 
         * Part of the Big Three that we must define because the class
         * allocates dynamic memory. This depends on your implementation
         * of the copy and clear funtions.
         *
         * @param rhs The right hand side of the assignment statement.
         */
        ImgTree& operator=(const ImgTree& rhs);

        /**
         * Render returns a PNG image consisting of the pixels
         * stored in the tree. may be used on pruned trees. Draws
         * every leaf node's rectangle onto a PNG canvas using the 
         * average color stored in the node.
         */
        PNG Render() const;

        /**
         *  Prune function trims subtrees as high as possible in the tree.
         *  A subtree is pruned (cleared) if all of the subtree's leaves are within 
         *  tol of the average color stored in the root of the subtree. 
         *  Pruning criteria should be evaluated on the original tree, not 
         *  on any pruned subtree. (we only expect that trees would be pruned once.)
         *  
         * You may want a recursive helper function for this one.
         * 
         * @param this tree has not previously been pruned, nor is copied from a previously pruned tree.
         */
        void Prune(double tol);

        /**
         *  FlipHorizontal rearranges the contents of the tree, so that
         *  its rendered image will appear mirrored across a vertical axis.
         *  This may be called on a previously pruned tree.
         * 
         *  You may want a recursive helper function for this one.
         */
        void FlipHorizontal();

        /* =============== end of public PA3 FUNCTIONS =========================*/

    private:
        /*
         * Private member variables.
         *
         * You must use these as specified in the spec and may not rename them.
         */
        Node* root; // ptr to the root of the ImgTree

        unsigned int height; // height of PNG represented by the tree
        unsigned int width; // width of PNG represented by the tree

        /* =================== private PA3 functions ============== */

        /**
         * Destroys all dynamically allocated memory associated with the
         * current ImgTree class. Complete for PA3.
         * You may want a recursive helper function for this one.
         */
        void Clear();

        /**
        * Copies the parameter other ImgTree into the current ImgTree.
        * Does not free any memory. Called by copy constructor and op=.
        * You may want a recursive helper function for this one.
        * @param other The ImgTree to be copied.
        */
        void Copy(const ImgTree& other);

        /**
         * Private helper function for the constructor. Recursively builds
         * the tree according to the specification of the constructor.
         * @param s Contains the data used to split the rectangles
         * @param ul upper left point of current node's rectangle.
         * @param lr lower right point of current node's rectangle.
         */
        Node* BuildTree(Stats& s, pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr);

        /**
         * Private helper function for counting the total number of nodes in the tree. GIVEN
         * @param nd the root of the subtree whose nodes we want to count
         */
        unsigned int CountNodes(Node* nd);

        /**
         * Private helper function for counting the number of leaves in the tree. GIVEN
         * @param nd the root of the subtree whose leaves we want to count
         */
        unsigned int CountLeaves(Node* nd);

        /* =================== end of private PA3 functions ============== */

        /**
         * If you require more private member attributes and/or private functions,
         * declare them in imgtree_private.h
         */
        #include "imgtree_private.h"
};

#endif
