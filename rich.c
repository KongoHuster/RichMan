#include "rich.h"
#include "function.h"
#include "assist.h"

PLAYER USERS[4]; //玩家信息
int USERS_NUMBER; //玩家个数
MAP MAPS[MAX_POSITION]; //地图信息
int NOW_ID = 0; 
int game_over;
ROOT_STATE root = ROOT_OFF;
ROUND_STATE round_state = ROUND_NULL;
int dice_num;
char TMP_DEBUG[50] = {'\0'};
char isFirst_tool = TRUE;
char isFirst_sell = TRUE;
char * DUMP_PATH;

int main(int argc,char *argv[])
{
    if (argc != 2){
        DUMP_PATH = (char*)malloc(sizeof(char)*20);
        strcmp(DUMP_PATH,"tmp.txt");
    }
    else
        DUMP_PATH = argv[1];

    game_over = 0;
    srand((unsigned)time(NULL));
    while (1) //main loop of the game
    {
        char init_user[] = {1,2,3,4};
        USERS_NUMBER = MAX_USER;
        _init_maps();
        _init_players(init_user,10000);
        
        printf("Enter a space to continue!\n");
        char start_tmp[50];
        int j = 0;
        while(1){
            fflush(stdin);
            my_getline(start_tmp,50);
            printf("%s",start_tmp);
            if(strcmp(start_tmp,"LOAD\n")==0){
                read_archive();
                j = 1;
                break;
            }
            if(preset_cmd(start_tmp)){
                j++;
                root = ROOT_ON;
                continue;
            }
            if(j==0){
                init_game();
            }
            break;
        }
        //printf("j=%d\n",j );
        //init_game();
        display(MAPS);
        while (!game_over) //
        {
            for (; NOW_ID < USERS_NUMBER; NOW_ID++)
            {
                player_round(USERS+NOW_ID);   
                if(game_over) break;
            }
            NOW_ID = 0;
        }
        if(game_over) break;
    }

        //fclose(stdout);   

    return 0;
}

