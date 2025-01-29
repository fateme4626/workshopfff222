
typedef struct {
    int x;
    int y;
} point;

typedef struct {
    int a;
    int b;
    int c;
} triangle;

int compare(const void *a, const void *b)
{
    point *p1 = (point *)a;
    point *p2 = (point *)b;
    if (p1->x == p2->x)
    {
        return p1->y - p2->y;
    }
    return p1->x - p2->x;
}

int ccw(point a, point b, point c)
{
    return (b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y);
}

void room_to_point(room *new_rooms, int rooms, point *points)
{
    for (int i = 0; i < rooms; i++)
    {
        points[i].x = new_rooms[i].first_x + new_rooms[i].width / 2;
        points[i].y = new_rooms[i].first_y + new_rooms[i].height / 2;
    }
}

void delaunay_triangulation(point *points, int n, triangle **triangles, int *tcount)
{
    qsort(points, n, sizeof(point), compare);
    *triangles = (triangle *)malloc(n * n * sizeof(triangle));
    *tcount = 0;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            int valid = 1;
            for (int k = 0; k < n; k++)
            {
                if (k != i && k != j && ccw(points[i], points[j], points[k]) == 0)
                {
                    valid = 0;
                    break;
                }
            }
            if (valid)
            {
                (*triangles)[*tcount].a = i;
                (*triangles)[*tcount].b = j;
                (*triangles)[*tcount].c = n;
                (*tcount)++;
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



void connect_rooms_delaunay(room *new_rooms, int rooms, char **map)
{
    point *points = (point *)malloc(rooms * sizeof(point));
    room_to_point(new_rooms, rooms, points);
    for (int i = 0; i < rooms; i++)
    {
        points[i].x = new_rooms[i].first_x + new_rooms[i].width / 2;
        points[i].y = new_rooms[i].first_y + new_rooms[i].height / 2;
    }

    triangle *triangles;
    int tcount;
    delaunay_triangulation(points, rooms, &triangles, &tcount);

    for (int i = 0; i < tcount; i++)
    {
        point p1 = points[triangles[i].a];
        point p2 = points[triangles[i].b];

        while (p1.x != p2.x)
        {
            map[p1.x][p1.y] = '.';
            p1.x += (p2.x > p1.x) ? 1 : -1;
        }
        while (p1.y != p2.y)
        {
            map[p1.x][p1.y] = '.';
            p1.y += (p2.y > p1.y) ? 1 : -1;
        }
    }

    free(points);
    free(triangles);
}

void draw_map()
{
    clear();
    srand(time(NULL));
    int COLS = getmaxx(stdscr);
    int LINES = getmaxy(stdscr);
    char** map = (char **)malloc(COLS * sizeof(char *));
    for (int i = 0; i < COLS; i++)
    {
        map[i] = (char *)malloc(LINES * sizeof(char));
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

 connect_rooms_delaunay(new_rooms, rooms, map); // Connect the rooms with Delaunay triangulation

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