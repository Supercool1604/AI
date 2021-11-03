#include<bits/stdc++.h>
#include<map>
#include<fstream>
using namespace std;


#define KOMI 2.5
#define boardSize 5
#define maxMoves 24
int originalPlayer ;
vector<vector<int> > originalBoard(5, vector<int> (5));

class GameGo{

public:
	vector<vector<int> > board;
	vector<vector<int> > previousBoard;
	vector<pair<int, int> > removedStones;
	int moveNumber;
	GameGo()
	{
		this->moveNumber = 0;
		this->board = vector<vector<int > >(boardSize, vector<int> (boardSize));
		this->previousBoard = this->board;
	}

	vector<pair<int, int> > friendlyStones(vector<vector<int> > board, int x, int y)
	{
		vector<pair<int, int> > friendlyNeighbors;
		if(x-1>=0 && board[x-1][y]==board[x][y])
		{
			friendlyNeighbors.push_back(make_pair(x-1,y));
		}
		if(y-1>=0 && board[x][y-1]==board[x][y])
		{
			friendlyNeighbors.push_back(make_pair(x,y-1));
		}
		if(x+1<boardSize && board[x+1][y]==board[x][y])
		{
			friendlyNeighbors.push_back(make_pair(x+1,y));
		}
		if(y+1<boardSize && board[x][y+1]==board[x][y])
		{
			friendlyNeighbors.push_back(make_pair(x,y+1));
		}
		return friendlyNeighbors;
	}

	pair<bool, int> findRhombus(vector<vector<int> > board, int x, int y)
	{
		// cout<<x<<" "<<y<<endl;
		vector<pair<int, int> > rhombus;
		int playerVal = -1;
		if(x-1>=0)
		{
			playerVal = board[x-1][y];
		}
		if(y-1>=0)
		{
			if(playerVal == -1)
			{
				playerVal = board[x][y-1];
			}
			else
			{
				if(board[x][y-1]!=playerVal)
					return make_pair(false, -1);
			}
		}
		if(x+1<boardSize)
		{
			if(playerVal == -1)
			{
				playerVal = board[x+1][y];
			}
			else
			{
				if(board[x+1][y]!=playerVal)
					return make_pair(false, -1);
			}
		}
		if(y+1<boardSize)
		{
			if(playerVal == -1)
			{
				playerVal = board[x][y+1];
			}
			else
			{
				if(board[x][y+1]!=playerVal)
					return make_pair(false, -1);
			}
		}
		// cout<<"true, "<<playerVal<<endl;
		return make_pair(true, playerVal);
	}

	vector<pair<int, int > > dfsFriendlyConnections(vector<vector<int> > board, int x, int y)
	{
		pair<int, int> start = make_pair(x,y);
		vector<pair<int, int > > ans; 
		map<pair<int, int> , int> visited;
		stack<pair<int, int> > st;
		vector<pair<int, int> > friendlyNeighbors;
		st.push(start);
		while(!st.empty())
		{
			pair<int, int> top = st.top();
			st.pop();

			if(!visited[top])
			{
				visited[top] = 1;
				ans.push_back(top);
			}
			friendlyNeighbors = friendlyStones(board, top.first, top.second);
			for(int i=0;i<friendlyNeighbors.size();i++)
			{
				if(!visited[friendlyNeighbors[i]])
				{
					st.push(friendlyNeighbors[i]);
				}
			}

		}
		return ans;

	}

	bool libertyFound(vector<vector<int> > board, int x, int y)
	{
		// vector<pair<int, int> > friendlyNeighbors = friendlyStones(board, x, y);
		vector<pair<int, int> > friendlyConnectedComponentMembers =  dfsFriendlyConnections(board, x, y);
		// cout<<"FNCCM Size: "<<friendlyConnectedComponentMembers.size()<<" for "<<x<<" "<<y<<endl;
		for(int i=0;i<friendlyConnectedComponentMembers.size();i++)
		{
			int xCoordinate = friendlyConnectedComponentMembers[i].first;
			int yCoordinate = friendlyConnectedComponentMembers[i].second;
			// cout<<xCoordinate<<" "<<yCoordinate<<" is an FNCCM"<<endl;

			if( (xCoordinate - 1 >=0 && board[xCoordinate-1][yCoordinate]==0) || (xCoordinate + 1 <boardSize && board[xCoordinate+1][yCoordinate]==0) || (yCoordinate - 1 >=0 && board[xCoordinate][yCoordinate-1]==0) || (yCoordinate + 1 <boardSize && board[xCoordinate][yCoordinate+1]==0))
			{
				return true;
			}


		}
		return false;

	}


	pair<bool, vector<pair<int, int> > > totalLibertyForXY(vector<vector<int> > board, int x, int y)
	{
		// vector<pair<int, int> > friendlyNeighbors = friendlyStones(board, x, y);
		vector<pair<int, int> > friendlyConnectedComponentMembers =  dfsFriendlyConnections(board, x, y);
		// cout<<friendlyConnectedComponentMembers.size()<<endl;
		// cout<<"FNCCM Size: "<<friendlyConnectedComponentMembers.size()<<" for "<<x<<" "<<y<<endl;
		vector<pair<int, int> > libertyPositionsForXY;
		bool flag = false;
		int visited[5][5];
		memset(visited, 0, sizeof(visited[0][0])*5*5);
		for(int i=0;i<friendlyConnectedComponentMembers.size();i++)
		{
			int xCoordinate = friendlyConnectedComponentMembers[i].first;
			int yCoordinate = friendlyConnectedComponentMembers[i].second;
			// cout<<xCoordinate<<" "<<yCoordinate<<" is an FNCCM"<<endl;

			if( (xCoordinate - 1 >=0 && !visited[xCoordinate-1][yCoordinate] && board[xCoordinate-1][yCoordinate]==0) )
			{
				libertyPositionsForXY.push_back(make_pair(xCoordinate-1, yCoordinate));
				visited[xCoordinate-1][yCoordinate] = 1;
				flag = true;
			}
			if(xCoordinate + 1 <boardSize && !visited[xCoordinate+1][yCoordinate] && board[xCoordinate+1][yCoordinate]==0)
			{
				libertyPositionsForXY.push_back(make_pair(xCoordinate+1, yCoordinate));
				visited[xCoordinate+1][yCoordinate] = 1;
				flag = true;
			}
			if(yCoordinate - 1 >=0 && !visited[xCoordinate][yCoordinate-1] && board[xCoordinate][yCoordinate-1]==0)
			{
				libertyPositionsForXY.push_back(make_pair(xCoordinate, yCoordinate-1));
				visited[xCoordinate][yCoordinate-1] = 1;
				flag=true;
			}
			if(yCoordinate + 1 <boardSize && !visited[xCoordinate][yCoordinate+1]  && board[xCoordinate][yCoordinate+1]==0)
			{
				libertyPositionsForXY.push_back(make_pair(xCoordinate, yCoordinate+1));
				visited[xCoordinate][yCoordinate+1] = 1;
				flag=true;
			}

		}
		if(flag)
		{
			return make_pair(flag, libertyPositionsForXY);
		}

		return make_pair(false,libertyPositionsForXY);

	}


	bool isKORuleViolated(vector<vector<int> > board, int x, int y, bool isAnyStoneRemoved, vector<vector<int> > previousBoard)
	{
		// cout<<"inside KO violation function"<<endl;
		// cout<<"was stone removed : " << isAnyStoneRemoved<<endl;
		if(board == this->previousBoard && isAnyStoneRemoved)
		{
			cout<<"Violated KO Rule!"<<endl;
			return true;
		}
/*
		cout<<"Previos Board: "<<endl;
			for(int i=0;i<boardSize;i++)
			{
				for(int j=0;j<boardSize;j++)
				{
					if(this->previousBoard[i][j]==0)
						cout<<"  ";
					if(this->previousBoard[i][j]==1)
						cout<<"X ";
					if(this->previousBoard[i][j]==2)
						cout<<"O ";
				}
				cout<<endl;

			}
			cout<<"Curr Board: "<<endl;
			for(int i=0;i<boardSize;i++)
			{
				for(int j=0;j<boardSize;j++)
				{
					if(this->board[i][j]==0)
						cout<<"  ";
					if(this->board[i][j]==1)
						cout<<"X ";
					if(this->board[i][j]==2)
						cout<<"O ";
				}
				cout<<endl;
*/
			// }



		return false;
	}

	pair<vector<vector<int> >, bool> removePossiblyDeadStones(int player, vector<vector<int> > board)
	{
		vector<pair<int, int> > removedStones;
		for (int i = 0; i < boardSize; i++)
		{
			for(int j=0;j<boardSize;j++)
			{
				if(board[i][j]==player && !libertyFound(board, i, j))
				{
					removedStones.push_back(make_pair(i,j));
					// board[i][j] = 0;
				}
				// else if(board[i][j]==player){
				// 	// cout<<"found liberty for "<<i<<" "<<j<<endl;
				// }
			}
		}
		// this->removedStones = removedStones;
		bool isAnyStoneRemoved = removedStones.size()>0 ? true : false;
		if(removedStones.size() == 0) return make_pair(board, isAnyStoneRemoved);
		// this->board = board;
		for(auto& stoneCoordinates:removedStones)
		{
			// cout<<"removing stone: "<<stoneCoordinates.first<<" "<<stoneCoordinates.second<<endl;
			board[stoneCoordinates.first][stoneCoordinates.second] = 0;
		} 
/*

		cout<<"Temp Board after removing stones: "<<endl;
			for(int i=0;i<boardSize;i++)
			{
				for(int j=0;j<boardSize;j++)
				{
					if(board[i][j]==0)
						cout<<"  ";
					if(board[i][j]==1)
						cout<<"X ";
					if(board[i][j]==2)
						cout<<"O ";
				}
				cout<<endl;

		// this->board = board;
			}
			*/
		return make_pair(board,isAnyStoneRemoved) ;

	
}

	bool isPlacementValid(int x, int y, int player, vector<vector<int> > board)
	{

		if(x>=0 && y>=0 && x<5 && y<5)
		{
			// if(this->board[x][y]!=0) return false;
			vector<vector<int> > temporaryBoard(boardSize, vector<int> (boardSize));
			vector<vector<int> > previousBoard(boardSize, vector<int> (boardSize));
			temporaryBoard = board;
			previousBoard = board;
			temporaryBoard[x][y] = player;
			if(libertyFound(temporaryBoard, x, y))
			{
				// cout<<"found liberty for placement at "<<x<<" "<<y<<endl;
				return true;
			}
			else
			{
				// check if any of opponents stone should be removed
				int opponentPlayer = player==1 ? 2 : 1;
				pair< vector<vector<int> >, bool> remStones;
				remStones = removePossiblyDeadStones(opponentPlayer, temporaryBoard);
				temporaryBoard = remStones.first;
				bool isAnyStoneRemoved = remStones.second;
				if(!libertyFound(temporaryBoard, x, y))
				{
					return false;
				}
				if(isKORuleViolated(temporaryBoard, x, y, isAnyStoneRemoved, previousBoard))
				{
					return false;
				}
				this->board = temporaryBoard;
				return true;

			}

		}
		return false;
	}

/**
 * 
 * 
 * 
 */
	bool isPutStoneFine(int x, int y, int player, vector<vector<int> > board)
	{
		bool validity = isPlacementValid(x, y, player, board);
		if(!validity){
			// cout<<"INVALID MOVE"<<endl;
			return false;
		}
		// need to udpate the moveNumber here??, Please check
		
		// cout<<endl;
		return true;
	}


	void visualizeBoard(vector<vector<int> > board)
	{
		for(int i=0;i<50;i++)
		{
			cout<<"- ";
		}
		cout<<endl;
		for(int i=0;i<boardSize;i++)
		{
			for(int j=0;j<boardSize;j++)
			{
				if(board[i][j]==0)
					cout<<"  ";
				if(board[i][j]==1)
					cout<<"X ";
				if(board[i][j]==2)
					cout<<"O ";
			}
				cout<<endl;

		}
		cout<<endl;
		for(int i=0;i<50;i++)
		{
			cout<<"- ";
		}
		cout<<endl;
	}

	vector<pair<int, int> > getAllValidMoves(int player, vector<vector<int> > board)
	{
		vector<pair<int, int> > allPossibleMoves;
		for(int i=0;i<boardSize; i++)
		{
			for(int j=0;j<boardSize;j++)
			{
				if(board[i][j]==0)
				{
					if(isPutStoneFine(i, j , player, board))
					{
						allPossibleMoves.push_back(make_pair(i,j));
					}
				}
			}
		}
		return allPossibleMoves;
	}

	;

	pair<double, pair<int, int> > evaluation(vector<vector<int> > board, int player, int moveNumber)
	{
		// visualizeBoard(board);
		int totalBlackStones=0, totalWhiteStones=0, edgeBlackStones=0, edgeWhiteStones = 0;
		int whiteStonesWithLowLiberty = 0, blackStonesWithLowLiberty=0, cornerBlackStones=0, cornerWhiteStones=0;
		int endangeredLibertyBlack = 0, endangeredLibertyWhite = 0;
		int numberOfDeadWhite = 0, numberOfDeadBlack = 0;
		int whiteRhombus = 0, blackRhombus = 0;
		int whiteKite = 0, blackKite = 0;

		for(int i=0;i<boardSize;i++)
		{
			for(int j=0;j<boardSize;j++)
			{
				

				if(board[i][j]==1)
				{
					// corner stones
					if((i==0 && j==0 )||(i==0 && j==4 )||(i==4 && j==0 )||(i==4 && j==4 ))
					{
						cornerBlackStones++;
					}

					//edge stones
					else if(i==0 || j == 0 || i == 4 || j == 4)
					{
						edgeBlackStones++;
					}


					pair<bool, vector<pair<int, int > > > libertiesForBlack;
					libertiesForBlack = totalLibertyForXY(board, i, j);

					if(libertiesForBlack.first==true)
					{
							blackStonesWithLowLiberty+=libertiesForBlack.second.size();
					}


					// endangered Liberty
					/*
					if(libertiesForBlack.second.size() <= 1)
						endangeredLibertyBlack++;
					*/
					if(blackStonesWithLowLiberty <= 1)
						endangeredLibertyBlack++;


					totalBlackStones++;

					if(originalBoard[i][j]==2)
						numberOfDeadWhite++;
				}
				else if(board[i][j]==2)
				{
					if((i==0 && j==0 )||(i==0 && j==4 )||(i==4 && j==0 )||(i==4 && j==4 ))
					{
						cornerWhiteStones++;
					}
					else if(i==0 || j == 0 || i == 4 || j == 4)
					{
						edgeBlackStones++;
					}
					pair<bool, vector<pair<int, int > > > libertiesForWhite;
					libertiesForWhite = totalLibertyForXY(board, i, j);
					// cout<<"liberty for "<<i<<" "<<j<<" : "<<libertiesForWhite.first<<" "<<libertiesForWhite.second.size();
					if(libertiesForWhite.first==true)
					{
						// if(libertiesForWhite.second.size()==1)
							whiteStonesWithLowLiberty+=libertiesForWhite.second.size();
					}

					//endangered Liberty

					/*
					if(libertiesForWhite.second.size() <= 1)
						endangeredLibertyWhite++;
					*/
					if(whiteStonesWithLowLiberty <= 1)
						endangeredLibertyWhite++;

					totalWhiteStones++;
					if(originalBoard[i][j]==1)
						numberOfDeadBlack++;
				}
				else
				{
					pair<bool, int> rhombus = findRhombus(board, i, j);
					if(rhombus.first)
					{
						if(rhombus.second == 1)
						{
							blackRhombus ++;
						}
						else
							whiteRhombus++;
					}

					if(originalBoard[i][j] == 1) numberOfDeadBlack++;
					if(originalBoard[i][j] == 2) numberOfDeadWhite++;
						
				}
			}
		}

		// cout<<" white: "<<numberOfDeadWhite<<" black: "<<numberOfDeadBlack<<endl;
		int whiteScore = 0;
		int blackScore = totalBlackStones ;

		whiteScore = totalWhiteStones + 1;
		double eval_value;


		int cornerBlackStonesMultiplier = 5;
		int edgeBlackStonesMultiplier = 2;
		int cornerWhiteStonesMultiplier = 5;
		int edgeWhiteStonesMultiplier = 2;	
		if(moveNumber > 22 )
		{
			cornerBlackStonesMultiplier = 2;
			edgeBlackStonesMultiplier = 1;
			cornerWhiteStonesMultiplier = 2;
			edgeWhiteStonesMultiplier = 1;			
		}

		if(player == 1)
		{
			eval_value = (blackScore + blackStonesWithLowLiberty) - (whiteScore + whiteStonesWithLowLiberty*2) - double(cornerBlackStones) * cornerBlackStonesMultiplier - double(edgeBlackStones) * edgeBlackStonesMultiplier  + 5 * blackRhombus + (numberOfDeadWhite - numberOfDeadBlack) * 8;

			// eval_value = (blackScore + blackStonesWithLowLiberty) - (whiteScore + whiteStonesWithLowLiberty*2) + 10 * endangeredLibertyBlack - 16 * endangeredLibertyWhite - double(cornerBlackStones) * 5 - double(edgeBlackStones) * 2  + 5 * blackRhombus;
			// eval_value = (blackScore + blackStonesWithLowLiberty) - (whiteScore + whiteStonesWithLowLiberty*2) + 10 * endangeredLibertyBlack - 16 * endangeredLibertyWhite - double(cornerBlackStones) * 5 - double(edgeBlackStones) * 2;
			// eval_value = 0.8 * double(double(blackScore) - double(whiteScore)) + 4*(blackStonesWithLowLiberty - whiteStonesWithLowLiberty)  - double(cornerBlackStones) * 0.8 - double(edgeBlackStones) * 0.2 + numberOfDeadWhite * 15 - numberOfDeadBlack * 5;
		}
		else if(player==2) 
		{
			eval_value = (whiteScore + whiteStonesWithLowLiberty) - (blackScore + blackStonesWithLowLiberty*2) - double(cornerWhiteStones) * cornerWhiteStonesMultiplier - double(edgeWhiteStones) * edgeWhiteStonesMultiplier + 5 * whiteRhombus + (numberOfDeadBlack - numberOfDeadWhite) * 8;
			
			// eval_value = 0.8 * double(double(whiteScore - blackScore)) + 4*(whiteStonesWithLowLiberty - blackStonesWithLowLiberty)  - double(cornerWhiteStones) * 0.8 - double(edgeWhiteStones) * 0.2 +  numberOfDeadBlack * 15 - numberOfDeadBlack*5;
		}
		pair<int, int> p;
		// cout<<eval_value<<" " <<blackRhombus<<" "<<whiteRhombus<<endl;
		// if(eval_value==-43)
		// cout<<"eval: "<<eval_value<<endl;
		// visualizeBoard(board);
		if(originalPlayer != player) eval_value *= -1;
		return make_pair(eval_value, p);

	}


	pair<double, pair<int, int> > minPlayerMove(vector<vector<int> > board, int player, int depth, double alphaValue, double betaValue, int moveNumber)
	{
		if(depth==0)
		{
			return evaluation(board, player, moveNumber);
		}
		vector<pair<int, int> > allValidMoves = getAllValidMoves(player, board);

		// if(depth==4){
		// cout<<"all valid moves for player: "<<player<<" at depth: "<<depth<<endl;
		// for (int i = 0; i < allValidMoves.size(); i++)
		// {
		// 	cout<<allValidMoves[i].first<<" "<<allValidMoves[i].second<<endl;
		// }
		// cout<<endl;
		// }	

		if(!allValidMoves.size()){
			// cout<<"PASS"<<endl;
			return make_pair(INT_MIN,make_pair(INT_MIN,INT_MIN));
		}

		double minScore = INT_MAX;
		pair<int, int> maxScoreForMinMove;
		for(int i=0;i<allValidMoves.size();i++)
		{
				vector<vector<int> > temporaryBoard(boardSize, vector<int> (boardSize));
				temporaryBoard = board;
				// this->previousBoard = this->board;
				temporaryBoard[allValidMoves[i].first][allValidMoves[i].second] = player;
				pair< vector<vector<int> >, bool> remStones;
				remStones = removePossiblyDeadStones(3-player, temporaryBoard);
				if(remStones.second)
					temporaryBoard = remStones.first;
				pair<double, pair<int, int> > maxPlayerScore = maxPlayerMove(temporaryBoard, 3-player, depth-1, alphaValue, betaValue, moveNumber+1);



				// visualizeBoard();


				if(minScore > maxPlayerScore.first){
					minScore = maxPlayerScore.first;
					maxScoreForMinMove = allValidMoves[i];
				}
				if(minScore < alphaValue)
				{
					return make_pair(minScore, maxScoreForMinMove);
				}
				if(minScore < betaValue)
				{
					betaValue = minScore;
				}
				// continue;
		}
		return make_pair(minScore, maxScoreForMinMove);
		
	}

	pair<double, pair<int, int> > maxPlayerMove(vector<vector<int> > board, int player, int depth, double alphaValue, double betaValue, int moveNumber)
	{
		if(depth==0)
		{
			return evaluation(board, player, moveNumber);
		}
		vector<pair<int, int> > allValidMoves = getAllValidMoves(player, board);

		// if(depth==4){
		// cout<<"all valid moves for player: "<<player<<" at depth: "<<depth<<endl;
		// for (int i = 0; i < allValidMoves.size(); i++)
		// {
		// 	cout<<allValidMoves[i].first<<" "<<allValidMoves[i].second<<endl;
		// }
		// cout<<endl;
		// }	

		if(!allValidMoves.size())
			return make_pair(INT_MIN,make_pair(INT_MIN,INT_MIN));

		double maxScore = INT_MIN;
		pair<int, int> maxScoreMove;
		for(int i=0;i<allValidMoves.size();i++)
		{
				vector<vector<int> > temporaryBoard(boardSize, vector<int> (boardSize));
				temporaryBoard = board;
				// this->previousBoard = this->board;
				temporaryBoard[allValidMoves[i].first][allValidMoves[i].second] = player;
				pair< vector<vector<int> >, bool> remStones;
				remStones = removePossiblyDeadStones(3-player, temporaryBoard);
				if(remStones.second)
					temporaryBoard = remStones.first;
				// if(depth==4)
				// 	cout<<"removed stones for player: "<<3-player<<" at depth: "<<depth<<" : "<<remStones.second<<endl;
				pair<double, pair<int, int> > minPlayerScore = minPlayerMove(temporaryBoard, 3-player, depth-1, alphaValue, betaValue, moveNumber+1);

				// if(depth==4)
				// cout<<"opponent player would place "<<3-player<<" at "<<minPlayerScore.second.first<<" "<<minPlayerScore.second.second<<" if we place "<<player<<" at "<<allValidMoves[i].first<<" "<<allValidMoves[i].second<<" with score: "<<minPlayerScore.first<<endl;


				// visualizeBoard();


				if(maxScore < minPlayerScore.first){
					maxScore = minPlayerScore.first;
					maxScoreMove = allValidMoves[i];

				}


				if(maxScore > betaValue)
				{
					return make_pair(maxScore, maxScoreMove);
				}
				if(maxScore > alphaValue)
				{
					alphaValue = maxScore;
				}
				// continue;
		}
		return make_pair(maxScore, maxScoreMove);
	}


	

};


int main()
{
	GameGo instanceOfGo;

	// fstream file;
	// file.open("input.txt", ios::in);
	// if(file.is_open())
	// {

	// }
	ifstream file("input.txt");
	string line, playerString, previousStateEncoded = "", currentStateEncoded="";
	int count = 0;
	while(getline(file, line))
	{
		if(count == 0)
		{
			playerString = line;
		}
		else if(count >=1 && count <= 5)
		{
			previousStateEncoded+=line;
		}
		else
		{
			currentStateEncoded += line;
		}
		count++;
	}

	int player = stoi(playerString);
	vector< vector<int > > previousBoardState(boardSize, vector<int> (boardSize));
	vector< vector<int > > currentBoardState(boardSize, vector<int> (boardSize));
	

	originalPlayer = player;

	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			previousBoardState[i][j] = previousStateEncoded[i*5+j] - '0';
			currentBoardState[i][j] = currentStateEncoded[i*5 + j] - '0';
		}
	}
	/*
	cout<<player<<endl;
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			cout<<previousBoardState[i][j]<<" ";
		}
		cout<<endl;
	}

	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			cout<<currentBoardState[i][j]<<" ";
		}
		cout<<endl;
	}
	*/


	int depth = 4;
	pair<int, pair<int, int> > ans;
	instanceOfGo.board = currentBoardState;
	originalBoard = instanceOfGo.board;
	instanceOfGo.previousBoard = previousBoardState;



	/*** Trying to add the moveNumber File ***/
	int moveNumber;
	int countZero = 0;
	for(int i=0;i<boardSize;i++)
	{
		for(int j=0;j<boardSize;j++)
		{
			if(currentBoardState[i][j] == 0)
				countZero++;
		}
	}
	if(countZero == 25)
	{
		moveNumber = 1;
		ofstream moveFile;
		moveFile.open("moveNumberFile.txt");
		moveFile<<to_string(moveNumber);
		moveFile.close();
	}
	else if(countZero == 24)
	{
		moveNumber = 2;
		ofstream moveFile;
		moveFile.open("moveNumberFile.txt");
		moveFile<<to_string(moveNumber);
		moveFile.close();
	}
	else
	{
		ifstream moveNumInputFile("moveNumberFile.txt");
		string moveNumString;
		while(getline(moveNumInputFile, line))
		{
			moveNumString = line;
		}
		moveNumber = stoi(moveNumString);

	}

	if(moveNumber >= 22) 
		depth = 25 - moveNumber;

	// int depth = 5;
	double alphaValue = INT_MIN;
	double betaValue = INT_MAX;
	ans = instanceOfGo.maxPlayerMove(instanceOfGo.board, player, depth, alphaValue, betaValue, moveNumber);




	ofstream moveFile;
	moveFile.open("moveNumberFile.txt");
	moveFile<<to_string(moveNumber + 2);
	moveFile.close();

	ofstream oFile;
	oFile.open("output.txt");
	if(ans.first==INT_MIN){
		oFile<<"PASS";
	}
	else
	{
		oFile<<ans.second.first<<","<<ans.second.second;
	}
	oFile.close();
	return 0;
	
	// cout<<ans.first<<" ("<<ans.second.first<<" "<<ans.second.second<<endl;
	// freopen('input.txt', "r");
	// string playerString;
	// getline(cin, playerString);
	// cout<<playerString<<endl;
	// for(int i=0;i<24;i++)
	// {
	// 	if(i % 2 ==0 )
	// 	{
	// 		cout<<"Black's turn"<<endl;
	// 		int x,y;
	// 		cin>>x>>y;
	// 		if(instanceOfGo.isPutStoneFine(x,y,1))
	// 		{
	// 			instanceOfGo.previousBoard = instanceOfGo.board;
	// 			instanceOfGo.board[x][y] = 1;
	// 			pair< vector<vector<int> >, bool> remStones;
	// 			remStones = instanceOfGo.removePossiblyDeadStones(2, instanceOfGo.board);
	// 			instanceOfGo.board = remStones.first;
	// 			instanceOfGo.visualizeBoard();
	// 			continue;
	// 		}
	// 		else{
	// 			cout<<"invalid"<<endl;
	// 			return 0;
	// 		}
			
	// 	}
	// 	else
	// 	{
	// 		cout<<"White's turn"<<endl;
	// 		int x,y;
	// 		cin>>x>>y;
	// 		if(instanceOfGo.isPutStoneFine(x,y,2))
	// 		{
	// 			instanceOfGo.previousBoard = instanceOfGo.board;
	// 			instanceOfGo.board[x][y] = 2;
	// 			pair< vector<vector<int> >, bool> remStones;
	// 			remStones = instanceOfGo.removePossiblyDeadStones(1, instanceOfGo.board);
	// 			instanceOfGo.board = remStones.first;
	// 			instanceOfGo.visualizeBoard();
	// 			continue;
	// 		}
	// 		else{
	// 			cout<<"invalid"<<endl;
	// 			return 0;
	// 		}

	// 	}
	// 	instanceOfGo.moveNumber++;
	// 	cout<<"mn :"<<instanceOfGo.moveNumber<<endl;
}



