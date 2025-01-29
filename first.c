#include <ncurses.h>
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
} player;

typedef struct
{
    int width;
    int y;

    int height;
    int x;
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



bool init = 0;
bool p_placed = 0;
int r_placed = 0;
bool t_placed = 0;
int p_gold = 0;
int initial_x, initial_y;
int ty, tx;

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
int check_possible_hallway(int temp_x, int temp_y, int x , int y,int rows,
 int cols, char map[rows][cols])
{
    int count=0;
        int path_y, path_x;
        for (path_y = temp_y; path_y != y;)
        {
            if (map[path_y][temp_x] == '#' )
            {
                return 1;
            }
            else if(map[path_y][temp_x] == '|' || map[path_y][temp_x] == '-'){
                count++;
            }

            if (temp_y < y)
            {
                path_y++;
            }
            else if (temp_y > y)
            {
                path_y--;
            }
        }
        for (int path_x = temp_x; path_x != x;)
        {
            if (map[path_y][path_x] == '#')
            {
                return 1;
            }
            else if(map[path_y][path_x] == '|' ||map[path_y][path_x] == '-'){
                count++;
            }
            if (temp_x < x)
            {
                path_x++;
            }
            else if (temp_x > x)
            {
                path_x--;
            }
        }
        if(count>2)
        return 1;
        return 0;
}
int check_collision_hallway_and_room(int y, int x,int rows, int cols, char map[rows][cols])
{
    if ( map[y-1][x] =='.' || map[y+1][x]=='.' || map[y][x+1]=='.' || map[y][x-1]=='.')
    return 1;
    
    return 0;
}

void display_map(int cols, int rows, char map[rows][cols])
{
    for (int y = 0; y <= rows; y++)
    {
        for (int x = 0; x <= cols; x++)
        {
          
            mvaddch(y, x, map[y][x]);
        }
    }
    return;
}


int draw_map(int rows, int cols, char map[rows][cols])
{

    srand(time(NULL));
    int first_y, first_x;
    int size_y;
    int size_x;
    int r_center_y;
    int r_center_x;
    int r_old_center_y;
    int r_old_center_x;
    int room_number = 6;
    int collision_of_hallway = 0;

    collision_of_hallway = 0;
    r_placed = 0;
    while (r_placed < room_number)
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

                size_y = rand() % 5 + 10;
                size_x = rand() % 5 + 12;

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
        r_placed++;

        if (r_placed > 0)
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
                if(map[path_y][r_old_center_x]==' '){
                map[path_y][r_old_center_x] = '#';}
                else if(map[path_y][r_old_center_x]=='-' || map[path_y][r_old_center_x]=='|')
                map[path_y][r_old_center_x] = '+';
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
                
                if(map[path_y][path_x] == ' ')
                map[path_y][path_x] = '#';
                else if(map[path_y][path_x] == '-' || map[path_y][path_x] == '|')
                map[path_y][path_x] = '+';
                if (r_old_center_x < r_center_x)
                {
                    path_x++;
                }
                else if (r_old_center_x > r_center_x)
                {
                    path_x--;
                }
            }
            
        }
    }

    display_map(cols, rows, map);

    mvprintw(rows, 0, "Gold: %d", p_gold);
    return 1;
}

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

    if (input == KEY_UP && (map[initial_y - 1][initial_x] == '.' || map[initial_y - 1][initial_x] == '#' ||
      map[initial_y - 1][initial_x] == '+' ))
        initial_y--;
    else if (input == KEY_DOWN && (map[initial_y + 1][initial_x] == '.' || map[initial_y + 1][initial_x] == '#' || 
    map[initial_y + 1][initial_x] == '+'))
        initial_y++;
    else if (input == KEY_RIGHT && (map[initial_y][initial_x + 1] == '.' || map[initial_y][initial_x + 1] == '#'||
    map[initial_y][initial_x + 1] == '+'))
        initial_x++;
    else if (input == KEY_LEFT && (map[initial_y][initial_x - 1] == '.' || map[initial_y][initial_x - 1] == '#'||
    map[initial_y][initial_x - 1] == '+'))
        initial_x--;

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
    int input = '\0';
    player user;
do{
    initilizing_map(rows - 1, cols - 1, map);
}while(!draw_map(rows - 1, cols - 1, map));

    do
    {
        display_map(cols - 1, rows - 1, map);
        move_char(input, &user, cols - 1, rows - 1, map);
    } while ((input = getch()) != 'q'); // 'q' means quit
    refresh();
    getch();
}
