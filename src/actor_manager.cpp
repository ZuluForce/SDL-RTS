#define INIT_ACTOR_ARRAY_SIZE 30

extern bool quit_threads;

cActor_manager::cActor_manager() {
    AM_thread = NULL;
    cID_dispatch* = new cID_dispatch();

    static_objs = malloc(INIT_ACTOR_ARRAY_SIZE * sizeof(cActor_static*));
    dynamic_objs = malloc(INIT_ACTOR_ARRAY_SIZE * sizeof(cActor_dynamic*));
    active_objs = malloc(INIT_ACTOR_ARRAY_SIZE * sizeof(cActor_active*));

    static_count = dynamic_count = active_count = 0;
    static_max = dynamic_max = active_max = INIT_ACTOR_ARRAY_SIZE;

    return;
}

void cActor_manager::AM_register(cActor_static* obj) {
    if ( static_count >= static_max ) {
        fprintf(stderr, "Resizing available static object slots: %s on line %d\n",__FILE__,__LINE__);
        return;
    }
    obj->ID = actor_id_manager->ID_getid();
    static_objs[static_count++] = obj;
    return;
}

void cActor_manager::AM_register(cActor_dynamic* obj) {
    if ( dynamic_count >= dynamic_max ) {
        fprintf(stderr, "Resizing available dynamic object slots: %s on line %d\n",__FILE__,__LINE__);
        return;
    }
    obj->ID = actor_id_manager->ID_getid();
    dynamic_objs[dynamic_count++] = obj;
    return;
}

void cActor_manager::AM_register(cActor_active* obj) {
if ( active_count >= active_max ) {
        fprintf(stderr, "Resizing available active object slots: %s on line %d\n",__FILE__,__LINE__);
        return;
    }
    obj->ID = actor_id_manager->ID_getid();
    active_objs[active_count++] = obj;
    return;
}

cActor_manager::AM_start_thread() {
    SDL_Thread* new_thread = SDL_CreateThread( AM_thread_loop );
    return new_thread;
}

cActor_manager::AM_thread_loop(void* ) {
    while ( quit_threads == false ) {

    }
}
