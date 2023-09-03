/**
 * @file main.cpp
 * @description Partial test of PA3 functionality
 *              CPSC 221 2023S
 * 
 *              Add your own images and testing code to this
 */

#include "cs221util/HSLAPixel.h"
#include "cs221util/PNG.h"
#include "imgtree.h"

using namespace cs221util;
using namespace std;

/**
 * Test function declarations
 */
void TestBuild();
void TestCopy();
void TestFlipHorizontal();
void TestPrune();
void TestPruneFlip();

int main()
{
	TestBuild();
	cout << "Exited TestBuild." << endl;

	TestCopy();
	cout << "Exited TestCopy." << endl;

	TestFlipHorizontal();
	cout << "Exited TestFlipHorizontal" << endl;

	TestPrune();
	cout << "Exited TestPrune" << endl;

	TestPruneFlip();
	cout << "Exited TestPruneFlip" << endl;

  return 0;
}

void TestBuild() {
	cout << "Entered TestBuild..." << endl;

	// read in an image
	PNG origimg;
	origimg.readFromFile("images-original/colours-8x6.png");

	// use it to build a ImgTree
	ImgTree t1(origimg);

	// render to file
	PNG outimg = t1.Render();

	// write the rendered PNG to file
	outimg.writeToFile("images-output/colours-8x6-render.png");
}

void TestCopy() {
	cout << "Entered TestCopy..." << endl;

	// read in two images
	PNG img1, img2;
	img1.readFromFile("images-original/bc-257x240.png");
	img2.readFromFile("images-original/colours-8x6.png");

	// build three trees
	ImgTree t1(img1); // first tree from img1
	ImgTree t2(t1);   // second tree as copy of t1
	ImgTree t3(img2); // third tree from img2

	// reassign t1 to check that t1 and t2 contents are separate
	t1 = t3;

	// render t1 and t2 to files
	// t1 should contain "colours" data, t2 should contain "bc" data
	PNG outimg1 = t1.Render();
	PNG outimg2 = t2.Render();

	// write the rendered PNGs to files
	outimg1.writeToFile("images-output/colours-8x6-operatorequals.png");
	outimg2.writeToFile("images-output/bc-copyconstructor.png");
}

void TestFlipHorizontal() {
	cout << "Entered TestFlipHorizontal..." << endl;

	// read in an image
	PNG origimg;
	origimg.readFromFile("images-original/fnr-354x200.png");

	// use it to build a ImgTree
	ImgTree t1(origimg);

	// flip the tree
	t1.FlipHorizontal();

	// render the ImgTree
	PNG outimg = t1.Render();

	// write the PNG to file
	outimg.writeToFile("images-output/fnr-flip.png");
}

void TestPrune() {
	cout << "Entered TestPrune..." << endl;

	// read in an image
	PNG origimg;
	origimg.readFromFile("images-original/tri-640x426.png");

	// use it to build a ImgTree
	ImgTree t1(origimg);

	// it might be useful to find out how many total nodes
	// and how many leaves there are before and after pruning

	// prune the tree
	t1.Prune(0.05);

	// render the ImgTree
	PNG outimg = t1.Render();

	// write the PNG to file
	outimg.writeToFile("images-output/tri-prune005.png");
}

void TestPruneFlip() {
	cout << "Entered TestPruneFlip..." << endl;

	// read in an image
	PNG origimg;
	origimg.readFromFile("images-original/kkkk-kmnn-480x270.png");

	// use it to build a ImgTree
	ImgTree t1(origimg);

	// prune the tree, followed by a flip
	t1.Prune(0.1);
	t1.FlipHorizontal();

	// render the ImgTree
	PNG outimg = t1.Render();

	// write the PNG to file
	outimg.writeToFile("images-output/kkkk-prune01-flip.png");
}