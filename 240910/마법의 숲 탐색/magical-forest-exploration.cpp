#include<iostream>
#include<vector>

struct vec2 {
	int x, y;
};

vec2 operator+(const vec2 a, const vec2 b) {
	return vec2{ a.x + b.x, a.y + b.y };
}

int R, C, K;
std::vector<int> start_col;
std::vector<int> exit_dirs;
std::vector<vec2> final_pos;

/*
direction representation
0 == north
1 == east
2 == south
3 == west
*/

const vec2 directions[4] =
{
	{-1, 0},
	{0, 1},
	{1, 0},
	{0, -1}
};


/*
map representation

		north
	0	1	C	C+1
0	|	x	x	|	<- empty spaces
1	|	x	x	|	<- empty spaces
2	|	x	x	|	<- empty spaces
3	|			|	<- 1st row
	|			|
	|			|	east
	|			|
R+2	|			|	<- Rth row
R+3	-------------
		south
*/

int to_real_row(int x) {
	return x - 2;
};

int main() {
 
	std::cin >> R >> C >> K;
	start_col.resize(K + 1);
	exit_dirs.resize(K + 1);
	final_pos.resize(K + 1);
	for (int i = 1; i <= K; ++i) {
		std::cin >> start_col[i] >> exit_dirs[i];
	}

	// initialize map
	std::vector<std::vector<int>> map(R+4, std::vector<int>(C+2));
	for (int i = 0; i < C + 2; ++i) {
		map[R+3][i] = -1;
	}
	for (int i = 0; i < R + 4; ++i) {
		map[i][0] = -1;
		map[i][C + 1] = -1;
	}

	int result = 0;


	for (int fairy = 1; fairy <= K; ++fairy) {
		// initialize
		int row = 1;
		int col = start_col[fairy];
		int exit_dir = exit_dirs[fairy];

		// search down
		while (true) {
			// check south direction
			if (map[row + 1][col - 1] == 0
				&& map[row + 1][col + 1] == 0
				&& map[row + 2][col] == 0)
			{
				++row;
				continue;
			} 
			// check west direction
			if (map[row - 1][col - 1] == 0
				&& map[row + 1][col - 1] == 0
				&& map[row][col - 2] == 0
				&& map[row + 1][col - 2] == 0
				&& map[row + 2][col - 1] == 0)
			{
				--col;
				++row;
				// rotate counter-clockwise
				--exit_dir;
				if (exit_dir == -1)
					exit_dir = 3;
				continue;
			}
			// check east direction
			if (map[row - 1][col + 1] == 0
				&& map[row + 1][col + 1] == 0
				&& map[row][col + 2] == 0
				&& map[row + 1][col + 2] == 0
				&& map[row + 2][col + 1] == 0)
			{
				++col;
				++row;
				// rotate clockwise
				++exit_dir;
				if (exit_dir == 4) {
					exit_dir = 0;
				}
				continue;
			} 

			// all directions are not available,
			// end searching
			break; 
		} 

		// check if the golem is not in forest
		if (row <= 3) {
			// clear map
			for (int i = 0; i <= R + 2; ++i) {
				for (int j = 1; j <= C; ++j) {
					map[i][j] = 0;
				}
			} 
			continue;
		}

		// map update
		map[row][col] = fairy;
		map[row+1][col] = fairy;
		map[row-1][col] = fairy;
		map[row][col+1] = fairy;
		map[row][col-1] = fairy; 

		exit_dirs[fairy] = exit_dir;
		final_pos[fairy] = { row,col }; 

		// evaluate lowest row reachable
		int max_row = 0;
		std::vector<int> golems{ fairy };
		std::vector<bool> visited(K + 1, false);
		visited[fairy] = true;
		while (golems.size()) {
			int current_golem = golems.back();
			golems.pop_back();
			vec2 current_pos = final_pos[current_golem];
			max_row = std::max(current_pos.x + 1, max_row);
			// find next reachable, unvisited golem
			vec2 exit_pos = current_pos + directions[exit_dirs[current_golem]];
			for (int i = 0; i < 4; ++i) {
				vec2 next = exit_pos + directions[i];
				int next_golem = map[next.x][next.y];
				if (next_golem > 0 && visited[next_golem] == false) {
					golems.push_back(next_golem);
					visited[next_golem] = true;
				}
			}
		}

		result += to_real_row(max_row); 
	}

	std::cout << result; 
}