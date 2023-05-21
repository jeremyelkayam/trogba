#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include "scene.h"
#include "common_stuff.h"


namespace trog{

    class title_scene : public scene{ 
    private:
        common_stuff &_common_stuff;


        bn::regular_bg_ptr _titlebg, _titlegraphic;
        bn::vector<bn::sprite_ptr,2> _version_label;
        
        uint16_t _frame_counter;
        uint8_t _secret_code_index;

        bn::vector<bn::keypad::key_type, 10> _secret_code;

        

    public:
        title_scene(common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
