#pragma once

struct initializedPositions {
    int y1;
    int y2;
    int x1;
    int x2;
};

struct point {
    int y;
    int x;
};

struct ships {
    int head_y;
    int head_x;
    int id;
    int shipclass;
    char direction;
    int shipparts[5];
    point cannon;
    point engine;
    point radar;
};