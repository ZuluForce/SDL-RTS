#include <cstddef>
#include <cstdlib>

typedef struct {
	void** data;
	int end;
	int max;
	bool rescale;

    /* Used for walking the tree */
    int left,right;

	/* Return (true) to fix heap */
	/* Return (false) if heap is fine */
	bool (*property) (void*, void*);
} p_queue;

p_queue* new_pqueue(int max_size, bool resize, bool (*prop) (void*, void*));
bool pq_insert(p_queue* iQ, void* item);
void* pq_extract(p_queue* iQ);
bool min_heap(void* parent, void* child);
bool max_heap(void* parent, void* child);
void* pq_top(p_queue*);
