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
/**
 * Private helper function for renderimg in the tree. GIVEN
 * @param nd the root of the subtree whose leaves we want to count
 */
void Render(const Node* nd, PNG* renderIm) const;
/**
 * Private helper function for checking whether all leves of subtree
*/
void Prune(Node* node, double tol);
/**
 * Private helper function for checking whether all leves of subtree
 * are within tol.
 */
int checkT(Node* node, double tol, HSLAPixel avg_pixel);

/**
 * Private helper function for flipping in horizontal
 */
void FlipHorizontal(Node* node, unsigned int w);


/**
 * Private helper function for clearing curr imge tree
 */
void Clear(Node*& node);


/**
 * Private helper function for copy given imge tree
 */
Node* Copy(Node* node);