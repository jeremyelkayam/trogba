#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_actions.h>
#include <bn_sound_items.h>
#include <bn_unordered_map.h>
#include "scene.h"
#include "session_info.h"




namespace trog{

    class movie_scene : public scene{ 
    protected:

        // going to need to refactor this into a class for every cutscene
        // i hate this 
        bn::vector<bn::sprite_ptr, 20> _text_sprites;
        bn::unordered_map<const char *, bn::sprite_ptr, 16> _sprites;
        bn::unordered_map<const char *, bn::sprite_move_to_action, 16> _move_actions;

        // fuck this, todo make it better somehow idk 
        bn::unordered_map<const char *, bn::sprite_animate_action<4>, 8> _anim_actions;

        unsigned short _timer, _cutscene_length;

        bn::fixed_point _start_pos, _finish_pos;


        bn::sprite_text_generator &_text_generator;
        
        session_info &_sesh;

        bool cutscene_over();

        void update_ending_cutscene();
        void update_lv5();

        void write_text(const char* str);

    public:
        explicit movie_scene(session_info &sesh, bn::sprite_text_generator &text_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update();
    };

}
