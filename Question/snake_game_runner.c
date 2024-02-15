#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <conio.h>
#else
	#include <stdlib.h>
	#include <string.h>
	#include <termios.h>
	#include <unistd.h>
	#include <sys/select.h>
#endif

#include "snake_game_runner.h"
#include "snake.h"

#include <stdio.h> 
#include <stdbool.h>
#include <stdint.h>


int run_snake_game()
{
	snake_t snake = create_snake(5, 5);		
	size_t snake_size = 1;
	uint8_t food[] = { 4,5 };				
	direction_t current_direction = RIGHT;
	bool is_game_time = true;
	draw_borders();
	int snake_file_num = 0;
	int score = 0;
	while (is_game_time)
	{
		draw_snake(snake, snake_size);
		draw_food(food);
		cross_platform_sleep(0.15);
		erase_snake_tail(snake, snake_size);
		if (_kbhit())
		{
			int key = _getch();
			int requested_direction = INVALID_DIRECTION;
			switch (key)
			{
			case 'w': requested_direction = UP; break;
			case 's': requested_direction = DOWN; break;
			case 'a': requested_direction = LEFT; break;
			case 'd': requested_direction = RIGHT; break;
			case 'q': is_game_time = false; break;
			}
			if (requested_direction != INVALID_DIRECTION)
			{
				if (can_snake_move_in_direction(current_direction, requested_direction))
					current_direction = requested_direction;
			}
		}
		if (will_snake_collide_border_on_next_move(snake, snake_size, current_direction))
		{
			is_game_time = false;
			printf("Game Over!! snake bumped into border, Your score is %d\n", score);
			continue;
		}
		if (will_snake_collide_with_itself_on_next_move(snake, snake_size, current_direction))
		{
			is_game_time = false;
			printf("Game Over!! snake bumped into itself, Your score is %d\n", score);
			continue;
		}
		bool should_grow = will_snake_reach_food_on_next_move(snake, snake_size, food, current_direction);
		if (should_grow)
		{
			snake_size = grow_snake(&snake, snake_size, current_direction);
			set_new_food_location(snake, snake_size, food);
			score += 10;
		}
		else
		{
			move_snake(snake, snake_size, current_direction);
		}
	}
	free_snake(snake, snake_size);

	return 0;
}


void cross_platform_sleep(double seconds) {
#ifdef _WIN32
	Sleep((unsigned int)(seconds * 1000));// Windows uses milliseconds in Sleep function
#else
	sleep((unsigned int)seconds);// Unix, Linux, and macOS use seconds in sleep function
#endif
}

#ifndef _WIN32

// Unix-like systems implementation of _kbhit()
int _kbhit() {
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv) > 0;
}

// Unix-like systems implementation of _getch()
int _getch() {
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

#endif