/**
 *  @file        stack.cpp
 *  @description Implementation of the Stack class.
 *
 *               SUBMIT THIS FILE WITH YOUR MODIFICATIONS
 */

/**
 * Default constructor. Remember to initialize any variables you need
 * and allocate any required memory. The initial size of `items` should
 * be DEFAULTCAPACITY defined in stack.h.
 */
template <class T>
Stack<T>::Stack()
{
	max_items = DEFAULTCAPACITY;
	num_items = 0;
	items = new T[max_items];
}

/**
 * Destructor. Remember to free any memory allocated.
 */
template <class T>
Stack<T>::~Stack()
{
	delete[] items;
}

/**
 * Adds the parameter object to the top of the Stack. That is, the
 * element should go at the end of the array. If the dynamic array
 * holding the stack elements is full the array should be resized. Resize should be
 * called by multiplying current capacity EXPANSIONFACTOR defined in stack.h.
 * To be clear resize should happen before adding the element to the stack and only
 * when the stack is completely full.
 *
 * @param item - the object to be added to the Stack.
 */
template <class T>
void Stack<T>::Push(const T& item) {
	if (num_items >= max_items) {
		Resize(max_items * EXPANSIONFACTOR);
	}

	items[num_items] = item;
	num_items++;

}

/**
 * Removes the object on top of the Stack, and returns it. That is, remove
 * the element at the end of the array. You may assume this function
 * is only called when the Stack is not empty. If the size of the stack changes
 * to strictly less than a ratio of 1.0 / SHRINKRATE (where SHRINKRATE is defined in stack.h)
 * after the item is removed the Stack should be resized to the larger of max_items / EXPANSIONFACTOR,
 * or DEFAULTCAPACITY.
 *
 * @return the element that used to be at the top of the Stack.
 */
template <class T>
T Stack<T>::Pop() {
	T pop_item = items[num_items - 1];
	if (num_items <= max_items/SHRINKRATE) {
		Resize(max_items/EXPANSIONFACTOR);
	}
	num_items--;
	return pop_item;
}

/**
 * Adds the given element to the ordering structure.
 * See OrderingStructure::Add()
 */
template <class T>
void Stack<T>::Add(const T& item)
{
	// complete your implementation below
	// Hint: this should call another Stack function
	//   to add the element to the Stack.
	Push(item);
}

/**
 * Removes an element from the ordering structure. You may assume
 * that this function is only called when the ordering structure is
 * not empty.
 * See OrderingStructure::Remove()
 */
template <class T>
T Stack<T>::Remove()
{
	// complete your implementation below
	// Hint: this should call another Stack function
	//   to remove an element from the Stack and return it.
  
	return Pop();
}

/**
 * Finds the object on top of the Stack, and returns it to the caller.
 * Unlike Pop(), this operation does not alter the Stack itself.
 * It should look at the end of the array. You may assume this function
 * is only called when the Stack is not empty.
 *
 * @return the element at the top of the Stack.
 */
template <class T>
T Stack<T>::Peek() {
	return items[num_items-1];
}

/**
 * Determines if the Stack is empty.
 * @return true if the stack is empty,
 *         false otherwise.
 */
template <class T>
bool Stack<T>::IsEmpty() const {
	return num_items == 0;
}

/**
 * Return the maximum number of items the stack can hold.
 *
 * Note: This is an implementation detail we would normally hide.
 * We include it here for testing purposes.
 * 
 * @return maximum number of items the stack can hold (int)
 */
template <class T>
size_t Stack<T>::Capacity() const {
	return max_items;
}

/**
 * Return the number of items on the stack.
 * @return number of items on the stack (int)
 */
template <class T>
size_t Stack<T>::Size() const {
	return num_items;
}

/**
 * Resize the the member variable `items`.
 * The minimum size is the number of items in the Stack.
 * Don't forget to free up any memory that is no longer used.
 * @pre n >= num_items, i.e. the new size has room for all items in the structure
 * @param n - the capacity of the stack after resizing
 */
template <class T>
void Stack<T>::Resize(size_t n) {
	max_items = n;
	T* new_i = new T[max_items];
	for (unsigned i = 0; i < num_items; i++) {
		new_i[i] = items[i];
	}

	delete[] items;
	items = new_i;

}
