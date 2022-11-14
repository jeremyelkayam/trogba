#pragma once

#define TROG_STARTING_LIVES 3
#define TROG_STARTING_LEVEL 1

#define TROG_COUNTRYSIDE_LEFT_BOUND -120
#define TROG_COUNTRYSIDE_RIGHT_BOUND 120
#define TROG_COUNTRYSIDE_TOP_BOUND -70
#define TROG_COUNTRYSIDE_BOTTOM_BOUND 81


// The maximum amount of peasants contained in your trogmeter.
// After stomping this many peasants, burnination is achieved
#define TROG_TROGMETER_MAX 10

#define TROG_SPAWN_X 0
#define TROG_SPAWN_Y 0

// The length of burnination in frames.
#define TROG_BURNINATE_TIME 600

// the length of invulnerability post-respawn.
// TODO: tweak these to be closer to the real deal
#define TROG_RESPAWN_INV_TIME 90
#define TROG_RESPAWN_TIME 60

#define TROG_FIREBREATH_XOFFSET 30
#define TROG_FIREBREATH_YOFFSET -8

#define TROG_COTTAGEFIRE_YOFFSET -10
#define TROG_COTTAGEFIRE_TIME 120
#define TROG_COTTAGE_BREATHBURN_SCORE 5
#define TROG_COTTAGE_PEASANTBURN_SCORE 10

#define TROG_HUD_PEASANT_HEAD_WIDTH 8
#define TROG_HUD_PEASANT_HEAD_SPACING 1
#define TROG_HUD_TROGMETER_LEFTBOUND -45
#define TROG_HUD_TROGMETER_YCOR -75

// TODO: change this 
#define TROG_PEASANT_MIN_SPAWN_INTERVAL 60
#define TROG_PEASANT_MAX_SPAWN_INTERVAL 300

// TODO: also change this 
#define TROG_PEASANT_MIN_WALK_DISTANCE 2
#define TROG_PEASANT_SPEED 0.5
#define TROG_PEASANT_STOMP_SCORE 2


#define TROG_KNIGHT_WIDTH 12
#define TROG_KNIGHT_HEIGHT 18
#define TROG_KNIGHT_SPEED 0.5



#define TROG_DEFAULT_VOLUME 1