#pragma once
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "common_stuff.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"



namespace trog{

    class credits_scene : public scene{ 
    private:

        bn::vector<bn::sprite_ptr, 256> _text_sprites;

        bn::vector<bn::sprite_ptr, 4> _title_sprites;

        unsigned int _time;
        void setup_credits();

        common_stuff &_common_stuff;

    public:
        explicit credits_scene(common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
