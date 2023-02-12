#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_actions.h>
#include <bn_unordered_map.h>
#include "scene.h"
#include "player.h"
#include "peasant.h"
#include "kerrek.h"
#include "strongbad.h"
#include "heart.h"
#include "session_info.h"




namespace trog{

    class movie_scene : public scene{ 
    protected:

        bn::vector<bn::unique_ptr<entity>, 20> _cutscene_objects;
        bn::vector<bn::sprite_ptr, 20> _text_sprites;

        unsigned short _timer, _cutscene_length;

        bn::fixed_point _start_pos, _finish_pos;


        bn::sprite_text_generator &_text_generator;
        
        session_info &_sesh;

        bool cutscene_over();

        // void update_ending_cutscene();
        // void update_lv5();

        void write_text(const char* str);

    public:
        [[nodiscard]] virtual bn::optional<scene_type> update();
        explicit movie_scene(session_info &sesh, bn::sprite_text_generator &text_generator);

    };
}
