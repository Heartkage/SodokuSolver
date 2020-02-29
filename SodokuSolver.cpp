#include<iostream>
#include<vector>
#include<fstream>


using namespace std;

struct Coordinate{
	int y;
	int x;
};

class Board_Info{
	int Setup()
	{
		int count = 0;
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<9; j++)
			{
				if(board[i][j] != '.')
					UpdateCell(i, j, board[i][j]);
				else
					count++;
			}
		}
		return count;
	}
public:
	vector<vector<char> > board;
	vector<vector<vector<bool> > > available;
	
	int Setup_Board_Info(vector<vector<char> >& b)
	{
		board = b;
		available.clear();
		
		for(int i=0; i<9; i++)
		{
			vector<vector<bool> > row;
			for(int j=0; j<9; j++)
			{
				vector<bool> table;
				for(int k=0; k<9; k++)
					table.push_back(true);
				row.push_back(table);
			}
			available.push_back(row);
		}
		return Setup();
	}
	
	bool CheckBoard(int y, int x, char c, vector<vector<char> >& board)
	{
		//Check Row
		for(int i=0; i<9; i++)
		{
			if(board[y][i] == c)
				return false;
		}
		//Check Column
		for(int i=0; i<9; i++)
		{
			if(board[i][x] == c)
				return false;
		}
		int index_y = y/3;
		int index_x = x/3;
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				int actual_y = index_y*3+i;
				int actual_x = index_x*3+j;		
				if(board[actual_y][actual_x] == c)	
					return false;
			}
		}
		
		return true;
	}
	
	void UpdateCell(int y, int x, char c)
	{
		int n = (int)c-49;
		//Update Row
		for(int i=0; i<9; i++)
		{
			available[y][i][n] = false;
		}
		
		//Update Column
		for(int i=0; i<9; i++)
		{
			available[i][x][n] = false;
		}
		
		//Update 3x3 Block
		int index_y = y/3;
		int index_x = x/3;
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				int actual_y = index_y*3+i;
				int actual_x = index_x*3+j;			
				available[actual_y][actual_x][n] = false;	
			}
		}
	}
	
	void Restore(int y, int x, char c, vector<vector<char> >& board)
	{
		int n = (int)c-49;
		available[y][x][n] = true;
		
		//Update 3x3 Block
		int index_y = y/3;
		int index_x = x/3;
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				int actual_y = index_y*3+i;
				int actual_x = index_x*3+j;		
				available[actual_y][actual_x][n] = CheckBoard(actual_y, actual_x, c, board)? true: false;	
			}
		}
		
		//Update Row
		for(int i=0; i<9; i++)
		{
			available[y][i][n] = CheckBoard(y, i, c, board)? true: false;
		}
		
		//Update Column
		for(int i=0; i<9; i++)
		{
			available[i][x][n] = CheckBoard(i, x, c, board)? true: false;
		}
		
		
	}

	
	void ShowAvailable(vector<vector<char> >& board)
	{
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<9; j++)
			{
				if(board[i][j] != '.')
					continue;
					
				cout << "(" << i << ", ";
				cout << j << "): ";
				for(int k=0; k<9; k++)
				{
					if(available[i][j][k])
						cout << "T ";
					else
						cout << "F ";
				}
				cout << endl;
			}
		}
	}
	

	
}; 

class Solution {
	
	Board_Info info;
	int emptyCount;
public:
	bool hasAnAnswerFound = false;
	
    void solveSudoku(vector<vector<char> >& board) {
        emptyCount = info.Setup_Board_Info(board);
      	if(Recursive(board, 0))
      		hasAnAnswerFound = true;
      	else
      		hasAnAnswerFound = false;
    }
    
    void ShowBoard(vector<vector<char> >& board)
    {
    	for(int i=0; i<9; i++)
		{
			for(int j=0; j<9; j++)
			{
				cout << board[i][j] << " ";
				if(j==2 || j==5)
					cout << " ";
			}
			cout << endl;
			if(i==2 || i==5)
				cout << endl;
		}
	}
    
    
    bool Recursive(vector<vector<char> >& board, int count)
    {
    	if(count >= emptyCount)
    		return true;
		
    	for(int j = 0; j<9; j++)
    	{
    		for(int i = 0; i<9; i++)
    		{
    			if(board[j][i] == '.')
    			{
    				for(int k=0; k<9; k++)
    				{
    					if(info.available[j][i][k])
    					{
    						board[j][i] = ((char)k)+49;
    						info.UpdateCell(j, i, board[j][i]);
    						/*ShowBoard(board);
    						info.ShowAvailable(board);
    						cout << endl;*/
    						if(Recursive(board, count+1))
    							return true;
    						else
    						{
    							char c = board[j][i];
    							board[j][i] = '.';
    							info.Restore(j, i, c, board);
							}
						}
					}
					return false;
				}
			}
		}
        return false;
	}
};

int main(int argc, char**argv)
{
	if(argc != 2)
	{
		cout << "[Usage] You need to give a text file." << endl;
		return 1;
	}
	
	ifstream ifile;
	ifile.open(argv[argc-1], ios::in);
	string input;
	vector<vector<char> > board;
	cout << "[Solver] Start to solve the board..." << endl << endl;
	
	cout << "[Initial Board]" << endl;
	for(int i=0; i<9; i++)
	{
		ifile >> input;
		vector<char> row;
		if(input.length() == 9)
		{
			for(int j=0; j<input.length(); j++)
			{
				row.push_back(input[j]);
				cout << input[j] << " "; 
				if(j==2 || j==5)
					cout << " ";
			}
			cout << endl;
			if(i==2 || i==5)
				cout << endl;
			board.push_back(row);
		}
		else
		{
			cout << "[Error] In your board text file, line(" << i+1 << ") is not 9 characters" << endl;
			return -1;
		}
	}
	cout << endl;
	
	Solution solve;
	
	solve.solveSudoku(board);
	if(solve.hasAnAnswerFound)
	{
		cout << "[Solved] Answer Found" << endl << endl;
		solve.ShowBoard(board);
	}
	else
	{
		cout << "The board was not able to solve" << endl;
	}
	
	return 0;
}
