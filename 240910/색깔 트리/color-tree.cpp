#include<iostream>
#include<unordered_map>
#include<vector>
#include<bitset>


int cur_cid = 0;

struct Node {
	int pid;
	int max_depth;
	int color;
	int color_changed_cid;
	std::vector<int> children;
};

std::unordered_map<int, Node> tree;
std::vector<int> roots;

void add_node();
void change_color();
void query_color();
void calculate_value();

int main() {
	tree.reserve(40000);

	std::cin.tie(0);
	std::ios_base::sync_with_stdio(0);

	int Q;
	std::cin >> Q;

	for (int i = 0; i < Q; ++i) {
		int command;
		std::cin >> command;
		switch (command) {
		case 100:
			add_node();
			break;
		case 200:
			change_color();
			break;
		case 300:
			query_color();
			break;
		case 400:
			calculate_value();
			break;
		default:
			break;
		}
		++cur_cid;
	} 
}

/* 
struct Node {
	int pid;
	int max_depth;
	int color;
	int color_changed_cid;
	std::vector<int> children;
};
*/
void add_node() {
	int mid, pid, color, max_depth;
	std::cin >> mid >> pid >> color >> max_depth;
	// pid == -1 means that this node is the root
	if (pid == -1) {
		tree[mid] = Node{ pid, max_depth, color, cur_cid, {} };
		roots.push_back(mid);
		return; 
	}
	// check if this node insertion is legit
	// by checking its parent
	if (tree[pid].max_depth == 1) return;

	max_depth = std::min(max_depth, tree[pid].max_depth - 1);

	// insert new node
	tree[pid].children.push_back(mid);
	tree[mid] = Node{ pid, max_depth, color, cur_cid, {} }; 
}

void change_color() {
	int mid, color;
	std::cin >> mid >> color;
	tree[mid].color = color;
	tree[mid].color_changed_cid = cur_cid; 
}
void query_color() {
	int mid;
	std::cin >> mid;
	// to check color, all of its ancestors should be checked
	std::vector<int> s;
	int pid = tree[mid].pid; 
	while (pid != -1)	// it means that current node is not the root
	{
		s.push_back(mid);
		mid = pid;
		pid = tree[mid].pid;
	}

	// now pid is -1
	// that means tree[mid] is the root
	// mid, parent of mid, ..., child of root are in vector s
	int lastly_changed_cid = tree[mid].color_changed_cid;
	int lastly_changed_color = tree[mid].color;
	while (s.size()) {
		mid = s.back(); 
		if (tree[mid].color_changed_cid > lastly_changed_cid) {
			lastly_changed_cid = tree[mid].color_changed_cid;
			lastly_changed_color = tree[mid].color;
		}
		else {
			tree[mid].color_changed_cid = lastly_changed_cid;
			tree[mid].color = lastly_changed_color;
		} 
		s.pop_back();
	} 

	// result output
	std::cout << tree[mid].color << '\n';
}

// calculate the squared sum of the values of all nodes
int result;
std::bitset<6> dfs(int, int, int);
void calculate_value() {
	// initialize the final result
	result = 0;

	// calculate
	for (int root : roots) {
		dfs(root, tree[root].color_changed_cid, tree[root].color); 
	}

	// output print
	std::cout << result << '\n'; 
}

std::bitset<6> dfs(int mid, int lastly_changed_cid, int lastly_changed_color) {
	// recolor current node if needed
	if (lastly_changed_cid > tree[mid].color_changed_cid) {
		tree[mid].color_changed_cid = lastly_changed_cid;
		tree[mid].color = lastly_changed_color;
	}
	else {
		lastly_changed_cid = tree[mid].color_changed_cid;
		lastly_changed_color = tree[mid].color; 
	}

	// default result set
	std::bitset<6> mid_value = 0;
	mid_value |= (1 << (tree[mid].color));

	for (int child_id : tree[mid].children) {
		mid_value |= dfs(child_id, lastly_changed_cid, lastly_changed_color);
	}

	// add the squared value of current node to
	// the final result
	result += mid_value.count() * mid_value.count(); 

	return mid_value;
}