#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>

#include <ext/hash_set>

using namespace std;
using __gnu_cxx::hash_set;

struct my_hash {
    size_t operator()(const string& s) const {
    	unsigned long sum = 0;
    	for (int i = 0; i < s.size(); i++) {
    		sum += s[i];
    	}
        return sum % (1UL << sizeof(size_t));
    }
};
struct my_equal_to {
    bool operator()(const string& lhs, const string& rhs) const {
        return lhs == rhs;
    }
};

struct State {
	string state;
	vector<int> path;
	int estiVal;
	int moves() {
		return path.size();
	}
};

class MagicCube {
private:
	string initial;
	State _state;
	string followState[12];
	list<State> fringe;
	hash_set<string, my_hash, my_equal_to> visited;
	int permumation[81] = {
	-1, -1, -1, 9,  10, 11, -1, -1, -1,
	-1, -1, -1, 12, 13, 14, -1, -1, -1,
	-1, -1, -1, 15, 16, 17, -1, -1, -1,
	18, 19, 20, 0,  1,  2,  27, 28, 29,
	21, 22, 23, 3,  4,  5,  30, 31, 32,
	24, 25, 26, 6,  7,  8,  33, 34, 35,
	-1, -1, -1, 36, 37, 38, -1, -1, -1,
	-1, -1, -1, 39, 40, 41, -1, -1, -1,
	-1, -1, -1, 42, 43, 44, -1, -1, -1
	};

	bool satisfy() {
		const string &state = _state.state;
		char color = state[0];
		for (int i = 1; i < 9; i++) {
			if (color != state[i])
				return false;
		}
		return true;
	}

	void getFollowStates() {
		for (int i = 0; i < 12; i++) {
			followState[i] = _state.state;
		}
		Col1Up(followState[0]);
		Col1Down(followState[1]);
		Col2Up(followState[2]);
		Col2Down(followState[3]);
		Col3Up(followState[4]);
		Col3Down(followState[5]);
		Row1Left(followState[6]);
		Row1Right(followState[7]);
		Row2Left(followState[8]);
		Row2Right(followState[9]);
		Row3Left(followState[10]);
		Row3Right(followState[11]);
		for (int i = 0; i < 12; i++) {
			if (visited.find(followState[i]) != visited.end()) {
				followState[i] = "";
			}
		}
	}

	int getEstimateValue(string t) {
		int count[4] = {0,0,0,0}, max =-1;
		for (int i =0; i < 9; i++) {
			if (t[i] == 'b')
				count[0]++;
			else if (t[i] =='g')
				count[1]++;
			else if (t[i] == 'r')
				count[2]++;
			else
				count[3]++;
		}
		for (int i = 0; i < 4; i++)
			if (max < count[i])
				max = count[i];
		return 9 - max;
	}

	void insertNode(State s) {
		if (fringe.empty()) {
			fringe.push_back(s);
			return;
		}
		for (list<State>::iterator it = fringe.begin(); it != fringe.end(); it++) {
			if (s.moves() + s.estiVal <= it->moves() + it->estiVal) {
				fringe.insert(it, s);
				return;
			}
		}
		fringe.push_back(s);
	}

public:
	bool readProblem(const char* filename) {
		ifstream input(filename);
		if (!input.is_open()) {
			cerr << "file can not be opened" << endl;
			return false;
		}
		char s[46]; s[45] = '\0';
		input 							>> s[9]  >> s[10] >> s[11]
										>> s[12] >> s[13] >> s[14]
										>> s[15] >> s[16] >> s[17]
			 >> s[18] >> s[19] >> s[20] >> s[0]  >> s[1]  >> s[2]  >> s[27] >> s[28] >> s[29]
			 >> s[21] >> s[22] >> s[23] >> s[3]  >> s[4]  >> s[5]  >> s[30] >> s[31] >> s[32]
			 >> s[24] >> s[25] >> s[26] >> s[6]  >> s[7]  >> s[8]  >> s[33] >> s[34] >> s[35]
			 							>> s[36] >> s[37] >> s[38]
			 							>> s[39] >> s[40] >> s[41]
			 							>> s[42] >> s[43] >> s[44];
		if (input.fail()) {
			cerr << "read error occurred" << endl;
			return false;
		}
		input.close();
		for (int i = 0; i < 45; i++)
			initial += s[i];
		return true;
	}
	
	bool Asolve() {
		bool isCompleted = false;
		_state.state = initial;
		_state.path = vector<int>();
		_state.estiVal = getEstimateValue(initial);
		fringe.push_back(_state);
		int totalTime = 100000;
		for (int i = 0; i < totalTime; i++) {
			_state = *fringe.begin();
			fringe.erase(fringe.begin());
			cout << "times: " << i << '\t' << "steps:" << _state.moves() << endl;
			printState(_state.state);
			if (satisfy()) {
				isCompleted = true;
				break;
			}
			getFollowStates();
			for (int i = 0; i < 12; i++) {
				if (followState[i] != "") {
					State s;
					s.state = followState[i];
					s.path = _state.path;
					s.path.push_back(i);
					s.estiVal = getEstimateValue(s.state);
					insertNode(s);
					visited.insert(followState[i]);
				}
			}
		}
		return isCompleted;
	}

	void printState(const string& s) {
		int *p = permumation;
		for (int i = 0; i < 81; i++) {
			if (p[i] == -1) {
				cout << "  ";
			} else {
				cout << s[p[i]] << ' ';
			}
			if (i % 9 == 8) cout << '\n';
		}
		cout << endl;
	}

	void Col1Up(string & s) {
		int *p = permumation;
		char tmp = s[p[0*9 + 3]];
		for (int i = 0; i < 8; i++) {
			s[p[i*9 + 3]] = s[p[(i+1)*9 + 3]];
		}
		s[p[8*9+3]] = tmp;
	}
	void Col1Down(string & s) {
		int *p = permumation;
		char tmp = s[p[8*9 + 3]];
		for (int i = 8; i > 0; i--) {
			s[p[i*9 + 3]] = s[p[(i-1)*9 + 3]];
		}
		s[p[0*9+3]] = tmp;
	}
	void Col2Up(string & s) {
		int *p = permumation;
		char tmp = s[p[0*9 + 4]];
		for (int i = 0; i < 8; i++) {
			s[p[i*9 + 4]] = s[p[(i+1)*9 + 4]];
		}
		s[p[8*9+4]] = tmp;
	}
	void Col2Down(string & s) {
		int *p = permumation;
		char tmp = s[p[8*9 + 4]];
		for (int i = 8; i > 0; i--) {
			s[p[i*9 + 4]] = s[p[(i-1)*9 + 4]];
		}
		s[p[0*9+4]] = tmp;
	}
	void Col3Up(string & s) {
		int *p = permumation;
		char tmp = s[p[0*9 + 5]];
		for (int i = 0; i < 8; i++) {
			s[p[i*9 + 5]] = s[p[(i+1)*9 + 5]];
		}
		s[p[8*9+5]] = tmp;
	}
	void Col3Down(string & s) {
		int *p = permumation;
		char tmp = s[p[8*9 + 5]];
		for (int i = 8; i > 0; i--) {
			s[p[i*9 + 5]] = s[p[(i-1)*9 + 5]];
		}
		s[p[0*9+5]] = tmp;
	}
	void Row1Left(string & s) {
		int *p = permumation;
		char tmp = s[p[3*9 + 0]];
		for (int i = 0; i < 8; i++) {
			s[p[3*9 + i]] = s[p[3*9 + i+1]];
		}
		s[p[3*9 + 8]] = tmp;
	}
	void Row1Right(string & s) {
		int *p = permumation;
		char tmp = s[p[3*9 + 8]];
		for (int i = 8; i > 0; i--) {
			s[p[3*9 + i]] = s[p[3*9 + i-1]];
		}
		s[p[3*9+0]] = tmp;
	}
	void Row2Left(string & s) {
		int *p = permumation;
		char tmp = s[p[4*9 + 0]];
		for (int i = 0; i < 8; i++) {
			s[p[4*9 + i]] = s[p[4*9 + i+1]];
		}
		s[p[4*9 + 8]] = tmp;
	}
	void Row2Right(string & s) {
		int *p = permumation;
		char tmp = s[p[4*9 + 8]];
		for (int i = 8; i > 0; i--) {
			s[p[4*9 + i]] = s[p[4*9 + i-1]];
		}
		s[p[4*9+0]] = tmp;
	}
	void Row3Left(string & s) {
		int *p = permumation;
		char tmp = s[p[5*9 + 0]];
		for (int i = 0; i < 8; i++) {
			s[p[5*9 + i]] = s[p[5*9 + i+1]];
		}
		s[p[5*9 + 8]] = tmp;
	}
	void Row3Right(string & s) {
		int *p = permumation;
		char tmp = s[p[5*9 + 8]];
		for (int i = 8; i > 0; i--) {
			s[p[5*9 + i]] = s[p[5*9 + i-1]];
		}
		s[p[5*9+0]] = tmp;
	}

	void printPath() const {
		string pathMap[12] = {
			"Col 1 Up",
			"Col 1 Down",
			"Col 2 Up",
			"Col 2 Down",
			"Col 3 Up",
			"Col 3 Down",
			"Row 1 Left",
			"Row 1 Right",
			"Row 2 Left",
			"Row 2 Right",
			"Row 3 Left",
			"Row 3 Right"
		};
		const vector<int> &path = _state.path;
		cout << "total steps: " << path.size() << endl;
		for (int i = 0; i < path.size(); i++) {
			cout << pathMap[path[i]] << endl;
		}
	}
};

// void test() {
// 	MagicCube mc;
// 	string state("123456789abcdefghiABCDEFGHIjklmnopqrJKLMNOPQR");
// 	mc.printState(state);
// 	mc.Col1Up(state);
// 	mc.printState(state);
// 	mc.Col2Up(state);
// 	mc.printState(state);
// 	mc.Col3Down(state);
// 	mc.printState(state);
// }

int main() {
	freopen("output.txt", "w", stdout);
	MagicCube mc;
	mc.readProblem("input.txt");
	if (mc.Asolve())
		mc.printPath();
	else
		cout << "Failed!" <<endl;
}
