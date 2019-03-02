#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

#define Height 10
#define Width 10

#define array_size 1000

#define cost 10

#define coorx 0
#define coory 1
#define fn    2
#define parentx 3
#define parenty 4
#define valid_bit 5

#define length 1000

void Display_map(int map[Height][Width]);
void Create_map(int map[Height][Width], int cost_map[Height][Width], char show_map[Height][Width]);
int A_star_path_finding(int map[Height][Width], int cost_map[Height][Width], int start_point[2], int end_point[2], int Path[length][2]);
int Get_heuristic_function(int current_point[2], int Goal[2]);
int check_end_point_in_openset(int end_point[2], int OpenSet[array_size][6]);
int Get_minimun_fn_coord(int Openset[array_size][6]);
int Get_gn(int current_point[2], int start_point[2]);
void Expend_current_point(int cost_map[Height][Height], int current_point[2], int Openset[array_size][6], int ClosedSet[array_size][6], int end_point[2], int start_point[2], int closed_list_counter);
bool Is_openset_empty(int Openset[array_size][6]);
void Init(int Openset[array_size][6], int ClosedSet[array_size][6]);
bool Is_in_open_or_closed_set(int point[2], int Openset[array_size][6], int Closedset[array_size][6], int closed_list_counter);
int Find_index(int set[array_size][6], int point[2]);
int Find_parent_index(int parent[2], int ClosedSet[array_size][6], int CloseSet_index);
int Get_path(int ClosedSet[array_size][6], int CloseSet_index, int Path[length][2], int start_point[2]);
void Draw_map_with_path(char  show_map[Height][Width], int Path[length][2], int Path_length);
// create a map using 2-D array
void Create_map(int map[Height][Width], int cost_map[Height][Width], char show_map[Height][Width])
{
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if ((i == 2 && j >= 2 && j <= 7) || ((i == 7 && j >= 2 && j <= 7)) ||
				(i >= 3 && i <= 6 && j == 7)|| (i==1&&j==3))
			{
				cost_map[i][j] = map[i][j] = 100;
				show_map[i][j] = 'x';
			}
			else
			{
				cost_map[i][j] = map[i][j] = 0;
				show_map[i][j] = '0';
			}
		}
	}
}

// path finding function,can be alternate to other algorithm like Dijstra
int A_star_path_finding(int map[Height][Width], int cost_map[Height][Width], int start_point[2], int end_point[2], int Path[length][2])
{
	// Set {x_coordinate,y_coodrinate,f(n),x_parent,y_parent,valid_bit}
	int ClosedSet[array_size][6];
	int OpenSet[array_size][6];
	int current_point[2];
	int min_index = 0;
	int CloseSet_index = 0;
	int hn = 0, gn = 0;
	int Path_length = 0;
	Init(OpenSet, ClosedSet);

	OpenSet[0][0] = start_point[0];
	OpenSet[0][1] = start_point[1];
	OpenSet[0][2] = 0;
	OpenSet[0][3] = start_point[0];
	OpenSet[0][4] = start_point[1];
	OpenSet[0][5] = 1;

	// get cost_map
	while (check_end_point_in_openset(end_point, OpenSet) == -1)
	{

		if (!Is_openset_empty(OpenSet)){
			min_index = Get_minimun_fn_coord(OpenSet);
			cout << min_index << endl;
			current_point[0] = OpenSet[min_index][0];
			current_point[1] = OpenSet[min_index][1];
			Expend_current_point(cost_map, current_point, OpenSet, ClosedSet, end_point, start_point, CloseSet_index);

			ClosedSet[CloseSet_index][0] = OpenSet[min_index][0];
			ClosedSet[CloseSet_index][1] = OpenSet[min_index][1];
			ClosedSet[CloseSet_index][2] = OpenSet[min_index][2];
			ClosedSet[CloseSet_index][3] = OpenSet[min_index][3];
			ClosedSet[CloseSet_index][4] = OpenSet[min_index][4];
			ClosedSet[CloseSet_index][5] = 1;
			OpenSet[min_index][5] = 0;
			CloseSet_index++;

			Display_map(cost_map);
			cout << endl;
		}
		else{
			cout << "there is no solution to this map" << endl;
			return 0;
		}
	}
	int index = check_end_point_in_openset(end_point, OpenSet);
	ClosedSet[CloseSet_index][coorx] = OpenSet[index][coorx];
	ClosedSet[CloseSet_index][coory] = OpenSet[index][coory];
	ClosedSet[CloseSet_index][fn] = OpenSet[index][fn];
	ClosedSet[CloseSet_index][parentx] = OpenSet[index][parentx];
	ClosedSet[CloseSet_index][parenty] = OpenSet[index][parenty];
	ClosedSet[CloseSet_index][valid_bit] = 1;
	OpenSet[index][5] = 0;
	CloseSet_index++;
	Path[0][coorx] = end_point[coorx];
	Path[0][coory] = end_point[coory];
	Path_length = Get_path(ClosedSet, CloseSet_index, Path, start_point);
	return Path_length;
}

// Get path 
int  Get_path(int ClosedSet[array_size][6], int CloseSet_index, int Path[length][2], int start_point[2]){
	int Path_index = 1;
	int parent[2];
	int parent_index = 0;
	parent[coorx] = ClosedSet[CloseSet_index - 1][parentx];
	parent[coory] = ClosedSet[CloseSet_index - 1][parenty];
	cout << "Path:" << endl;
	cout << "(" << Path[0][coorx] << "," << Path[0][coory] << ")" << endl;
	while (!(Path[Path_index - 1][coorx] == start_point[coorx] && Path[Path_index - 1][coory] == start_point[coory])){
		parent_index = Find_parent_index(parent, ClosedSet, CloseSet_index);
		Path[Path_index][coorx] = ClosedSet[parent_index][coorx];
		Path[Path_index][coory] = ClosedSet[parent_index][coory];
		parent[coorx] = ClosedSet[parent_index][parentx];
		parent[coory] = ClosedSet[parent_index][parenty];
		cout << "(" << Path[Path_index][coorx] << "," << Path[Path_index][coory] << ")" << endl;
		Path_index++;
	}
	return Path_index;
}

// Find parent coordinate index from closed set
int Find_parent_index(int parent[2], int ClosedSet[array_size][6], int CloseSet_index){
	for (int i = CloseSet_index - 1; i >= 0; i--){
		if (parent[coorx] == ClosedSet[i][coorx] && parent[coory] == ClosedSet[i][coory]){
			return i;
		}
	}
	return 0;
}

//initialzation of close set and open set,just set valid_bit to 0
void Init(int Openset[array_size][6], int ClosedSet[array_size][6]){
	for (int i = 0; i < array_size; i++){
		Openset[i][valid_bit] = ClosedSet[i][valid_bit] = 0;
	}
}

// check if the point is in the open set or closed set.
bool Is_in_open_or_closed_set(int point[2], int Openset[array_size][6], int Closedset[array_size][6], int closed_list_counter){
	for (int i = 0; i < array_size; i++){
		if (Openset[i][valid_bit] == 1 && (Openset[i][0] == point[0] && Openset[i][1] == point[1])){
			return true;
		}
	}
	for (int i = 0; i < closed_list_counter; i++){
		if (Closedset[i][0] == point[0] && Closedset[i][1] == point[1]){
			return true;
		}
	}
	return false;
}

// check if openset is empty or not 
bool Is_openset_empty(int Openset[array_size][6]){
	for (int i = 0; i < array_size; i++){
		if (Openset[i][valid_bit] == 1)
			return false;
	}
	return true;
}

void Expend_current_point(int cost_map[Height][Height], int current_point[2], int Openset[array_size][6], int ClosedSet[array_size][6], int end_point[2], int start_point[2], int closed_list_counter){

	int point[2];

	// upper
	point[0] = current_point[0] - 1;
	point[1] = current_point[1];
	if (current_point[0] > 0 && !Is_in_open_or_closed_set(point, Openset, ClosedSet, closed_list_counter) && cost_map[point[0]][point[1]] != 100){
		// create a point variable
		int index = 0;
		cost_map[point[0]][point[1]] = Get_heuristic_function(point, end_point) + Get_gn(point, start_point);
		for (int i = 0; i < array_size; i++){
			if (Openset[i][valid_bit] == 0){
				index = i;
				break;
			}
			else if (i == array_size){
				cout << "open list fulled." << endl;
			}
		}
		Openset[index][0] = point[0];
		Openset[index][1] = point[1];
		Openset[index][2] = cost_map[point[0]][point[1]];
		Openset[index][3] = current_point[0];
		Openset[index][4] = current_point[1];
		Openset[index][5] = 1;
	}
	// upper left
	point[0] = current_point[0] - 1;
	point[1] = current_point[1] - 1;
	if (current_point[1] > 0 && current_point[0] > 0 && !Is_in_open_or_closed_set(point, Openset, ClosedSet, closed_list_counter) && cost_map[point[0]][point[1]] != 100){
		// create a point variable
		int index = 0;
		cost_map[point[0]][point[1]] = Get_heuristic_function(point, end_point) + Get_gn(current_point, start_point) + 4;
		for (int i = 0; i < array_size; i++){
			if (Openset[i][valid_bit] == 0){
				index = i;
				break;
			}
			else if (i == array_size){
				cout << "open list fulled." << endl;
			}
		}
		Openset[index][0] = point[0];
		Openset[index][1] = point[1];
		Openset[index][2] = cost_map[point[0]][point[1]];
		Openset[index][3] = current_point[0];
		Openset[index][4] = current_point[1];
		Openset[index][5] = 1;
	}

	// upper right
	point[0] = current_point[0] - 1;
	point[1] = current_point[1] + 1;
	if (current_point[1] < Width && current_point[0] > 0 && !Is_in_open_or_closed_set(point, Openset, ClosedSet, closed_list_counter) && cost_map[point[0]][point[1]] != 100){
		// create a point variable
		int index = 0;
		cost_map[point[0]][point[1]] = Get_heuristic_function(point, end_point) + Get_gn(current_point, start_point) + 4;
		for (int i = 0; i < array_size; i++){
			if (Openset[i][valid_bit] == 0){
				index = i;
				break;
			}
			else if (i == array_size){
				cout << "open list fulled." << endl;
			}
		}
		Openset[index][0] = point[0];
		Openset[index][1] = point[1];
		Openset[index][2] = cost_map[point[0]][point[1]];
		Openset[index][3] = current_point[0];
		Openset[index][4] = current_point[1];
		Openset[index][5] = 1;
	}

	// left
	point[0] = current_point[0];
	point[1] = current_point[1] - 1;
	if (current_point[1] > 0 && !Is_in_open_or_closed_set(point, Openset, ClosedSet, closed_list_counter) && cost_map[point[0]][point[1]] != 100){
		int index = 0;
		cost_map[point[0]][point[1]] = Get_heuristic_function(point, end_point) + Get_gn(point, start_point);
		for (int i = 0; i < array_size; i++){
			if (Openset[i][valid_bit] == 0){
				index = i;
				break;
			}
			else if (i == array_size){
				cout << "open list fulled." << endl;
			}
		}
		Openset[index][0] = point[0];
		Openset[index][1] = point[1];
		Openset[index][2] = cost_map[point[0]][point[1]];
		Openset[index][3] = current_point[0];
		Openset[index][4] = current_point[1];
		Openset[index][5] = 1;
	}
	// right
	point[0] = current_point[0] ;
	point[1] = current_point[1]+1;
	if (current_point[1] < Width&& !Is_in_open_or_closed_set(point, Openset, ClosedSet, closed_list_counter) && cost_map[point[0]][point[1]] != 100){

		int index = 0;
		cost_map[point[0]][point[1]] = Get_heuristic_function(point, end_point) + Get_gn(point, start_point);
		for (int i = 0; i < array_size; i++){
			if (Openset[i][valid_bit] == 0){
				index = i;
				break;
			}
			else if (i == array_size){
				cout << "open list fulled." << endl;
			}
		}


		Openset[index][0] = point[0];
		Openset[index][1] = point[1];
		Openset[index][2] = cost_map[point[0]][point[1]];
		Openset[index][3] = current_point[0];
		Openset[index][4] = current_point[1];
		Openset[index][5] = 1;
	}
	// down
	point[0] = current_point[0] + 1;
	point[1] = current_point[1];
	if (current_point[0] < Height&& !Is_in_open_or_closed_set(point, Openset, ClosedSet, closed_list_counter) && cost_map[point[0]][point[1]] != 100){
		int index = 0;
		cost_map[point[0]][point[1]] = Get_heuristic_function(point, end_point) + Get_gn(point, start_point);
		for (int i = 0; i < array_size; i++){
			if (Openset[i][valid_bit] == 0){
				index = i;
				break;
			}
			else if (i == array_size){
				cout << "open list fulled." << endl;
			}
		}
		Openset[index][0] = point[0];
		Openset[index][1] = point[1];
		Openset[index][2] = cost_map[point[0]][point[1]];
		Openset[index][3] = current_point[0];
		Openset[index][4] = current_point[1];
		Openset[index][5] = 1;
	}

	// lower left
	point[0] = current_point[0] + 1;
	point[1] = current_point[1] - 1;
	if (current_point[1] > 0 && current_point[0] < Height && !Is_in_open_or_closed_set(point, Openset, ClosedSet, closed_list_counter) && cost_map[point[0]][point[1]] != 100){
		// create a point variable
		int index = 0;
		cost_map[point[0]][point[1]] = Get_heuristic_function(point, end_point) + Get_gn(current_point, start_point) + 4;
		for (int i = 0; i < array_size; i++){
			if (Openset[i][valid_bit] == 0){
				index = i;
				break;
			}
			else if (i == array_size){
				cout << "open list fulled." << endl;
			}
		}
		Openset[index][0] = point[0];
		Openset[index][1] = point[1];
		Openset[index][2] = cost_map[point[0]][point[1]];
		Openset[index][3] = current_point[0];
		Openset[index][4] = current_point[1];
		Openset[index][5] = 1;
	}

	// lower right
	point[0] = current_point[0] + 1;
	point[1] = current_point[1] + 1;
	if (current_point[1] < Width && current_point[0] < Height && !Is_in_open_or_closed_set(point, Openset, ClosedSet, closed_list_counter) && cost_map[point[0]][point[1]] != 100){
		// create a point variable
		int index = 0;
		cost_map[point[0]][point[1]] = Get_heuristic_function(point, end_point) + Get_gn(current_point, start_point) + 4;
		for (int i = 0; i < array_size; i++){
			if (Openset[i][valid_bit] == 0){
				index = i;
				break;
			}
			else if (i == array_size){
				cout << "open list fulled." << endl;
			}
		}
		Openset[index][0] = point[0];
		Openset[index][1] = point[1];
		Openset[index][2] = cost_map[point[0]][point[1]];
		Openset[index][3] = current_point[0];
		Openset[index][4] = current_point[1];
		Openset[index][5] = 1;
	}
}

// get Manhaton distance
int Get_heuristic_function(int current_point[2], int Goal[2])
{
	return (abs(Goal[1] - current_point[1]) + abs(Goal[0] - current_point[0]));
}

int Get_gn(int current_point[2], int start_point[2])
{
	return (abs(start_point[1] - current_point[1]) + abs(start_point[0] - current_point[0]));
}


int check_end_point_in_openset(int end_point[2], int OpenSet[array_size][6])
{
	for (int i = 0; i < array_size; i++)
	{
		if (OpenSet[i][coorx] == end_point[0] && OpenSet[i][coory] == end_point[1])
			return i;
	}
	return -1;
}

int Get_minimun_fn_coord(int Openset[array_size][6])
{
	int result_index = 0;
	int minimun = 0;
	int i = 0;
	for (i = 0; i < array_size; i++){
		if (Openset[i][valid_bit] == 1){
			minimun = Openset[i][fn];
			result_index = i;
			break;
		}
	}
	for (; i < array_size; i++)
	{
		if (Openset[i][fn] < minimun  &&  Openset[i][valid_bit] == 1)
		{
			result_index = i;
			minimun = Openset[i][fn];
			break;
		}
	}
	return result_index;
}

// display the map with grid
void Display_map(int map[Height][Width])
{
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			printf("%3d", map[i][j]);
		}
		cout << endl;
	}
}

void Draw_map_with_path(char  show_map[Height][Width], int Path[length][2], int Path_length){
	for (int i = 0; i < Path_length; i++){
		show_map[Path[i][coorx]][Path[i][coory]] = '.';
	}
	for (int i = 0; i < Height; i++){
		for (int j = 0; j < Width; j++){
			printf("%3c", show_map[i][j]);
		}
		cout << endl;
	}
}

int main(int argc, char **argv)
{
	int map[Height][Width], cost_map[Height][Width];
	char show_map[Height][Width];
	int Path[length][2];
	int Path_index = 0;
	Create_map(map,cost_map,show_map);
	cout << "original map" << endl;
	Display_map(map);
	cout << endl;
	int start_point[2] = { 0, 0 };
	int end_point[2] = { 9, 9 };
	int Path_length = A_star_path_finding(map, cost_map, start_point, end_point, Path);
	cout << "There is ";
	cout << Path_length << " steps in total" << endl;
	Draw_map_with_path(show_map, Path, Path_length);
	Display_map(cost_map);
	system("pause");
	return 0;
}
