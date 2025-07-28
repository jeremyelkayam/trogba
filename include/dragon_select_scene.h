#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_palette_actions.h>
#include "scene.h"
#include "session_info.h"
#include "tutorial_box.h"
#include "player.h"
#include "big_text.h"
#include "lock_icon.h"

namespace trog{
    class dragon_option {
        public:
            dragon dragon_type;
            bn::string<16> name, ability;
            bn::unique_ptr<entity> player_entity;
            bn::optional<lock_icon> lock;

            dragon_option(const dragon &dtype, 
                session_info &sesh, common_stuff &common_stuff,
                const bool &tutorial, const bool &locked);

            bool locked() const {return lock.has_value();}
            void move_to(const short &time, const bn::fixed &x, 
                const bn::fixed &y)
                { player_entity->move_to(time, x, y);
                if(lock) lock->move_to(time, x, y);}
        
            void update()
            {
                player_entity->update_anim();
                if(lock) lock->update_anim();
            }
            void scale_to(const int &duration_updates, const bn::fixed &scale_factor)
            {
                player_entity->scale_to(duration_updates,
                    scale_factor);
                if(lock) lock->scale_to(duration_updates,
                    scale_factor);
            }
            void set_scale(const bn::fixed &scale) {
                player_entity->set_scale(scale);
                if(lock) lock->set_scale(scale);
            }
            void set_x(const bn::fixed &xcor)
            {
                player_entity->set_x(xcor);
                if(lock) lock->set_x(xcor);

            }
    


    };

    class dragon_select_scene : public scene{ 
    private:
        uint8_t _index, _selection_timer, _selection_wait_time;
        bn::vector<dragon_option, 8> _selectable_dragons;
        bn::vector<bn::sprite_ptr, 16> _selected_text;
        bn::sprite_ptr _left_arrow, _right_arrow;
        bn::optional<bn::sprite_move_to_action> _left_arrow_move, 
            _right_arrow_move;
        bn::sprite_text_generator _serif_white, _small_white;

        big_text _title;


        session_info &_sesh;
        common_stuff &_common_stuff;

        void update_text();

        bn::fixed dragon_xcor(uint8_t index);

    public:
        explicit dragon_select_scene(session_info &sesh, common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
