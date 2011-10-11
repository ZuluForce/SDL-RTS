#ifndef MENU_ITEMS_H_INCLUDED
#define MENU_ITEMS_H_INCLUDED

#include "physics.h"
#include "Audio/audio_manager.h"
#include "Actors/simple_actors.h"
#include "FastDelegate.h"
#include "FastDelegateBind.h"

using namespace fastdelegate;

typedef FastDelegate1<int&,void> std_clbck;
typedef FastDelegate1<int*,void> array_clbck;
typedef SDL_Surface* surfp;

/* Warning:
   Other than standard menu_buttons, these menu objects
   have very flaky functionality. Rather than use a pre-existing
   api like wxWidgets I wanted to build a menu system from the
   ground up using parts of the engine I have already created.

   There may be a way to use wxWidgets within the SDL window and if
   so I would highly recommend using that for the time being.
   */

/* Future Intentions:

   I intend to build a good set of menu objects that one would
   normally use but using them straight from their implementation
   is/will be very tedious where everything must be set manually.

   I intend to build a wrapper around this that will automatically
   set up the buttons for you or possibly another program all together
   that will allow you to visually set up the menu how you wish and
   it will build the code required for it.
   */


class menu_obj : public cActor {
    protected:

        collision_zone click_box;
        std_clbck callback;
        vector<Uint8> _event_binds;

        /*
        bool click_state;
        bool hover_state;
        */

        virtual void call_clbck();
        virtual void change_img(surfp);
    public:
        virtual void set_ID(int);
        virtual void reg_callback(std_clbck);
        virtual void show();
        virtual void hide();
        bool visible();
};

class menu_button: public menu_obj {
    protected:
        sDisplay_info curr_info;

        SDL_Surface* std;
        SDL_Surface* hover;
        SDL_Surface* clicked;

        collision_zone click_box;

        std_clbck callback;
        std_fuse click_delay;

        bool click_state;
        bool hover_state;

        void call_clbck();
        void change_img(surfp);
    public:
        menu_button(int,int,SDL_Surface*,SDL_Surface*,SDL_Surface*);
        virtual ~menu_button();

        int set_priority(int);
        void set_ID(int);
        void set_typeID(int);
        void reg_callback(std_clbck);

        vector<Uint8>* event_binds();

        bool check();
        void check_events(event_vector** events, int* load, Uint8* key_states);
        sDisplay_info* get_display();
        void show();
        void hide();
};

class back_button : public menu_button {
	private:
		int *menu_list;

		array_clbck callback;
		void call_clbck();

	public:
		back_button(int x, int y, surfp std, surfp hover, surfp clicked);
		void set_menu_list(int* list, bool clear);
		void reg_callback(array_clbck);
};

class mute_button : public menu_button {
    private:
        bool muted;
        int saved_vol;
        void call_clbck();
        void change_img(surfp);
    public:
        mute_button(int x, int y, surfp std, surfp hover, surfp clicked);
        ~mute_button();
        int music_vol();
};

class menu_slider : public menu_obj {
    protected:
        /* Things being overloaded from cActor or other base classes */

        /* --------------------------------------------------------- */
        surfp scale;
        surfp s_load;
        surfp slider;

        bool horiz; /* true = horizontal orientation */
        Uint8 style;

        static_obj* slider_actor;
        static_obj* load_bar;
        SDL_Rect load_clip; /* Used for blitting the load overlay onto the scale */

        int s_bound[2]; /* {low, high} */
        int ret_vals[2];
        int ret_range;

        bool click_state;

        void move_slider(int&,int&,bool call_foo = true);

        void move_load_horiz(int x);
        void move_load_vert(int y);

    public:
        menu_slider(int x, int y, surfp scale, surfp s_load, surfp slider);
        void set_slider_bound(int low, int high);
        void set_slider_pos(int percent);

        void set_return_val(int low, int high);
        /* If !horiz, default to vertical */
        /* Style 1 : Adds ticks above the scale w/ values and places current pos under
           Style 2 : Excludes displaying the current slider position under
           Style 3 : Excludes the tick marks
           */
        void set_style(bool horiz, Uint8 style);

        void set_typeID(int);

        void check_events(event_vector** events, int* load, Uint8* key_states);
        /* Functions being overridden */
        int set_priority(int);
        char* get_name();

        void show();
        void hide();
        /* -------------------------- */
};

void build_click_box(int x, int y, SDL_Surface* std, collision_zone& click_box);
void build_click_box(int x, int y, int w, int h, collision_zone& click_box);
void move_click_box(int x, int y, collision_zone& click_box);
void print_click_box(collision_zone&);
#endif // MENU_ITEMS_H_INCLUDED
