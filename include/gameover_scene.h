#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "session_info.h"
#include "bloody_text.h"


namespace trog{

    class gameover_scene : public scene{ 
    private:
        bn::regular_bg_ptr _dead_trogdor;
        bn::sprite_text_generator& _text_generator;
        bn::vector<bn::sprite_ptr, 10> _challengeagain_text_sprites;
        
        session_info &_sesh;
        bloody_text _itsover_text;
    public:
        explicit gameover_scene(session_info &sesh, bn::sprite_text_generator &text_generator, bn::sprite_text_generator &big_text_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
