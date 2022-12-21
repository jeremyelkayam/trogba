#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_affine_bg_ptr.h>
#include <bn_affine_bg_actions.h>
#include <bn_sprite_text_generator.h>
#include <bn_forward_list.h>
#include "scene.h"
#include "constants.h"
#include "session_info.h"
#include "player.h"
#include "hud.h"
#include "cottage.h"
#include "peasant_factory.h"
#include "archer_factory.h"
#include "knight.h"
#include "troghammer.h"
#include "big_text.h"

namespace trog{

    class play_scene : public scene{ 
    private:
        
        session_info& _sesh;
        bn::unique_ptr<player> _trogdor;
        hud& _hud;
        peasant_factory _pfact;
        archer_factory _afact;


        unsigned short _burninate_pause_time;
        unsigned short _win_pause_time;
        bool _player_paused;

        bn::regular_bg_ptr _countryside;
        bn::sprite_text_generator &_text_generator;

        bn::unique_ptr<big_text> _overlay_text;
        bn::vector<bn::sprite_ptr, 8> _paused_text;

        bn::vector<cottage, TROG_MAX_COTTAGES> _cottages; 
        bn::forward_list<peasant, 20> _peasants;
        bn::forward_list<archer, 4> _archers;

        //there will be at most 2 knights: red knight, blue knight.
        bn::forward_list<knight, 2> _knights;
        bn::optional<troghammer> _troghammer;

        bool level_complete();

        //probably would be better if i figured out how to harness friends for this.
        static bool peasant_deletable(peasant &p) {return p.remove_from_map();}
        static bool archer_deletable(archer &a) {return a.remove_from_map();}

        void set_visible(bool visible);

        void set_paused_text_visible(bool visible);

    public:
        play_scene(session_info& sesh, hud& hud, bn::sprite_text_generator &text_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };
}
