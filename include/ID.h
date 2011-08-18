#ifndef ID_H_INCLUDED
#define ID_H_INCLUDED

#include <cstddef>
#include <cstdio>
#include <climits>

typedef struct node {
    public:
        int value;
        node* next;
        node();
        node(int, node*);
} free_ID;

typedef class cID_dispatch {
    private:
        int ID_counter;
        int ID_limit;
        int recent_ID;
        node* free_buffer;

    public:
        cID_dispatch();
        cID_dispatch(int limit);
        int ID_getid();
        int ID_recall();
        void ID_returnid(int id);
} cID_dispatch;

#endif // ID_H_INCLUDED
