
void display_map(player *user, int cols, int rows, char map[rows][cols])
{

    for (int y = 0; y <= rows; y++)
    {
        for (int x = 0; x <= cols; x++)
        {
            if(map[y][x]=='.' || map[y][x]=='|' || map[y][x]=='-'|| map[y][x]=='+')
            {
            attron(COLOR_PAIR(1));
            mvaddch(y+3, x+3, map[y][x]);
            attroff(COLOR_PAIR(1));
            }
            if(map[y][x]=='#')
            {
            attron(COLOR_PAIR(2));
            mvaddch(y+3, x+3, map[y][x]);
            attroff(COLOR_PAIR(2));
            }

        }
    }
    mvprintw(rows, 2, "level: %d", user->level);
    mvprintw(rows, 20, "Gold: %d", user->gold);

    return;
}

int draw_map_for_other_floor(int rows, player *user,
                             int cols, char map[rows][cols], room map_rooms[6],
                             int floor, char all_floor_room[4][rows][cols])
{

    srand(time(NULL));
    int first_y, first_x;
    int size_y;
    int size_x;
    int current_center_y;
    int current_center_x;
    int previous_center_y;
    int previous_center_x;
    int room_number = 6;
    int collision_of_hallway = 0;

    collision_of_hallway = 0;
    num_rooms = 0;

    // draw_first_room_of_floor
    // center of_room is initial_x and initial_y
    do
    {
        size_y = (rand() % 5 + 8);
        size_x = (rand() % 5 + 8);
        first_x = user->position.x - size_x / 2;
        first_y = user->position.y - size_y / 2;
    } while (first_x < 2 || first_y < 2);
    map_rooms[num_rooms].size_x = size_x;
    map_rooms[num_rooms].size_y = size_y;
    map_rooms[num_rooms].x = first_x;
    map_rooms[num_rooms].y = first_y;
    for (int y = first_y + 1; y <= first_y + size_y - 1; y++)
    {
        for (int x = first_x + 1; x <= first_x + size_x - 1; x++)
        {
            map[y][x] = '.';
        }
    }
    for (int yy = first_y; yy <= first_y + size_y; yy++)
    {
        map[yy][first_x] = '|';
        map[yy][first_x + size_x] = '|';
    }
    for (int xx = first_x; xx <= first_x + size_x; xx++)
    {
        map[first_y][xx] = '-';
        map[first_y + size_y][xx] = '-';
    }

    num_rooms++;
    current_center_y = first_y + size_y / 2;
    current_center_x = first_x + size_x / 2;

    while (num_rooms < room_number)
    {
        bool collision;
        int room_loop_counter = 0;
        int temp_x = first_x, temp_y = first_y, temp_size_y = size_y, temp_size_x = size_x;
        do
        {
            collision = 0;
            do
            {
                first_y = rand() % rows - 2;
                first_x = rand() % cols - 2;

                size_y = rand() % 5 + 8;
                size_x = rand() % 5 + 8;

            } while (first_y < 2 || first_x + size_x > cols - 2 || first_y + size_y > rows - 2 || first_x < 2);

            room_loop_counter++;
            if (room_loop_counter > 100)
            {

                return 0;
            }
        } while (check_collision(cols, map, first_y, first_x, size_y, size_x) ||

                 check_possible_hallway(temp_x + temp_size_x / 2, temp_y + temp_size_y / 2,
                                        first_x + size_x / 2, first_y + size_y / 2,
                                        rows, cols, map));

        map_rooms[num_rooms].size_x = size_x;
        map_rooms[num_rooms].size_y = size_y;
        map_rooms[num_rooms].x = first_x;
        map_rooms[num_rooms].y = first_y;

        for (int y = first_y + 1; y <= first_y + size_y - 1; y++)
        {
            for (int x = first_x + 1; x <= first_x + size_x - 1; x++)
            {
                map[y][x] = '.';
            }
        }
        for (int yy = first_y; yy <= first_y + size_y; yy++)
        {
            map[yy][first_x] = '|';
            map[yy][first_x + size_x] = '|';
        }
        for (int xx = first_x; xx <= first_x + size_x; xx++)
        {
            map[first_y][xx] = '-';
            map[first_y + size_y][xx] = '-';
        }
        num_rooms++;

        previous_center_x = current_center_x;
        previous_center_y = current_center_y;

        // room center
        current_center_y = first_y + size_y / 2;
        current_center_x = first_x + size_x / 2;
        if (num_rooms > 1)
        {
            int y_in_way, x_in_way;
            for (y_in_way = previous_center_y; y_in_way != current_center_y;)
            {
                if (map[y_in_way][previous_center_x] == ' ')
                {
                    map[y_in_way][previous_center_x] = '#';
                }
                else if (map[y_in_way][previous_center_x] == '-' || map[y_in_way][previous_center_x] == '|')
                    map[y_in_way][previous_center_x] = '+';
                if (previous_center_y < current_center_y)
                {
                    y_in_way++;
                }
                else if (previous_center_y > current_center_y)
                {
                    y_in_way--;
                }
            }
            for (int x_in_way = previous_center_x; x_in_way != current_center_x;)
            {

                if (map[y_in_way][x_in_way] == ' ')
                    map[y_in_way][x_in_way] = '#';
                else if (map[y_in_way][x_in_way] == '-' || map[y_in_way][x_in_way] == '|')
                    map[y_in_way][x_in_way] = '+';
                if (previous_center_x < current_center_x)
                {
                    x_in_way++;
                }
                else if (previous_center_x > current_center_x)
                {
                    x_in_way--;
                }
            }
        }
    }

    map[user->position.y][user->position.x] = '<';

    display_map(user, cols, rows, map);

    do
    {
        initial_y = rand() % rows;
        user->position.y = initial_y;
        initial_x = rand() % cols;
        user->position.x = initial_x;
    } while (map[initial_y][initial_x] != '.');

    return 1;
}

int draw_map_for_first_floor(int rows, player *user, int cols, char map[rows][cols], room map_rooms[6],
                             int floor, char all_floor_room[4][rows][cols])
{

    srand(time(NULL));
    int first_y, first_x;
    int size_y;
    int size_x;
    int current_center_y;
    int current_center_x;
    int previous_center_y;
    int previous_center_x;
    int room_number = 6;
    int collision_of_hallway = 0;

    collision_of_hallway = 0;
    num_rooms = 0;
    while (num_rooms < room_number)
    {
        bool collision;
        int room_loop_counter = 0;
        int temp_x = first_x, temp_y = first_y, temp_size_y = size_y, temp_size_x = size_x;
        do
        {
            collision = 0;
            do
            {
                first_y = rand() % rows - 2;
                first_x = rand() % cols - 2;

                size_y = rand() % 5 + 8;
                size_x = rand() % 5 + 8;

            } while (first_y < 2 || first_x + size_x > cols - 2 || first_y + size_y > rows - 2 || first_x < 2);

            room_loop_counter++;
            if (room_loop_counter > 100)
            {

                return 0;
            }
        } while (check_collision(cols, map, first_y, first_x, size_y, size_x) ||

                 check_possible_hallway(temp_x + temp_size_x / 2, temp_y + temp_size_y / 2,
                                        first_x + size_x / 2, first_y + size_y / 2,
                                        rows, cols, map));

        map_rooms[num_rooms].size_x = size_x;
        map_rooms[num_rooms].size_y = size_y;
        map_rooms[num_rooms].x = first_x;
        map_rooms[num_rooms].y = first_y;

        for (int y = first_y + 1; y <= first_y + size_y - 1; y++)
        {
            for (int x = first_x + 1; x <= first_x + size_x - 1; x++)
            {
                map[y][x] = '.';
            }
        }
        for (int yy = first_y; yy <= first_y + size_y; yy++)
        {
            map[yy][first_x] = '|';
            map[yy][first_x + size_x] = '|';
        }
        for (int xx = first_x; xx <= first_x + size_x; xx++)
        {
            map[first_y][xx] = '-';
            map[first_y + size_y][xx] = '-';
        }
        num_rooms++;

        if (num_rooms > 0)
        {
            previous_center_x = current_center_x;
            previous_center_y = current_center_y;
        }
        // room center
        current_center_y = first_y + size_y / 2;
        current_center_x = first_x + size_x / 2;
        if (num_rooms > 1)
        {
            int y_in_way, x_in_way;
            for (y_in_way = previous_center_y; y_in_way != current_center_y;)
            {
                if (map[y_in_way][previous_center_x] == ' ')
                {
                    map[y_in_way][previous_center_x] = '#';
                }
                else if (map[y_in_way][previous_center_x] == '-' || map[y_in_way][previous_center_x] == '|')
                    map[y_in_way][previous_center_x] = '+';
                if (previous_center_y < current_center_y)
                {
                    y_in_way++;
                }
                else if (previous_center_y > current_center_y)
                {
                    y_in_way--;
                }
            }
            for (int x_in_way = previous_center_x; x_in_way != current_center_x;)
            {

                if (map[y_in_way][x_in_way] == ' ')
                    map[y_in_way][x_in_way] = '#';
                else if (map[y_in_way][x_in_way] == '-' || map[y_in_way][x_in_way] == '|')
                    map[y_in_way][x_in_way] = '+';
                if (previous_center_x < current_center_x)
                {
                    x_in_way++;
                }
                else if (previous_center_x > current_center_x)
                {
                    x_in_way--;
                }
            }
        }
    }


    display_map(user, cols, rows, map);

    int yy, xx;
    do
    {
        yy = rand() % rows - 3;
        xx = rand() % cols - 3;

    } while (map[yy][xx] != '.');
    map[yy][xx] = '<';

    return 1;
}


int move_char(int input, player *user, int cols, int rows, char map[rows][cols])
{
    display_map(user, cols, rows, map);

    if (p_placed == 0)
    {
        do
        {
            initial_y = rand() % rows - 3;
            user->position.y = initial_y - 3;
            initial_x = rand() % cols;
            user->position.x = initial_x;
        } while (map[initial_y][initial_x] != '.');
        p_placed = 1;
    }

    if (input == KEY_UP)
        initial_y--;
    else if (input == KEY_DOWN)
        initial_y++;
    else if (input == KEY_RIGHT)
        initial_x++;
    else if (input == KEY_LEFT)
        initial_x--;

    switch (map[initial_y][initial_x])
    {
    case '.':
        user->position.y = initial_y;
        user->position.x = initial_x;
        mvaddch(user->position.y, user->position.x, 'H');
        return 1;

   case '+':
        user->position.y = initial_y;
        user->position.x = initial_x;
        mvaddch(user->position.y, user->position.x, 'H');
        return 1;
        

    case '#':
        user->position.y = initial_y;
        user->position.x = initial_x;
        mvaddch(user->position.y, user->position.x, 'H');
        return 1;

    case '|':
    case '-':
    case ' ':
        initial_x = user->position.x;
        initial_y = user->position.y;
        mvaddch(user->position.y, user->position.x, 'H');
        return 1;

    case '<':
        initial_x = user->position.x;
        initial_y = user->position.y;
        user->level++;
        return 2;
    }
}
void control_play_in_a_floor(int rows, int cols, int floor,
                             char all_floor[4][rows][cols],
                             room all_floor_rooms[4][6], player *user)

{


    if (floor > 4)
    {
        return;
    }
    char map[rows+1][cols+1];
    room map_rooms[6];

    if (floor == 0)
    {
        do
        {
            for (int i = 0; i < 6; i++)
            {
                map_rooms[i].size_x = 0;
                map_rooms[i].size_y = 0;
                map_rooms[i].x = 0;
                map_rooms[i].y = 0;
            }
            initilizing_map(rows - 1, cols - 1, map);
        } while (!draw_map_for_first_floor(rows - 1, user, cols - 1, map, map_rooms, floor, all_floor));
    }
    else
    {
        do
        {
            for (int i = 0; i < 6; i++)
            {
                map_rooms[i].size_x = 0;
                map_rooms[i].size_y = 0;
                map_rooms[i].x = 0;
                map_rooms[i].y = 0;
            }
            initilizing_map(rows - 1, cols - 1, map);
        } while (!draw_map_for_other_floor(rows - 1, user, cols - 1, map, map_rooms, floor, all_floor));
    }
    int input;


    int k;
    do
    {
        k = move_char(input, user, cols - 1, rows - 1, map);
        if (k == 2)
            break;
    } while ((input = getch()) != 'q'); // q means quit
    for (int i = 0; i < rows; i++)
    {
        strcpy(all_floor[floor][i], map[i]);
    }

    for (int i = 0; i < 6; i++)
    {
        all_floor_rooms[floor][i] = map_rooms[i];
    }

    if (k == 2)
    {
        control_play_in_a_floor(rows, cols, floor + 1,
                                all_floor, all_floor_rooms, user);
    }
    return;
}

void new_game(Gamer *g)
{
    clear();
    int rows;
    int cols;
    getmaxyx(stdscr, rows, cols);
    int level = 4;
    char all_floor[level][rows][cols];
    player user;
    user.level = 1;
    room all_floor_rooms[4][6];

    control_play_in_a_floor(rows * 2 / 3, cols *2 /3, 0,
                            all_floor, all_floor_rooms, &user);

    refresh();
    getch();
}
