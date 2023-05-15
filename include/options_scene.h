#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_unordered_map.h>

#include "scene.h"


#include "session_info.h"


namespace trog{

    class option {
        protected:
            bn::vector<bn::sprite_ptr, 8> _text_sprites;
            option() = default;
        public: 
            virtual ~option() = default;
            virtual void left() = 0;
            virtual void right() = 0;
            virtual void redraw(const bool &selected, bn::sprite_text_generator &text_generator, const bn::fixed &ycor) = 0;
            void hide(){_text_sprites.clear();}
    };

    // dang ... hope our govt figures out that public option 
    class bool_option : public option {
        private:
            bn::vector<bool, 2> _options;
            bool &_value;
            bn::string<32> _name;
        public:
            bool_option(const bn::string<32> &name, bool &value);
            virtual void left() final;
            virtual void right() final;
            virtual void redraw(const bool &selected, bn::sprite_text_generator &text_generator, const bn::fixed &ycor) final;
    };

    class options_scene : public scene{ 
    private:

        common_stuff &_common_stuff;
        
        bn::vector<bn::sprite_ptr, 8> _header_sprites;
        bn::vector<bn::unique_ptr<option>, 8> _options_vec;
        bn::regular_bg_ptr _scroll;

        bool _go_to_credits;
        uint8_t _index;

    public:
        explicit options_scene(common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
