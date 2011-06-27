#ifndef ID_H_INCLUDED
#define ID_H_INCLUDED

#include <cstddef>
#include <cstdio>

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
        node* free_buffer;

    public:
        cID_dispatch();
        cID_dispatch(int limit);
        int ID_getid();
        void ID_returnid(int id);
} cID_dispatch;

#endif // ID_H_INCLUDED
