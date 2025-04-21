#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_palette_actions.h>
#include "scene.h"
#include "session_info.h"
#include "tutorial_box.h"
#include "player.h"


namespace trog{

    class char_unlock_scene : public scene{ 
    private:
        bn::regular_bg_ptr _happy_dragon;
        bn::unique_ptr<player> player_entity;
        bn::vector<bn::sprite_ptr, 64> _text_sprites;
        bn::sprite_ptr _a_button;
        bn::sprite_animate_action<2> _a_button_anim;

        // bn::optional<bn::sprite_palette_ptr> _flashing_palette;
        bn::optional<bn::sprite_palette_hue_shift_loop_action> 
            _hueshift;
        
        session_info &_sesh;
        common_stuff &_common_stuff;

        scene_type _next_scene;

        void save();

    public:
        explicit char_unlock_scene(
            session_info &sesh, 
            common_stuff &common_stuff,
            const scene_type &next_scene);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
