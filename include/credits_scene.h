#pragma once
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "common_stuff.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"



namespace trog{

    struct credit_line { 
        bn::vector<bn::sprite_ptr, 8> sprites;
        bn::fixed ycor;
        bn::string<40> str;
    };

    class credits_scene : public scene{ 
    private:

        const scene_type _last_scene;

        bn::vector<credit_line, 64> _credits;
        bn::optional<bn::regular_bg_ptr> _titlegraphic;

        void setup_credits();

        common_stuff &_common_stuff;

    public:
        explicit credits_scene(common_stuff &common_stuff, 
            const scene_type &last_scene);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
