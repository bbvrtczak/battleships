#pragma once
#include "structs.h"

#define BOARD_X 10
#define BOARD_Y 21

#define INGAME 0
#define END 1

#define BASE 0
#define STATE 1
#define PLAYER 2

#define NONE -1
#define A 0
#define B 1

#define FLEET_TYPES 6
#define SHIP_TYPE_BOUND 10

#define CAR 5 
#define BAT 4
#define CRU 3
#define DES 2

#define NOT_DEFINED -1
#define SHOT 0
#define PLACED 1

#define ON 1
#define OFF 0
using namespace std;
void FILL_BOARD(char board[][BOARD_X]) {
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            board[i][j] = ' ';
        }
    }
}
void FILL_BOARD_ID(int board[][BOARD_X]) {
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            board[i][j] = -1;
        }
    }
}

void PRINT_BOARD(char board[][BOARD_X]) {
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
}

void EXTENDED_PRINT_BOARD(char board[][BOARD_X]) {
    cout << "   ";
    for (int i = 0; i < BOARD_X; i++) {
        cout << i;
    }
    cout << endl;
    for (int i = 0; i < BOARD_Y; i++) {
        if (i<10){
            cout << "0";
        }
        cout << i;
        for (int j = 0; j < BOARD_X; j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
}

void place_ship(char board[][BOARD_X], int y, int x, char ship_direction, int ship_class) {
    if (ship_direction == 'N') {
        for (int i = y; i < ship_class + y; i++) {
            board[i][x] = '+';
        }   
    }
    else if (ship_direction == 'W') {
        for (int i = x; i < ship_class + x; i++) {
            board[y][i] = '+';
        }
    }
    else if (ship_direction == 'S') {
        for (int i = y; i > y - ship_class; i--) {
            board[i][x] = '+';
        }
    }
    else if (ship_direction == 'E') {
        for (int i = x; i > x - ship_class; i--) {
            board[y][i] = '+';
        }
    }
}

void place_ship_parts(char board[][BOARD_X], int y, int x, char ship_direction, int ship_class, char parts[6]) {
    int index = 0;
    if (ship_direction == 'N') {
        for (int i = y; i < ship_class + y; i++) {
            if (parts[index] == '1')
                board[i][x] = '+';
            else
                board[i][x] = 'x';
            index++;
        }
    }
    else if (ship_direction == 'W') {
        for (int i = x; i < ship_class + x; i++) {
            if (parts[index] == '1')
                board[y][i] = '+';
            else
                board[y][i] = 'x';
            index++;
        }
    }
    else if (ship_direction == 'S') {
        for (int i = y; i > y - ship_class; i--) {
            if (parts[index] == '1')
                board[i][x] = '+';
            else
                board[i][x] = 'x';
            index++;
        }
    }
    else if (ship_direction == 'E') {
        for (int i = x; i > x - ship_class; i--) {
            if (parts[index] == '1')
                board[y][i] = '+';
            else
                board[y][i] = 'x';
            index++;
        }
    }
}

void place_ship_id(int board[][BOARD_X], int y, int x, char ship_direction, int id, int ship_class) {
    if (ship_direction == 'N') {
        for (int i = y; i < ship_class + y; i++) {
            board[i][x] = id;
        }
    }
    else if (ship_direction == 'W') {
        for (int i = x; i < ship_class + x; i++) {
            board[y][i] = id;
        }
    }
    else if (ship_direction == 'S') {
        for (int i = y; i > y - ship_class; i--) {
            board[i][x] = id;
        }
    }
    else if (ship_direction == 'E') {
        for (int i = x; i > x - ship_class; i--) {
            board[y][i] = id;
        }
    }
}

int get_ship_class(char ship_class_given[]) {
    if (strcmp(ship_class_given, "CAR") == 0)
        return 5;
    else if (strcmp(ship_class_given, "BAT") == 0)
        return 4;
    else if (strcmp(ship_class_given, "CRU") == 0)
        return 3;
    else
        return 2;
}

int check_presence(const int fleetIds[][SHIP_TYPE_BOUND], int ship_class, int number) {
    if (fleetIds[ship_class][number] == 0) {
        return 0;
    }
    return 1;
}

int count_ships(const int fleetIdsA[][SHIP_TYPE_BOUND], const int fleetIdsB[][SHIP_TYPE_BOUND]) {
    int amount = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < SHIP_TYPE_BOUND; j++) {
            amount += fleetIdsA[i][j];
            amount += fleetIdsB[i][j];
        }
    }
    return amount;
}

int AI__count_ships_enemy(const int fleetIds[][SHIP_TYPE_BOUND]) {
    int amount = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < SHIP_TYPE_BOUND; j++) {
            amount += fleetIds[i][j];
        }
    }
    return amount;
}
int AI__ships_set_enemy(const int fleet_set[]) {
    int amount = 0;
    for (int i = 2; i < FLEET_TYPES; i++) {
        amount += fleet_set[i];
    }
    return amount;
}

int check_fleet(const int playerfleet[], const int fleet_set[], const int ship_class) {
    if (playerfleet[ship_class] < fleet_set[ship_class])
        return 1;
    return 0;
}

int ships_set_count(const int fleet_set_A[], const int fleet_set_B[]) {
    int amount = 0;
    for (int i = 2; i < FLEET_TYPES; i++) {
        amount += fleet_set_A[i];
        amount += fleet_set_B[i];
    }
    return amount;
}

int check_position_A(int y, int x, char ship_direction, int ship_class, const initializedPositions PlA) {
    if (ship_direction == 'N') {
        if (y + ship_class - 1 <= PlA.y2 && y >= PlA.y1 && x >= PlA.x1 && x <= PlA.x2)
            return 1;
        else
            return 0;
    }
    else if (ship_direction == 'W') {
        if (x >= PlA.x1 && x + ship_class - 1 <= PlA.x2 && y <= PlA.y2 && y >= PlA.y1)
            return 1;
        else
            return 0;
    }
    else if (ship_direction == 'S') {
        if (y - ship_class + 1 >= PlA.y1 && y <= PlA.y2 && x >= PlA.x1 && x <= PlA.x2)
            return 1;
        else
            return 0;
    }
    else if (ship_direction == 'E') {
        if (x <= PlA.x2 && x - ship_class + 1 >= PlA.x1 && y <= PlA.y2 && y >= PlA.y1)
            return 1;
        else
            return 0;
    }
    return 0;
}

int check_position_B(int y, int x, char ship_direction, int ship_class, const initializedPositions PlB) {
    if (ship_direction == 'N') {
        if (y + ship_class - 1 <= PlB.y2 && y >= PlB.y1 && x >= PlB.x1 && x <= PlB.x2)
            return 1;
        else
            return 0;
    }
    else if (ship_direction == 'W') {
        if (x >= PlB.x1 && x + ship_class - 1 <= PlB.x2 && y <= PlB.y2 && y >= PlB.y1)
            return 1;

        else
            return 0;
    }
    else if (ship_direction == 'S') {
        if (y - ship_class + 1 >= PlB.y1 && y <= PlB.y2 && x >= PlB.x1 && x <= PlB.x2)
            return 1;
        else
            return 0;
    }
    else if (ship_direction == 'E') {
        if (x <= PlB.x2 && x - ship_class + 1 >= PlB.x1 && y < PlB.y2 && y >= PlB.y1)
            return 1;
        else
            return 0;
    }
    return 0;
}

char* get_default_ship(ships* ship) {
    char parts[] = "11111";
    parts[ship->shipclass] = '\0';
    char* partsPtr = parts;
    return partsPtr;
}

point get_cannon_cord(const ships ship) {
    point cannon;
    if (ship.direction == 'N') {
        cannon.x = ship.head_x;
        cannon.y = ship.head_y + 1;
    }
    else if (ship.direction == 'W') {
        cannon.x = ship.head_x + 1;
        cannon.y = ship.head_y;
    }
    else if (ship.direction == 'S') {
        cannon.x = ship.head_x;
        cannon.y = ship.head_y - 1;
    }
    else if (ship.direction == 'E') {
        cannon.x = ship.head_x - 1;
        cannon.y = ship.head_y;
    }
    return cannon;
}

point get_engine_cord(const ships ship) {
    point engine;
    if (ship.direction == 'N') {
        engine.x = ship.head_x;
        engine.y = ship.head_y + ship.shipclass - 1;
    }
    else if (ship.direction == 'W') {
        engine.x = ship.head_x + ship.shipclass - 1;
        engine.y = ship.head_y;
    }
    else if (ship.direction == 'S') {
        engine.x = ship.head_x;
        engine.y = ship.head_y - ship.shipclass + 1;
    }
    else if (ship.direction == 'E') {
        engine.x = ship.head_x - ship.shipclass + 1;
        engine.y = ship.head_y;
    }
    return engine;
}

void add_info(ships* ship, int cord_y, int cord_x, int number, int ship_class, char ship_direction) {
    ship->head_y = cord_y;
    ship->head_x = cord_x;
    ship->id = number;
    ship->shipclass = ship_class;
    ship->direction = ship_direction;
    ship->cannon = get_cannon_cord(*ship);
    ship->engine = get_engine_cord(*ship);
    ship->radar.y = cord_y;
    ship->radar.x = cord_x;
}

int check_reef(const char board[][BOARD_X], int cord_y, int cord_x, char ship_direction, int ship_class) {
    if (cord_y >= 0 && cord_y < BOARD_Y && cord_x >= 0 && cord_x < BOARD_X) {
        if (ship_direction == 'N') {
            for (int i = cord_y; i < ship_class + cord_y; i++) {
                if (board[i][cord_x] == '#') {
                    return 1;
                }
            }
        }
        else if (ship_direction == 'W') {
            for (int i = cord_x; i < ship_class + cord_x; i++) {
                if (board[cord_y][i] == '#') {
                    return 1;
                }
            }
        }
        else if (ship_direction == 'S') {
            for (int i = cord_y; i > cord_y - ship_class; i--) {
                if (board[i][cord_x] == '#') {
                    return 1;
                }
            }
        }
        else if (ship_direction == 'E') {
            for (int i = cord_x; i > cord_x - ship_class; i--) {
                if (board[cord_y][i] == '#') {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void fillTmpBoard(const char board[][BOARD_X], char tmp_board[][BOARD_X + 2]) {
    for (int i = 0; i < BOARD_Y + 2; i++) {
        for (int j = 0; j < BOARD_X + 2; j++) {
            tmp_board[i][j] = ' ';
        }
    }
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            tmp_board[i + 1][j + 1] = board[i][j];
        }
    }
}

int checkSurrounding(const char board[][BOARD_X + 2], int cord_y, int cord_x, char ship_direction, int ship_class) {
    cord_y++;
    cord_x++;
    if (cord_y >= 1 && cord_y < BOARD_Y + 1 && cord_x >= 1 && cord_x < BOARD_X + 1) {
        if (ship_direction == 'N') {
            for (int i = cord_y; i < ship_class + cord_y; i++) {
                if (board[i][cord_x - 1] == '+' || board[i][cord_x + 1] == '+' || board[i + 1][cord_x] == '+' || board[i - 1][cord_x] == '+' || board[i - 1][cord_x - 1] == '+' || board[i - 1][cord_x + 1] == '+' || board[i + 1][cord_x - 1] == '+' || board[i + 1][cord_x + 1] == '+') {
                    return 1;
                }
            }
        }
        else if (ship_direction == 'W') {
            for (int i = cord_x; i < ship_class + cord_x; i++) {
                if (board[cord_y - 1][i] == '+' || board[cord_y + 1][i] == '+' || board[cord_y][i - 1] == '+' || board[cord_y][i + 1] == '+' || board[cord_y - 1][i - 1] == '+' || board[cord_y - 1][i + 1] == '+' || board[cord_y + 1][i - 1] == '+' || board[cord_y + 1][i + 1] == '+') {
                    return 1;
                }
            }
        }
        else if (ship_direction == 'S') {
            for (int i = cord_y; i > cord_y - ship_class; i--) {
                if (board[i - 1][cord_x] == '+' || board[i + 1][cord_x] == '+' || board[i][cord_x - 1] == '+' || board[i][cord_x + 1] == '+' || board[i - 1][cord_x - 1] == '+' || board[i - 1][cord_x + 1] == '+' || board[i + 1][cord_x - 1] == '+' || board[i + 1][cord_x + 1] == '+') {
                    return 1;
                }
            }
        }
        else if (ship_direction == 'E') {
            for (int i = cord_x; i > cord_x - ship_class; i--) {
                if (board[cord_y - 1][i] == '+' || board[cord_y + 1][i] == '+' || board[cord_y][i - 1] == '+' || board[cord_y][i + 1] == '+' || board[cord_y - 1][i - 1] == '+' || board[cord_y - 1][i + 1] == '+' || board[cord_y + 1][i - 1] == '+' || board[cord_y + 1][i + 1] == '+') {
                    return 1;
                }
            }
        }
    }
    return 0;
}
int checkShipsNearby(const char board[][BOARD_X], int cord_y, int cord_x, char ship_direction, int ship_class) {
    char tmp_board[BOARD_Y + 2][BOARD_X + 2];
    fillTmpBoard(board, tmp_board);
    int ifAnyShip = checkSurrounding(tmp_board, cord_y, cord_x, ship_direction, ship_class);
    return ifAnyShip;
}

void reset_moves(int movesP[][SHIP_TYPE_BOUND]) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < SHIP_TYPE_BOUND; j++) {
            movesP[i][j] = 0;
        }
    }
}

void fill_moves_limit_array(int movesLimit[][SHIP_TYPE_BOUND]) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < SHIP_TYPE_BOUND; j++) {
            if (i == CAR)
                movesLimit[i][j] = 2;
            else
                movesLimit[i][j] = 3;
        }
    }
}

void fill_shots_limit_array(int shotsLimit[][SHIP_TYPE_BOUND]) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < SHIP_TYPE_BOUND; j++) {
            if (i == CAR)
                shotsLimit[i][j] = 5;
            else if (i == BAT)
                shotsLimit[i][j] = 4;
            else if (i == CRU)
                shotsLimit[i][j] = 3;
            else if (i == DES)
                shotsLimit[i][j] = 2;
            else
                shotsLimit[i][j] = 0;
        }
    }
}

void reset_shots(int shotsP[][SHIP_TYPE_BOUND]) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < SHIP_TYPE_BOUND; j++) {
            shotsP[i][j] = 0;
        }
    }
}

ships& search_ship(ships shipsP[], int ship_class, int number) {
    for (int i = 0; shipsP[i].id != 99; i++) {
        if (shipsP[i].id == number && shipsP[i].shipclass == ship_class)
            return shipsP[i];
    }
    return shipsP[40];
}

void place_parts(char board[][BOARD_X], ships* moving_ship) {
    if (moving_ship->direction == 'N') {
        int i = moving_ship->head_y;
        for (int c = 0; c < moving_ship->shipclass; c++) {
            if (moving_ship->shipparts[c] == 1)
                board[i][moving_ship->head_x] = '+';
            else
                board[i][moving_ship->head_x] = 'x';
            i++;
        }
    }
    else if (moving_ship->direction == 'W') {
        int i = moving_ship->head_x;
        for (int c = 0; c < moving_ship->shipclass; c++) {
            if (moving_ship->shipparts[c] == 1)
                board[moving_ship->head_y][i] = '+';
            else
                board[moving_ship->head_y][i] = 'x';
            i++;
        }
    }
    else if (moving_ship->direction == 'S') {
        int i = moving_ship->head_y;
        for (int c = 0; c < moving_ship->shipclass; c++) {
            if (moving_ship->shipparts[c] == 1)

                board[i][moving_ship->head_x] = '+';
            else
                board[i][moving_ship->head_x] = 'x';
            i--;
        }
    }
    else if (moving_ship->direction == 'E') {
        int i = moving_ship->head_x;
        for (int c = 0; c < moving_ship->shipclass; c++) {
            if (moving_ship->shipparts[c] == 1)

                board[moving_ship->head_y][i] = '+';
            else
                board[moving_ship->head_y][i] = 'x';
            i--;
        }
    }
}

void move_forward(char board[][BOARD_X], int boardIdA[][BOARD_X], int boardIdB[][BOARD_X], ships* moving_ship, int currPlayer) {
    if (moving_ship->direction == 'N') {
        moving_ship->head_y--;
        moving_ship->cannon.y--;
        moving_ship->engine.y--;
        moving_ship->radar.y--;
    }
    else if (moving_ship->direction == 'S') {
        moving_ship->head_y++;
        moving_ship->cannon.y++;
        moving_ship->engine.y++;
        moving_ship->radar.y++;
    }
    else if (moving_ship->direction == 'W') {
        moving_ship->head_x--;
        moving_ship->cannon.x--;
        moving_ship->engine.x--;
        moving_ship->radar.x--;
    }
    else if (moving_ship->direction == 'E') {
        moving_ship->head_x++;
        moving_ship->cannon.x++;
        moving_ship->engine.x++;
        moving_ship->radar.x++;
    }
    place_parts(board, moving_ship);
    if (currPlayer == A) {
        place_ship_id(boardIdA, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
    }
    else if (currPlayer == B) {
        place_ship_id(boardIdB, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
    }
}


void move_right(char board[][BOARD_X], int boardIdA[][BOARD_X], int boardIdB[][BOARD_X], ships* moving_ship, int currPlayer) {
    if (moving_ship->direction == 'N') {
        moving_ship->head_y--;
        moving_ship->head_x += moving_ship->shipclass - 1;
        moving_ship->cannon.y = moving_ship->head_y;
        moving_ship->cannon.x = moving_ship->head_x - 1;
        moving_ship->engine.y = moving_ship->head_y;
        moving_ship->engine.x = moving_ship->head_x - moving_ship->shipclass + 1;
        moving_ship->radar.y = moving_ship->head_y;
        moving_ship->radar.x = moving_ship->head_x;
        moving_ship->direction = 'E';
    }
    else if (moving_ship->direction == 'S') {
        moving_ship->head_y++;
        moving_ship->head_x -= moving_ship->shipclass - 1;
        moving_ship->cannon.y = moving_ship->head_y;
        moving_ship->cannon.x = moving_ship->head_x + 1;
        moving_ship->engine.y = moving_ship->head_y;
        moving_ship->engine.x = moving_ship->head_x + moving_ship->shipclass - 1;
        moving_ship->radar.y = moving_ship->head_y;
        moving_ship->radar.x = moving_ship->head_x;
        moving_ship->direction = 'W';
    }
    else if (moving_ship->direction == 'W') {
        moving_ship->head_y -= moving_ship->shipclass - 1;
        moving_ship->head_x--;
        moving_ship->cannon.y = moving_ship->head_y + 1;
        moving_ship->cannon.x = moving_ship->head_x;
        moving_ship->engine.y = moving_ship->head_y + moving_ship->shipclass - 1;
        moving_ship->engine.x = moving_ship->head_x;
        moving_ship->radar.y = moving_ship->head_y;
        moving_ship->radar.x = moving_ship->head_x;
        moving_ship->direction = 'N';
    }
    else if (moving_ship->direction == 'E') {
        moving_ship->head_y += moving_ship->shipclass - 1;
        moving_ship->head_x++;
        moving_ship->cannon.y = moving_ship->head_y - 1;
        moving_ship->cannon.x = moving_ship->head_x;
        moving_ship->engine.y = moving_ship->head_y - moving_ship->shipclass + 1;
        moving_ship->engine.x = moving_ship->head_x;
        moving_ship->radar.y = moving_ship->head_y;
        moving_ship->radar.x = moving_ship->head_x;
        moving_ship->direction = 'S';
    }
    place_parts(board, moving_ship);
    if (currPlayer == A) {
        place_ship_id(boardIdA, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
    }
    else if (currPlayer == B) {
        place_ship_id(boardIdB, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
    }
}

void move_left(char board[][BOARD_X], int boardIdA[][BOARD_X], int boardIdB[][BOARD_X], ships* moving_ship, int currPlayer) {
    if (moving_ship->direction == 'N') {
        moving_ship->head_y--;
        moving_ship->head_x -= moving_ship->shipclass - 1;
        moving_ship->cannon.y = moving_ship->head_y;
        moving_ship->cannon.x = moving_ship->head_x + 1;
        moving_ship->engine.y = moving_ship->head_y;
        moving_ship->engine.x = moving_ship->head_x + moving_ship->shipclass - 1;
        moving_ship->radar.y = moving_ship->head_y;
        moving_ship->radar.x = moving_ship->head_x;
        moving_ship->direction = 'W';
    }
    else if (moving_ship->direction == 'S') {
        moving_ship->head_y++;
        moving_ship->head_x += moving_ship->shipclass - 1;
        moving_ship->cannon.y = moving_ship->head_y;
        moving_ship->cannon.x = moving_ship->head_x - 1;
        moving_ship->engine.y = moving_ship->head_y;
        moving_ship->engine.x = moving_ship->head_x - moving_ship->shipclass + 1;
        moving_ship->radar.y = moving_ship->head_y;
        moving_ship->radar.x = moving_ship->head_x;
        moving_ship->direction = 'E';
    }
    else if (moving_ship->direction == 'W') {
        moving_ship->head_y += moving_ship->shipclass - 1;
        moving_ship->head_x--;
        moving_ship->cannon.y = moving_ship->head_y - 1;
        moving_ship->cannon.x = moving_ship->head_x;
        moving_ship->engine.y = moving_ship->head_y - moving_ship->shipclass + 1;
        moving_ship->engine.x = moving_ship->head_x;
        moving_ship->radar.y = moving_ship->head_y;
        moving_ship->radar.x = moving_ship->head_x;
        moving_ship->direction = 'S';
    }
    else if (moving_ship->direction == 'E') {
        moving_ship->head_y -= moving_ship->shipclass - 1;
        moving_ship->head_x++;
        moving_ship->cannon.y = moving_ship->head_y + 1;
        moving_ship->cannon.x = moving_ship->head_x;
        moving_ship->engine.y = moving_ship->head_y + moving_ship->shipclass - 1;
        moving_ship->engine.x = moving_ship->head_x;
        moving_ship->radar.y = moving_ship->head_y;
        moving_ship->radar.x = moving_ship->head_x;
        moving_ship->direction = 'N';
    }
    place_parts(board, moving_ship);
    if (currPlayer == A) {
        place_ship_id(boardIdA, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
    }
    else if (currPlayer == B) {
        place_ship_id(boardIdB, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
    }
}


void remove_ship(char board[][BOARD_X], ships* moving_ship) {
    if (moving_ship->direction == 'N') {
        for (int i = moving_ship->head_y; i < moving_ship->shipclass + moving_ship->head_y; i++) {
            board[i][moving_ship->head_x] = ' ';
        }
    }
    else if (moving_ship->direction == 'W') {
        for (int i = moving_ship->head_x; i < moving_ship->shipclass + moving_ship->head_x; i++) {
            board[moving_ship->head_y][i] = ' ';
        }
    }
    else if (moving_ship->direction == 'S') {
        for (int i = moving_ship->head_y; i > moving_ship->head_y - moving_ship->shipclass; i--) {
            board[i][moving_ship->head_x] = ' ';
        }
    }
    else if (moving_ship->direction == 'E') {
        for (int i = moving_ship->head_x; i > moving_ship->head_x - moving_ship->shipclass; i--) {
            board[moving_ship->head_y][i] = ' ';
        }
    }
}

void remove_shipId(int board[][BOARD_X], ships* moving_ship) {
    if (moving_ship->direction == 'N') {
        for (int i = moving_ship->head_y; i < moving_ship->shipclass + moving_ship->head_y; i++) {
            board[i][moving_ship->head_x] = -1;
        }
    }
    else if (moving_ship->direction == 'W') {
        for (int i = moving_ship->head_x; i < moving_ship->shipclass + moving_ship->head_x; i++) {
            board[moving_ship->head_y][i] = -1;
        }
    }
    else if (moving_ship->direction == 'S') {
        for (int i = moving_ship->head_y; i > moving_ship->head_y - moving_ship->shipclass; i--) {
            board[i][moving_ship->head_x] = -1;
        }
    }
    else if (moving_ship->direction == 'E') {
        for (int i = moving_ship->head_x; i > moving_ship->head_x - moving_ship->shipclass; i--) {
            board[moving_ship->head_y][i] = -1;
        }
    }
}

ships& search_hit_ship(int cord_y, int cord_x, ships shipsP[]) {
    for (int s = 0; shipsP[s].id != 99; s++) {
        if (shipsP[s].direction == 'N') {
            for (int i = shipsP[s].head_y; i < shipsP[s].shipclass + shipsP[s].head_y; i++) {
                if (cord_y == i && cord_x == shipsP[s].head_x)
                    return shipsP[s];
            }
        }
        else if (shipsP[s].direction == 'W') {
            for (int i = shipsP[s].head_x; i < shipsP[s].shipclass + shipsP[s].head_x; i++) {
                if (cord_y == shipsP[s].head_y && cord_x == i)
                    return shipsP[s];
            }
        }
        else if (shipsP[s].direction == 'S') {
            for (int i = shipsP[s].head_y; i > shipsP[s].head_y - shipsP[s].shipclass; i--) {
                if (cord_y == i && cord_x == shipsP[s].head_x)
                    return shipsP[s];
            }
        }
        else if (shipsP[s].direction == 'E') {
            for (int i = shipsP[s].head_x; i > shipsP[s].head_x - shipsP[s].shipclass; i--) {
                if (cord_y == shipsP[s].head_y && cord_x == i)
                    return shipsP[s];
            }
        }
    }
    return shipsP[40];
}

void update_parts(ships* hit_ship, int cord_y, int cord_x) {
    if (hit_ship->direction == 'N') {
        for (int i = hit_ship->head_y; i < hit_ship->shipclass + hit_ship->head_y; i++) {
            if (cord_y == i) {
                hit_ship->shipparts[i - hit_ship->head_y] = 0;
                break;
            }
        }
    }
    else if (hit_ship->direction == 'W') {
        for (int i = hit_ship->head_x; i < hit_ship->shipclass + hit_ship->head_x; i++) {
            if (cord_x == i) {
                hit_ship->shipparts[i - hit_ship->head_x] = 0;
                break;
            }
        }
    }
    else if (hit_ship->direction == 'S') {
        for (int i = hit_ship->head_y; i > hit_ship->head_y - hit_ship->shipclass; i--) {
            if (cord_y == i) {
                hit_ship->shipparts[hit_ship->shipclass - 1 - i + hit_ship->head_y] = 0;
                break;
            }
        }
    }
    else if (hit_ship->direction == 'E') {
        for (int i = hit_ship->head_x; i > hit_ship->head_x - hit_ship->shipclass; i--) {
            if (cord_x == i) {
                hit_ship->shipparts[hit_ship->shipclass - 1 - i + hit_ship->head_x] = 0;
                break;
            }
        }
    }
}

void change_pos_F(ships* tmp_ship) {
    if (tmp_ship->direction == 'N') {
        tmp_ship->head_y--;
    }
    else if (tmp_ship->direction == 'S') {
        tmp_ship->head_y++;
    }
    else if (tmp_ship->direction == 'W') {
        tmp_ship->head_x--;
    }
    else if (tmp_ship->direction == 'E') {
        tmp_ship->head_x++;
    }
}
void change_pos_R(ships* tmp_ship) {
    if (tmp_ship->direction == 'N') {
        tmp_ship->head_y--;
        tmp_ship->head_x += tmp_ship->shipclass - 1;
        tmp_ship->direction = 'E';
    }
    else if (tmp_ship->direction == 'S') {
        tmp_ship->head_y++;
        tmp_ship->head_x -= tmp_ship->shipclass - 1;
        tmp_ship->direction = 'W';
    }
    else if (tmp_ship->direction == 'W') {
        tmp_ship->head_y -= tmp_ship->shipclass - 1;
        tmp_ship->head_x--;
        tmp_ship->direction = 'N';
    }
    else if (tmp_ship->direction == 'E') {
        tmp_ship->head_y += tmp_ship->shipclass - 1;
        tmp_ship->head_x++;
        tmp_ship->direction = 'S';
    }
}
void change_pos_L(ships* tmp_ship) {
    if (tmp_ship->direction == 'N') {
        tmp_ship->head_y--;
        tmp_ship->head_x -= tmp_ship->shipclass - 1;
        tmp_ship->direction = 'W';
    }
    else if (tmp_ship->direction == 'S') {
        tmp_ship->head_y++;
        tmp_ship->head_x += tmp_ship->shipclass - 1;
        tmp_ship->direction = 'E';
    }
    else if (tmp_ship->direction == 'W') {
        tmp_ship->head_y += tmp_ship->shipclass - 1;
        tmp_ship->head_x--;
        tmp_ship->direction = 'S';
    }
    else if (tmp_ship->direction == 'E') {
        tmp_ship->head_y -= tmp_ship->shipclass - 1;
        tmp_ship->head_x++;
        tmp_ship->direction = 'N';
    }
}

int check_is_on_board(ships* tmp_ship) {
    if (tmp_ship->direction == 'N') {
        if (tmp_ship->head_y + tmp_ship->shipclass - 1 < BOARD_Y && tmp_ship->head_y >= 0 && tmp_ship->head_x >= 0 && tmp_ship->head_x <= BOARD_X)
            return 1;
        else
            return 0;
    }
    else if (tmp_ship->direction == 'W') {
        if (tmp_ship->head_x >= 0 && tmp_ship->head_x + tmp_ship->shipclass - 1 < BOARD_X && tmp_ship->head_y < BOARD_Y && tmp_ship->head_y >= 0)
            return 1;
        else
            return 0;
    }
    else if (tmp_ship->direction == 'S') {
        if (tmp_ship->head_y - tmp_ship->shipclass + 1 >= 0 && tmp_ship->head_y < BOARD_Y && tmp_ship->head_x >= 0 && tmp_ship->head_x < BOARD_X)
            return 1;
        else
            return 0;
    }
    else if (tmp_ship->direction == 'E') {
        if (tmp_ship->head_x < BOARD_X && tmp_ship->head_x - tmp_ship->shipclass + 1 >= 0 && tmp_ship->head_y < BOARD_Y && tmp_ship->head_y >= 0)
            return 1;
        else
            return 0;
    }
    return 0;
}

double points_distance(ships* shooting_ship, int cord_y, int cord_x) {
    double distance = sqrt((shooting_ship->cannon.y - cord_y) * (shooting_ship->cannon.y - cord_y) + (shooting_ship->cannon.x - cord_x) * (shooting_ship->cannon.x - cord_x));
    return distance;
}

double points_distance_view(ships* shooting_ship, int cord_y, int cord_x) {
    double distance = sqrt((shooting_ship->radar.y - cord_y) * (shooting_ship->radar.y - cord_y) + (shooting_ship->radar.x - cord_x) * (shooting_ship->radar.x - cord_x));
    return distance;
}


void render_board_extended(char board_extended[][BOARD_X], const char board[][BOARD_X], ships shipsA[], ships shipsB[], int indexA, int indexB) {
    FILL_BOARD(board_extended);
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            board_extended[i][j] = board[i][j];
        }
    }
    for (int i = 0; i < indexA; i++) {
        board_extended[shipsA[i].cannon.y][shipsA[i].cannon.x] = '!';
        board_extended[shipsA[i].engine.y][shipsA[i].engine.x] = '%';
        board_extended[shipsA[i].radar.y][shipsA[i].radar.x] = '@';
    }
    for (int i = 0; i < indexB; i++) {
        board_extended[shipsB[i].cannon.y][shipsB[i].cannon.x] = '!';
        board_extended[shipsB[i].engine.y][shipsB[i].engine.x] = '%';
        board_extended[shipsB[i].radar.y][shipsB[i].radar.x] = '@';
    }
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            if (board[i][j] == 'x') {
                board_extended[i][j] = 'x';
            }
        }
    }
}

void print_ship_class(const ships ship) {
    if (ship.shipclass == 5)
        cout << "CAR";
    else if (ship.shipclass == 4)
        cout << "BAT";
    else if (ship.shipclass == 3)
        cout << "CRU";
    else if (ship.shipclass == 2)
        cout << "DES";
}

void keep_ship_pos(ships* ship, int board_players_view_tmp[][BOARD_X]) {
    if (ship->direction == 'N') {
        for (int i = ship->head_y; i < ship->shipclass + ship->head_y; i++) {
            board_players_view_tmp[i][ship->head_x] = 1;
        }
    }
    else if (ship->direction == 'W') {
        for (int i = ship->head_x; i < ship->shipclass + ship->head_x; i++) {
            board_players_view_tmp[ship->head_y][i] = 1;
        }
    }
    else if (ship->direction == 'S') {
        for (int i = ship->head_y; i > ship->head_y - ship->shipclass; i--) {
            board_players_view_tmp[i][ship->head_x] = 1;
        }
    }
    else if (ship->direction == 'E') {
        for (int i = ship->head_x; i > ship->head_x - ship->shipclass; i--) {
            board_players_view_tmp[ship->head_y][i] = 1;
        }
    }
}

void render_board_player(char board_player[][BOARD_X], const char board[][BOARD_X], int board_players_view_tmp[][BOARD_X], ships shipsP[], int indexP, int spyBoard[][BOARD_X]) {
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            board_player[i][j] = board[i][j];
            board_players_view_tmp[i][j] = 0;
        }
    }
    for (int s = 0; s < indexP; s++) {
        ships* ship = &shipsP[s];
        for (int i = 0; i < BOARD_Y; i++) {
            for (int j = 0; j < BOARD_X; j++) {
                double dst = points_distance_view(ship, i, j);
                if (board[ship->radar.y][ship->radar.x] == 'x') {
                    if (dst <= 1) {
                        board_players_view_tmp[i][j] = 1;
                    }
                }
                else if (dst <= ship->shipclass || spyBoard[i][j]==1) {
                    board_players_view_tmp[i][j] = 1;
                }
            }
        }
        keep_ship_pos(ship, board_players_view_tmp);
    }
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            if (board_players_view_tmp[i][j] == 0)
                board_player[i][j] = '?';
        } 
    }
}

void reset_spy(int spyBoard[][BOARD_X]) {
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            spyBoard[i][j] = 0;
        }
    }
}