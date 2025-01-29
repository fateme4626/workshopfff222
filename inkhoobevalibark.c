
int draw_map(int input, int rows, int cols, char map[rows][cols])
{
    srand(time(NULL));

    if (r_placed == 0)
    {

        for (int yy = 0; yy <= rows; yy++)
        {
            for (int xx = 0; xx <= cols; xx++)
            {

                // top and bottom line
                if (yy == 0 || yy == rows - 1)
                {
                    map[yy][xx] = ' ';
                }
                // UI bottom line
                else if (yy == rows)
                {
                    map[yy][xx] = ' ';
                }
                // right and left border
                else if ((xx == 0 || xx == cols) && yy != rows)
                {
                    map[yy][xx] = ' ';
                }

                // else if (map[yy][xx] == '|')
                //  mvaddch(yy, xx, '|');
                // else if (map[yy][xx] == '-')
                // mvaddch(yy, xx, '-');

                // internal walls
                else
                    map[yy][xx] = ' ';
            }
        }
    }
    // room generation
    if (r_placed == 0)
    {
        int ry, rx;
        int r_size_y;
        int r_size_x;
        int r_center_y;
        int r_center_x;
        int r_old_center_y;
        int r_old_center_x;
        int room_number =  rand() % 10 + 5;

        while (r_placed < room_number)
        {
            bool collision;
            int room_loop_counter=0;

            do
            {
                collision = 0;
                // gen room size
                do
                {
                    ry = rand() % rows - 3;
                    rx = rand() % cols - 3;

                    r_size_y = rand() % 5 + 8;
                    r_size_x = rand() % 5 + 4;

                } while (ry < 1 || rx < 1);

                // check for collision
                for (int yy = ry; yy <= ry + r_size_y; yy++)
                {
                    for (int xx = rx; xx <= rx + r_size_x; xx++)
                    {
                        if (map[yy][xx] == ' ' || map[yy][xx] == '%' ||
                            map[yy + 2][xx] == ' ' || map[yy][xx + 2] == ' ' ||
                            map[yy - 2][xx] == ' ' || map[yy][xx - 2] == ' ')
                        {
                            collision = 1;
                            yy = ry + r_size_y; // to exit upper loop ...
                            break;              //  ... and exit current loop
                        }
                    }
                }
                // for (int yy = ry; yy <= ry + r_size_y; yy++)
                //{
                //   map[yy][rx] = '|';
                // mvaddch(yy, rx, '|');
                //  map[yy][rx + r_size_x] = '|';
                // mvaddch(yy, rx + r_size_x, '|');
                // }
                /* for (int xx = rx; xx <= rx + r_size_x; xx++)
                 {
                     map[ry][xx] = '-';
                     mvaddch(ry, xx, '-');
                     map[ry + r_size_y][xx] = '-';
                     mvaddch(ry + r_size_y, xx, '-');
                 } */
                room_loop_counter++;
                if(room_loop_counter > 100){
                    ry = 11 ; rx=11;
                    r_size_y = 2; r_size_x= 2;
                    break;
                }
            } while (collision);

            // room fill map db with room coords
            for (int yy = ry; yy <= ry + r_size_y; yy++)
            {
                for (int xx = rx; xx <= rx + r_size_x; xx++)
                {
                    if (map[yy][xx] != '%' && yy > 0 && yy < rows - 1 && xx > 0 && xx < cols)
                    {
                        map[yy][xx] = ' ';
                    }
                }
            }
            r_placed++;
//save center of previous room
            if(r_placed > 0)
            {
            r_old_center_x= r_center_x;
            r_old_center_y= r_center_y;
            }
            //room center
            r_center_y = ry + r_size_y / 2;
            r_center_x = rx + r_size_x / 2;
            if (r_placed > 1)
            {
                int path_y;
                for (path_y = r_old_center_y; path_y != r_center_y ; )
                {
                    map[path_y][r_old_center_x]=' ';
                    if(r_old_center_y < r_center_y)
                    {
                        path_y++;
                    }
                    else if (r_old_center_y > r_center_y)
                    {
                        path_y--;
                    }
                }
                for (int xx = r_old_center_x; xx != r_center_x ; )
                {
                    map[path_y][xx]=' ';
                     if(r_old_center_x < r_center_x)
                    {
                        xx++;
                    }
                    else if (r_old_center_x > r_center_x)
                    {
                        xx--;
                    }
                }
            }
        }
    }

    // drawing map
    for (int yy = 0; yy <= rows; yy++)
    {
        for (int xx = 0; xx <= cols; xx++)
        {
            if (map[yy][xx] == ' ')
                mvaddch(yy, xx, ' ');

            else if (yy == 0 || yy == rows - 1)
            {
                mvaddch(yy, xx, '%');
            }
            else if (yy == rows)
            {
                mvaddch(yy, xx, ' ');
            }
            else if (xx == 0 || xx == cols)
            {
                mvaddch(yy, xx, '%');
            }

            // else if (map[yy][xx] == '|')
            //  mvaddch(yy, xx, '|');
            // else if (map[yy][xx] == '-')
            // mvaddch(yy, xx, '-');
            else
                mvaddch(yy, xx, '#');
        }
    }

    if (input == KEY_UP && map[y - 1][x] == ' ')
        y--;
    else if (input == KEY_DOWN && map[y + 1][x] == ' ')
        y++;
    else if (input == KEY_RIGHT && map[y][x + 1] == ' ')
        x++;
    else if (input == KEY_LEFT && map[y][x - 1] == ' ')
        x--;

    if (p_placed == 0)
    {
        do
        {
            y = rand() % rows;
            x = rand() % cols;
        } while (map[y][x] != ' ');

        p_placed = 1;
    }
    if (t_placed == 0)
    {
        do
        {
            ty = rand() % rows - 1;
            tx = rand() % cols - 1;
        } while (map[ty][tx] != ' ');
        t_placed = 1;
    }

    if (ty == y && tx == x)
    {
        t_placed = 0;
        // p_placed=0;
        // r_placed=0;
        p_gold += rand() % 10 + 1;
    }

    mvaddch(ty, tx, 't');
    mvaddch(y, x, '@');
    mvprintw(rows, 0, "Gold: %d", p_gold);
    return 1;
}

void new_game(Gamer *g)
{
    clear();
    int rows;
    int cols;
    getmaxyx(stdscr, rows, cols);
    char map[rows][cols];
    int input;
    do
    {
        draw_map(input, rows - 1, cols - 1, map);

    } while ((input = getch()) != 'q'); // 'q' means quit
    // draw_map(&dungeon, cols, lines);
    refresh();
    getch();
}

int main()
{
    Gamer g;
    initscr();
    clear();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE); //
    setlocale(LC_ALL, "");
    start_color();
    halfdelay(1);
    can_change_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
     menu(&g);
    clear();
     draw_border();
    const char *filename = "scores.txt";
     game_menu(&g, filename);

    char game_all_scores[100][1000];
    int c = '\0';
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    char map[rows][cols];
    do
    {
        dungeon(c, rows - 1, cols - 1, map);

    } while ((c = getch()) != 27); // 27 = 'ESC'

    refresh();
    getch();
    endwin();
    return 0;
}

