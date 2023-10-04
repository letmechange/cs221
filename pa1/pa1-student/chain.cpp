/**
 * File: chain.cpp
 * Description: student implementation for Chain functions
 *              for CPSC 221 2023S PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
 */

#include <math.h>

#include "chain.h"

/**
 * Constructs a Chain from an input image
 * Each Node will cover a nodedimension x nodedimension (in pixels)
 * region in the source image.
 * @param img the input image
 * @param nodedimension number of pixels along the width and height of the Block region
 * @pre input image's width and height are evenly divisible by nodedimension
 */
Chain::Chain(PNG& img, unsigned int nodedimension) {

	length_ = 0;
	Block block;
	unsigned w = img.width();
	unsigned h = img.height();
	unsigned x = 0;
	unsigned y = 0;
	unsigned l_x = w - nodedimension;
	unsigned l_y = h - nodedimension;
	unsigned cols = w / nodedimension;
	unsigned rows = h / nodedimension;
	Node* node = new Node();
	NW = node;
	PNG png = PNG(20,20);
	for (unsigned j = 0; j < rows; j++) {
		for (unsigned i = 0; i < cols; i++) {
			std::cout << "line " << __LINE__ << ": x = " << x << std::endl;
			std::cout << "line " << __LINE__ << ": y = " << y << std::endl;
			block.Build(img, x,y,nodedimension);
			if (x == l_x && y == l_y) {
				node->data = block;
			} else {
				Node* curr = new Node();
				node->data = block;
				node->next = curr;
				curr->prev = node;
				node = node->next;
			}
			length_++;
			x += nodedimension;
		}
		y += nodedimension;
		x = 0;
	}
	SE = node;

}

/**
 * Renders the Chain's pixel data into an output PNG.
 * The PNG should be sized according to the number of nodes
 * specified by the cols parameter.
 * It is possible that there may not be enough nodes to fill
 * the bottom row of the PNG. If this happens, just render
 * as many nodes as there are, from left to right, and leave
 * the rest of the row as opaque white pixels.
 * @pre this Chain is not empty
 * @param cols the number of Blocks to use for the width of the image
 */
PNG Chain::Render(unsigned int cols) {
	if (IsEmpty()) {
		return PNG();
	}
	unsigned l = length_;
	Node* node = NW;
	unsigned k = 0;
	unsigned d = node->data.Dimension();
	if (l%cols == 0) {
		k = l/cols;
	} else {
		k = l/cols + 1;
	}
	unsigned img_height =  k*d;
	unsigned img_width = cols*d;
	PNG png = PNG(img_width, img_height);
	for (unsigned j = 0; j < k; j++)
	{
		for (unsigned i = 0; i < cols; i++) { 
			if (node != NULL) {
				node->data.Render(png, i*d, j*d);
				node = node->next;
			} 		
		}
	}
	return png;
}

/**
 * Inserts a new Node containing ndata at the back of the Chain
 */
void Chain::InsertBack(const Block& ndata) {

	Node* i_node = new Node();
	Node* pre = SE;

	i_node->data = ndata; 
	i_node->prev = pre;
	pre->next = i_node;
	SE = i_node;
}

/**
 * Reverses the structure of the list. This must be done by
 * pointer assignments. You may not deallocate or allocate any Nodes.
 * Example:
 *	before:	NW -> A <-> B <-> C <-> D <-> E <-> F <-> G <-> H <- SE
 *  
 *  after:	NW -> H <-> G <-> F <-> E <-> D <-> C <-> B <-> A <- SE
 */
void Chain::Reverse() {

	unsigned l = length_;
	Node* start = NW;
	Node* end = SE;
	Node* curr = NW;

	for (unsigned i=0; i<l; i++) {
		Node* curr_next = curr->next;
		Node* curr_prev = curr->prev;
		curr->next = curr_prev;
		curr->prev = curr_next;
		curr = curr_next;
	}

	SE = start;
	NW = end;
	
}

/**
 * Rearranges the Node structure and internal pixel data to be flipped over a vertical axis.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *  before, flipping with 3 columns (2 rows):
 *
 *		NW -> A> <-> B> <-> C> <->
 *            D> <-> E> <-> F> <- SE
 * 
 *  after, visualized with 3 columns (2 rows):
 * 
 *		NW -> <C <-> <B <-> <A <->
 *            <F <-> <E <-> <D <- SE
 * 
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
 */
void Chain::FlipHorizontal(unsigned int cols) {
	unsigned l = length_;
	Node* curr = NW;
	unsigned row = l/ cols;
	vector<vector<Node*>> n_matrix;

	for (unsigned i = 0; i<row; i++) {
		vector<Node*> n_col;
		for (unsigned j = 0; j<cols; j++) {
			n_col.push_back(curr);
			curr = curr->next;
		}
		n_matrix.push_back(n_col);
	}
	


	vector<Node*> r_col;
	for (unsigned i=0; i<row; i++) {
		for (unsigned j=cols; j>0; j--) {
			r_col.push_back(n_matrix[i][j-1]);
		}
	}

	for (unsigned x = 0; x < l; x++) {
		Node* r_data = r_col[x];
		r_data->data.FlipHorizontal();
		if (x==0) {
			NW = r_data;
			r_data->prev = NULL;
			r_data->next = r_col[x+1];
		} else if (x==l-1) {
			SE = r_data;
			r_data->next = NULL;
			r_data->prev = r_col[x-1];
		} else {
			r_data->prev = r_col[x-1];
			r_data->next = r_col[x+1];
		}
	}

}


/**
 * Rearranges the Node structure and internal pixel data to be rotated counter-clockwise.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *	before, rotating with 3 columns (2 rows):
 *            ^     ^     ^
 *      NW -> A <-> B <-> C <->
 *            ^     ^     ^
 *            D <-> E <-> F <- SE
 * 
 *  after, visualized with 2 columns (3 rows):
 * 
 *      NW -> <C <-> <F <->
 *            <B <-> <E <->
 *            <A <-> <D <- SE
 * 
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
 */
void Chain::RotateCCW(unsigned int cols) {
	unsigned l = length_;
	Node* curr = NW;
	unsigned row = l/ cols;
	vector<vector<Node*>> n_matrix;
	for (unsigned i = 0; i<row; i++) {
		vector<Node*> n_col;
		for (unsigned j = 0; j<cols; j++) {
			n_col.push_back(curr);
			curr = curr->next;
		}
		n_matrix.push_back(n_col);
	}
	
	vector<Node*> r_col;
	for (unsigned l = cols; l > 0; l--) {
		for (unsigned m =0; m<row; m++) {
			r_col.push_back(n_matrix[m][l-1]);
		}
	}

	for (unsigned x = 0; x < l; x++) {
		Node* r_data = r_col[x];
		r_data->data.RotateCCW();
		if (x==0) {
			NW = r_data;
			r_data->prev = NULL;
			r_data->next = r_col[x+1];
		} else if (x==l-1) {
			SE = r_data;
			r_data->next = NULL;
			r_data->prev = r_col[x-1];
		} else {
			r_data->prev = r_col[x-1];
			r_data->next = r_col[x+1];
		}
	}
}

/**
 * Destroys all dynamically allocated memory associated with
 * this Chain object. Called by destructor and operator=.
 * You must complete its implementation for PA1.
 */
void Chain::Clear() {
	if (SE == NULL) {return;}
	if (NW == NULL) {return;}
	Node* tmp = NW;

	for (unsigned i=0; i<length_; i++) {
		tmp = tmp->next;
		delete(tmp->prev);
		length_--;
	}
	delete tmp;

	// Node* node = NW;
	// delete NW;
	// while(length_ > 1) {
	// 	node = node->next;
	// 	delete(node->prev);
	// 	length_--;
	// }
	// delete(SE);
	// delete(SE);
	NW = NULL;
	SE = NULL;
	// length_--;
}

/**
 * Copies the parameter other Chain into the current Chain.
 * Does not free any memory. Called by copy constructor and
 * operator=.
 * You must complete its implementation for PA1.
 * @param other The Chain to be copied.
 */
void Chain::Copy(const Chain& other) {
	Node* other_node = other.NW;

	length_ = other.Length();
	
	vector<Node*> nodes;
	for (unsigned i=0; i<length_; i++) {
		Node* tmp = new Node();
		nodes.push_back(tmp);
	}

	for (unsigned j=0; j<length_-1; j++) {
		nodes[j]->next = nodes[j+1];
		nodes[j+1]->prev = nodes[j];
		nodes[j]->data = other_node->data;
		other_node = other_node->next;
	}

	nodes[length_-1]->data = other_node->data;
	SE = nodes[length_-1];
	NW = nodes[0];




	// NW = other.NW;
	// SE = other.SE;
	// length_ = other.length_;
	// Node* node = new Node();
	// NW = node;
	// for (unsigned i = 0; i < length_; i++) {
	// 	Node* curr_next = new Node();
	// 	node->data = other_node->data;
	// 	node->next = curr_next;
	// 	node->next->prev = node;
	// 	node = node->next;
	// 	other_node = other_node->next;
	// }
	// SE = node;
}

/**
 * If you have declared any private helper functions in chain_private.h,
 * add your completed implementations below.
 */
