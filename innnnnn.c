
typedef struct node
{
    int x;
    int y;
    int32_t g;               // cost from start to this node
    int32_t h;               // heuristic cost to goal
    int32_t f;               // total cost (g + h)
    struct node *parent; // parent node in path
} node;

#define MAX_SIZE 300

typedef struct
{
    node nodes[400][400];
    int width;
    int height;
} grid;

typedef struct
{
    int a;
    int b;
    int c;
} triangle;

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

void room_to_point(room *new_rooms, int rooms, point *points)
{
    for (int i = 0; i < rooms; i++)
    {
        points[i].x = new_rooms[i].first_x + new_rooms[i].width / 2;
        points[i].y = new_rooms[i].first_y + new_rooms[i].height / 2;
    }
}

int heuristic(node *a, node *b)
{
    int k;
    if (a->x > b->x)
    {
        if (a->y > b->y)
        {
            k = a->x - b->x + a->y - b->y;
        }
        else
        {
            k = a->x - b->x + b->y - a->y;
        }
    }
    else
    {
        if (a->y > b->y)
        {
            k = b->x - a->x + a->y - b->y;
        }
        else
        {
            k = b->x - a->x + b->y - a->y;
        }
    }
    return k;
}

node *get_node_with_lowest_f(node **open_list, int open_list_size)
{
    int lowest_index = 0;
    for (int i = 1; i < open_list_size; i++)
    {
        if (open_list[i]->f < open_list[lowest_index]->f)
        {
            lowest_index = i;
        }
    }
    return open_list[lowest_index];
}

void remove_node(node **open_list, int *open_list_size, node *node_to_remove)
{
    for (int i = 0; i < *open_list_size; i++)
    {
        if (open_list[i] == node_to_remove)
        {
            for (int j = i; j < *open_list_size - 1; j++)
            {
                open_list[j] = open_list[j + 1];
            }
            (*open_list_size)--;
            return;
        }
    }
}

int is_in_list(node **list, int list_size, node *node_to_check)
{
    for (int i = 0; i < list_size; i++)
    {
        if (list[i] == node_to_check)
        {
            return 1;
        }
    }
    return 0;
}

void mark_path(grid *g, node *end) {
    node *current = end;
    int counter = 0;
    
    while (current != NULL && counter < MAX_SIZE * MAX_SIZE) {
        g->nodes[current->x][current->y].f = 'q'; // علامت‌گذاری مسیر
        current = current->parent;
        counter++;
    }
    
    if (counter == MAX_SIZE * MAX_SIZE) {
        printf("Infinite loop detected while marking the path.\n");
    }
}


void a_star_pathfinding(grid *g, node *start, node *end)
{
    node *open_list[MAX_SIZE * MAX_SIZE];
    int open_list_size = 0;
    node *closed_list[MAX_SIZE * MAX_SIZE];
    int closed_list_size = 0;

    start->g = 0;
    start->h = heuristic(start, end);
    start->f = start->g + start->h;

    open_list[open_list_size++] = start;

    while (open_list_size > 0)
    {
        node *current = get_node_with_lowest_f(open_list, open_list_size);
        if (current == end)
        {
            break;
        }

        remove_node(open_list, &open_list_size, current);
        closed_list[closed_list_size++] = current;

        int neighbor_count = 0;
        if (current->x > 0)
        {
            neighbor_count++;
        }
        if (current->x < g->width - 1)
        {
            neighbor_count++;
        }
        if (current->y > 0)
        {
            neighbor_count++;
        }
        if (current->y < g->height-1)
        {
            neighbor_count++;
        }

        node **neighbors = (node **)malloc(neighbor_count * sizeof(node *));
        neighbor_count = 0;

        if (current->x > 0)
        {
            neighbors[neighbor_count++] = &g->nodes[current->x - 1][current->y];
        }
        if (current->x < g->width - 1)
        {
            neighbors[neighbor_count++] = &g->nodes[current->x + 1][current->y];
        }
        if (current->y > 0)
        {
            neighbors[neighbor_count++] = &g->nodes[current->x][current->y - 1];
        }
        if (current->y < g->height - 1)
        {
            neighbors[neighbor_count++] = &g->nodes[current->x][current->y + 1];
        }

        for (int i = 0; i < neighbor_count; i++)
        {
            node *neighbor = neighbors[i];
            if (is_in_list(closed_list, closed_list_size, neighbor))
            {
                continue;
            }
            int tentative_g = current->g + 1;
            if (tentative_g < neighbor->g || !is_in_list(open_list, open_list_size, neighbor))
            {
                neighbor->g = tentative_g;
                neighbor->h = heuristic(neighbor, end);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;
                if(neighbor->parent==neighbor)
                {
                    printf("Warning: Neighbor %d,%d is set as its own parent, skipping\n",neighbor-> x, neighbor-> y); 
                    continue;
                }
                if(!is_in_list(open_list, open_list_size, neighbor))
                {
                    open_list[open_list_size++] = neighbor;
                }
            }
        }
       free(neighbors);
    }
    mark_path(g, end);
}


int check_for_rooms(grid *g, point p1, point p2) {
    // تابع بررسی عدم برخورد راهرو به اتاق‌ها
    int x_direction = (p2.x - p1.x) != 0 ? (p2.x - p1.x) / abs(p2.x - p1.x) : 0;
    int y_direction = (p2.y - p1.y) != 0 ? (p2.y - p1.y) / abs(p2.y - p1.y) : 0;

    int x = p1.x;
    int y = p1.y;

    while (x != p2.x || y != p2.y) {
        if (g->nodes[x][y].f != 'q' && g->nodes[x][y].f != '0') {
            return 0;
        }
        if (x != p2.x) {
            x += x_direction;
        }
        if (y != p2.y) {
            y += y_direction;
        }
    }
    return 1;
}

node *initialize_path(grid *g, point p1, point p2) {
    if (check_for_rooms(g, p1, p2)) {
        a_star_pathfinding(g, &g->nodes[p1.x][p1.y], &g->nodes[p2.x][p2.y]);
    }
}

void connect_all_rooms(room *new_rooms, int rooms, char **map, grid *g) {
    point *points = (point *)malloc(rooms * sizeof(point));
    room_to_point(new_rooms, rooms, points);

    // اتصال راهروهای اصلی، بدون تداخل در اتاق ها
    for (int i = 0; i < rooms; i++) {
        for (int j = i + 1; j < rooms; j++) {
             point start = {
                .x= new_rooms[i].first_x + new_rooms[i].width / 2,
                .y= new_rooms[i].first_y + new_rooms[i].height / 2};
             point end = {
                .x= new_rooms[j].first_x + new_rooms[j].width / 2,
                .y= new_rooms[j].first_y + new_rooms[j].height / 2};

             initialize_path(g, start, end);
        }
    }
    // علامت گذاری مسیرها
    for(int i = 0; i < g->width; i++) {
        for(int j = 0; j < g->height; j++) {
            if (g->nodes[i][j].f == 'q') {
                map[i][j] = '$'; 
            }
        }
    }

    free(points);
}









void draw_map()
{
    clear();
    srand(time(NULL));
    int COLS = getmaxx(stdscr);
    int LINES = getmaxy(stdscr);
    char **map = (char **)malloc(COLS * sizeof(char *));
    grid g;
    g.width = COLS;
    g.height = LINES;
    for (int i = 0; i < COLS; i++)
    {
        map[i] = (char *)malloc(LINES * sizeof(char));

        for (int j = 0; j < LINES - 1; j++)
        {
            g.nodes[i][j].x = i;
            g.nodes[i][j].y = j;
            g.nodes[i][j].g = INT32_MAX;
            g.nodes[i][j].h = 0;
            g.nodes[i][j].f = INT32_MAX;
            g.nodes[i][j].parent = NULL;
        }
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

        new_room.entry.x = new_room.first_x + new_room.width / 2;
        new_room.entry.y = new_room.first_y;
        new_room.exit.x = new_room.first_x + new_room.width / 2;
        new_room.exit.y = new_room.first_y + new_room.height - 1;

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

        map[new_room.entry.x][new_room.entry.y] = '+';
        map[new_room.exit.x][new_room.exit.y] = '+';
    }

    connect_all_rooms(new_rooms, rooms, map, &g); // Connect the rooms with Delaunay triangulation

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
