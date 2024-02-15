#ifndef __SNAKE_RUNNER__
#define __SNAKE_RUNNER__
int run_snake_game();


// utilities:
void cross_platform_sleep(double seconds);
#ifndef _WIN32
int _kbhit();
int _getch();
#endif //_WIN32

#endif //__SNAKE_RUNNER__
