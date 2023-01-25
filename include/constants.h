#pragma once

#define SECONDS * 60

#define DEBUG

#define TROG_TITLE_TEXT_X -85
#define TROG_TITLE_TEXT_Y -62
#define TROG_TITLE_BG_X -3
#define TROG_TITLE_BG_Y 48

#define TROG_FLASHING_TEXT_VISIBLE_TIME 40
#define TROG_FLASHING_TEXT_INVISIBLE_TIME 10

#define TROG_INSTRUCTION_TITLE_Y -56

#define TROG_STARTING_LIVES 3
#define TROG_STARTING_LEVEL 1
#define TROG_FORMAT_TAG "TROGv10"
#define TROG_POINTS_FOR_1UP 300

#define TROG_COUNTRYSIDE_LEFT_BOUND -120
#define TROG_COUNTRYSIDE_RIGHT_BOUND 120
#define TROG_COUNTRYSIDE_TOP_BOUND -66
#define TROG_COUNTRYSIDE_BOTTOM_BOUND 79

#define TROG_COUNTRYSIDE_PLAYER_TOP_BOUND -72

#define TROG_KNIGHT_LEFT_BOUND -110
#define TROG_KNIGHT_RIGHT_BOUND 110
#define TROG_KNIGHT_TOP_BOUND -58
#define TROG_KNIGHT_BOTTOM_BOUND 69

#define RED_PALETTE bn::sprite_items::trogdor_variable_8x16_font_red.palette_item()
#define WHITE_PALETTE bn::sprite_items::trogdor_variable_8x16_font.palette_item()
#define BROWN_PALETTE bn::sprite_items::trogdor_variable_8x16_font_brown.palette_item()
#define GRAY_PALETTE bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item()

// The maximum amount of peasants contained in your trogmeter.
// After stomping this many peasants, burnination is achieved
#define TROG_TROGMETER_MAX 1

#define TROG_PLAYER_SPAWN_X 40
#define TROG_PLAYER_SPAWN_Y 0
#define TROG_PLAYER_WIDTH 24
#define TROG_PLAYER_HEIGHT 40

#define TROG_PLAYER_SPEED bn::fixed(0.87)

// The length of burnination in frames.
#define TROG_BURNINATE_TIME (6 SECONDS)
#define TROG_BURNINATE_PAUSETIME (1 SECONDS)
#define TROG_BURNINATE_ANIM_TIME_BTWN_FRAMES 6 
#define TROG_WIN_PAUSETIME 150
#define TROG_MAJESTY_FLASH_INTERVAL 15

// the length of invulnerability post-respawn.
// TODO: tweak these to be closer to the real deal
#define TROG_RESPAWN_INV_TIME (2 SECONDS)
#define TROG_RESPAWN_TIME (2 SECONDS)

#define TROG_FIREBREATH_XOFFSET 30
#define TROG_FIREBREATH_YOFFSET -8

#define TROG_FIREBREATH_WIDTH 20
#define TROG_FIREBREATH_HEIGHT 12

#define TROG_MAX_COTTAGES 6

#define TROG_COTTAGEFIRE_YOFFSET -10
#define TROG_COTTAGEFIRE_TIME (2 SECONDS)
#define TROG_COTTAGE_BREATHBURN_SCORE 5
#define TROG_COTTAGE_PEASANTBURN_SCORE 10
#define TROG_COTTAGE_HITBOX_WIDTH 20
#define TROG_COTTAGE_HITBOX_HEIGHT 20

#define TROG_HUD_PEASANT_HEAD_WIDTH 8
#define TROG_HUD_PEASANT_HEAD_SPACING 1
#define TROG_HUD_TROGMETER_LEFTBOUND -65
#define TROG_HUD_TROGMETER_YCOR -75

#define TROG_HUD_BURNINATEMETER_LEFTBOUND (TROG_HUD_TROGMETER_LEFTBOUND - 3)
#define TROG_HUD_BURNINATEMETER_WIDTH 98
#define TROG_HUD_BURNINATEMETER_CENTER (TROG_HUD_BURNINATEMETER_LEFTBOUND + TROG_HUD_BURNINATEMETER_WIDTH / 2)

#define TROG_HUD_TEXT_SCROLL_SPEED 1

// TODO: change this to be more like the real trogdor
#define TROG_PEASANT_MIN_SPAWN_INTERVAL 40
#define TROG_PEASANT_MAX_SPAWN_INTERVAL 210

// TODO: also change this 
#define TROG_PEASANT_MIN_WALK_DISTANCE 2
#define TROG_PEASANT_SPEED TROG_PLAYER_SPEED*bn::fixed(0.5)
#define TROG_PEASANT_STOMP_SCORE 2

#define TROG_KNIGHT_WIDTH 11
#define TROG_KNIGHT_HEIGHT 16
#define TROG_RED_KNIGHT_SPAWN_X -59
#define TROG_RED_KNIGHT_SPAWN_Y 31
#define TROG_BLUE_KNIGHT_SPAWN_X 33
#define TROG_BLUE_KNIGHT_SPAWN_Y -50

#define TROG_KNIGHT_SPEED TROG_PLAYER_SPEED*bn::fixed(0.6)
#define TROG_KNIGHT_MOVE_CYCLE_TIME 180

#define TROG_HAMMER_SPEEDUP_FACTOR bn::fixed(1.4)
#define TROG_HAMMER_WIDTH 16
#define TROG_HAMMER_HEIGHT 18

#define TROG_ARCHER_DRAWBOW_WAITTIME 60
#define TROG_ARCHER_SHOOT_WAITTIME 90
#define TROG_ARCHER_DISAPPEAR_WAITTIME 120

#define TROG_ARCHER_MIN_SPAWN_HEIGHT -65
#define TROG_ARCHER_MAX_SPAWN_HEIGHT 70

// TODO change this
#define TROG_ARCHER_MIN_SPAWN_INTERVAL (15 SECONDS)
#define TROG_ARCHER_MAX_SPAWN_INTERVAL (30 SECONDS)

#define TROG_ARROW_SPEED TROG_PLAYER_SPEED*bn::fixed(1.75)
#define TROG_ARROW_WIDTH 10
#define TROG_ARROW_HEIGHT 3

//TREASURE HUT 
// #define TROG_COTTAGEINTERIOR_BG_X 119
#define TROG_COTTAGEINTERIOR_BG_X 0
#define TROG_COTTAGEINTERIOR_BG_Y 58

#define TROG_MONEYBAG_WIDTH 15
#define TROG_MONEYBAG_HEIGHT 15
#define TROG_MONEYBAG_POINTS 50

#define TROG_DEFAULT_VOLUME bn::fixed(0.75)

#define TROG_GAMEOVER_BG_X 8
#define TROG_GAMEOVER_BG_Y 43

#define TROG_GAMEOVER_MENU_X 8
#define TROG_GAMEOVER_MENU_Y 58

#define TROG_GAMEOVER_BIGTEXT_X 0
#define TROG_GAMEOVER_BIGTEXT_Y -50

#define TROG_GAMEOVER_SECRET_TEXT_X -50
#define TROG_GAMEOVER_SECRET_TEXT_Y 76
#define TROG_GAMEOVER_SECRET_SCORE 2000
#define TROG_GAMEOVER_BEAT_THAT_SCORE 1000