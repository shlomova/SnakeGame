#ifndef __SNAKE__H__
#define __SNAKE__H__


#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#endif
// ░▒▓█
#define GAME_WIDTH (20)
#define GAME_HEIGHT (20)
#define WALLS_COLOR ('\xca')
#define SNAKE_COLOR (219)
#define ERASE_COLOR (' ')
#define FOOD_COLOR ('O')
#define CX 0
#define CY 1


#define snake_t uint8_t**
enum directions_enum
{
    UP, DOWN, RIGHT, LEFT, INVALID_DIRECTION
};
#define direction_t enum directions_enum

// Helping functions:
void print_snake(snake_t snake, size_t snake_size);
uint8_t* creat_new_link(uint8_t x, uint8_t y);
snake_t add_new_link(snake_t *snake_ptr, size_t snake_size, uint8_t* new);
uint8_t* snake_head(snake_t snake, size_t snake_size);
void get_next_snake_head_location(snake_t snake, size_t snake_size, direction_t direction, uint8_t next_snake_head_pos[2]);
void erase_snake_tail(snake_t snake, size_t snake_size);
void gotoxy(int x, int y);

// step 1
snake_t create_snake(uint8_t x, uint8_t y);
void free_snake(snake_t snake, size_t snake_size);

// step 2
// declare in up

// step 3
// returns the new snake size
size_t grow_snake(snake_t *snake_ptr, size_t snake_size, direction_t direction);

// step 4
void move_snake(snake_t snake, size_t snake_size, direction_t to_where);

// step 5
// in move_snake_test.c file
// don't touch:
bool are_snakes_equal(snake_t a, size_t a_size, snake_t b, size_t b_size);
snake_t create_long_snake(uint8_t x, uint8_t y, direction_t* growth_directions,size_t growth_directions_size);

// step 6
bool will_snake_reach_food_on_next_move(snake_t snake, size_t snake_size, const uint8_t * food,direction_t direction);

// step 7
void draw_at_xy(int x, int y, unsigned char color); //implemented
void draw_snake(snake_t snake, size_t snake_size);
void clear_snake(snake_t snake, size_t snake_size);
void clear_cursor(); //implemented
void draw_food(uint8_t* food);

// step 8
void set_new_food_location(snake_t snake, size_t snake_size, uint8_t food[2]);
bool is_food_on_snake(snake_t snake, size_t snake_size, const uint8_t food[2]);

// step 9
// Returns true if snakes will reach food after a single move in the given direction.
bool will_snake_collide_with_itself_on_next_move(snake_t snake, size_t snake_size, direction_t direction);
void draw_borders(); //implemented
bool will_snake_collide_border_on_next_move(snake_t snake, size_t snake_size, direction_t direction);
bool can_snake_move_in_direction(direction_t current_direction, direction_t requested_direction);


#endif