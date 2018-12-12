#ifndef _RICH_H_
#define _RICH_H_
//工程基本结构
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "rich.h"

#define MAX_HORIZONTAL_NUM    30    //horizontial
#define MAX_VERTICAL_NUM      8     //vertical

typedef short BOOL;
#define TRUE 1
#define FALSE 0

#define TOOL_NUMBER 3
#define MAX_POSITION 70

#define MAX_USER 4

#define RED_Q "\033[0;32;31m"
#define GREEN_A "\033[0;32;32m"
#define BLUE_J "\033[0;32;34m"
#define YELLOW_S "\033[0;32;33m"
#define ORI "\033[0m"

typedef enum 
{
    RICH_FALSE  = 0,
    RICH_TURE   = 1,
}RICH_BOOL;

typedef enum
{//already done sth
    ROUND_NULL = 0,
    ROUND_TOOL ,
    ROUND_DICE ,
    ROUND_RENT ,
    ROUND_BUY  ,
    ROUND_SELL ,
    ROUND_IDLE     //BOMB OR PRISON
}ROUND_STATE;

typedef enum
{
    ROOT_OFF = 0,
    ROOT_PRE ,
    ROOT_ON  ,
}ROOT_STATE;

typedef enum
{
  USER_NULL      = 0,
  QIAN      = 1, //Qian
  ATUBO     = 2, //Atubo
  SUN       = 3, //Sun Meimei
  JING      = 4, //Jing Beibei
  USER_NUM
}USER_NAME;

typedef enum 
{
  LEVEL_NULL  = 0,
  LEVEL_1     = 1,
  LEVEL_2     = 2,
  LEVEL_3     = 3,
}HOUSE_LEVEL;

typedef enum
{
	SKIP_NULL = 0,
	SKIP_P    = 2,
	SKIP_H    = 3,
	SKIP_DEAD = 4,
}SKIP_NUM;

typedef struct 
{
   BOOL position;
   char level;
}HOUSE;

typedef enum 
{
    TOOL_NULL  = -1,
    TOOL_L     = 0,
    TOOL_B     = 1,
    TOOL_R     = 2,
    TOOL_NUM   = 3,
}TOOL_TYPE;


typedef struct 
{
   TOOL_TYPE type;
   char num;
}TOOL;

typedef enum
{
    MAP_COM = 0, //
    MAP_PRI = 1, //private
    MAP_H   = 2, //hospital
    MAP_PRS = 3, 
    MAP_G   = 4, //game
    MAP_M   = 5, //magic
    MAP_$   = 6, //
    MAP_T   = 7, //tool
    MAP_START = 8,
    MAP_PARK = 9,
    MAP_NUM 
}MAP_TYPE;

typedef enum
{
    PRICE_0,
    PRICE_1,
    PRICE_2,
    PRICE_3,
    PRICE_NUM
}PRICE;

typedef enum
{
    SYMBOL_Q, //Qian
    SYMBOL_A,
    SYMBOL_S,
    SYMBOL_J,
    SYMBOL_0, //house level
    SYMBOL_1,
    SYMBOL_2,
    SYMBOL_3,
    SYMBOL_ST,
    SYMBOL_H, 
    SYMBOL_T,
    SYMBOL_G,
    SYMBOL_P,
    SYMBOL_M,
    SYMBOL_$,
    SYMBOL_L,
    SYMBOL_R,
    SYMBOL_B,
    SYMBOL_PARK, //park
    SYMBOL_NUM
}SYMBOL;

typedef struct 
{
    USER_NAME name; //用户姓名
    char short_name; //用户显示缩写
    char position; //用户位置
    char id;  //用户掷骰子先后
    char skip_num; //跳过轮数
    long money; //拥有金钱
    long point; //拥有点数
    int house[MAX_POSITION]; //拥有的房产
    TOOL tool[TOOL_NUMBER]; //拥有的道具
    char lucky_god; //拥有财神回合数
}PLAYER;

typedef struct
{
    MAP_TYPE        type;  //类型
    USER_NAME       owner; //拥有者
    TOOL_TYPE       tool; //路上的道具
    char            mine; //矿地点数
    PRICE           price; //地段
    SYMBOL          symbol; //当前标志
    SYMBOL pre_symbol[MAX_USER]; //保留标志
    int             price_all; //当前地产价值
}MAP;

#endif


