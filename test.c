#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double row;
    double col;
} pair;

typedef struct {
    pair start;
    pair end;
    double length;
} edge;

typedef struct {
    double first_x;
    double first_y;
    double width;
    double height;
    pair entrance;
    pair exit;
} room;

void draw_room(room Room) {
    int x1 = (int)Room.first_x;
    int y1 = (int)Room.first_y;
    int x2 = x1 + (int)Room.width;
    int y2 = y1 + (int)Room.height;

    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
            if (y == y1 || y == y2 || x == x1 || x == x2) {
                mvprintw(y, x, "#");
            } else {
                mvprintw(y, x, " ");
            }
        }
    }
}

void draw_hallway(edge Edge) {
    int x1 = (int)Edge.start.col;
    int y1 = (int)Edge.start.row;
    int x2 = (int)Edge.end.col;
    int y2 = (int)Edge.end.row;
    mvprintw(y1, x1, "*");
    while (x1 != x2 || y1 != y2) {
        if (x1 < x2) x1++;
        else if (x1 > x2) x1--;
        if (y1 < y2) y1++;
        else if (y1 > y2) y1--;
        mvprintw(y1, x1, "*");
    }
}

void room_to_point(room* new_rooms, int rooms) {
    for (int i = 0; i < rooms; i++) {
        new_rooms[i].entrance.row = new_rooms[i].first_y + new_rooms[i].height / 2;
        new_rooms[i].entrance.col = new_rooms[i].first_x + new_rooms[i].width - 1;

        new_rooms[i].exit.row = new_rooms[i].first_y + new_rooms[i].height / 2;
        new_rooms[i].exit.col = new_rooms[i].first_x;
    }
}

int main() {
    initscr();
    noecho();
    cbreak();

    room new_rooms[3];
    new_rooms[0] = (room){3, 3, 5, 5};
    new_rooms[1] = (room){15, 15, 5, 5};
    new_rooms[2]=(room){24,90,35,6};


    room_to_point(new_rooms, 3);

    for (int i = 0; i < 3; i++) {
        draw_room(new_rooms[i]);
    }

    edge hallway;
    hallway.start = new_rooms[0].entrance;
    hallway.end = new_rooms[1].exit;
    draw_hallway(hallway);

    refresh();
    getch();
    endwin();

    return 0;
}