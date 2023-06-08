#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_sprite_animate_actions.h>
#include <bn_random.h>
#include "scene.h"
#include "session_info.h"


namespace trog{

    class level_win_scene : public scene{ 
    private:
        bn::sprite_text_generator& _text_generator;
        bn::vector<bn::sprite_ptr, 8> _nicework_text_sprites;
        bn::vector<bn::sprite_ptr, 8> _levelbeated_text_sprites;
        bn::sprite_ptr _flames;
        bn::sprite_animate_action<4> _burningflames;

        session_info &_sesh;

        unsigned short _timer;

    public:
        explicit level_win_scene(session_info &sesh, bn::sprite_text_generator &text_generator, bn::random &random);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
