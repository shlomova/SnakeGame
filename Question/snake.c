#include "snake.h"
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

#define ERASE_COLOR (' ')

#define CX 0
#define CY 1
uint8_t* creat_new_link(uint8_t x, uint8_t y);
snake_t add_new_link(snake_t *snake_ptr, size_t snake_size, uint8_t* new);

/// /// /// /// Helping functions: /// /// /// ///
void print_snake(snake_t snake, size_t snake_size) {
    if (snake == NULL || snake_size == 0) {
        // Handle invalid snake or size
        return;
    }

    for (int i = 0; i < snake_size; i++) {
        printf("%d) %d %d \n", i, snake[i][CX], snake[i][CY]);
    }
    printf("\n");
}

uint8_t *creat_new_link(uint8_t x, uint8_t y) {
    // Allocate memory for x,y values
    uint8_t *coordinate = malloc(2 * sizeof(uint8_t));
    // Assign values to the allocated memory
    coordinate[CX] = x;
    coordinate[CY] = y;
    return coordinate;
}

snake_t add_new_link(snake_t*snake_ptr, size_t snake_size, uint8_t *new) {
    if (snake_ptr == NULL || *snake_ptr == NULL || snake_size == 0) {
        // Handle invalid snake or size
        return 0;
    }

    snake_t s = malloc((snake_size + 1) * sizeof(uint8_t *));
    for (int i = 0; i < snake_size; i++) {
        // copy pointer for current x,y values
        s[i] = (*snake_ptr)[i];
    }
    s[snake_size] = new;
    free(*snake_ptr);
    return s;
}

uint8_t *snake_head(snake_t snake, size_t snake_size) {
    return snake[snake_size - 1];
}

void get_next_snake_head_location(snake_t snake, size_t snake_size, direction_t direction,
                                  uint8_t next_snake_head_pos[2]) {
    next_snake_head_pos[CX] = snake[snake_size - 1][CX];
    next_snake_head_pos[CY] = snake[snake_size - 1][CY];
    switch (direction) {
        case UP:
            next_snake_head_pos[CY] -= 1;
            break;
        case DOWN:
            next_snake_head_pos[CY] += 1;
            break;
        case RIGHT:
            next_snake_head_pos[CX] += 1;
            break;
        case LEFT:
            next_snake_head_pos[CX] -= 1;
            break;
        case INVALID_DIRECTION:
            break;
    }
}

void erase_snake_tail(snake_t snake, size_t snake_size) {
    draw_at_xy(snake[0][CX], snake[0][CY], ERASE_COLOR);
}


// TODO step 1
snake_t create_snake(uint8_t x, uint8_t y) {
    // Allocate memory for a pointer to a pointer to uint8_t
    snake_t s = malloc(sizeof(uint8_t *));

    // get pointer for x,y values
    *s = creat_new_link(x, y);

    return s;
}

void free_snake(snake_t snake, size_t snake_size) {
    if (snake == NULL) {
        // Handle invalid snake or size
        return;
    }

    for (int i = 0; i < snake_size; i++) {
        free(snake[i]);
    }
    free(snake);
}

//TODO step 2
//done

//TODO step 3
size_t grow_snake(snake_t*snake_ptr, size_t snake_size, direction_t direction) {
    if (snake_ptr == NULL || *snake_ptr == NULL || snake_size == 0) {
        // Handle invalid snake or size
        return 0;
    }
    uint8_t *next_snake_head_pos = malloc(2 * sizeof(uint8_t));
    uint8_t *new = NULL;
    get_next_snake_head_location(
            *snake_ptr, snake_size, direction,
            next_snake_head_pos
    );
    new = creat_new_link(next_snake_head_pos[CX], next_snake_head_pos[CY]);

    *snake_ptr = add_new_link(snake_ptr, snake_size, new);

    return snake_size + 1;
}

//TODO step 4

void move_snake(snake_t snake, size_t snake_size, direction_t to_where) {
    if (snake == NULL || snake_size == 0) {
        // Handle invalid snake or size
        return;
    }

    for (int coordinate = 0; coordinate < (snake_size - 1); coordinate++) {
        uint8_t *prev = snake[coordinate + 1];
        snake[coordinate][CX] = prev[CX];
        snake[coordinate][CY] = prev[CY];
    }
    get_next_snake_head_location(
            snake, snake_size, to_where,
            snake_head(snake, snake_size));

}

// TODO step 5
// correctly implemented, don't touch:
bool are_snakes_equal(snake_t a, size_t a_size, snake_t b, size_t b_size) {
    if (a_size != b_size) {
        return false;
    }
    for (int i = 0; i < a_size; i++) {
        if (a[i][CX] != b[i][CX] || a[i][CY] != b[i][CY]) {
            return false;
        }
    }
    return true;
}

snake_t create_long_snake(uint8_t x, uint8_t y, direction_t *growth_directions, size_t growth_directions_size) {
    snake_t snake = create_snake(x, y);
    for (int i = 0; i < growth_directions_size; i++) {
        grow_snake(&snake, (i + 1), growth_directions[i]);
    }
    return snake;
}

// TODO step 6

bool will_snake_reach_food_on_next_move(snake_t snake, size_t snake_size, const uint8_t *food, direction_t direction) {
    uint8_t next_head[2] = {snake_head(snake, snake_size)[CX], snake_head(snake, snake_size)[CY]};
    get_next_snake_head_location(snake, snake_size, direction, next_head);
    return food[CX] == next_head[CX] && food[CY] == next_head[CY];
}

// TODO step 7
void draw_at_xy(int x, int y, unsigned char color) {
#ifdef _WIN32
    // Windows-specific code
    COORD coord;
    coord.X = (SHORT) x;
    coord.Y = (SHORT) y;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(consoleHandle, coord);
#else
    // Unix/Linux/MacOS-specific code
    printf("\033[%d;%dH", y, x);
#endif
    printf("%c", color);
}

void draw_snake(snake_t snake, size_t snake_size) {
    for (int i = 0; i < snake_size; i++) {
        draw_at_xy(snake[i][CX], snake[i][CY], SNAKE_COLOR);
    }
}

void clear_snake(snake_t snake, size_t snake_size) {
    for (int i = 0; i < snake_size; i++) {
        draw_at_xy(snake[i][CX], snake[i][CY], ERASE_COLOR);
    }
}

void draw_food(uint8_t *food) {
    draw_at_xy(food[CX], food[CY], FOOD_COLOR);
    clear_cursor();
}

void clear_cursor() {
    draw_at_xy(GAME_WIDTH + 1, 0, ERASE_COLOR);
}

// TODO step 8
void set_new_food_location(snake_t snake, size_t snake_size, uint8_t food[2]) {
    while (is_food_on_snake(snake, snake_size, food)) {
        srand((unsigned int) time(NULL));
        // Generate
        food[CX] = (rand() % (GAME_WIDTH - 2)) + 1;
        food[CY] = (rand() % (GAME_HEIGHT - 2)) + 1;
    }
}

bool is_food_on_snake(snake_t snake, size_t snake_size, const uint8_t food[2]) {
    for (int i = 0; i < snake_size; i++) {
        if (food[CX] == snake[i][CX] && food[CY] == snake[i][CY]) {
            return true;
        }
    }
    return false;
}


// TODO step 9
bool will_snake_collide_with_itself_on_next_move(snake_t snake, size_t snake_size, direction_t direction) {
    uint8_t next_head[2] = {snake_head(snake, snake_size)[CX], snake_head(snake, snake_size)[CY]};
    get_next_snake_head_location(snake, snake_size, direction, next_head);

    for (int i = 0; i < (snake_size - 1); i++) {
        if (next_head[CX] == snake[i][CX] && next_head[CY] == snake[i][CY]) {
            return true;
        }
    }
    return false;
}

void draw_borders() {
    // vertical walls:
    for (int i = 0; i <= GAME_HEIGHT; i++) {
        draw_at_xy(0, i, WALLS_COLOR);
        draw_at_xy(GAME_WIDTH, i, WALLS_COLOR);
    }

    // Horizontal walls:
    for (int i = 0; i <= GAME_WIDTH; i++) {
        draw_at_xy(i, 0, WALLS_COLOR);
        draw_at_xy(i, GAME_HEIGHT, WALLS_COLOR);
    }
}

bool will_snake_collide_border_on_next_move(snake_t snake, size_t snake_size, direction_t direction) {
    uint8_t next_head[2] = {snake_head(snake, snake_size)[CX], snake_head(snake, snake_size)[CY]};
    get_next_snake_head_location(snake, snake_size, direction, next_head);
    if (next_head[CX] == 0 || next_head[CX] == GAME_WIDTH
        || next_head[CY] == 0 || next_head[CY] == GAME_HEIGHT) {
        return true;
    }
    return false;
}

bool can_snake_move_in_direction(direction_t current_direction, direction_t requested_direction) {
    switch (current_direction) {
        case UP:
            if (requested_direction == DOWN) return false;
            break;
        case DOWN:
            if (requested_direction == UP) return false;
            break;
        case RIGHT:
            if (requested_direction == LEFT) return false;
            break;
        case LEFT:
            if (requested_direction == RIGHT) return false;
            break;
        case INVALID_DIRECTION:
            break;
    }

    return true;
}

// TODO step 10
// enjoy!!