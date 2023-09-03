/**
 * @file imgtree.cpp
 * @description implementation of ImgTree class used for storing image data
 *              CPSC 221 2023S PA3
 *
 *              SUBMIT THIS FILE WITH YOUR MODIFICATIONS
 */

#include "imgtree.h"
#include "stats.h"
#include <limits>
#include <cfloat>

/* ImgTree constructor */
ImgTree::ImgTree(const PNG& imIn){ 
    Stats s(imIn);

	width = imIn.width();
	height = imIn.height();

	root = BuildTree(s, make_pair<unsigned int,unsigned int>(0,0), make_pair<unsigned int, unsigned int>(width - 1, height - 1));
}

/* BuildTree helper for ImgTree constructor */
Node* ImgTree::BuildTree(Stats& s, pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr) {
    Node* currNode = new Node(ul, lr, s.GetAvg(ul, lr));

    unsigned int w = lr.first - ul.first + 1;
    unsigned int h = lr.second - ul.second + 1;

    if (w == 1 && h == 1) 
    {
        currNode->LT = nullptr;
        currNode->RB = nullptr;

        return currNode;
    }

    double minVal = DBL_MAX;

    unsigned int splitCoord = 0;

    if (w >= h) 
    { 
        for (unsigned int i = ul.first; i < lr.first; i++) 
        {
            double firstEntroVal = s.Entropy(ul, make_pair(i, lr.second)) * (i - ul.first + 1) / w;
            double secondEntroVal = s.Entropy(make_pair(i + 1, ul.second), lr) * (lr.first - i) / w;

            if (firstEntroVal + secondEntroVal < minVal) 
            {
                minVal = firstEntroVal + secondEntroVal;
                splitCoord = i;

            }
        }

        currNode->LT = BuildTree(s, ul, make_pair(splitCoord, lr.second));
        currNode->RB = BuildTree(s, make_pair(splitCoord + 1, ul.second), lr);
    } 
    
    else 
    { 
        for (unsigned int i = ul.second; i < lr.second; i++) 
        {
            double firstEntroVal = s.Entropy(ul, make_pair(lr.first, i)) * (i - ul.second + 1) / h;
            double secondEntroVal = s.Entropy(make_pair(ul.first, i + 1), lr) * (lr.second - i) / h;

            if (firstEntroVal + secondEntroVal < minVal)
            {
                minVal = firstEntroVal + secondEntroVal;
                splitCoord = i;

            }
        }

        currNode->LT = BuildTree(s, ul, make_pair(lr.first, splitCoord));
        currNode->RB = BuildTree(s, make_pair(ul.first, splitCoord + 1), lr);
    }

    return currNode;
}

/* Render your ImgTree into a png */
PNG ImgTree::Render() const {
    PNG img(width, height);

    RenderHelper(root, img);

    return img;
}

void ImgTree::RenderHelper(Node* currNode, PNG& img) const {
    if (currNode == nullptr) {
        return;
    }

    if (currNode->RB == nullptr && currNode->LT == nullptr) 
	{
        for (unsigned int i = currNode->upLeft.first; i <= currNode->lowRight.first && i < width; i++) 
		{
            for (unsigned int j = currNode->upLeft.second; j <= currNode->lowRight.second && j < height; j++) 
			{
                *img.getPixel(i, j) = currNode->avg;
            }
        }
    } 
	
	else 
	{
        RenderHelper(currNode->LT, img);
        RenderHelper(currNode->RB, img);
    }
}

/* Prune function modifies tree by cutting off
 * subtrees whose leaves are all within tol of 
 * the average pixel value contained in the root
 * of the subtree
 */
void ImgTree::Prune(double tol){
    PruneHelper(root, tol);
}

void ImgTree::PruneHelper(Node* &root, double tol) {
    if (ToleranceTestHelper(root, root->avg, tol)) 
    {
        DeleteRootHelper(root->LT);
        DeleteRootHelper(root->RB);
    } 
    
    else 
    {
        PruneHelper(root->LT, tol);
        PruneHelper(root->RB, tol);
    }
}

bool ImgTree::ToleranceTestHelper(Node* curr, HSLAPixel pixel, double tol) {
    if((curr->lowRight.first - curr->upLeft.first == 0) && (curr->lowRight.second - curr->upLeft.second == 0))
    {
        return (pixel.distanceTo(curr->avg) <= tol);
    }

    return (ToleranceTestHelper(curr->RB, pixel, tol) && ToleranceTestHelper(curr->LT, pixel, tol));
}

void ImgTree::DeleteRootHelper(Node*& root) {
    if (root == nullptr) 
    {
        return;
    }

    DeleteRootHelper(root->LT);
    DeleteRootHelper(root->RB);

    delete(root);
    
    root = nullptr;
}

/* Modifies node contents so that the rendered tree
 * will appear to be flipped horizontally across
 * a vertical axis
 */
void ImgTree::FlipHorizontal() {
    FlipHorizontalHelper(root);    
}

void ImgTree::FlipHorizontalHelper(Node* currNode) {
    if (currNode == nullptr) 
    {
        return;
    }

    pair<unsigned int, unsigned int> startCoord = currNode->upLeft;
    pair<unsigned int, unsigned int> endCoord = currNode->lowRight;

    currNode->upLeft = make_pair(width - endCoord.first - 1, startCoord.second);
    currNode->lowRight = make_pair(width - startCoord.first - 1, endCoord.second);


    if (currNode->lowRight.first - currNode->upLeft.first > currNode->lowRight.second - currNode->upLeft.second) 
    {
        Node* temp = currNode->LT;

        currNode->LT = currNode->RB;
        currNode->RB = temp;

        temp = nullptr;
    }

    FlipHorizontalHelper(currNode->LT);
    FlipHorizontalHelper(currNode->RB);
}

/* helper function for destructor and op= */
/* frees dynamic memory associated w the ImgTree */
void ImgTree::Clear() {
    ClearHelper(root);
    root = nullptr;  
}

void ImgTree::ClearHelper(Node* currNode) {
    if (currNode == nullptr) {
        return;
    }

    ClearHelper(currNode->LT);
    ClearHelper(currNode->RB);

    delete currNode;
}

/* helper function for copy constructor and op= */
void ImgTree::Copy(const ImgTree& orig){
    width = orig.width;

    height = orig.height;

    root = CopyHelper(orig.root);    
}

Node* ImgTree::CopyHelper(const Node* currNode) {
    if (currNode == NULL) {
        return NULL;
    }

    Node* newNode = new Node(currNode->upLeft, currNode->lowRight, currNode->avg);
    
    newNode->RB = CopyHelper(currNode->RB);

    newNode->LT = CopyHelper(currNode->LT);

    
    return newNode;
}

/**
 * Add your private ImgTree function implementations below
 */
