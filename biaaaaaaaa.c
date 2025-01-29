
bool init = 0;
bool p_placed = 0;
int r_placed = 0;
bool t_placed = 0;
int p_gold = 0;

void initilizing_map(int rows, int cols, char map[rows][cols])
{

    for (int y = 0; y <= rows; y++)
    {
        for (int x = 0; x <= cols; x++)
        {
            map[y][x] = ' ';
        }
    }
    return;
}

int check_collision(int cols, char map[][cols], int y, int x, int width, int height)
{
    for (int y1 = y; y1 <= y + width; y1++)
    {
        for (int x1 = x; x1 <= x + height; x1++)
        {
            if (map[y1][x1] == '.' ||
                map[y1 + 3][x1] == '.' || map[y1][x1 + 3] == '.' ||
                map[y1 - 3][x1] == '.' || map[y1][x1 - 3] == '.')
            {
                return 1;
            }
        }
    }
    return 0;
}

int draw_map(int input, int rows, int cols, char map[rows][cols])
{
    srand(time(NULL));
    int first_y, first_x;
    int size_y;
    int size_x;
    int r_center_y;
    int r_center_x;
    int r_old_center_y;
    int r_old_center_x;
    int room_number = 1; //rand() % 2 + 4;
    int collision_of_hallway = 0;

    // do
    //{
    collision_of_hallway = 0;
    r_placed = 0;
    while (r_placed < room_number)
    {
        bool collision;
        int room_loop_counter = 0;

        do
        {
            collision = 0;
            // gen room size
            do
            {
                first_y = rand() % rows - 3;
                first_x = rand() % cols - 3;

                size_y = rand() % 5 + 8;
                size_x = rand() % 5 + 4;

            } while (first_y < 2 || first_x + size_x > cols - 2 || first_y + size_y > rows - 2 || first_x < 2);

            room_loop_counter++;
            if (room_loop_counter > 100)
            {
                first_y = 11;
                first_x = 11;
                size_y = 2;
                size_x = 2;
                break;
            }
        } while (check_collision(cols, map, first_y, first_x, size_y, size_x));

        // room fill map db with room coords
        for (int y = first_y ; y <= first_y + size_y ; y++)
        {
            for (int x = first_x  ; x <= first_x + size_x ; x++)
            {
                map[y][x] = '.';
            }
        }
       /* for (int yy = first_y; yy <= first_y + size_y; yy++)
        {
            map[yy][first_x] = '|';
            map[yy][first_x + size_x] = '|';
        }
        for (int xx = first_x; xx <= first_x + size_x; xx++)
        {
            map[first_y][xx] = '-';
            map[first_y + size_y][xx] = '-';
        } */

        r_placed++;
        // save center of previous room
        /*       if (r_placed > 0)
               {
                   r_old_center_x = r_center_x;
                   r_old_center_y = r_center_y;
               }
               // room center
               r_center_y = first_y + size_y / 2;
               r_center_x = first_x + size_x / 2;
               if (r_placed > 1)
               {
                   int path_y, path_x;
                   for (path_y = r_old_center_y; path_y != r_center_y;)
                   {
                       if (map[path_y][r_old_center_x] == '.' || map[path_y][r_old_center_x] == '-' ||
                           map[path_y][r_old_center_x] == '|')
                       {
                           collision_of_hallway = 1;
                           path_x = r_center_x;
                           break;
                       }
                       map[path_y][r_old_center_x] = '#';
                       if (r_old_center_y < r_center_y)
                       {
                           path_y++;
                       }
                       else if (r_old_center_y > r_center_y)
                       {
                           path_y--;
                       }
                   }
                   for (int path_x = r_old_center_x; path_x != r_center_x;)
                   {
                       if (map[path_y][r_old_center_x] == '.' || map[path_y][r_old_center_x] == '-' ||
                           map[path_y][r_old_center_x] == '|')
                       {
                           collision_of_hallway = 1;
                           break;
                       }
                       map[path_y][path_x] = ' ';
                       if (r_old_center_x < r_center_x)
                       {
                           path_x++;
                       }
                       else if (r_old_center_x > r_center_x)
                       {
                           path_x--;
                       }
                   }
               } */
    }
    // } while (!collision_of_hallway);

    // drawing map
    for (int yy = 0; yy <= rows; yy++)
    {
        for (int xx = 0; xx <= cols; xx++)
        {
            if (map[yy][xx] == '.')
                mvaddch(yy, xx, '.');
            else if (map[yy][xx] == '-')
                mvaddch(yy, xx, '-');
            else if (map[yy][xx] == '|')
                mvaddch(yy, xx, '|');
            else if (map[yy][xx] == '#')
                mvaddch(yy, xx, '#');
            else
                mvaddch(yy, xx, ' ');

            // else if (yy == 0 || yy == rows - 1)
            //  {
            //  mvaddch(yy, xx, '%');
            // }
            /**  else if (yy == rows)
              {
                  mvaddch(yy, xx, ' ');
              }
              else if (xx == 0 || xx == cols)
              {
                  mvaddch(yy, xx, '%');
              } */

            // else if (map[yy][xx] == '|')
            //  mvaddch(yy, xx, '|');
            // else if (map[yy][xx] == '-')
            // mvaddch(yy, xx, '-');
        }
    }

    mvprintw(rows, 0, "Gold: %d", p_gold);
    return 1;
}

int initial_x, initial_y;
int ty, tx;

void move_char(int input, player *user, int cols, int rows, char map[rows][cols])
{
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
    if (input == KEY_UP && map[initial_y - 1][initial_x] == '.')
        initial_y--;
    else if (input == KEY_DOWN && map[initial_y + 1][initial_x] == '.')
        initial_y++;
    else if (input == KEY_RIGHT && map[initial_y][initial_x + 1] == '.')
        initial_x++;
    else if (input == KEY_LEFT && map[initial_y][initial_x - 1] == '.')
        initial_x--;

    for (int yy = 0; yy <= rows; yy++)
    {
        for (int xx = 0; xx <= cols; xx++)
        {
            if (map[yy][xx] == '.')
                mvaddch(yy, xx, '.');
            else if (map[yy][xx] == '-')
                mvaddch(yy, xx, '-');
            else if (map[yy][xx] == '|')
                mvaddch(yy, xx, '|');
            else if (map[yy][xx] == '#')
                mvaddch(yy, xx, '#');
            else
                mvaddch(yy, xx, ' ');
        }
    }

    if (t_placed == 0)
    {
        do
        {
            ty = rand() % rows - 1;
            tx = rand() % cols - 1;
        } while (map[ty][tx] != '.');
        t_placed = 1;
    }

    if (ty == initial_y && tx == initial_x)
    {
        t_placed = 0;
        // p_placed=0;
        // r_placed=0;
        p_gold += rand() % 10 + 1;
    }

    mvaddch(ty, tx, 't');
    mvaddch(initial_y, initial_x, 'H');
    user->position.y = initial_y;
    user->position.x = initial_x;
}

void new_game(Gamer *g)
{
    clear();
    int rows;
    int cols;
    getmaxyx(stdscr, rows, cols);
    char map[rows][cols];
    int input;
    player user;

    initilizing_map(rows - 1, cols - 1, map);
    draw_map(input, rows - 1, cols - 1, map);

    do
    {
        move_char(input, &user, cols, rows, map);

    } while ((input = getch()) != 'q'); // 'q' means quit
    // draw_map(&dungeon, cols, lines);
    refresh();
    getch();
}
