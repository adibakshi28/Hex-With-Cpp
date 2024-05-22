/*
The game of Hex is implemented using graph. All the positions of the hex board are the nodes of the graph and these nodes have a bi-directional
edge between its adjcent valid positions as per Hex board. The nodes can have any one of the three possible values/tags
'E' -> Empty Node
'X' -> Player 1 occupied Node
'O' -> Player 2 occupied Node
Initially all the nodes have value 'E' ie all are initially empty and they value gets updated as the game progresses and players occupy nodes.
After every move it is checked wheather a path is generated spanning between opposite edges of the board which results in a win for the player
*/

#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<unordered_set>

#define MAX_SIZE 26   // Maximum size of Hex board possible

using namespace std;

// Global Variables
int boardSize;   // Current Hex board size
vector<int> adj[MAX_SIZE * MAX_SIZE];   // Adjency list for the graph generated
vector<char> nodes;		// Information of the node. Could be any of the following ['E'- > Empty Cell ,'X' -> First Player(Vertical) ,'O' -> Second Player(Horizontal)]
unordered_set<int> src1, src2, dest1, dest2;   // Source and Destination nodes for implementing BFS
// src1 -> top edge nodes , dest1 -> bottom edge nodes, src2 -> left edge nodes, dest2 -> right edge nodes


void AddEdge(int u, int v) {   // This is a helper function to ConnectNodes() used to add edge between two nodes of graph
	adj[u].push_back(v);
}

bool CheckValid(int x) {     // This is a helper function to ConnectNodes() used to validate that the node index exists in graph
	return (x >= 0 && x <= (boardSize * boardSize) - 1);
}

void ConnectNodes(int curNodeIndex) {   // This function is used to make connections (edges) between 'curNodeIndex' node and other valid nodes in a hex board
	vector<int> conn;

	if (curNodeIndex == 0) {     // Top Left corner node
		vector<int> tempConn = { 1,boardSize };
		conn = tempConn;
	}
	else if (curNodeIndex == boardSize - 1) {   // Top right corner node
		vector<int> tempConn = { -1,boardSize - 1,boardSize };
		conn = tempConn;
	}
	else if (curNodeIndex == boardSize*boardSize - boardSize) {   // Bottom left corner node
		vector<int> tempConn = { -boardSize,-boardSize + 1,1 };
		conn = tempConn;
	}
	else if (curNodeIndex == boardSize*boardSize - 1) {   // Bottom right corner node
		vector<int> tempConn = { -boardSize,-1 };
		conn = tempConn;
	}
	else if (curNodeIndex % boardSize == 0) {   // Left edge nodes
		vector<int> tempConn = { -boardSize,-boardSize + 1,1,boardSize };
		conn = tempConn;
	}
	else if ((curNodeIndex + 1) % boardSize == 0) {  // Right edge nodes
		vector<int> tempConn = { -boardSize,-1,boardSize - 1,boardSize };
		conn = tempConn;
	}
	else {   // Remaining nodes (Internal nodes, top edge nodes, bottom edge nodes)
		vector<int> tempConn = { -boardSize,-boardSize + 1,-1,1,boardSize - 1,boardSize };
		conn = tempConn;
	}

	for (int i = 0;i < conn.size();i++) {
		if (CheckValid(curNodeIndex + conn[i])) {
			AddEdge(curNodeIndex, curNodeIndex + conn[i]);
		}
	}
}

void DrawDirection() {    // This function Draws the direction indicator for both players
	cout << endl;
	cout <<"\t   X"<<endl<<"\t   |"<<endl<<"\tO--|--O"<<endl<<"\t   |"<<endl<<"\t   X";
	cout << endl;
}

void DrawBoard() {     // This function draws the updated hex board 
	DrawDirection();
	cout << endl;

	for (int i = 0;i < boardSize;i++) {
		char c = 'a' + i;
		cout <<"  "<<c;
	}
	cout << endl;

	int curNode = 0;
	for (int i = 0;i < boardSize;i++) {
		for (int j = 0;j < i;j++) {
			cout << "  ";
		}

		cout << i + 1 << "  ";

		for (int j = 0;j < boardSize;j++) {
			if (nodes[curNode] == 'O') {
				cout << "O";
			}
			else if (nodes[curNode] == 'X') {
				cout << "X";
			}
			else {
				cout << "-";
			}
			cout << "  ";
			curNode++;
		}

		cout <<i + 1<< endl;
	}

	for (int i = 0;i < boardSize + 1;i++) {
		cout << "  ";
	}
	for (int i = 0;i < boardSize;i++) {
		char c = 'a' + i;
		cout << c << "  ";
	}
	cout << endl << endl;
}


bool CheckWin(bool player1Move) {  //This Function checks wheather current player has won. It applies BFS to check for existance of path 
	vector<char> nodeInfo = nodes;
	queue<int> q;

	if (player1Move) {   // Checking win for player 1 (X)
		// Standard BFS algorithm implementation 
		for (auto it = src1.begin();it != src1.end();it++) {
			if (nodeInfo[*it] == 'X') {
				q.push(*it);
				nodeInfo[*it] = 'E';
			}
		}

		while (!q.empty()) {
			int u = q.front();

			if (dest1.find(u) != dest1.end()) {
				return true;
			}

			q.pop();
			for (auto it = adj[u].begin();it != adj[u].end();it++) {
				int v = *it;
				if (nodeInfo[v] == 'X') {
					nodeInfo[v] = 'E';
					q.push(v);
				}
			}
		}
		return false;
	}
	else {		// Checking win for player 2 (O)
		// Standard BFS algorithm implementation 
		for (auto it = src2.begin();it != src2.end();it++) {
			if (nodeInfo[*it] == 'O') {
				q.push(*it);
				nodeInfo[*it] = 'E';
			}
		}

		while (!q.empty()) {
			int u = q.front();

			if (dest2.find(u) != dest2.end()) {
				return true;
			}

			q.pop();
			for (auto it = adj[u].begin();it != adj[u].end();it++) {
				int v = *it;
				if (nodeInfo[v] == 'O') {
					nodeInfo[v] = 'E';
					q.push(v);
				}
			}
		}

		return false;
	}

	return false;
}


void SetSrcDest() {   // This function populates values of source and destination nodes 
	for (int i = 0;i < boardSize;i++) {  // src1 -> top edge nodes
		src1.insert(i);
	}

	for (int i = boardSize * boardSize - boardSize;i < boardSize * boardSize;i++) {  //dest1 -> bottom edge nodes
		dest1.insert(i);
	}

	int y = 0;
	while (src2.size() < boardSize) {  // src2 -> left edge nodes
		src2.insert(y);
		y += boardSize;
	}

	y = 4;
	while (dest2.size() < boardSize) {   // dest2 -> right edge nodes
		dest2.insert(y);
		y += boardSize;
	}
}


bool IsValidMove(string move,int &moveCellIndex) {  // This Function parses the entered moves and returns true/false if it is valid move or not. It also calculates nodes index fro the index of Hex board node entered by the user 
	if (move.length() != 2 && move.length() != 3) {
		return false;
	}

	int row, col;

	if (isalpha(move[0]) && isdigit(move[move.length() - 1])) {
		col = tolower(move[0]) - 'a' + 1;
		if (col > boardSize) {
			return false;
		}

		if (move.length() == 2) {
			row = move[1] - '0';
		}
		else {
			row = ((move[1] - '0') * 10) + (move[2] - '0');
		}

	}
	else if (isalpha(move[move.length() - 1]) && isdigit(move[0])) {
		col = tolower(move[move.length() - 1]) - 'a' + 1;
		if (col > boardSize) {
			return false;
		}

		if (move.length() == 2) {
			row = move[0] - '0';
		}
		else {
			row = ((move[0] - '0') * 10) + (move[1] - '0');
		}
	}
	else {
		return false;
	}

	moveCellIndex = ((row - 1) * boardSize) + col - 1;
	return true;
}


int main() {  // Main Function
	do {
		cout << "Enter Board Size (min 5) : ";
		cin >> boardSize;
		cout << endl;
	} while (boardSize < 5);

	vector<char> nds(boardSize*boardSize,'E');    // Initially all the nodes are tagged as 'E' ie empty
	nodes = nds;
	for (int i = 0;i < boardSize * boardSize;i++) {  // Make connections (Edges) for all nodes of the graph
		ConnectNodes(i);
	}

	SetSrcDest();  
	DrawBoard();

	bool player1Move = true;

	while (true) {
		char curMoveChar;
		string move = "";

		if (player1Move) {
			curMoveChar = 'X';
			cout << "Player 1 (X) move ... Enter cell index : ";
		}
		else {
			curMoveChar = 'O';
			cout << "Player 2 (O) move ... Enter cell index : ";
		}
		
		cin >> move;
		
		int moveCellIndex = -1;
		if (!IsValidMove(move,moveCellIndex) || nodes[moveCellIndex] != 'E') {
			cout << endl;
			cout << "Invalid Move .... Try again !";
			cout << endl;
			DrawBoard();
			continue;
		}

		nodes[moveCellIndex] = curMoveChar;

		DrawBoard();
		
		if (CheckWin(player1Move)) {
			break;
		}

		player1Move = !player1Move;
	}

	if (player1Move) {
		cout << "Player 1 (X) has won !!!...";
	}
	else {
		cout << "Player 2 (O) has won !!!...";
	}
	cout << endl;

	return 0;
}