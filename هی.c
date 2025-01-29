bool check_collision(grid *g, point start, point end) {
    int x, y;
    // جهات درست و مقادیر هر پیشروی گره را چک میکنیم
    if (start.x < end.x) x = start.x; else x = end.x;
    if (start.y < end.y) y = start.y; else y = end.y;
    for (; x <= end.x; x++) {
        if (g->nodes[x][start.y].f == 'q') return false;
    }
    // ادامه حرکت روی محور x
        for (; x != end.x; x += (start.x < end.x) ? 1 : -1)
        if (g->nodes[x][y].f != '0' && g->nodes[x][y].f != '.')
        return false;
    return true;
}


void connect_path_safe(grid *g, point p1, point p2, char **map) {
    if (check_collision(g, p1, p2)) {
        while (p1.x != p2.x) {
            map[p1.x][p1.y] = '.';
            p1.x += (p2.x > p1.x) ? 1 : -1;
        }
        while (p1.y != p2.y) {
            map[p1.x][p1.y] = '.';
            p1.y += (p2.y > p1.y) ? 1 : -1;
        }
    }
}


void connect_rooms_delaunay(room *new_rooms, int rooms, char **map, grid *g) {
    point *points = (point *)malloc(rooms * sizeof(point));
    room_to_point(new_rooms, rooms, points);

    triangle *triangles;
    int tcount;
    delaunay_triangulation(points, rooms, &triangles, &tcount);

    for (int i = 0; i < tcount; i++) {
        point p1 = points[triangles[i].a];
        point p2 = points[triangles[i].b];

        connect_path_safe(g, p1, p2, map);
    }

    for(int i = 0; i < g->width; i++) {
        for(int j = 0; j < g->height; j++) {
            if (g->nodes[i][j].f == 'q') {
                map[i][j] = '$'; 
            }
        }
    }
    
    free(points);
    free(triangles);
}
