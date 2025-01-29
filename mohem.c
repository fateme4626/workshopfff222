
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
    char **map = (char **)malloc(COLS * sizeof(char *));
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

    room_to_point(new_rooms, rooms);
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

typedef struct
{
    pair a1;
    pair a2;
    pair a3;
    pair center_of_surround_circle;
} triangle;
typedef struct
{
    pair first;
    pair second;
    double tool;
} edge;

void calculate_center_of_circle(triangle *tri)
{
    // محاسبه‌ی مرکز دایره محیطی
    double ax = tri->a1.row;
    double ay = tri->a1.col;
    double bx = tri->a2.row;
    double by = tri->a2.col;
    double cx = tri->a3.row;
    double cy = tri->a3.col;

    double d = 2 * (ax*(by-cy) + bx*(cy-ay) + cx*(ay-by));
    double ux = ((ax*ax + ay*ay) * (by-cy) + (bx*bx + by*by) * (cy-ay) + (cx*cx + cy*cy) * (ay-by)) / d;
    double uy = ((ax*ax + ay*ay) * (cx-bx) + (bx*bx + by*by) * (ax-cx) + (cx*cx + cy*cy) * (bx-ax)) / d;

    tri->center_of_surround_circle.row = (double)ux;
    tri->center_of_surround_circle.col = (double)uy;
}



   int is_inside_of_circle(pair p, triangle tri, double radius)
{
    double dx = p.row - tri.center_of_surround_circle.row;
    double dy = p.col - tri.center_of_surround_circle.col;
    double distance = sqrt(dx * dx + dy * dy);
    return distance <= radius;
}



void draw_hallway(edge Edge)
{
    int x1 = (int)Edge.first.col;
    int y1 = (int)Edge.first.row;
    int x2 = (int)Edge.second.col;
    int y2 = (int)Edge.second.row;
    mvprintw(y1, x1, "*");
    while (x1 != x2 || y1 != y2)
    {
        if (x1 < x2)
            x1++;
        else if (x1 > x2)
            x1--;
        if (y1 < y2)
            y1++;
        else if (y1 > y2)
            y1--;
        mvprintw(y1, x1, "*");
    }
}
double calculate_distance(pair a, pair b)
{
    double d = sqrt(pow(a.row - b.row, 2) + pow(a.col - b.col, 2));
    return d;
}
void sort_edged(edge *Edge, int counter)
{
    for (int i = 0; i < counter; i++)
    {
        for (int j = 0; j < counter - i; j++)
        {
            if (Edge[i].tool > Edge[i + 1].tool)
            {
                edge temp = Edge[i];
                Edge[i] = Edge[i + 1];
                Edge[i + 1] = temp;
            }
        }
    }
}

typedef struct
{
    int parent;
    int rank;
} subset;

int find(subset *subsets, int i, int size) 
{
    if (i < 0 || i >= size) 
    {
        printf("Error: Invalid index %d\n", i);
        exit(1);
    }

    if (subsets[i].parent != i) 
    {
        subsets[i].parent = find(subsets, subsets[i].parent, size);
    }
    return subsets[i].parent;
}



void union_sets(subset *subsets, int x, int y, int size)
{
    int root_x = find(subsets, x, size); // افزودن پارامتر size برای بررسی دقیق‌تر
    int root_y = find(subsets, y, size); // افزودن پارامتر size برای بررسی دقیق‌تر

    if (root_x != root_y)
    {
        if (subsets[root_x].rank < subsets[root_y].rank)
        {
            subsets[root_x].parent = root_y;
        }
        else if (subsets[root_x].rank == subsets[root_y].rank)
        {
            subsets[root_y].parent = root_x;
            subsets[root_x].rank++;
        }
        else
        {
            subsets[root_y].parent = root_x;
        }
    }
}


void kruskal(edge *Edge, int num_edges, int num_points)
{
    edge *result = (edge *)malloc(num_points * sizeof(edge));
    if (!result) {
        printf("Error: Memory could not be allocated for result\n");
        exit(1);
    }

    int e = 0, i = 0;

    subset *subsets = (subset *)malloc(num_points * sizeof(subset));
    if (!subsets) {
        printf("Error: Memory could not be allocated for subsets\n");
        free(result);
        exit(1);
    }

    for (int v = 0; v < num_points; v++)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (e < num_points - 1 && i < num_edges)
    {
        edge next = Edge[i++];
        if (next.first.row < num_points && next.second.row < num_points) {
            int x = find(subsets, next.first.row, num_points);
            int y = find(subsets, next.second.row, num_points);

            if (x != y)
            {
                result[e++] = next;
                union_sets(subsets, x, y, num_points);
            }
        } else {
            printf("Error: Index out of bounds at row %d or row %d\n", next.first.row, next.second.row);
        }
    }

    for (i = 0; i < e; i++)
    {
        draw_hallway(result[i]);
    }

    free(result);
    free(subsets);
    printf("Completed kruskal with %d edges\n", e);
}



void room_to_point(room *new_rooms, int rooms)
{
    pair *entrances_exits = (pair *)malloc(rooms * 2 * sizeof(pair));
    printw(" tedad otagh  %d \n", rooms);
    for (int i = 0; i < rooms; i++)
    {
        new_rooms[i].entrance.row = new_rooms[i].first_y + new_rooms[i].height / 2;
        new_rooms[i].entrance.col = new_rooms[i].first_x + new_rooms[i].width;

        new_rooms[i].exit.row = new_rooms[i].first_y + new_rooms[i].height / 2;
        new_rooms[i].exit.col = new_rooms[i].first_x;

        entrances_exits[2 * i] = new_rooms[i].entrance;
        entrances_exits[2 * i + 1] = new_rooms[i].exit;
    }
    triangle *delaunay_triangle = (triangle *)malloc(300 * sizeof(triangle));

    triangle first = {{-10000, -10000}, {20000, -10000}, {20000, 20000}};
    delaunay_triangle[0] = first;
    int number_of_triangle = 1;
    calculate_center_of_circle(&delaunay_triangle[0]);

   for (int i = 0; i < rooms * 2; i++)
{
    int original_count = number_of_triangle; 

    for (int j = 0; j < original_count; j++)
    {
        double radius = sqrt(pow(delaunay_triangle[j].a1.row - delaunay_triangle[j].center_of_surround_circle.row, 2) +
                             pow(delaunay_triangle[j].a1.col - delaunay_triangle[j].center_of_surround_circle.col, 2));
        if (is_inside_of_circle(entrances_exits[i], delaunay_triangle[j], radius))
        {
            triangle first_new = {delaunay_triangle[j].a1, delaunay_triangle[j].a2, entrances_exits[i]};
            delaunay_triangle[j] = first_new;
            calculate_center_of_circle(&delaunay_triangle[j]);

            triangle second_new = {delaunay_triangle[j].a2, delaunay_triangle[j].a3, entrances_exits[i]};
            delaunay_triangle[number_of_triangle++] = second_new;
            calculate_center_of_circle(&delaunay_triangle[number_of_triangle - 1]);

            triangle third_new = {delaunay_triangle[j].a3, delaunay_triangle[j].a1, entrances_exits[i]};
            delaunay_triangle[number_of_triangle++] = third_new;
            calculate_center_of_circle(&delaunay_triangle[number_of_triangle - 1]);

        }
    }
    printf("Number of triangles after iteration %d: %d\n", i, number_of_triangle);
}
    edge *Edge = (edge *)malloc(900 * sizeof(edge));
    int ll = -1;
    for (int i = 0; i < number_of_triangle; i++)
    {
        Edge[++ll] = (edge){delaunay_triangle[i].a1, delaunay_triangle[i].a2, calculate_distance(delaunay_triangle[i].a1, delaunay_triangle[i].a2)};
        Edge[++ll] = (edge){delaunay_triangle[i].a2, delaunay_triangle[i].a3, calculate_distance(delaunay_triangle[i].a2, delaunay_triangle[i].a3)};
        Edge[++ll] = (edge){delaunay_triangle[i].a3, delaunay_triangle[i].a1, calculate_distance(delaunay_triangle[i].a1, delaunay_triangle[i].a3)};
    }
    sort_edged(Edge, ll + 1);
    kruskal(Edge, ll + 1, 2 * rooms);
   
}