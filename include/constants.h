#pragma once

#define SECONDS * 60

#define TROG_TITLE_TEXT_X 0
#define TROG_TITLE_TEXT_Y 52
#define TROG_TITLE_BG_X -3
#define TROG_TITLE_BG_Y 48

#define TROG_INSTRUCTION_TITLE_Y 57

#define TROG_STARTING_LIVES 3
#define TROG_STARTING_LEVEL 1

#define TROG_COUNTRYSIDE_LEFT_BOUND -120
#define TROG_COUNTRYSIDE_RIGHT_BOUND 120
#define TROG_COUNTRYSIDE_TOP_BOUND -70
#define TROG_COUNTRYSIDE_BOTTOM_BOUND 81


// The maximum amount of peasants contained in your trogmeter.
// After stomping this many peasants, burnination is achieved
#define TROG_TROGMETER_MAX 10

#define TROG_PLAYER_SPAWN_X 0
#define TROG_PLAYER_SPAWN_Y 0
#define TROG_PLAYER_WIDTH 20
#define TROG_PLAYER_HEIGHT 45

#define TROG_PLAYER_SPEED 1.5

// The length of burnination in frames.
#define TROG_BURNINATE_TIME (10 SECONDS)

// the length of invulnerability post-respawn.
// TODO: tweak these to be closer to the real deal
#define TROG_RESPAWN_INV_TIME (2 SECONDS)
#define TROG_RESPAWN_TIME (2 SECONDS)

#define TROG_FIREBREATH_XOFFSET 30
#define TROG_FIREBREATH_YOFFSET -8

#define TROG_COTTAGEFIRE_YOFFSET -10
#define TROG_COTTAGEFIRE_TIME (2 SECONDS)
#define TROG_COTTAGE_BREATHBURN_SCORE 5
#define TROG_COTTAGE_PEASANTBURN_SCORE 10

#define TROG_HUD_PEASANT_HEAD_WIDTH 8
#define TROG_HUD_PEASANT_HEAD_SPACING 1
#define TROG_HUD_TROGMETER_LEFTBOUND -65
#define TROG_HUD_TROGMETER_YCOR -75

#define TROG_HUD_BURNINATEMETER_LEFTBOUND (TROG_HUD_TROGMETER_LEFTBOUND - 3)
#define TROG_HUD_BURNINATEMETER_WIDTH 98
#define TROG_HUD_BURNINATEMETER_CENTER (TROG_HUD_BURNINATEMETER_LEFTBOUND + TROG_HUD_BURNINATEMETER_WIDTH / 2)

// TODO: change this to be more like the real trogdor
#define TROG_PEASANT_MIN_SPAWN_INTERVAL 20
#define TROG_PEASANT_MAX_SPAWN_INTERVAL 150

// TODO: also change this 
#define TROG_PEASANT_MIN_WALK_DISTANCE 2
#define TROG_PEASANT_SPEED 0.5
#define TROG_PEASANT_STOMP_SCORE 2


#define TROG_KNIGHT_WIDTH 12
#define TROG_KNIGHT_HEIGHT 18
#define TROG_KNIGHT_SPEED 0.5

#define TROG_ARCHER_DRAWBOW_WAITTIME 60
#define TROG_ARCHER_SHOOT_WAITTIME 90
#define TROG_ARCHER_DISAPPEAR_WAITTIME 120
#define TROG_ARCHER_MAX_HEIGHT -65
#define TROG_ARCHER_MIN_HEIGHT 70


#define TROG_ARROW_SPEED TROG_PLAYER_SPEED
#define TROG_ARROW_WIDTH 10
#define TROG_ARROW_HEIGHT 3

#define TROG_DEFAULT_VOLUME 1