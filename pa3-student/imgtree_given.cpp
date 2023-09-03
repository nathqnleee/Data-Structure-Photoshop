/**
 * @file imgtree_given.cpp
 * @description partial implementation of ImgTree class used for storing image data
 *              CPSC 221 2023S PA3
 *
 *              THIS FILE WILL NOT BE SUBMITTED
 */

#include "imgtree.h"

 /* node constructor given */
Node::Node(pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr, HSLAPixel a)
	:upLeft(ul), lowRight(lr), avg(a), LT(NULL), RB(NULL)
{}

/* destructor given */
ImgTree::~ImgTree() {
	Clear();
}

/* copy constructor given */
ImgTree::ImgTree(const ImgTree& other) {
	Copy(other);
}

/* operator= given */
ImgTree& ImgTree::operator=(const ImgTree& rhs) {
	if (this != &rhs) {
		Clear();
		Copy(rhs);
	}
	return *this;
}

/**
 * Counts the number of nodes in the tree
 */
unsigned int ImgTree::CountNodes() {
	return CountNodes(root);
}

/**
 * Counts the number of leaves in the tree
 */
unsigned int ImgTree::CountLeaves() {
	return CountLeaves(root);
}

/**
 * Private helper function for counting the total number of nodes in the tree. GIVEN
 * @param nd the root of the subtree whose nodes we want to count
 */
unsigned int ImgTree::CountNodes(Node* nd) {
	if (nd == NULL) {
		return 0;
	}
	return 1 + CountNodes(nd->LT) + CountNodes(nd->RB);
}

/**
 * Private helper function for counting the number of leaves in the tree. GIVEN
 * @param nd the root of the subtree whose leaves we want to count
 */
unsigned int ImgTree::CountLeaves(Node* nd) {
	if (nd == NULL) {
		return 0;
	}
	else if (nd->LT == NULL && nd->RB == NULL) {
		return 1;
	}
	else
		return CountLeaves(nd->LT) + CountLeaves(nd->RB);
}