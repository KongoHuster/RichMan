#include "common.h"
#include "display.h"

bool DisplayMap(GAME *game_state)
{
    bool ret = false;
    int curr_pos_id = 0;
    char connect;
    char _owner = SPACE;
    POS pos;

    if(!game_state){
        printf("The game pointer is null!\n");
        goto Exit;
    }

    CLEAN_GAME_FACE();
    pos.y = START_Y;
    pos.x = START_X;

    for(int i = 0 ; curr_pos_id < 29; ++i, ++curr_pos_id){
        pos.x += 2;
        GET_MAP_FLAG(game_state, curr_pos_id, connect);
        GET_OWNER_COLOR(game_state->map[curr_pos_id].house_owner_id, game_state->player, _owner);
        DisplayBlock(pos, connect, _owner);
    }

    for(int i = 0; curr_pos_id < 35; ++i, ++curr_pos_id){
        pos.y += 2;
        GET_MAP_FLAG(game_state, curr_pos_id, connect);
        GET_OWNER_COLOR(game_state->map[curr_pos_id].house_owner_id, game_state->player, _owner);
        DisplayBlock(pos, connect, _owner);
    }

    pos.y += 2;

    for(int i = 0 ; curr_pos_id < 64; ++i, ++curr_pos_id){
        GET_MAP_FLAG(game_state, curr_pos_id, connect);
        GET_OWNER_COLOR(game_state->map[curr_pos_id].house_owner_id, game_state->player, _owner);
        DisplayBlock(pos, connect, _owner);
        pos.x -= 2;
    }

    pos.x += 2;

    for(int i = 0; curr_pos_id < 70; ++i, ++curr_pos_id){
        pos.y -= 2;
        GET_MAP_FLAG(game_state, curr_pos_id, connect);
        GET_OWNER_COLOR(game_state->map[curr_pos_id].house_owner_id, game_state->player, _owner);
        DisplayBlock(pos, connect, _owner);
    }

    pos.y += 14;
    GET_PLAYER_FLAG(game_state, connect);
    ShowPlayers(game_state);
    ShowTips(game_state->current_player->player_name, connect, pos);
    ret = true;

    Exit:
    return ret;
}

void ShowPlayers(GAME *game)
{
    POS pos = {115, 5};
    PLAYER *tmp_ply;

    for(int i = 4; i < 10; ++ i){
        GOTOXY(pos.x, pos.y + i);
        printf("\033[K");
    }

    GOTOXY(pos.x, pos.y);
    printf("+---------------------------------------------+");
    pos.y += 1;
    GOTOXY(pos.x, pos.y);
    printf("|\t姓名\t序号\t位置\t资金\t点数\t|");
    pos.y += 1;
    GOTOXY(pos.x, pos.y);
    printf("+---------------------------------------------+");

    tmp_ply = game->player;
    for(int i = 0; i < game->player_num; ++ i){
        pos.y += 1;
        GOTOXY(pos.x, pos.y);

        printf("|");
        switch (tmp_ply->player_name[0])
        {
            case P_QFR:
                COLOR_IS_RED();
                break;
            case P_ASB:
                COLOR_IS_GREEN();
                break;
            case P_SXM:
                COLOR_IS_BLUE();
                break;
            case P_JBB:
                COLOR_IS_YELLOW();
                break;
            default:
                COLOR_IS_WHITE();
                break;
        }

        printf("    %s   %-2d      %2d      %-5d     %-5d ",
               &(tmp_ply->player_name[1]), tmp_ply->player_id, tmp_ply->cur_pos, tmp_ply->money, tmp_ply->point);

        COLOR_IS_WHITE();
        printf("|");
        tmp_ply = tmp_ply->next;
    }
    pos.y += 1;
    GOTOXY(pos.x, pos.y);
    COLOR_IS_WHITE();
    printf("+---------------------------------------------+");
}

// there is a bug to fix tue color
void ShowTips(char *name, char _flag, POS _pos)
{
    switch (_flag)
    {
        case P_QFR:
            COLOR_IS_RED();
            break;
        case P_ASB:
            COLOR_IS_GREEN();
            break;
        case P_SXM:
            COLOR_IS_BLUE();
            break;
        case P_JBB:
            COLOR_IS_YELLOW();
            break;
        default:
            COLOR_IS_WHITE();
            break;
    }

    GOTOXY(_pos.x, _pos.y);
    printf(" %s > ", name);
    CLEAN_LINE();
    GOTOXY(_pos.x + 11, _pos.y);

}

void DisplayBlock(POS pos, char connect, char owner)
{
    GOTOXY(pos.x, pos.y);

    //set color
    switch (connect)
    {
        case P_QFR:
            COLOR_IS_RED();
            break;
        case P_ASB:
            COLOR_IS_GREEN();
            break;
        case P_SXM:
            COLOR_IS_BLUE();
            break;
        case P_JBB:
            COLOR_IS_YELLOW();
            break;
        default:
            switch (owner)
            {
                case P_QFR:
                    COLOR_IS_RED();
                    break;
                case P_ASB:
                    COLOR_IS_GREEN();
                    break;
                case P_SXM:
                    COLOR_IS_BLUE();
                    break;
                case P_JBB:
                    COLOR_IS_YELLOW();
                    break;
                default:
                    COLOR_IS_WHITE();
                    break;
            }
            break;
    }

    printf("%c", connect);
}

