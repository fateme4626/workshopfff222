void draw_path(pair start, pair current, pair **parent, char**map) {
    map[current.row][current.col] = '*';
    if (current.row != start.row || current.col != start.col) {
        draw_path(start, parent[current.row][current.col], parent, map);
    }
}

int dfs_visit(char**map, char **mark, int x, int y, pair **parent, pair finish, int COLS, int LINES) {
    mark[x][y] = 'v';
    if (x == finish.row && y == finish.col) {
        return 1;
    }
    int deltax[] = {-1, 0, 0, +1};
    int deltay[] = {0, +1, -1, 0};
    for (int i = 0; i < 4; i++) {
        int nx = x + deltax[i];
        int ny = y + deltay[i];
        if (nx >= 0 && nx < COLS && ny >= 0 && ny < LINES) {
            if (map[nx][ny] == '&' && mark[nx][ny] == 'n') {
                parent[nx][ny].row = x;
                parent[nx][ny].col = y;
                if (dfs_visit(map, mark, nx, ny, parent, finish, COLS, LINES)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int draw_hallway_point(int i, int num, room *new_rooms, char **map) {
    int COLS = getmaxx(stdscr);
    int LINES = getmaxy(stdscr);

    char **mark = (char** )malloc(COLS * sizeof(char *));
    for (int j = 0; j < COLS; j++) {
        mark[j] = (char *)malloc(LINES * sizeof(char));
    }

    pair** parent = (pair** )malloc(COLS * sizeof(pair *));
    for (int j = 0; j < COLS; j++) {
        parent[j] = (pair *)malloc(LINES * sizeof(pair));
    }

    for (int k = 0; k < COLS; k++) {
        for (int l = 0; l < LINES; l++) {
            mark[k][l] = 'n';
        }
    }

    pair start = {new_rooms[i].first_x + new_rooms[i].width / 2, new_rooms[i].first_y + new_rooms[i].height / 2};
    pair finish = {new_rooms[num].first_x + new_rooms[num].width / 2, new_rooms[num].first_y + new_rooms[num].height / 2};

    if (dfs_visit(map, mark, start.row, start.col, parent, finish, COLS, LINES)) {
        draw_path(start, finish, parent, map);
        for (int k = 0; k < COLS; k++) {
            free(mark[k]);
            free(parent[k]);
        }
        free(mark);
        free(parent);
        return 1;
    }

    for (int k = 0; k < COLS; k++) {
        free(mark[k]);
        free(parent[k]);
    }
    free(mark);
    free(parent);
    return 0;
}

void draw_hallway(room *new_rooms, char **map, int rooms) {
    int COLS = getmaxx(stdscr);
    int LINES = getmaxy(stdscr);
    srand(time(NULL));
    int num;
    for (int i = 0; i < rooms; i++) {
        if (new_rooms[i].number_of_door == 0) {
            do {
                num = rand() % rooms;
            } while (num == i || !draw_hallway_point(i, num, new_rooms, map));
        }
    }
}