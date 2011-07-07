#include "priority.h"

#define ePQ(member) iQ->member

#define MAX(a,b) a >= b ? a : b
#define MIN(a,b) a <= b ? a : b

#define LEFT(curr) curr * 2
#define RIGHT(curr) (curr * 2) + 1
#define PARENT(curr) curr / 2;

/* This was originally written in C so you will find a mix of C/C++
 * elements. It could be re-written in C++ and made very readable.
 */
/*------------------ Priority Queue Implementation -------------*/

p_queue* new_pqueue(int max_size, bool resize, bool (*prop) (void*, void*)) {
	p_queue* temp = (p_queue* ) malloc( sizeof(p_queue) );
	temp->data = (void** ) malloc((max_size + 1) * sizeof(void*));
	temp->end = 0;
	temp->max = max_size;
	temp->rescale = resize;
	temp->property = prop;
	temp->left = 1;
	temp->right = 1;
	return temp;
}

void* pq_extract(p_queue* iQ) {
	if (ePQ(end) == 0) return NULL;

	int end_index = ePQ(end);
	void* return_val = ePQ(data)[1];
	ePQ(data)[1] = ePQ(data)[end_index];
	ePQ(end)--;

	int left,right,largest,current;
	largest = MIN(1,ePQ(end));
	current = largest;
	left = LEFT(current);
	right = RIGHT(current);

	bool (*check_prop) (void*, void*) = ePQ(property);

	while (largest <= ePQ(end) && largest > 0) {
		if (left <= ePQ(end)) {
			if ( (*check_prop) (ePQ(data)[largest], ePQ(data)[left]) ) {
				largest = left;
			}
		}

		if (right <= ePQ(end)) {
			if ( (*check_prop) (ePQ(data)[largest], ePQ(data)[right]) ) {
				largest = right;
			}
		}

		if (largest == current) return return_val;

		void* temp = ePQ(data)[current];
		ePQ(data)[current] = ePQ(data)[largest];
		ePQ(data)[largest] = temp;
		current = largest;
		left = LEFT(current);
		right = RIGHT(current);
	}
	return return_val;
}

void heapify(p_queue* iQ, int location) {
	int parent = PARENT(location);
	bool (*check_prop) (void*, void*) = ePQ(property);
	while ( parent > 0) {
		if ( (*check_prop) (ePQ(data)[parent], ePQ(data)[location])) {
			void* temp = ePQ(data)[parent];
			ePQ(data)[parent] = ePQ(data)[location];
			ePQ(data)[location] = temp;

			location = parent;
			parent = PARENT(location);
		} else {
			return;
		}
	}
}

bool pq_insert(p_queue* iQ, void* item) {
	int end_index = ePQ(end);
	if (end_index == ePQ(max)) return false;
	++end_index;
	ePQ(data)[end_index] = item;
	ePQ(end) = end_index;
	heapify(iQ, end_index);
	return true;
}

bool min_heap(void* parent, void* child) {
	if (*((int*) parent) > *((int*) child)) return true;
	return false;
}

bool max_heap(void* parent, void* child) {
	if (*((int*) parent) < *((int*) child)) return true;
	return false;
}

void* pq_top(p_queue* iQ) {
    return ePQ(data)[1];
}

/*------------------ End Priority Queue ------------------------*/
