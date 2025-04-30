#pragma once

#include "trogdor.h"
#include "sucks.h"
#include "chiaroscuro.h"
#include "wormdingler.h"


namespace trog
{
    static player * create_dragon(const dragon &dtype,
        const bn::fixed &xcor, const bn::fixed &ycor,
        session_info &sesh, const bool &iframes, 
        common_stuff &common_stuff, 
        uint8_t initial_trogmeter=0)
    {
        switch(dtype)
        {
            case dragon::TROGDOR:
                return new trogdor(xcor, ycor, sesh, iframes,
                    common_stuff, initial_trogmeter);
            case dragon::SUCKS:
                return new sucks(xcor, ycor, sesh, iframes,
                    common_stuff, initial_trogmeter);
            case dragon::CHIAROSCURO:
                return new chiaroscuro(xcor, ycor, sesh, iframes,
                    common_stuff, initial_trogmeter);
            case dragon::WORMDINGLER:
                return new wormdingler(xcor, ycor, sesh, iframes,
                    common_stuff, initial_trogmeter);
            default:
                BN_ERROR("Invalid dragon type passed into create_dragon function");
        }
        return nullptr;
    }

};