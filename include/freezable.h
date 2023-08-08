#pragma once
#include "entity.h"

namespace trog {
    class freezable : public entity { 
        protected:

            unsigned short _freeze_timer;
            
        public:
            freezable(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite)
             : entity(xcor, ycor, width, height, sprite), _freeze_timer(0) {}

            virtual void update(){
                --_freeze_timer;
                if(_freeze_timer % 4 == 0){
                    set_x(_pos.x() + 1);
                }else if (_freeze_timer % 4 == 2){
                    set_x(_pos.x() - 1);
                }
            }
             

            virtual void freeze()=0;
            virtual void alert()=0;
    };
}