

#define MAX_TILES 40

void connect_rooms(room* rooms, int room_count, char** map) {
    for (int i = 0; i < room_count - 1; i++) {
        room roomA = rooms[i];
        room roomB = rooms[i + 1];

        int centerX1 = roomA.first_x + roomA.width / 2;
        int centerY1 = roomA.first_y + roomA.height / 2;
        int centerX2 = roomB.first_x + roomB.width / 2;
        int centerY2 = roomB.first_y + roomB.height / 2;

        // افقی و سپس عمودی
        if (rand() % 2) {
            for (int x = centerX1; x != centerX2; x += (centerX2 > centerX1 ? 1 : -1)) {
                map[x][centerY1] = '#';
            }
            for (int y = centerY1; y != centerY2; y += (centerY2 > centerY1 ? 1 : -1)) {
                map[centerX2][y] = '#';
            }
        } else {
            // عمودی و سپس افقی
            for (int y = centerY1; y != centerY2; y += (centerY2 > centerY1 ? 1 : -1)) {
                map[centerX1][y] = '#';
            }
            for (int x = centerX1; x != centerX2; x += (centerX2 > centerX1 ? 1 : -1)) {
                map[x][centerY2] = '#';
            }
        }
    }
}


int check_rooms(int rooms, room *new_rooms, room new_room)
{
    for (int k = 0; k <= rooms; k++)
    {
        if ((new_rooms[k].first_x < new_room.first_x + new_room.width &&
             new_rooms[k].first_x + new_rooms[k].width + 5 > new_room.first_x &&
             new_rooms[k].first_y < new_room.first_y + new_room.height &&
             new_rooms[k].first_y + new_rooms[k].height + 5 > new_room.first_y) ||
            (new_rooms[k].first_x < new_room.first_x + new_room.width + 5 &&
             new_rooms[k].first_x + new_rooms[k].width > new_room.first_x &&
             new_rooms[k].first_y < new_room.first_y + new_room.height + 5 &&
             new_rooms[k].first_y + new_rooms[k].height > new_room.first_y))

        {
            return 0;
        }
    }
    return 1;
}


void draw_map()
{
    clear();
    srand(time(NULL));
    int COLS = getmaxx(stdscr);
    int LINES = getmaxy(stdscr);
    char** map = (char **)malloc(COLS * sizeof(char *));
    // grid g;
    //g.width = COLS;
    //g.height = LINES;
    for (int i = 0; i < COLS; i++)
    {
        map[i] = (char *)malloc(LINES * sizeof(char));
        /* for (int j = 0; j < LINES - 1; j++)
        {
            map[i][j] = '&'; //& means it's empty
            g.nodes[i][j].x = i;
            g.nodes[i][j].y = j;
            g.nodes[i][j].g = INT32_MAX;
            g.nodes[i][j].h = 0;
            g.nodes[i][j].f = INT32_MAX;
            g.nodes[i][j].parent = NULL;
        }*/
    }
    for (int i = 0; i < COLS; i++)
    {
        for (int j = 0; j < LINES - 1; j++)
        {
            map[i][j] = '&'; //& means it's empty

        }
    }
    for (int i = 0; i < COLS; i++)
    {
        map[i][LINES - 1] = '\0';
    }

    int number_of_rooms = rand() % (max_number_of_room - min_number_of_rooms) + min_number_of_rooms;
    int max_size = ((COLS - 5) / (number_of_rooms * 8)) < ((LINES - 5) / (number_of_rooms * 8)) ? ((COLS - 5) / (number_of_rooms * 8)) : ((LINES - 7) / (number_of_rooms * 8));
    room *new_rooms = (room *)malloc(number_of_rooms * sizeof(room));
    int rooms = 0;

    for (int i = 0; i < number_of_rooms; i++)
    {
        int width = rand() % (max_size - min_size_of_h_w) + min_size_of_h_w;
        int height = rand() % (max_size - min_size_of_h_w) + min_size_of_h_w;
        room new_room;
        new_room.width = width;
        new_room.height = height;
        do
        {
            new_room.first_x = rand() % (COLS - 4 - width);
            new_room.first_y = rand() % (LINES - 4 - height);
        } while (!check_rooms(rooms, new_rooms, new_room));
        new_room.number_of_door = 0;
        new_rooms[rooms++] = new_room;

        

        for (int w = 1; w < width - 1; w++)
        {
            for (int h = 1; h < height - 1; h++)
            {
                map[new_room.first_x + w][new_room.first_y + h] = '.';
            }
        }
        for (int p = 1; p < height - 1; p++)
        {
            map[new_room.first_x][new_room.first_y + p] = '|';
            map[width - 1 + new_room.first_x][new_room.first_y + p] = '|';
        }
        for (int p = 1; p < width - 1; p++)
        {
            map[new_room.first_x + p][new_room.first_y] = '-';
            map[p + new_room.first_x][new_room.first_y + height - 1] = '-';
        }
        map[new_room.first_x][new_room.first_y + height - 1] = '#';
        map[new_room.first_x][new_room.first_y] = '#';
        map[new_room.first_x + width - 1][new_room.first_y] = '#';
        map[new_room.first_x + width - 1][new_room.first_y + height - 1] = '#';

        
    }

    connect_rooms(new_rooms, rooms, map); // Connect the rooms with Delaunay triangulation

    for (int j = 0; j < LINES; j++)
    {
        for (int i = 0; i < COLS; i++)
        {
            if (map[i][j] != '&')
            {
                mvprintw(j, i, "%c", map[i][j]);
            }
        }
    }
    refresh();
    getch();
}

void new_game(Gamer*g) {
 clear();
 draw_border(); // از تابع شما استفاده می‌کنیم
 draw_map();
 refresh();
 getch();
}

