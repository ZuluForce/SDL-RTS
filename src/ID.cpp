#include "ID.h"

using namespace std;

node::node() {
    value = 0;
    next = NULL;
    return;
}

node::node(int id, node* next_node) {
    value = id;
    next = next_node;
    return;
}

cID_dispatch::cID_dispatch() {
    ID_counter = 0;
    ID_limit = INT_MAX;
    recent_ID = -1;

    /* free_buffer is a singly-linked list of free ID's */
    free_buffer = NULL;
    return;
}

cID_dispatch::cID_dispatch(int limit) {
    ID_counter = 0;
    ID_limit = limit;
    recent_ID = -1;
    free_buffer = NULL;
    return;
}

int cID_dispatch::ID_getid() {
    if ( free_buffer == NULL ) {
        recent_ID = ID_counter;
        return ID_counter++;
    }
    int new_id = free_buffer->value;

    free_ID* temp = free_buffer;
    free_buffer = free_buffer->next;
    delete(temp);

    recent_ID = new_id;
    return new_id;
}

int cID_dispatch::ID_recall() {
    return recent_ID;
}

void cID_dispatch::ID_returnid(int id) {
    if ( id == ID_counter ) {
        --ID_counter;
    } else {
        free_ID* reclaimed = new free_ID(id, free_buffer);
        free_buffer = reclaimed;
    }
    return;
}
