#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "session_info.h"
#include "bloody_text.h"


namespace trog{

    class gameover_scene : public scene{ 
    private:
        bn::regular_bg_ptr _dead_dragon;
        bn::optional<bn::regular_bg_ptr> _menu;
        bn::vector<bn::sprite_ptr, 8> _challengeagain_text_sprites, _hiscores_highlight_sprites, _back_text_sprites, _hiscores_text_sprites;        
        bn::vector<bn::sprite_ptr, 16> _new_high_score_text_sprites;
        bn::vector<bn::sprite_ptr, 3> _secret_sprites;
        bn::vector<bn::sprite_animate_action<6>, 3> _secret_anims;
        bn::sprite_text_generator _small_red_gen,
            _serif_red_gen, _serif_white_gen, _serif_gray_gen;
        
        session_info &_sesh;
        bloody_text _itsover_text;
        common_stuff &_common_stuff;
        uint8_t _timer, _menu_option;

        //todo make this a global or something
        void set_current_menu_option_visible();        
    public:
        explicit gameover_scene(session_info &sesh, common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
