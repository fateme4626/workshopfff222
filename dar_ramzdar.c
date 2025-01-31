#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <locale.h>
#include <math.h>
#define min_number_of_rooms 5
#define max_number_of_room 10
#define min_size_of_h_w 8

typedef struct
{
    int x;
    int y;
} pair;

typedef struct
{
    int score;
    int max_items;
    char name[50];
    char email[70];
    char password[50];
    char security_word[200];
    char difficulty[20];
    char hero_color[10];
} Gamer;

typedef struct
{
    char name[80];
    int score;
    int gold;
    pair position;
    int level;
    int health;
} player;

typedef struct
{
    int size_y;
    int y;

    int size_x;
    int x;

    int doors_num;
    pair doors[4];

    bool have_password_door;
    bool unlock;
    bool have_password;

} room;

void menu(Gamer *g);
void draw_border();
int sign_in(Gamer *g);
int validation_password(char *password);
int validation_email(char *eamil);
int login(Gamer *g);
void login_as_guest(Gamer *g);
char *random_pass();
void game_menu(Gamer *g, const char *filename);
void sort_scores(const char *filename, Gamer *g);
void show_scores(player *scores, int i, Gamer *g);
void new_game(Gamer *g);
int dfs_visit(char **map, char **mark, int x, int y, pair **parent, pair finish, int, int);
void initialize(char ***mark, pair ***parent);
void draw_path(pair start, pair current, pair **parent, char **map);
void setting(Gamer *g);
void difficulty(Gamer *g);
void hero_setting(Gamer *g);

int main()
{
    Gamer g;
    setlocale(LC_ALL, "");
    initscr();
    clear();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE); //

    start_color();
    halfdelay(1);
    can_change_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_RED);


    menu(&g);
    clear();
    draw_border();
    const char *filename = "scores.txt";
    game_menu(&g, filename);

    char game_all_scores[100][1000];

    refresh();
    getch();
    endwin();
    return 0;
}

void menu(Gamer *g)
{
    char *menu[] = {"sign in", "login", "login as guest"};
    int choice = 0;

    while (1)
    {
        clear();
        draw_border();
        for (int i = 0; i < 3; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(menu[i])) / 2, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 3 - 1 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 3 - 1) ? 0 : choice + 1;
        else if (ch == 10)
        {

            if (choice == 0)
            {
                if (sign_in(g))
                    break;
            }
            else if (choice == 1)
            {
                if (login(g))
                    break;
            }
            else if (choice == 2)
            {
                login_as_guest(g);
                break;
            }
        }
    }
    refresh();
    return;
}

void draw_border()
{
    start_color();
    for (int x = 0; x < COLS; x += 2)
    {
        mvprintw(0, x, "-");
        mvprintw(LINES - 1, x, "-");
    }
    for (int x = 1; x < COLS; x += 2)
    {
        mvprintw(0, x, "*");
        mvprintw(LINES - 1, x, "*");
    }
    for (int y = 0; y < LINES; y += 2)
    {
        mvprintw(y, 0, "^");
        mvprintw(y, COLS - 1, "^");
    }
    for (int y = 1; y < LINES; y += 2)
    {
        mvprintw(y, 0, "*");
        mvprintw(y, COLS - 1, "*");
    }
}

int sign_in(Gamer *g)
{
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2, (COLS - strlen("Enter your name :")) / 2, "Enter your name : ");
    getnstr(g->name, 50);

    char new_user[52];
    sprintf(new_user, "./%s", g->name);
    if (access(new_user, F_OK) == 0)
    {
        attron(COLOR_PAIR(1));
        mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! USERNAMME IS ALREADY TAKEN !!")) / 2, "!! USERNAMME IS ALREADY TAKEN !!");
        attroff(COLOR_PAIR(1));
        refresh();
        getch();
        return 0;
    }

    clear();
    draw_border();
    mvprintw((LINES - 2) / 2, (COLS - strlen("welcome !") - 30) / 2 + 4, "welcom %s !", g->name);
    mvprintw((LINES - 2) / 2 + 3, (COLS - strlen("Enter your email :")) / 2, "Enter your email : ");
    getnstr(g->email, 70);
    char email[100];
    strcpy(email, g->email);

    if (!validation_email(email))
    {
        attron(COLOR_PAIR(1));
        mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! INVALID EMAL ADDRESS !!")) / 2, "!! INVALID EMAIL ADDRESS !!");
        attroff(COLOR_PAIR(1));
        refresh();
        getch();
        return 0;
    }

    clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("Enter your password if you need random password write ::rand :")) / 2, "Enter your password if you need random password write ::rand : ");
    getnstr(g->password, 50);
    if (strcmp(g->password, "::rand") == 0)
    {
        strcpy(g->password, random_pass());
    }

    char password[100];
    strcpy(password, g->password);

    if (!validation_password(password))
    {
        attron(COLOR_PAIR(1));
        mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! INVALID PASSWORD !!")) / 2, "!! INVALID PASSWORD !!");
        attroff(COLOR_PAIR(1));
        refresh();
        return 0;
    }
    clear();
    draw_border();
    mvprintw(((LINES - 2) / 2), (COLS - 2 - strlen("please write your highschool name you can use it instead of password ")) / 2, "please write your highschool name you can use it instead of password ");
    getnstr(g->security_word, 100);

    mkdir(new_user, 0777); // new folder with name of user

    char user_info[200];
    sprintf(user_info, "./%s/%s.info.txt", g->name, g->name);
    FILE *info = fopen(user_info, "w");
    fprintf(info, "name : %s\n", g->name);
    fprintf(info, "email : %s\n", g->email);
    fprintf(info, "password : %s\n", g->password);
    fprintf(info, "security_word : %s\n", g->security_word);
    fclose(info);
    refresh();
    return 1;
}

int validation_email(char *email)
{
    int length = strlen(email);
    int sign = 0;
    int sign_num;
    int signdot = 0;
    for (int i = 0; i < strlen(email); i++)
    {
        if (email[i] == '@' && i > 0)
        {
            sign = 1;
            sign_num = i;
        }
        if (email[i] == '.' && i > sign_num + 1 && sign_num > 0 && i < length - 1)
        {
            signdot = 1;
        }
    }
    if (sign && signdot)
    {
        return 1;
    }

    return 0;
}

int validation_password(char *password)
{
    int length = strlen(password);
    if (length < 7)
    {
        return 0;
    }
    int digit = 0;
    int upercase = 0;
    int small = 0;
    for (int i = 0; i < length; i++)
    {
        char c = password[i];

        if (c >= '1' && c <= '9')
        {
            digit = 1;
        }
        if (c >= 'a' && c <= 'z')
        {
            small = 1;
        }
        if (c >= 'A' && c <= 'Z')
        {
            upercase = 1;
        }
    }
    if (digit && upercase && small)
    {
        return 1;
    }
    return 0;
}

int login(Gamer *g)
{
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2, (COLS - strlen("Enter your name :")) / 2, "Enter your name : ");
    getnstr(g->name, 50);

    char new_user[52];
    sprintf(new_user, "./%s", g->name);
    if (access(new_user, F_OK) != 0)
    {
        attron(COLOR_PAIR(1));
        mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! username not found !!")) / 2, "!! username not found !!");
        attroff(COLOR_PAIR(1));
        refresh();
        getch();
        return 0;
    }

    clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("Enter your password  if you forgot it write ::forgot:")) / 2, "Enter your password  if you forgot it write ::forgot: ");
    getnstr(g->password, 50);

    char password[100];
    strcpy(password, g->password);
    char user_info[200];
    sprintf(user_info, "./%s/%s.info.txt", g->name, g->name);

    FILE *file = fopen(user_info, "r");

    if (strcmp(g->password, "::forgot") == 0)
    {
        clear();
        draw_border();
        mvprintw((LINES - 2) / 2, (COLS - strlen("write your highschool name : ")) / 2, "write your highschool name : ");
        char school[100], school_name[100];
        getnstr(school_name, 100);
        char temp[100];
        while (fgets(temp, 100, file))
        {
            if (strstr(temp, "security_word : ") != NULL)
            {
                sscanf(temp, "security_word : %s", school);
                break;
            }
        }
        fclose(file);
        if (strcmp(school, school_name) == 0)
        {
            return 1;
        }
        else
        {
            attron(COLOR_PAIR(1));
            mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! it's wrong !!")) / 2, "!! it's wrong !!");
            attroff(COLOR_PAIR(1));
            return 0;
        }
    }

    else
    {
        char temp[200];
        char user_password[100];
        while (fgets(temp, 200, file))
        {
            if (strstr(temp, "password : ") != NULL)
            {
                sscanf(temp, "password : %s", user_password);
                break;
            }
        }
        fclose(file);

        if (strcmp(password, user_password) == 0)
        {
            return 1;
        }
        else
        {
            attron(COLOR_PAIR(1));
            mvprintw((LINES - 2) / 2 + 4, (COLS - 2 - strlen("!! password is incorrect !!")) / 2, "!! password is incorrect !!");
            attroff(COLOR_PAIR(1));
            refresh();
            getch();
            return 0;
        }
    }
}
void login_as_guest(Gamer *g)
{
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("Enter your name :")) / 2, "Enter your name : ");
    getnstr(g->name, 50);
    clear();
    draw_border();
    mvprintw((LINES - 2) / 2 + 1, (COLS - strlen("press a key to start")) / 2, "press a key to start");
    getch();
}
char *random_pass()
{
    static char pass[12];
    srand(time(NULL)); // yeksan nabashand
    char number[2];
    char uper[5];
    char small[5];
    for (int i = 0; i < 2; i++)
    {
        int n = rand();
        number[i] = (n % 10) + '0'; // random digit}
    }
    for (int i = 0; i < 5; i++)
    {
        int up = rand();
        uper[i] = (up % 26) + 'A';
    }
    for (int i = 0; i < 4; i++)
    {
        int sma = rand();
        small[i] = (sma % 26) + 'a';
    }
    pass[0] = number[1];
    pass[1] = uper[0];
    pass[2] = uper[1];
    pass[3] = small[0];
    pass[4] = number[0];
    pass[5] = uper[2];
    pass[6] = uper[3];
    pass[7] = small[1];
    pass[8] = small[2];
    pass[9] = small[3];
    pass[10] = uper[4];
    pass[11] = '\0';
    return pass;
}

void game_menu(Gamer *g, const char *filename)
{
    char *choices[] = {
        "New Game",
        "Continue Previous Game",
        "Score Table",
        "Setting"};
    int choice = 0;
    int num_choices = 4;

    while (1)
    {
        clear();
        draw_border();
        for (int i = 0; i < num_choices; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw((LINES - num_choices) / 2 + i, (COLS - strlen(choices[i])) / 2, "%s", choices[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? num_choices - 1 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == num_choices - 1) ? 0 : choice + 1;
        else if (ch == 10)
        {
            if (choice == 0)
            {
                new_game(g);
                break;
            }
            else if (choice == 1)
            {
                // continue_game(g);
                break;
            }
            else if (choice == 2)
            {
                sort_scores(filename, g);
            }
            else if (choice == 3)
            {
                setting(g);
            }
        }
    }

    refresh();
    return;
}

void continue_game()
{
}

void score_table()
{
}

void setting(Gamer *g)
{
    char *string[] = {"Difficulty", "Hero setting", "Back"};
    int choice = 0;
    int ch;

    while (1)
    {
        clear();
        draw_border();
        for (int i = 0; i < 3; i++)
        {
            if (i == choice)
            {
                attron(A_REVERSE);
            }
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(string[i])) / 2, "%s", string[i]);
            if (i == choice)
            {
                attroff(A_REVERSE);
            }
        }
        refresh();

        ch = getch();
        if (ch == KEY_UP)
        {
            choice = (choice == 0) ? 2 : choice - 1;
        }
        else if (ch == KEY_DOWN)
        {
            choice = (choice == 2) ? 0 : choice + 1;
        }
        else if (ch == 10)
        { // کد ASCII برای Enter
            if (choice == 0)
            {
                difficulty(g);
            }
            else if (choice == 1)
            {
                hero_setting(g);
            }
            else if (choice == 2)
            {
                return;
            }
        }
    }
}

void difficulty(Gamer *g)
{
    char *string[] = {"EASY", "MEDIUM", "HARD"};
    int choice = 0;
    int ch;

    while (1)
    {
        clear();
        draw_border();
        for (int i = 0; i < 3; i++)
        {
            if (i == choice)
            {
                attron(A_REVERSE);
            }
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(string[i])) / 2, "%s", string[i]);
            if (i == choice)
            {
                attroff(A_REVERSE);
            }
        }
        refresh();

        ch = getch();
        if (ch == KEY_UP)
        {
            choice = (choice == 0) ? 2 : choice - 1;
        }
        else if (ch == KEY_DOWN)
        {
            choice = (choice == 2) ? 0 : choice + 1;
        }
        else if (ch == 10)
        { // کد ASCII برای Enter
            if (choice == 0)
            {
                strcpy(g->difficulty, "EASY");
            }
            else if (choice == 1)
            {
                strcpy(g->difficulty, "MEDIUM");
            }
            else if (choice == 2)
            {
                strcpy(g->difficulty, "HARD");
            }
            break;
        }
    }
}

void hero_setting(Gamer *g)
{

    char *hero_color[] = {"RED", "BLUE", "GREEN"};
    int choice = 0;
    int ch;

    while (1)
    {
        clear();
        draw_border();
        for (int i = 0; i < 3; i++)
        {
            if (i == choice)
            {
                attron(A_REVERSE);
            }
            mvprintw((LINES - 3) / 2 + i, (COLS - strlen(hero_color[i])) / 2, "%s", hero_color[i]);
            if (i == choice)
            {
                attroff(A_REVERSE);
            }
        }
        refresh();

        ch = getch();
        if (ch == KEY_UP)
        {
            choice = (choice == 0) ? 2 : choice - 1;
        }
        else if (ch == KEY_DOWN)
        {
            choice = (choice == 2) ? 0 : choice + 1;
        }
        else if (ch == 10)
        { // کد ASCII برای Enter
            if (choice == 0)
            {
                strcpy(g->hero_color, "RED");
            }
            else if (choice == 1)
            {
                strcpy(g->hero_color, "BLUE");
            }
            else if (choice == 2)
            {
                strcpy(g->hero_color, "GREEN");
            }
            break;
        }
    }
}

/* void scores_save(g)
{
FILE * file= fopen (scores, "a");
if(file==NULL){
    file = fopen(scores, "w");
}
fprintf(file, "%s : %d\n",g->name, g->score);
fclose(file);
} */

void sort_scores(const char *filename, Gamer *g)
{
    player scores[100];
    int i = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file!\n");
        return;
    }

    while (fscanf(file, "%[^:]:%d:%d", scores[i].name, &scores[i].score, &scores[i].gold) != EOF)
    {
        i++;
    }
    fclose(file);

    for (int j = 0; j < i - 1; j++)
    {
        for (int h = j + 1; h < i; h++)
        {
            if (scores[h].score > scores[j].score)
            {
                player temp = scores[j];
                scores[j] = scores[h];
                scores[h] = temp;
            }
        }
    }

    show_scores(scores, i, g);
}

void show_scores(player *scores, int count, Gamer *g)
{
    clear();
    draw_border();

    if (count == 0)
    {
        mvprintw((LINES - 2) / 2, (COLS - 2) / 2, "No score to display");
    }
    else
    {
        mvprintw(3, (COLS - 2) / 2, "💰");
        //  for (int y = 0; y < count; y++) {
        int line = (LINES - count) / 2 + 1;
        //  if (y == 0) {
        mvprintw(line, (COLS - strlen("Legend")) / 2 - 2, "\xF0\x9F\x8F\x86 Legend");
        // }
        /*
         mvprintw(line, (COLS - strlen(scores[y].name)) / 2, "%s", scores[y].name);
         mvprintw(line, (COLS - 2) / 2, "%d", scores[y].gold);
         mvprintw(line, (COLS - 2) * 4 / 5, "%d", scores[y].score); */
        // }
    }
    refresh();
    getch();
}

bool player_initial_pos = 0;
int num_rooms = 0;
int initial_x, initial_y;

void initilizing_map(int rows, int cols, char map[rows][cols])
{

    for (int y = 2; y <= rows; y++)
    {
        for (int x = 2; x <= cols; x++)
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
            if (map[y1][x1] != ' ' ||
                map[y1 + 3][x1] != ' ' || map[y1][x1 + 3] != ' ' ||
                map[y1 - 3][x1] != ' ' || map[y1][x1 - 3] != ' ')
            {
                return 1;
            }
        }
    }
    return 0;
}

int check_possible_hallway(int temp_x, int temp_y, int x, int y, int rows,
                           int cols, char map[rows][cols])
{
    int count = 0;
    int y_in_way, x_in_way;
    for (y_in_way = temp_y; y_in_way != y;)
    {
        if (map[y_in_way][temp_x] == '#')
        {
            return 1;
        }
        else if (map[y_in_way][temp_x] == '|' || map[y_in_way][temp_x] == '-')
        {
            count++;
            if (count > 2)
                return 1;
        }

        if (temp_y < y)
        {
            y_in_way++;
        }
        else if (temp_y > y)
        {
            y_in_way--;
        }
    }
    for (x_in_way = temp_x; x_in_way != x;)
    {
        if (map[y_in_way][x_in_way] == '#')
        {
            return 1;
        }
        else if (map[y_in_way][x_in_way] == '|' || map[y_in_way][x_in_way] == '-')
        {
            count++;
            if (count > 2)
                return 1;
        }
        if (temp_x < x)
        {
            x_in_way++;
        }
        else if (temp_x > x)
        {
            x_in_way--;
        }
    }
    if (count > 2)
        return 1;
    return 0;
}

typedef struct
{
    char c;
    bool visited;

} cell;

int check_collision_hallway_and_room(int y, int x, int rows, int cols, char map[rows][cols])
{
    if (map[y - 1][x] == '.' || map[y + 1][x] == '.' || map[y][x + 1] == '.' || map[y][x - 1] == '.')
        return 1;

    return 0;
}

int check_position_of_object(int y, int x, int rows, int cols, char map[rows][cols], room map_rooms[6])
{
    for (int i = 0; i < 6; i++)
    {
        for (int yl = map_rooms[i].y; yl <= map_rooms[i].y + map_rooms[i].size_y; yl++)
        {
            for (int xl = map_rooms[i].x; xl <= map_rooms[i].x + map_rooms[i].size_x; xl++)
            {
                if (y == yl && x == xl)
                {
                    return i;
                }
            }
        }
    }

    // if it isn't in any room
    return 11;
}

int room_number_of_password_door;

void generate_password_door(room map_rooms[6], player user, int rows, int cols, char map[rows][cols])
{
    int current_room = check_position_of_object(user.position.y, user.position.x, rows, cols, map,
                                                map_rooms);
    int password_x;
    int password_y;

    for (int i = 0; i < 6; i++)
    {
        if (map_rooms[i].doors_num == 1 )
        {
            map[map_rooms[i].doors[map_rooms[i].doors_num - 1].y][map_rooms[i].doors[map_rooms[i].doors_num - 1].x] = '@';
            room_number_of_password_door = i;
            map_rooms[i].have_password_door = 1;
            map_rooms[i].unlock = 0;

            if (user.position.x == map_rooms[current_room].x + 1 && user.position.y == map_rooms[current_room].y + 1)
            {
                map[map_rooms[current_room].y + map_rooms[current_room].size_y - 1][map_rooms[current_room].x + map_rooms[current_room].size_x - 1] = '&';
            }
            else
            {
                map[map_rooms[current_room].y + 1][map_rooms[current_room].x + 1] = '&';
            }

            map_rooms[current_room].have_password = 1;
            return;
        }
    }
}

void display_map(player *user, int cols, int rows, char map[rows][cols], room map_rooms[6], int n)
{
    if (!n){
        clear();
    }

    for (int y = 2; y <= rows; y++)
    {
        for (int x = 2; x <= cols; x++)
        {
            if (map[y][x] == '.' || map[y][x] == '|' || map[y][x] == '-' || map[y][x] == '+')
            {
                attron(COLOR_PAIR(4) | A_BOLD);
                mvaddch(y, x, map[y][x]);
                attroff(COLOR_PAIR(4));
            }
            else if (map[y][x] == '#')
            {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, map[y][x]);
                attroff(COLOR_PAIR(2));
            }
            else if (map[y][x] == '<')
            {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, map[y][x]);
                attroff(COLOR_PAIR(2));
            }
            else if (map[y][x] == '&')
            {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, map[y][x]);
                attroff(COLOR_PAIR(2));
            }
            else if (map[y][x] == '@')
            {
                int i = check_position_of_object(y, x, rows, cols, map, map_rooms);
                if (map_rooms[i].unlock == 1)
                {
                    attron(COLOR_PAIR(3));
                    mvaddch(y, x, map[y][x]);
                    attroff(COLOR_PAIR(3));
                }
                else
                {
                    attron(COLOR_PAIR(1));
                    mvaddch(y, x, map[y][x]);
                    attroff(COLOR_PAIR(1));
                }
            }
            else
                mvaddch(y, x, map[y][x]);
        }
    }
    attron(COLOR_PAIR(3));
    mvprintw(LINES - 1, 2, "level: %d", user->level);
    mvprintw(LINES - 1, 15, "Gold: %d %lc", user->gold, L'💰');
    mvprintw(LINES - 1, 30, "score : %d %lc", user->score, L'⭐');
    mvprintw(LINES - 1, 45, "Health :");
    for (int i = 1; i <= user->health; i++)
    {
        mvprintw(LINES - 1, i + 45 + strlen("Health :"), "%lc", L'💚');
    }
    attroff(COLOR_PAIR(3) | A_BOLD);

    return;
}
int password;
bool the_password_is_shown;
int attemps;

int generate_random_password()
{
    srand(time(NULL));
    return rand() % 1000 + 1000;
}

void control_the_password_door(int input_password, room map_rooms[6],int rows, int cols,
 char map[rows][cols])
{
    if (input_password == password)
    {
        map_rooms[room_number_of_password_door].unlock = 1;
        the_password_is_shown = 0;
        attemps = 0;
        password = 0;
        for(int i=2; i<=rows; i++)
        {
            for(int j=0; j<=cols; j++)
            {
                if(map[i][j]=='&')
                map[i][j]=='.';
            }
        }
        return;
    }
    else
    {
        attemps++;
        if (attemps == 1)
        {
            attron(COLOR_PAIR(5));
            mvprintw(2, COLS / 2 - 10, "The password is incorrect");
            attroff(COLOR_PAIR(5));
            getch();
        }
    }
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
        size_y = (rand() % 3 + 6);
        size_x = (rand() % 3 + 6);
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

                size_y = rand() % 3 + 6;
                size_x = rand() % 3 + 6;

            } while (first_y < 4 || first_x + size_x > cols - 4 || first_y + size_y > rows - 4 || first_x < 4);

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
            for (x_in_way = previous_center_x; x_in_way != current_center_x;)
            {

                if (map[y_in_way][x_in_way] == ' ')
                    map[y_in_way][x_in_way] = '#';
                else if (map[y_in_way][x_in_way] == '-' || map[y_in_way][x_in_way] == '|')
                {
                    map[y_in_way][x_in_way] = '+';
                    int i = check_position_of_object(y_in_way, x_in_way, rows, cols, map,
                                                     map_rooms);
                    map_rooms[i].doors[map_rooms[i].doors_num].x = x_in_way;
                    map_rooms[i].doors[map_rooms[i].doors_num++].y = y_in_way;
                }
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

    display_map(user, cols, rows, map, map_rooms, 0);

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

                size_y = rand() % 3 + 6;
                size_x = rand() % 3 + 6;

            } while (first_y < 5 || first_x + size_x > cols - 5 || first_y + size_y > rows - 5 || first_x < 5);

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
                {
                    map[y_in_way][x_in_way] = '+';
                    int i = check_position_of_object(y_in_way, x_in_way, rows, cols, map,
                                                     map_rooms);
                    map_rooms[i].doors[map_rooms[i].doors_num].x = x_in_way;
                    map_rooms[i].doors[map_rooms[i].doors_num++].y = y_in_way;
                }
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

    display_map(user, cols, rows, map, map_rooms, 0);

    int yy, xx;
    do
    {
        yy = rand() % rows - 3;
        xx = rand() % cols - 3;

    } while (map[yy][xx] != '.');
    map[yy][xx] = '<';

    do
    {
        initial_y = rand() % rows;
        user->position.y = initial_y;
        initial_x = rand() % cols;
        user->position.x = initial_x;
    } while (map[initial_y][initial_x] != '.');

    return 1;
}

int move_char(int input, player *user, int cols, int rows, char map[rows][cols], room map_rooms[6], int n)
{

    display_map(user, cols, rows, map, map_rooms, n);

    /* if (player_initial_pos == 0)
     {
         do
         {
             initial_y = rand() % rows - 3;
             user->position.y = initial_y - 3;
             initial_x = rand() % cols;
             user->position.x = initial_x;
         } while (map[initial_y][initial_x] != '.');
         player_initial_pos = 1;
     } */

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
        return 2;

    case '&':
        // return 5;
        if (the_password_is_shown == 0 && map_rooms[room_number_of_password_door].unlock==0)
        {
            password = generate_random_password();
            mvprintw(0, 2, "password : %d ", password);
            the_password_is_shown = 1;
            time_t start_time = time(NULL);
            while (time(NULL) - start_time < 30)
            {
                timeout(100);
                int c = getch();
                int y = move_char(c, user, cols, rows, map, map_rooms, 1);
                if(y == 'q'){
                    return 0;
                }
            }
            mvprintw(0, 2, "                ");
            the_password_is_shown = 0;
        }
        user->position.x = initial_x;
        user->position.y = initial_y;
        break;

    case '@':
        if (map_rooms[room_number_of_password_door].unlock == 1)
        {
            initial_x = user->position.x;
            initial_y = user->position.y;
            mvaddch(user->position.y, user->position.x, 'H');
        }
        else
        {
            mvprintw( 1 , COLS/2-20, "Enter the password : ");
            int input_pass; char str[4];
            getnstr(str, 3);
            input_pass= atoi(str);
            control_the_password_door(input_pass, map_rooms,rows, cols, map );
        }

        
    }
}

void control_play_in_a_floor(int rows, int cols, int floor,
                             char all_floor[4][rows][cols],
                             room all_floor_rooms[4][6], player *user)

{
    the_password_is_shown = 0;
    if (floor > 4)
    {
        return;
    }
    char map[rows][cols];
    room map_rooms[6];

    for (int i = 0; i < 6; i++)
    {
        map_rooms[i].doors_num = 0;
        map_rooms[i].have_password = 0;
        map_rooms[i].have_password_door = 0;
        map_rooms[i].unlock = 0;
    }

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

    rows--;
    cols--;
    int k;
    int password;
    generate_password_door(map_rooms, *user, rows, cols, map);
    do
    {
        display_map(user, cols, rows, map, map_rooms, 0);
        k = move_char(input, user, cols, rows, map, map_rooms, 0);

        if (k == 2 || k==0)
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
        user->level++;
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
    user.gold = 0;
    user.score = 0;
    user.level = 1;
    user.health = 10;
    room all_floor_rooms[4][6];
    player_initial_pos = 0;
    control_play_in_a_floor(rows - 6, cols - 6, 0,
                            all_floor, all_floor_rooms, &user);

    refresh();
    getch();
}

/// rahro hhaye chasbide be divar
// levek ta 5 mire