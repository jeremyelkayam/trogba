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
            bn::string<32> _name;
            bn::sprite_text_generator &_text_generator;
        public: 
            option(const bn::string<32> &name, bn::sprite_text_generator &text_generator, const bn::fixed &ycor);
            virtual ~option() = default;
            virtual void update() {}
            virtual void set_selected(const bool &selected);
            void hide(){_text_sprites.clear();}
            virtual bool taller() {return false;}
    };

    // dang ... hope our govt figures out that public option 
    class bool_option : public option {
        private:
            bool &_value;
            bn::sprite_ptr _checkbox;
        public:
            bool_option(const bn::string<32> &name, bn::sprite_text_generator &text_generator, const bn::fixed &ycor, bool &value);
            virtual void update();
            virtual void set_selected(const bool &selected);
    };
    class percent_option : public option {
        private:
            // bn::vector<bn::fixed, 5> _options;
            bn::fixed &_value;
            // uint8_t current_index();
            bn::vector<bn::sprite_ptr, 4> _vol_text_sprites;
            bn::vector<bn::sprite_ptr, 3> _vol_graph;
            bn::sprite_ptr _slider_bar;
        public:
            percent_option(const bn::string<32> &name, bn::sprite_text_generator &text_generator, const bn::fixed &ycor, bn::fixed &value);
            virtual void update();
            void update_text_and_slider();
            virtual void set_selected(const bool &selected);

    };

    class options_scene : public scene{ 
    private:

        common_stuff &_common_stuff;
        
        bn::vector<bn::sprite_ptr, 8> _header_sprites;
        bn::vector<bn::unique_ptr<option>, 8> _options_vec;
        bn::regular_bg_ptr _scroll;

        bool _go_to_credits;
        uint8_t _index;
        bn::fixed ycor(const uint8_t &index);

    public:
        explicit options_scene(common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
