#include<bits/stdc++.h>
#include<map>
#include<fstream>
using namespace std;


#define KOMI 2.5
#define boardSize 5
#define maxMoves 24
int originalPlayer ;
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
		// cout<<"FNCCM Size: "<<friendlyConnectedComponentMembers.size()<<" for "<<x<<" "<<y<<endl;
		vector<pair<int, int> > libertyPositionsForXY;
		bool flag = false;
		for(int i=0;i<friendlyConnectedComponentMembers.size();i++)
		{
			int xCoordinate = friendlyConnectedComponentMembers[i].first;
			int yCoordinate = friendlyConnectedComponentMembers[i].second;
			// cout<<xCoordinate<<" "<<yCoordinate<<" is an FNCCM"<<endl;

			if( (xCoordinate - 1 >=0 && board[xCoordinate-1][yCoordinate]==0) )
			{
				libertyPositionsForXY.push_back(make_pair(xCoordinate-1, yCoordinate));
				flag = true;
			}
			if(xCoordinate + 1 <boardSize && board[xCoordinate+1][yCoordinate]==0)
			{
				libertyPositionsForXY.push_back(make_pair(xCoordinate+1, yCoordinate));
				flag = true;
			}
			if(yCoordinate - 1 >=0 && board[xCoordinate][yCoordinate-1]==0)
			{
				libertyPositionsForXY.push_back(make_pair(xCoordinate, yCoordinate-1));
				flag=true;
			}
			if(yCoordinate + 1 <boardSize && board[xCoordinate][yCoordinate+1]==0)
			{
				libertyPositionsForXY.push_back(make_pair(xCoordinate, yCoordinate+1));
				flag=true;
			}

		}
		if(flag)
		{
			return make_pair(flag, libertyPositionsForXY);
		}

		return make_pair(false,libertyPositionsForXY);

	}


	bool isKORuleViolated(vector<vector<int> > board, int x, int y, bool isAnyStoneRemoved)
	{
		// cout<<"inside KO violation function"<<endl;
		// cout<<"was stone removed : " << isAnyStoneRemoved<<endl;
		if(this->board == this->previousBoard && isAnyStoneRemoved)
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
				else if(board[i][j]==player){
					// cout<<"found liberty for "<<i<<" "<<j<<endl;
				}
			}
		}
		this->removedStones = removedStones;
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

	bool isPlacementValid(int x, int y, int player)
	{

		if(x>=0 && y>=0 && x<5 && y<5)
		{
			if(this->board[x][y]!=0) return false;
			vector<vector<int> > temporaryBoard(boardSize, vector<int> (boardSize));
			temporaryBoard = this->board;

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
				if(isKORuleViolated(temporaryBoard, x, y, isAnyStoneRemoved))
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
	bool isPutStoneFine(int x, int y, int player)
	{
		bool validity = isPlacementValid(x, y, player);
		if(!validity){
			// cout<<"INVALID MOVE"<<endl;
			return false;
		}
		// need to udpate the moveNumber here??, Please check
		
		// cout<<endl;
		return true;
	}


	void visualizeBoard()
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
				if(this->board[i][j]==0)
					cout<<"  ";
				if(this->board[i][j]==1)
					cout<<"X ";
				if(this->board[i][j]==2)
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
					if(isPutStoneFine(i, j , player))
					{
						allPossibleMoves.push_back(make_pair(i,j));
					}
				}
			}
		}
		return allPossibleMoves;
	}

	;

	pair<int, pair<int, int> > evaluation(vector<vector<int> > board, int player)
	{
		int totalBlackStones=0, totalWhiteStones=0;
		int whiteStonesWithLowLiberty = 0, blackStonesWithLowLiberty=0;
		// int whiteStonesWithNoLiberty = 0, blackStonesWithNoLiberty=0;
		for(int i=0;i<boardSize;i++)
		{
			for(int j=0;j<boardSize;j++)
			{
				if(board[i][j]==1)
				{
					pair<bool, vector<pair<int, int > > > libertiesForBlack;
					libertiesForBlack = totalLibertyForXY(board, i, j);
					if(libertiesForBlack.first==true)
					{
						// if(libertiesForBlack.second.size()==1)
						{
							blackStonesWithLowLiberty+=libertiesForBlack.second.size();
						}

					}
					totalBlackStones++;

				}
				else if(board[i][j]==2)
				{
					pair<bool, vector<pair<int, int > > > libertiesForWhite;
					libertiesForWhite = totalLibertyForXY(board, i, j);
					if(libertiesForWhite.first==true)
					{
						// if(libertiesForWhite.second.size()==1)
						{
							whiteStonesWithLowLiberty+=libertiesForWhite.second.size();
						}

					}
					totalWhiteStones++;
				}
			}
		}


		int whiteScore = 0;
		int blackScore = totalBlackStones + 0.5;
		whiteScore = totalWhiteStones;
		int eval_value;
		if(player == 1)
		{
			eval_value = blackScore - whiteScore - whiteStonesWithLowLiberty + blackStonesWithLowLiberty;
		}
		else if(player==2) 
		{
			eval_value = whiteScore - blackScore - blackStonesWithLowLiberty + whiteStonesWithLowLiberty;
		}
		pair<int, int> p;
		// cout<<"eval: "<<eval_value<<endl;
		if(originalPlayer != player) eval_value *= -1;
		return make_pair(eval_value, p);

	}


	pair<int, pair<int, int> > minPlayerMove(vector<vector<int> > board, int player, int depth, int alphaValue, int betaValue)
	{
		if(depth==0)
		{
			return evaluation(board, player);
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
			cout<<"PASS"<<endl;
			return make_pair(-1,make_pair(-1,-1));
		}

		int minScore = INT_MAX;
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
				pair<int, pair<int, int> > maxPlayerScore = maxPlayerMove(temporaryBoard, 3-player, depth-1, alphaValue, betaValue);



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

	pair<int, pair<int, int> > maxPlayerMove(vector<vector<int> > board, int player, int depth, int alphaValue, int betaValue)
	{
		if(depth==0)
		{
			return evaluation(board, player);
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
			return make_pair(-1,make_pair(-1,-1));

		int maxScore = INT_MIN;
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
				pair<int, pair<int, int> > minPlayerScore = minPlayerMove(temporaryBoard, 3-player, depth-1, alphaValue, betaValue);

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
	pair<int, pair<int, int> > ans;
	instanceOfGo.board = currentBoardState;
	instanceOfGo.previousBoard = previousBoardState;
	int depth = 5;
	ans = instanceOfGo.maxPlayerMove(instanceOfGo.board, player, depth, INT_MIN, INT_MAX);

	ofstream oFile;
	oFile.open("output.txt");
	if(ans.first==-1){
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



