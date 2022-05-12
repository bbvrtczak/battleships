#pragma once

#define BOARD_X 10
#define BOARD_Y 21

#define FLEET_TYPES 6
#define SHIP_TYPE_BOUND 10

#define CAR 5
#define BAT 4
#define CRU 3
#define DES 2


#include "structs.h"
#include "board_func.h"
#include "ship_func.h"
#include "functions.h"

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