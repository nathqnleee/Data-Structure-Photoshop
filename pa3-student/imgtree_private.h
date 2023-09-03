/**
 * @file imgtree_private.h
 * @description student declarations of private functions
 *              for ImgTree, CPSC 221 2023S PA3
 *
 *              THIS FILE WILL BE SUBMITTED.
 *
 *              Simply declare your function prototypes here.
 *              No other scaffolding is necessary.
 */

// begin your declarations below
void RenderHelper(Node* currNode, PNG& img) const;
Node* CopyHelper(const Node* currNode);
void ClearHelper(Node* currNode);
void PruneHelper(Node* &root, double tol);
void FlipHorizontalHelper(Node* currNode);
bool ToleranceTestHelper(Node* curr, HSLAPixel pixel, double tol);
void DeleteRootHelper(Node*& root);
void FlipCurrentNode(Node* curr);