#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "session_info.h"
#include "big_text.h"


namespace trog{

    class gameover_scene : public scene{ 
    private:
        session_info &_sesh;
        big_text _itsover_text;

    public:
        explicit gameover_scene(session_info &sesh, bn::sprite_text_generator &text_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
