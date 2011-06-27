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
    ID_limit = !0 >> 1;
    free_buffer = NULL;
    printf("The max ID value is: %d\n", ID_limit);
    return;
}

cID_dispatch::cID_dispatch(int limit) {
    ID_counter = 0;
    ID_limit = limit;
    free_buffer = NULL;
    return;
}

int cID_dispatch::ID_getid() {
    if ( free_buffer == NULL ) return ++ID_counter;
    int new_id = free_buffer->value;

    free_ID* temp = free_buffer;
    free_buffer = free_buffer->next;
    delete(temp);

    return new_id;
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
