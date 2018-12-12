#ifndef _FUNCTION_H_
#define _FUNCTION_H_
//主要的功能函数文件
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rich.h"

void _start_game(char* users);
void _init_players(char *users, int init_money);
void _init_one_map(MAP *map, int posi, MAP_TYPE type, USER_NAME name, TOOL_TYPE tool,
                   char mine, PRICE price, SYMBOL symbol);
void _init_maps();
void init_game();
void display(MAP *maps);
void player_round(PLAYER *player);
void bolck_cmd(PLAYER *plary, int position, BOOL *end_round);
void bomb_cmd(PLAYER *plary, int position, BOOL *end_round);
void robot_cmd(PLAYER *player, BOOL *end_round);
void su_cmd_pre(PLAYER *player, BOOL *end_round);
void su_cmd(PLAYER *player, char* str, BOOL *end_round);
void exit_cmd(PLAYER *player, BOOL *end_round);
void step_cmd(PLAYER *plary, int position, BOOL *end_round);
void pass_cmd(BOOL* end_round);
void help_cmd(void);
void query_cmd(PLAYER *player, BOOL *end_round);
void quit_cmd(PLAYER *player, BOOL *end_round);
void sell_cmd(PLAYER *player, int position, BOOL *end_round);
void dice_cmd(PLAYER *player, BOOL *end_round);
void read_archive();
void save_cmd(void);

BOOL preset_cmd(char *cmd);
void dump(PLAYER *player, BOOL *end_round,char * path);
void dump_user();
void dump_map();
void dump_money();
void dump_point();
void dump_tool_user();
void dump_tool_map();
void dump_loc();
void dump_next();

#endif //_FUNCTION_H_
