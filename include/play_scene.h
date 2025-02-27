#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_affine_bg_ptr.h>
#include <bn_affine_bg_actions.h>
#include <bn_sprite_text_generator.h>
#include <bn_forward_list.h>
#include "scene.h"
#include "constants.h"
#include "session_info.h"
#include "trogdor.h"
#include "hud.h"
#include "cottage.h"
#include "peasant_factory.h"
#include "archer_factory.h"
#include "knight.h"
#include "troghammer.h"
#include "big_text.h"
#include "tutorial_box.h"
#include "tutorial_arrow.h"
#include "common_stuff.h"

namespace trog{

    class troghammer_sound{
        private:
            const unsigned short _phrase;
            unsigned short _timer;
            const unsigned short _length;
            bn::fixed _vol;
        public:
            troghammer_sound(const unsigned short &phrase, const bn::fixed &volume);
            bool done() const {return _timer > _length;} 
            void update();
    };

    class play_scene : public scene{ 
    private:
        
        session_info& _sesh;
        common_stuff &_common_stuff;
        bn::unique_ptr<player> _player;
        hud& _hud;
        peasant_factory _pfact;
        archer_factory _afact;

        unsigned long _total_time;

        bn::sprite_text_generator _serif_red_gen;


        unsigned short _burninate_pause_time, _win_pause_time, _autosave_visibility_time, _shake_timer;
        bool _player_paused;
        uint8_t _timer, _tutorial_cutscene_timer;
        int8_t _pause_menu_index;

        bn::optional<uint8_t> _shake_interval;

        bn::regular_bg_ptr _countryside;
        bn::optional<bn::regular_bg_ptr>_pause_menu, _gray_bg;

        bn::unique_ptr<big_text> _overlay_text;
        bn::vector<bn::sprite_ptr, 4> _paused_selected_option, _paused_label;
        

        bn::vector<cottage, TROG_MAX_COTTAGES> _cottages; 
        bn::forward_list<peasant, 20> _peasants;
        bn::forward_list<archer, 4> _archers;

        //there will be at most 2 knights: red knight, blue knight.
        bn::forward_list<knight, 2> _knights;
        bn::unique_ptr<troghammer> _troghammer;
        bn::unique_ptr<troghammer_sound> _th_sound;
        bn::optional<bn::sprite_ptr> _void_tower;
        bn::unique_ptr<tutorial_arrow> _tutorial_arrow;

        bn::unique_ptr<tutorial_box> _tutorial_box;
        bn::optional<bn::fixed> _voices_volume, _shake_y_offset;

        bool level_complete();

        //probably would be better if i figured out how to harness friends for this.
        // haha wouldn't it be great if i knew how to make friends? 
        static bool peasant_deletable(peasant &p) {return p.remove_from_map();}
        static bool archer_deletable(archer &a) {return a.remove_from_map();}

        void set_visible(bool visible);

        void set_paused_text_visible(const bool &visible) {_common_stuff.set_sprite_arr_visible(_paused_selected_option, visible);}

        void autosave(bool just_died);

        void spawn_troghammer(bool alert);


        void update_tutorial();

        void redraw_pause_menu_option();

        void setup_pause_menu();
        void unpause();
        [[nodiscard]] bn::optional<scene_type> update_pause_menu();

        void respawn(const bool &iframes, const uint8_t &init_trogmeter=0);
        bn::vector<freezable *, 23> all_freezables();
        bn::vector<entity *, 33> all_entities();

        void shake_screen(bn::fixed yoffset, uint8_t interval, uint8_t num_shakes);

    public:
        play_scene(session_info& sesh, hud& hud, common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };
}
