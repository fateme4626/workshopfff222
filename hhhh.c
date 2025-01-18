#include<stdio.h>
#include<stdlib.h>

struct pair {
    int row;
    int col;
}; 

void printtt(int size, char** a) {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            printf("%c", a[i][j]); 
        }
        printf("\n");
    }
}

int read_input(char* addr, char*** map) {
    FILE *fptr = fopen(addr, "r"); 
    int size; 
    fscanf(fptr, "%d", &size);
    char temp[10];
    fgets(temp, 10, fptr); 
    
    *map = (char **)malloc(sizeof(char *) * size);
    for(int i = 0; i < size; i++) {
        (*map)[i] = (char *)malloc(sizeof(char)*size);
    }
    for(int i = 0; i < size; i++) {
        fgets((*map)[i], 100, fptr);
    }
    return size;
}

void initialize(int size,
                char** map, 
                char*** mark, 
                struct pair ***parent,
                struct pair *start,
                struct pair *finish) {
    *mark = (char **)malloc(sizeof(char *) * size);
    for(int i = 0; i < size; i++) {
        (*mark)[i] = (char *)malloc(sizeof(char)*size);
        for(int j = 0; j < size; j++) {
            (*mark)[i][j] = 'n'; 
        }
    }  
    *parent = (struct pair **)malloc(sizeof(struct pair *) * size); 
    for(int i = 0; i < size; i++) {
        (*parent)[i] = (struct pair *)malloc(sizeof(struct pair)*size); 
    }
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(map[i][j] == 'H') {
                start->row = i; 
                start->col = j;
            }
            if(map[i][j] == 'F') {
                finish->row = i; 
                finish->col = j;
            }
        }
    }
}


int dfs_visit(int size, 
               char **map,
               char **mark,
               int x, 
               int y, 
               struct pair** parent) {
    mark[x][y] = 'v';
    if(map[x][y] == 'F') {
        return 1;
    }
    int deltax[4] = {0, -1, 0, +1};
    int deltay[4] = {+1, 0, -1, 0};

    for(int i = 0; i < 4; i++) {
        int nx = x + deltax[i]; 
        int ny = y + deltay[i];
        if(nx >= 0 && nx < size) {
            if(ny >= 0 && ny < size) {
                if(mark[nx][ny] == 'n' && map[nx][ny]!= '#') {
                    parent[nx][ny].row = x; 
                    parent[nx][ny].col = y;
                    if(dfs_visit(size, map, mark, nx, ny, parent))
                        return 1;
                }
            }
        }
    }
    return 0;

}

void print_path(struct pair start, 
                struct pair current,
                struct pair** parent) {
    printf("%d %d \n", current.row, current.col);
    if(current.row != start.row || current.col != start.col) {
        print_path(start, parent[current.row][current.col], parent);
    }
}
int main() {
    char** map = (char **)malloc(sizeof(char**));
    char** mark = (char **)malloc(sizeof(char**));
    struct pair *start  = (struct pair *) malloc(sizeof(struct pair));
    struct pair *finish = (struct pair *) malloc(sizeof(struct pair));
    struct pair **parent = (struct pair **) malloc(sizeof(struct pair **));

    int size; 
    size = read_input("map.txt", &map);
    initialize(size, map, &mark, &parent, start, finish);
    printf("%d \n", dfs_visit(size, map, mark, start->row, start->col, parent)); 
    //printtt(size, mark);
    print_path(*start, *finish, parent);
    return 0;

}





