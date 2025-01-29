
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
 room_to_pair(new_rooms, rooms);

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