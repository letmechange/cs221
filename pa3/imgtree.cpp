/**
 * @file imgtree.cpp
 * @description implementation of ImgTree class used for storing image data
 *              CPSC 221 2023S PA3
 *
 *              SUBMIT THIS FILE WITH YOUR MODIFICATIONS
 */

#include "imgtree.h"

/* ImgTree constructor */
ImgTree::ImgTree(const PNG& imIn){ 
	Stats s = Stats(imIn);
	height = imIn.height();
	width = imIn.width();

	pair<unsigned int, unsigned int> ul;
	pair<unsigned int, unsigned int> lr;
	
	ul.first = 0;
	ul.second = 0;
	lr.first= width-1;
	lr.second = height-1;
	root = BuildTree(s, ul, lr);
}

/* BuildTree helper for ImgTree constructor */
Node* ImgTree::BuildTree(Stats& s, pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr) {
	unsigned int rec_height = lr.second - ul.second + 1;
	unsigned int rec_width = lr.first - ul.first + 1;
	pair<unsigned int, unsigned int> ul_tmp;
	pair<unsigned int, unsigned int> lr_tmp;

	pair<unsigned int, unsigned int> ul_split;
	pair<unsigned int, unsigned int> lr_split;
	double e = 500000000000.0;
	double curr_e = 0;

	if (rec_height == 0 && rec_width ==0) return NULL;

	// parent node
	Node* node = new Node(ul, lr, s.GetAvg(ul,lr));


	// only one pixel return this node
	if (rec_height == 1 && rec_width == 1) {
		return node;
	}



	double area1;
	double area2;

	// tall image
	if (rec_height > rec_width) {
		ul_tmp.first = ul.first;
		lr_tmp.first = lr.first;

		for (unsigned i=ul.second+1; i<=lr.second; i++) {
			ul_tmp.second = i;
			lr_tmp.second = i-1;
			area1 = s.RectArea(ul, lr_tmp);
			area2 = s.RectArea(ul_tmp, lr);
			curr_e = (double)(area1/(area1+area2)) * (s.Entropy(ul, lr_tmp)) + (double)(area2/(area1+area2)) * (s.Entropy(ul_tmp, lr));
			if (curr_e < e) {
				e = curr_e;
				ul_split.first = ul_tmp.first;
				ul_split.second = ul_tmp.second;
				lr_split.first = lr_tmp.first;
				lr_split.second = lr_tmp.second;
			}
		}
	
	} else {
		ul_tmp.second = ul.second;
		lr_tmp.second = lr.second;

		for (unsigned i=ul.first+1; i<=lr.first; i++) {
			ul_tmp.first = i;
			lr_tmp.first = i-1;
			area1 = s.RectArea(ul, lr_tmp);
			area2 = s.RectArea(ul_tmp, lr);
			curr_e = area1/(area1+area2) * s.Entropy(ul, lr_tmp) + area2/(area1+area2) * s.Entropy(ul_tmp, lr);
			if (curr_e < e) {
				e = curr_e;
				ul_split.first = ul_tmp.first;
				ul_split.second = ul_tmp.second;
				lr_split.first = lr_tmp.first;
				lr_split.second = lr_tmp.second;
			}
		}

	}
	node->LT = BuildTree(s,ul, lr_split);
	node->RB = BuildTree(s,ul_split, lr);
	return node;

}

/* Render your ImgTree into a png */
PNG ImgTree::Render() const{
	PNG renderImg = PNG(width,height);
	Render(root, &renderImg);
	return renderImg;
}

void ImgTree::Render(const Node* nd, PNG* renderIm) const {
	if (nd== NULL) return;
		

	
	for (unsigned i = nd->upLeft.first; i <= nd->lowRight.first; i++) {
		for (unsigned j = nd->upLeft.second; j <= nd->lowRight.second; j++) {
			HSLAPixel* pixel = renderIm->getPixel(i,j);
			*pixel = nd->avg;
		}
	}

	Render(nd->LT, renderIm);
	Render(nd->RB, renderIm);


	
}
/* Prune function modifies tree by cutting off
 * subtrees whose leaves are all within tol of 
 * the average pixel value contained in the root
 * of the subtree
 */
void ImgTree::Prune(double tol){
	// complete your implementation below
    Prune(root, tol);
}

void ImgTree::Prune(Node* node, double tol) {
	if (node == NULL || (node->LT == NULL && node->RB == NULL)) {
		return;
	}
	HSLAPixel pixel = node->avg;
	int num = CountLeaves(node);

	if ((checkT(node->LT, tol, pixel) + checkT(node->RB, tol, pixel)) >= num) {
		Clear(node->LT);
		Clear(node->RB);
	} else {
		Prune(node->LT, tol);
		Prune(node->RB, tol);
	}
}


int ImgTree::checkT(Node* node, double tol, HSLAPixel avg_pixel) {
	if (node==NULL) {
		return 0;
	}

	if (node->LT == NULL && node->RB == NULL) {
		if (node->avg.distanceTo(avg_pixel) <= tol) {
			return 1;
		} else {
			return 0;
		}
	} else {
		return checkT(node->LT, tol, avg_pixel) + checkT(node->RB, tol, avg_pixel);
	}
}

/* Modifies node contents so that the rendered tree
 * will appear to be flipped horizontally across
 * a vertical axis
 */
void ImgTree::FlipHorizontal() {
	// complete your implementation below
    FlipHorizontal(root, width);
}

void ImgTree::FlipHorizontal(Node* node, unsigned int w) {
	if (node == NULL) {
		return;
	}

	unsigned rec_width = node->lowRight.first - node->upLeft.first + 1;
	unsigned rec_height = node->lowRight.second - node->upLeft.second + 1;
	Node* l = node->LT;
	Node* r = node->RB;

	
	if (rec_width > rec_height) {
		node->LT = r;
		node->RB = l;
	}

	unsigned int lt = node->upLeft.first;
	node->upLeft.first = w - node->lowRight.first - 1;
	node->lowRight.first = w- lt -1;



	FlipHorizontal(l, w);
	FlipHorizontal(r, w);

	



	
}
/* helper function for destructor and op= */
/* frees dynamic memory associated w the ImgTree */
void ImgTree::Clear() {
	// complete your implementation below
	
    Clear(root);
}

void ImgTree::Clear(Node*& node) {
	if (node == NULL) return;
		
	Clear(node->LT);
	Clear(node->RB);
	
	delete node;
	node = NULL;
}

/* helper function for copy constructor and op= */
void ImgTree::Copy(const ImgTree& orig){
	// complete your implementation below
	
	width = orig.width;
	height = orig.height;
    root = Copy(orig.root);
}

Node* ImgTree::Copy(Node* node){
	if (node == NULL) {
		return NULL;
	}
	
	Node* newRoot = new Node(node->upLeft, node->lowRight, node->avg);
	
	newRoot->LT = Copy(node->LT);
	
	newRoot->RB = Copy(node->RB);

	return newRoot;
}

/**
 * Add your private ImgTree function implementations below
 */
