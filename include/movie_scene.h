#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_actions.h>
#include <bn_sound_items.h>
#include <bn_unordered_map.h>
#include "scene.h"
#include "trogdor.h"
#include "peasant.h"
#include "kerrek.h"
#include "strongbad.h"
#include "heart.h"
#include "session_info.h"




namespace trog{

    class movie_scene : public scene{ 
    protected:

        bn::vector<bn::unique_ptr<entity>, 60> _cutscene_objects;
        bn::vector<bn::sprite_ptr, 20> _text_sprites;

        bn::sprite_text_generator _serif_white;

        
        const scene_type _last_scene;
        unsigned short _timer, _cutscene_length;

        bn::fixed_point _start_pos, _finish_pos;

        common_stuff &_common_stuff;
        
        session_info &_sesh;

        cottage _dummy_cottage;

        bool cutscene_over();
        
        void write_text(bn::string<32> str);

        void draw_dragon(const dragon &dtype);

    public:
        [[nodiscard]] virtual bn::optional<scene_type> update();
        explicit movie_scene(session_info &sesh, common_stuff &common_stuff, const scene_type &last_scene);

    };
}
