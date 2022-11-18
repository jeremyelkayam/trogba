#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "bn_regular_bg_items_trogdead.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"


namespace trog{

    class gameover_scene : public scene{ 
    private:
        bn::regular_bg_ptr _dead_trogdor;
        // bn::sprite_text_generator& text_generator;
        bn::vector<bn::sprite_ptr, 64> _itsover_text_sprites;
        bn::vector<bn::sprite_ptr, 64> _challengeagain_text_sprites;
        
    public:
        explicit gameover_scene();
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
