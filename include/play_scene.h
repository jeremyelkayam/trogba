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
#include "text_box.h"
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
        bn::unique_ptr<player> _trogdor;
        hud& _hud;
        peasant_factory _pfact;
        archer_factory _afact;


        unsigned short _burninate_pause_time, _win_pause_time, _flashing_text_time, _autosave_visibility_time;
        bool _player_paused;
        uint8_t _tutorial_timer, _tutorial_cutscene_timer;

        bn::regular_bg_ptr _countryside;

        bn::unique_ptr<big_text> _overlay_text;
        bn::vector<bn::sprite_ptr, 8> _paused_text;

        bn::vector<cottage, TROG_MAX_COTTAGES> _cottages; 
        bn::forward_list<peasant, 20> _peasants;
        bn::forward_list<archer, 4> _archers;

        //there will be at most 2 knights: red knight, blue knight.
        bn::forward_list<knight, 2> _knights;
        bn::optional<troghammer> _troghammer;
        bn::optional<troghammer_sound> _th_sound;
        bn::optional<bn::sprite_ptr> _void_tower;
        bn::optional<tutorial_arrow> _tutorial_arrow;

        bn::optional<text_box> _text_box;

        bool level_complete();

        //probably would be better if i figured out how to harness friends for this.
        static bool peasant_deletable(peasant &p) {return p.remove_from_map();}
        static bool archer_deletable(archer &a) {return a.remove_from_map();}

        void set_visible(bool visible);

        void set_paused_text_visible(const bool &visible) {_common_stuff.set_sprite_arr_visible(_paused_text, visible);}

        void autosave(bool just_died);

        void spawn_troghammer(bool alert);


        void update_tutorial();

    public:
        play_scene(session_info& sesh, hud& hud, common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };
}
