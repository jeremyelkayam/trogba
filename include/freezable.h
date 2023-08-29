#pragma once
#include "entity.h"
#include "bn_sprite_items_alert_icon.h"
#include <bn_assert.h>
#include <bn_log.h>

namespace trog {
    class freezable : public entity { 
        protected:

            unsigned short _freeze_timer;
            bn::sprite_ptr _alert_icon;
            
        public:
            freezable(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite)
             : entity(xcor, ycor, width, height, sprite), _freeze_timer(0),
             _alert_icon(bn::sprite_items::alert_icon.create_sprite(0,0)) 
             { _alert_icon.set_visible(false);}

            void update_freeze() {
                BN_ASSERT(_freeze_timer, "Enemy must be frozen (_freeze_timer != 0) to update freeze");
                --_freeze_timer;
                if(_freeze_timer % 4 == 0){
                    set_x(_pos.x() + 1);
                }else if (_freeze_timer % 4 == 2){
                    set_x(_pos.x() - 1);
                }
            }
            void update_alert(){
                BN_ASSERT(_alert_icon.visible(), "Enemy must be alerted to update alert");
                _alert_icon.set_position(_pos.x() + 2, _pos.y() - 16);
            }
             

            virtual void freeze()=0;
            virtual void alert(bn::fixed_point stomp_pos) {_alert_icon.set_visible(true); _alert_icon.set_position(_pos.x() + 2, _pos.y() - 16); BN_LOG("stomp pos x", stomp_pos.x(), "stomp pos y", stomp_pos.y());};
            bool alerted() {return _alert_icon.visible();}
    };
}