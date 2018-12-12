#include <stdio.h>
#include <string.h>
#include "function.h"
#include "assist.h"
extern PLAYER USERS[4];
extern int USERS_NUMBER;
extern MAP MAPS[MAX_POSITION];
extern int NOW_ID;
extern int game_over;
extern ROOT_STATE root;
extern ROUND_STATE round_state;
extern char isFirst_tool;
extern char isFirst_sell;
extern char * DUMP_PATH;

FILE * pdump;
char order_buf[5] = {'\0'};
extern char TMP_DEBUG[50];

void _start_game(char* users){
    // 开始游戏,等待玩家选择角色
    _clear();
    print_red("1:Qian  ");
    print_green("2:Atubo  ");
    print_blue("3:Sun Meimei  ");
    print_yellow("4:Jing Beibei\n");
    printf("please input 1-4 to choose roles(at least 2 roles):");

    _read_number(users);
}

void _init_players(char *users, int init_money){
    //初始化用户信息   users为用户编号数组  数组index为掷骰子先后顺序
    //init_money 为初始化金钱数
    for (int i = 0; i < USERS_NUMBER; i++){
        USERS[i].name = users[i];
        USERS[i].short_name = _get_player_symbol(USERS[i].name); 
        USERS[i].id = i + 1;
        USERS[i].lucky_god = 0;
        USERS[i].money = init_money;
        USERS[i].point = 0;
        USERS[i].position = 0;
        USERS[i].skip_num = 0;
        for (int j = 0; j < TOOL_NUMBER; j++){
            USERS[i].tool[j].num = 0;
            USERS[i].tool[j].type = j;
        }
        for (int j = 0; j < MAX_POSITION; j++) USERS[i].house[j] = 0;
    }
}

void _init_one_map(MAP *map, int posi, MAP_TYPE type, USER_NAME name, TOOL_TYPE tool,
                   char mine, PRICE price, SYMBOL symbol){
    //初始化地图当中的一个点
    map[posi].type = type;
    map[posi].owner = name;
    map[posi].price = price;
    map[posi].symbol = symbol;
    map[posi].tool = tool;
    map[posi].mine = mine;
    for (int i = 0; i < MAX_USER; i++)
        map[posi].pre_symbol[i] = symbol;
    map[posi].price_all = _get_place_price(price);
}

void _init_maps(){
    //地图初始化
    _init_one_map(MAPS, 0, MAP_START, USER_NULL, TOOL_NULL, 0, PRICE_0, SYMBOL_ST);
    for (int i = 1; i <= 27; i++){
        if (i == 14){
            _init_one_map(MAPS, i, MAP_PARK, USER_NULL, TOOL_NULL, 0, PRICE_0, SYMBOL_PARK);
            continue;
        }
        _init_one_map(MAPS, i, MAP_COM, USER_NULL, TOOL_NULL, 0, PRICE_1, SYMBOL_0);
    }
    _init_one_map(MAPS, 28, MAP_T, USER_NULL, TOOL_NULL, 0, PRICE_0, SYMBOL_T);
    for (int i = 29; i <= 34; i++){
        _init_one_map(MAPS, i, MAP_COM, USER_NULL, TOOL_NULL, 0, PRICE_2, SYMBOL_0);
    }
    _init_one_map(MAPS, 35, MAP_G, USER_NULL, TOOL_NULL, 0, PRICE_0, SYMBOL_G);
    for (int i = 36; i <= 62; i++){
        if (i == 49){
            _init_one_map(MAPS, i, MAP_PARK, USER_NULL, TOOL_NULL, 0, PRICE_0, SYMBOL_PARK);
            continue;
        }
        _init_one_map(MAPS, i, MAP_COM, USER_NULL, TOOL_NULL, 0, PRICE_3, SYMBOL_0);
    }
    _init_one_map(MAPS, 63, MAP_M, USER_NULL, TOOL_NULL, 0, PRICE_0, SYMBOL_M);

    _init_one_map(MAPS, 64, MAP_$, USER_NULL, TOOL_NULL, 60, PRICE_0, SYMBOL_$);
    _init_one_map(MAPS, 65, MAP_$, USER_NULL, TOOL_NULL, 80, PRICE_0, SYMBOL_$);
    _init_one_map(MAPS, 66, MAP_$, USER_NULL, TOOL_NULL, 40, PRICE_0, SYMBOL_$);
    _init_one_map(MAPS, 67, MAP_$, USER_NULL, TOOL_NULL, 100, PRICE_0, SYMBOL_$);
    _init_one_map(MAPS, 68, MAP_$, USER_NULL, TOOL_NULL, 80, PRICE_0, SYMBOL_$);
    _init_one_map(MAPS, 69, MAP_$, USER_NULL, TOOL_NULL, 20, PRICE_0, SYMBOL_$);
    return;
}

void init_game(){
    // 开始游戏,完成游戏的初始化
    char users[5];
    int init_money;
    char tmp[6] = "\0";
//    puts(tmp);
    _start_game(users);
//    for(char j = 0;j < USERS_NUMBER;j++)printf("The users  is %d\n", users[j]);
    while (1){
        char i = 0;
        char c, clear;

        printf("please input init money(1000-50000):");
        while(1) {
            c = getchar();
            if (('\n' == c) && (0 == i)) {
                init_money = 10000;
                i = 0;
                printf("Set default money -> 10000\n");
                break;
            } else if ((c >= '0') && (c <= '9') && (i < 5)) {
                tmp[i] = c;
               //  printf("this input:");
               // puts(tmp);
                i++;
                continue;
            }
            if (('\n' == c) && (i < 6)) {//normal end, add a '\0', exchange, reset i
                tmp[i] = '\0';
                i = 0;
                init_money = atoi(tmp);
//                printf("init_money is %d\n", init_money);
                break;
            } else {
                while (((clear = getchar()) != '\n') && (clear != EOF));
                break;
            }
        }
        if (init_money > 50000 || init_money < 1000)
            continue;
        else break;
    }
    _init_players(users, init_money);
    _init_maps();
}

void display(MAP* maps){
    //打印地图 
    _clear();
    for(int i=0;i<29;i++)
        _print_map_symbol(maps + i);
    for(int i=29;i<35;i++){
        printf("\n");
        _print_map_symbol(maps + MAX_POSITION - (i-28));
        for(int j=0;j<27;j++) printf(" ");
        _print_map_symbol(maps+i);
    }
    printf("\n");
    for(int i=63;i>=35;i--) _print_map_symbol(maps+i);
    printf("\n");
}

void player_round(PLAYER* player){
    //玩家回合控制总函数
    //TODO: cmd table complete
    BOOL end_round=FALSE;
    if((*player).skip_num>0){
        (*player).skip_num--;
        return;
    }
    while(1){
        if(TMP_DEBUG[0]!='\0'){
            if(root==ROOT_OFF){
                TMP_DEBUG[0] = '\0';
                continue;
            }
            if (args_parse(TMP_DEBUG, player)){
                TMP_DEBUG[0] = '\0';
                break;
            }
            else{
                TMP_DEBUG[0] = '\0';
                continue;
            }
        }
        printf("\n");
        print_prompt(player);
        char _args[12] = "\0";
        char c, clear, i=0;
        do{
            c = getchar();
            if(('\n' == c) && (0 == i)) break;
            if(_isdivider(c) && (0 == i)) continue;
            if((_isalph(c) || _isnum(c) || _isdivider(c)
                || '-'==c || '+' == c || '*'==c || '#' == c)&& i < 11)
            {
                if(!_isdivider(c))
                {
                    _args[i] = c;
                    i++;
                }
                else {
                    i -= (_isdivider(_args[i-1]))? 1 : 0;
                    _args[i] = ' ';
                    i++;
                }
                // printf("test :");
                // puts(_args);
                continue;
            }
            // '\n' and over
            if(('\n' == c) && (i < 12))
            {//normal end
                if(_args[i-1] == ' ') i--;
                _args[i] = '\0';
                i = 0;
                break;
            }else {
                _args[11] = '\0';
                while (((clear = getchar()) != '\n') && (clear != EOF));
                break;
            }

        }while(1);
        if(('\n' == c) && (0 == i) && (!strlen(_args))) continue;
        if(args_parse(_args, player)){ isFirst_tool = TRUE; isFirst_sell = TRUE;/*round_state = ROUND_NULL;*/ break;}//end round
        //printf("=======----%s-----\n", _args);
    }
    if(USERS_NUMBER<2){
        dump(player, &end_round, DUMP_PATH);
        printf("游戏结束，任意键退出！\n");
        getchar();
        game_over = 1;
    }
    if(player->lucky_god) player->lucky_god--;
}

void bolck_cmd(PLAYER *player, int position,BOOL* end_round){
    //printf("Function is developing\n");
    int pos = position;
    int pos_tool;

    if(!isFirst_tool)   printf("Use cmd: query,use tool(once),sell house(once)-> roll dice(once) !\n");
    else {
        if ((pos > 10) || (pos < -10) || (!pos)) {
            printf("To use this tool, please input a number between -10 and 10 indicate behind or before you, 0 is not allowed! \r\n");
        } else {
            pos_tool = (player->position + pos + MAX_POSITION) % MAX_POSITION;
            if ((MAPS[pos_tool].tool > TOOL_NULL) || _isuser_symbol(MAPS[pos_tool].symbol)) {
                printf("Can not be used in this place !\r\n");
                *end_round = FALSE;
                return;
            }
            if ((player->tool[TOOL_L].num > 0) && (player->tool[TOOL_L].num < 11)) {
                player->tool[TOOL_L].num--;
                MAPS[pos_tool].tool = TOOL_L;
//                round_state = ROUND_TOOL;
                isFirst_tool = FALSE;
                display(MAPS);
                print_player_name(player);
                printf(":\n");
                printf("Block is used successfully !\r\n");
            } else {
                player->tool[TOOL_L].num = 0;
                printf("Sorry, you don't have available tool(Block) now !\r\n");
            }
        }
    }
    *end_round = FALSE;
}

//void bomb_cmd(PLAYER *player, int position,BOOL* end_round){
//    //printf("Function is developing\n");
//    int pos = position;
//    int pos_tool;
//
//    if((pos > 10) || (pos < -10) || (!pos))
//    {
//        printf("To use this tool, please input a number between -10 and 10 indicate behind or before you, 0 is not allowed! \r\n");
//    }
//    else
//    {
//        pos_tool = (player->position + pos + MAX_POSITION) % MAX_POSITION;
//        if((MAPS[pos_tool].tool > TOOL_NULL) || _isuser_symbol(MAPS[pos_tool].symbol))
//        {
//            printf("Can not be used in this place !\r\n");
//            *end_round = FALSE;
//            return;
//        }
//        if((player->tool[TOOL_B].num > 0) && (player->tool[TOOL_B].num < 11))
//        {
//            player->tool[TOOL_B].num--;
//            MAPS[pos_tool].tool = TOOL_B;
//            round_state = ROUND_TOOL;
//            display(MAPS);
//            print_player_name(player); printf(":\n");
//            printf("Bomb is used successfully !\r\n");
//        }
//        else
//        {
//            player->tool[TOOL_B].num = 0;
//            printf("Sorry, you don't have available tool(Bomb) now !\r\n");
//        }
//    }
//    *end_round = FALSE;
//}

void robot_cmd(PLAYER* player,BOOL* end_round){
    //printf("Function is developing\n");
    char pos = player->position;
    char pos_scan = pos;

    if(!isFirst_tool) printf("Use cmd: query,use tool(once),sell house(once)-> roll dice(once) !\n");
    else
    {
        if ((player->tool[TOOL_R].num > 0) && (player->tool[TOOL_R].num < 11)) {
            player->tool[TOOL_R].num--;
            isFirst_tool = FALSE;
            while ((++pos_scan % MAX_POSITION) <= ((pos + 10) % MAX_POSITION)) {
                pos_scan %= MAX_POSITION;
                if ((MAPS[pos_scan].tool > TOOL_NULL) && (MAPS[pos_scan].tool < TOOL_R)) {
//                    round_state = ROUND_TOOL;
                    if (TOOL_L == MAPS[pos_scan].tool) {
                        MAPS[pos_scan].tool = TOOL_NULL;
                        display(MAPS);
                        print_player_name(player);
                        printf(":\n");
                        printf("Your robot found a Block !\r\n");
                    }
//                else
//                {
//                    MAPS[pos_scan].tool = TOOL_NULL;
//                    display(MAPS);
//                    print_player_name(player); printf(":\n");
//                    printf("Your robot found a Bomb !\r\n");
//                }
                    break;
                } else {
                    MAPS[pos_scan].tool = TOOL_NULL;
                    if (pos_scan == ((pos + 10) % MAX_POSITION)) {
                        printf("Your robot found nothing !\r\n");
                    }
                }
            }

        } else {
            player->tool[TOOL_R].num = 0;
            printf("Sorry, you don't have available tool(Robot) now !\r\n");
        }
    }
    *end_round = FALSE;
}

void su_cmd_pre(PLAYER *player, BOOL *end_round) {
    static char try = 0;

    if(ROOT_OFF == root) {
        if (++try < 3) {
            help_cmd();
        } else {
            try = 0;
            root = ROOT_PRE;
            printf("***** Invalid Cmd *****\n");
        }
    }
    else if(ROOT_PRE == root)
    {
        try = 0;
        printf("***** Invalid Cmd *****\n");
    }
    else if(ROOT_ON == root)
    {
        printf("You already are ROOT now!\n");
    }
    else
    {
        root = ROOT_OFF;
        printf("SU err!\n");
    }
}

void su_cmd(PLAYER *player, char* str, BOOL *end_round) {
    char* ps_in = str;
    char* pass_wd = "***";
//    printf("ps_in is %s\n", ps_in);
//    printf("pass_wd is %s\n", pass_wd);
//    printf("the strcmp is %d\n", strcmp(pass_wd, ps_in));

    if(strcmp(pass_wd, ps_in) == 0)
    {
        root = ROOT_ON;
        display(MAPS);
        printf("SU successfully!\n");
    }
    else
    {
        root = ROOT_OFF;
        help_cmd();
    }
}

void exit_cmd(PLAYER *player, BOOL *end_round) {
    root = ROOT_OFF;
    display(MAPS);
    printf("Exit root successfully!\n");
}


void step_cmd(PLAYER *player, int position, BOOL *end_round){
    *end_round = FALSE;
    players_run_in_the_way(player, position, end_round);
    display_run_map(player, player->position);
    print_player_name(player);
    printf(":\n");
    players_end_run(player, end_round);
    *end_round = TRUE;

}

void help_cmd(void) {
    print_yellow("Roll :");
    printf(" Roll the dice 1~6 \n");
    print_yellow("Sell n :");
    printf(" When it's the buyer's turn to play, sell a property with an absolute n on his map at twice the total cost of investment. \n");
    print_yellow("Block n :");
    printf(" You can place the roadblock anywhere in the front and back 10 steps of the current position. Any player passing the roadblock will be intercepted. The props are effective once. \n");
//    print_yellow("Bomb n :");
//    printf(" You can place the bomb anywhere in the front and back 10 steps of the current position. Any player passing through the bomb will be wounded. To the hospital for three days.  \n");
    print_yellow("Robot :");
    printf(" Using this prop, you can clean up any other props in the 10 steps on the road ahead.\n");//, such as bombs and roadblocks. \n");
    print_yellow("Query :");
    printf(" Display your assets. \n");
    print_yellow("Help :");
    printf(" View command help. \n");
    print_yellow("Quit :");
    printf(" Forced return. \n");
    if(ROOT_ON == root)
    {
        print_red("Pass  :");
        printf(" Pass round directly. \n");
        print_red("Step n :");
        printf(" Control step. \n");
        print_red("Su :");
        printf(" Su for 3 times, then \"Su pass_word\", to get root.\n");
        print_red("Exit :");
        printf(" Exit root.\n");
    }
}

void query_cmd(PLAYER *player, BOOL *end_round) {
//    char rank = 0;
    PLAYER* top;
    top = _find_top_rank();
    print_player_name(player);
    printf(":\nPosition: %d\n", player -> position);
    printf("Money: %ld\t", player->money);
    print_player_name(top);
    printf(": %ld""\tPlayer: %d\n", top->money, USERS_NUMBER);
    printf("Point: %ld\n", player->point);
    printf("Skip round: %d\n", player->skip_num);
    printf("Luck god  : %d\n", player->lucky_god);
    printf("House:\n""\t   position\tlevel\t cost\n");                                      //显示房产（等级和位置）
    for(char i = 0;i < MAX_POSITION; i++)
    {
     if(player->house[i]){
         printf("\t*     %d  \t  %d\t  %d\n", i, (player->house[i] - 1), (MAPS[i].price_all));
     }
    }
    printf("--------------------------------------------------------------\n");
    printf("Tool:\n");                            //显示道具（种类和数量）
    printf("\t* Block's number: %d\n", player->tool[TOOL_L].num); //路障数量
//    printf("\t* Bomb's  number: %d\n", player->tool[TOOL_B].num);  //炸弹数量
    printf("\t* Robot's number: %d\n", player->tool[TOOL_R].num); //路障数量
    printf("**************************************************************\n");
}

void quit_cmd(PLAYER *player,BOOL* end_round){
    *end_round = 1;
    game_over = 1;
}

void pass_cmd(BOOL* end_round)
{
    *end_round = TRUE;
    display(MAPS);
}

void sell_cmd(PLAYER *player, int position, BOOL *end_round) {
    if(!isFirst_sell) printf("Use cmd: query,use tool(once),sell house(once)-> roll dice(once) !\n");
    else {
        if (player->house[position] == 0) {
            printf("It's NOT your house!\n");
            return;
        }
        {
            int price = MAPS[position].price_all * 2;
            MAPS[position].owner = USER_NULL;
            MAPS[position].type = MAP_COM;
            player->money += price;
            player->house[position] = 0;
//        round_state = ROUND_SELL;
            isFirst_sell = FALSE;
            display(MAPS);
            print_player_name(player);
            printf(":\nYou SELL the house in %d and get money %d.\n""You money: %ld -> %ld\n",
                   position, price, ((player->money) - price), (player->money));
        }
    }
    *end_round = FALSE;

}

void dice_cmd(PLAYER* player,BOOL* end_round){
    //玩家执行Roll命令后的一系列情况
    int steps = _get_rand(1, 6);
    char pos_pre = player->position;
//    *end_round = FALSE;
    players_run_in_the_way(player,steps, end_round);
//    if (*end_round) return;
    display_run_map(player,player->position);
    print_player_name(player);
    /*if(ROUND_IDLE != round_state)*/
    printf(":\nYou walked %d steps forward !!\n", ((player->position) + MAX_POSITION - pos_pre)%MAX_POSITION);
    players_end_run(player, end_round);
    *end_round = TRUE;
}

void save_cmd()
{
    //TODO:CHANGE LOGIC TO SAVE isFirst_tool,isFirst_sell,USERS[]. MAKE SURE to reload to the player who "save"
    FILE *fp;
    if ((fp = fopen("save.txt", "w+")) == NULL)
    {
        printf("The file save.txt can not be opened.\n");
        return;
    }
    fprintf(fp, "%d\n", USERS_NUMBER);
    //save all player's info to save.txt
    for (int i = 0; i < USERS_NUMBER; i++)
    {
        fprintf(fp, "%d %d %d %d %d %ld %ld ",
                USERS[i].name, USERS[i].short_name, USERS[i].position, USERS[i].id, USERS[i].skip_num, USERS[i].money, USERS[i].point);
        for (int j = 0; j < MAX_POSITION; j++)
        {
            fprintf(fp, "%d ", USERS[i].house[j]);
        }
        for (int j = 0; j < TOOL_NUMBER; j++)
        {
            fprintf(fp, "%d %d ", USERS[i].tool[j].type, USERS[i].tool[j].num);
        }
        fprintf(fp, "%d \n", USERS[i].lucky_god);
    }
    //save all map's info to save.txt
    for (int i = 0; i < MAX_POSITION; i++)
    {
        fprintf(fp, "%d %d %d %d %d %d ",
                MAPS[i].type, MAPS[i].owner, MAPS[i].tool, MAPS[i].mine, MAPS[i].price, MAPS[i].symbol);
        for (int j = 0; j < USERS_NUMBER; j++)
        {
            fprintf(fp, "%d ", MAPS[i].pre_symbol[j]);
        }
        fprintf(fp, "%d \n", MAPS[i].price_all);
    }
    fprintf(fp,"%d\n", NOW_ID);
    fprintf(fp,"%d %d\n",isFirst_tool,isFirst_sell);
    fclose(fp);
}

void read_archive(){
    FILE *fp;
    //char str[1000];
    //char *single;
    //int flag = 0, file_row = 0, count = 0;
    if((fp = fopen("save.txt","r"))==NULL){
         printf("The file save.txt can not be opened.\n");
         return;
    }
    fscanf(fp, "%d\n", &USERS_NUMBER);
    for(int i = 0;i < USERS_NUMBER; i++){
        fscanf(fp,"%d %d %d %d %d %ld %ld ",
        &(USERS[i].name), &(USERS[i].short_name), &(USERS[i].position), &(USERS[i].id), &(USERS[i].skip_num), &(USERS[i].money), &(USERS[i].point));
        for(int j = 0;j < MAX_POSITION; j++){
            fscanf(fp, "%d ", &(USERS[i].house[j]));
        }
        for(int j = 0;j < TOOL_NUMBER; j++){
            fscanf(fp, "%d %d ", &(USERS[i].tool[j].type), &(USERS[i].tool[j].num));
        }
        fscanf(fp, "%d \n", &(USERS[i].lucky_god));
    }
    for(int i = 0;i < MAX_POSITION;i++){
        fscanf(fp,"%d %d %d %d %d %d ",
        &(MAPS[i].type), &(MAPS[i].owner), &(MAPS[i].tool), &(MAPS[i].mine), &(MAPS[i].price), &(MAPS[i].symbol));
        for(int j = 0;j < USERS_NUMBER;j++){
            fscanf(fp,"%d ", &(MAPS[i].pre_symbol[j]));
        }
        fscanf(fp,"%d \n",&(MAPS[i].price_all));
    }
    fscanf(fp, "%d\n", &NOW_ID);
    fscanf(fp, "%d %d\n", &isFirst_tool, &isFirst_sell);
    fclose(fp);
    display(MAPS);
}

BOOL preset_cmd(char* cmd){
    //测试接口,preset命令,详情见测试文档
    char *tmp = strtok(cmd, " ");
    int init_money = 10000;
    if (strcmp(tmp, "preset") == 0){
        tmp = strtok(NULL, " ");
        if (strcmp(tmp, "user") == 0){
            tmp = strtok(NULL, " ");
            USERS_NUMBER = strlen(tmp) - 1;        
            char users[USERS_NUMBER];
            for (int i = 0; i < USERS_NUMBER; i++)
            {
                if (tmp[i] == 'A')
                    users[i] = 2;
                else if (tmp[i] == 'Q')
                    users[i] = 1;
                else if (tmp[i] == 'S')
                    users[i] = 3;
                else if (tmp[i] == 'J')
                    users[i] = 4;
            }
            _init_players(users, init_money);
        }
        else if (strcmp(tmp, "map") == 0){
            tmp = strtok(NULL, " ");
            int posi = atoi(tmp);
            tmp = strtok(NULL, " ");
            char name = tmp[0];
            tmp = strtok(NULL," ");
            int level = atoi(tmp);
            preset_map(MAPS,posi,_get_player(name),level);
        }
        else if (strcmp(tmp, "fund") == 0){
            tmp = strtok(NULL, " ");
            char name = tmp[0];
            tmp = strtok(NULL," ");
            long money = atol(tmp);
            preset_fund(_get_player(name),money);
        }
        else if (strcmp(tmp, "credit") == 0){
            tmp = strtok(NULL, " ");
            char name = tmp[0];
            tmp = strtok(NULL," ");
            long credit = atol(tmp);
            preset_credit(_get_player(name),credit);
        }
        else if (strcmp(tmp, "gift") == 0){
            tmp = strtok(NULL," ");
            char name = tmp[0];
            tmp = strtok(NULL," ");
            int tn_size = sizeof(tmp)/sizeof(char);
            char tool[tn_size+1];
            strcpy(tool,tmp);
            tmp = strtok(NULL," ");
            int n = atoi(tmp);
            preset_gift(_get_player(name),tool,n);
        }
        else if (strcmp(tmp, "bomb") == 0){
            tmp = strtok(NULL, " ");
            MAPS[atoi(tmp)].tool = TOOL_B;
        }
        else if (strcmp(tmp, "barrier") == 0){
            tmp = strtok(NULL, " ");
            MAPS[atoi(tmp)].tool = TOOL_L;
        }
        else if (strcmp(tmp, "userloc") == 0){
            tmp = strtok(NULL," ");
            char name = tmp[0];
            tmp = strtok(NULL," ");
            int posi = atoi(tmp);
            tmp = strtok(NULL, " ");
            int m = atoi(tmp);
            preset_userloc(MAPS,_get_player(name),posi,m);
        }
        else if (strcmp(tmp, "nextuser") == 0){
            
        }
    }
    else {
        for(int i=0;i<50;i++){
            if(cmd[i]!='\n'){
                TMP_DEBUG[i] = cmd[i];
            }
            else break;
        }
        return FALSE;
    }
    return TRUE;
}

void dump_user()
{
    char buf[5] = {'\0'};
    char temp[5] = {'\0'};

    memset(buf,'\0',5);
    memset(temp,'\0',5);
    memset(order_buf,'\0',5);


    for (int i = 0; i < USERS_NUMBER; ++i)
        buf[i] = USERS[i].short_name;


    for (int i = 0; i < USERS_NUMBER; ++i)
    {
        if (buf[i] == 'A')
            temp[0] = 'A';
        if (buf[i] == 'Q')
            temp[1] = 'Q';
        if (buf[i] == 'S')
            temp[2] = 'S';
        if (buf[i] == 'J')
            temp[3] = 'J';
    }


    int j=0;
    for (int i = 0; i < 4; ++i)
    {
        if (temp[i] != '\0')
        {
            order_buf[j++]=temp[i];
        }
    }

    fprintf(pdump,"user %s\n",order_buf);

    return;
}

void dump_map()
{   
    for (int i = 0; i < MAX_POSITION; ++i)
    {
        if (MAPS[i].owner != USER_NULL)
            fprintf(pdump,"map[%d] %c %d\n",i,_get_player_symbol(MAPS[i].owner),MAPS[i].pre_symbol[MAX_USER-1]-SYMBOL_0);      
    }

    return;
}

void dump_money()
{
    for (int i = 0; i < USERS_NUMBER; ++i)
        fprintf(pdump,"fund %c %d\n",order_buf[i],_get_money_by_symbol(order_buf[i]));
    return;
}

void dump_point()
{
    for (int i = 0; i < USERS_NUMBER; ++i)
        fprintf(pdump,"credit %c %d\n",order_buf[i],_get_point_by_symbol(order_buf[i]));
    return;
}

void dump_tool_user()
{
    for (int i = 0; i < USERS_NUMBER; ++i)
    {
        for (int j = 0; j < USERS_NUMBER; ++j)
        {
            if (USERS[j].short_name == order_buf[i])
            {
                //fprintf(pdump,"gift %c bomb %d\n",order_buf[i],USERS[j].tool[1].num);
                fprintf(pdump,"gift %c barrier %d\n",order_buf[i],USERS[j].tool[0].num);
                fprintf(pdump,"gift %c robot %d\n",order_buf[i],USERS[j].tool[2].num);
                fprintf(pdump,"gift %c god %d\n",order_buf[i],USERS[j].lucky_god);
            }
        }   
    }
    return;
}


void dump_tool_map()
{

    for (int i = 0; i < MAX_POSITION; ++i)
    {
        if (MAPS[i].tool == 0)
            fprintf(pdump,"barrier in %d\n",i);      
    }
    return;
}

void dump_loc()
{
    for (int i = 0; i < USERS_NUMBER; ++i)
    {
        for (int j = 0; j < USERS_NUMBER; ++j)
        {
            if (USERS[j].short_name == order_buf[i])
            {
                if (USERS[j].skip_num == 0)
                    fprintf(pdump,"userloc %c %d\n",order_buf[i],USERS[j].position);
                else
                    fprintf(pdump,"userloc %c %d %d\n",order_buf[i],USERS[j].position,USERS[j].skip_num);
            }

        }
    }
    return;
    
}

void dump_next()
{
    fprintf(pdump, "nextuser %c\n", USERS[(NOW_ID + 1)%USERS_NUMBER].short_name);
    return;
}


void dump(PLAYER *player, BOOL *end_round,char * path)
{

    //if (!(pdump = fopen("./TestCase/dump.txt","w")))
    if (!(pdump = fopen(path,"w")))
    {
        printf("open dump.txt failed\ndump failed!\n");
        goto EXIT;
    }

    dump_user();

    dump_map();
   
    dump_money();

    dump_point();

    dump_tool_user();

    dump_tool_map();

    dump_loc();

    dump_next();

    printf("dump finished\n");

    fclose(pdump);

EXIT:
    if(pdump)
        fclose(pdump);
    
    quit_cmd(player,end_round);
    return;
}

