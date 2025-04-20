#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sprite_ptr.h>
#include <bn_regular_bg_item.h>
#include "scene.h"
#include "session_info.h"
#include "common_stuff.h"


namespace trog{

    class menu_option {
        private:
            bn::vector<bn::sprite_ptr, 4> _text_sprites;
            bn::vector<bn::sprite_ptr, 4> _drop_shadow_sprites;
            bn::vector<bn::sprite_ptr, 4> _red_text_sprites;
            bn::fixed _x, _y, _width;
            scene_type _scene_type; 
        public:
            menu_option(const bn::fixed &x, 
                const bn::fixed &y, const char *text, 
                bn::sprite_text_generator& red_generator,
                bn::sprite_text_generator& gray_generator,
                bn::sprite_text_generator& white_generator,
                const scene_type &scene_type);
            bn::fixed y() const {return _y;}
            bn::fixed x() const {return _x;}
            bn::fixed width() const {return _width;}
            void turn_red();
            void turn_white();
            void set_y(const bn::fixed &y);
            const scene_type &next_scene() {return _scene_type;}
    };
    class menu_scene : public scene{ 
    protected:

        bn::vector<menu_option, 7> _menu_options;

        common_stuff & _common_stuff;

        bn::sprite_text_generator _red_gen, _gray_gen, _white_gen;

        bn::vector<bn::sprite_ptr, 64> _menu_text_sprites;

        bn::regular_bg_ptr _bg;

        
        int _selected_option_index;


    public:
        explicit menu_scene(common_stuff &common_stuff, 
            const bn::regular_bg_item &bgitem);

        bool done() {return false;}

        [[nodiscard]] virtual bn::optional<scene_type> update();
    };


}
