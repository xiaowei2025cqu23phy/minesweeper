#include <bits/stdc++.h>
using namespace std;

struct Game {
    int rows, cols, mines;
    vector<vector<int>> board; // -1 mine, >=0 number
    vector<vector<bool>> revealed;
    vector<vector<bool>> flagged;
    int revealedCount = 0;
    bool gameOver = false;
    bool gameWon = false;
    chrono::steady_clock::time_point startTime;

    Game(int r=9,int c=9,int m=10):rows(r),cols(c),mines(m){
        reset();
    }

    void reset(){
        board.assign(rows, vector<int>(cols, 0));
        revealed.assign(rows, vector<bool>(cols,false));
        flagged.assign(rows, vector<bool>(cols,false));
        revealedCount = 0;
        gameOver = false;
        gameWon = false;
        startTime = chrono::steady_clock::now();
        placeMines();
        calcNumbers();
    }

    void placeMines(){
        vector<int> cells(rows*cols);
        iota(cells.begin(), cells.end(), 0);
        random_device rd; mt19937 g(rd());
        shuffle(cells.begin(), cells.end(), g);
        for(int i=0;i<mines && i<(int)cells.size();++i){
            int id = cells[i];
            int r = id / cols;
            int c = id % cols;
            board[r][c] = -1;
        }
    }

    void calcNumbers(){
        for(int r=0;r<rows;++r) for(int c=0;c<cols;++c){
            if(board[r][c]==-1) continue;
            int cnt=0;
            for(int dr=-1;dr<=1;++dr) for(int dc=-1;dc<=1;++dc){
                int nr=r+dr, nc=c+dc;
                if(nr>=0 && nr<rows && nc>=0 && nc<cols && board[nr][nc]==-1) cnt++;
            }
            board[r][c]=cnt;
        }
    }

    void reveal(int r,int c){
        if(gameOver || !inBounds(r,c) || revealed[r][c] || flagged[r][c]) return;
        revealed[r][c]=true;
        revealedCount++;
        if(board[r][c]==-1){ gameOver=true; revealAllMines(); return; }
        if(board[r][c]==0) floodFill(r,c);
        checkWin();
    }

    void floodFill(int r,int c){
        for(int dr=-1;dr<=1;++dr) for(int dc=-1;dc<=1;++dc){
            int nr=r+dr, nc=c+dc;
            if(inBounds(nr,nc) && !revealed[nr][nc] && !flagged[nr][nc]){
                revealed[nr][nc]=true; revealedCount++;
                if(board[nr][nc]==0) floodFill(nr,nc);
            }
        }
    }

    void toggleFlag(int r,int c){
        if(gameOver || !inBounds(r,c) || revealed[r][c]) return;
        flagged[r][c]=!flagged[r][c];
    }

    void revealAllMines(){
        for(int r=0;r<rows;++r) for(int c=0;c<cols;++c) if(board[r][c]==-1) revealed[r][c]=true;
    }

    bool inBounds(int r,int c){ return r>=0 && r<rows && c>=0 && c<cols; }

    void checkWin(){
        int total = rows*cols;
        if(revealedCount == total - mines){ gameWon=true; gameOver=true; }
    }

    void draw(){
        // clear screen
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
        cout<<"Minesweeper ("<<rows<<"x"<<cols<<", mines="<<mines<<")\n";
        auto now = chrono::steady_clock::now();
        auto sec = chrono::duration_cast<chrono::seconds>(now - startTime).count();
        cout<<"Time: "<<sec<<"s\n";
        cout<<"Commands: r row col (reveal), f row col (flag), restart, quit\n";
        cout<<"   ";
        for(int c=0;c<cols;++c) cout<<c%10<<" "; cout<<"\n";
        for(int r=0;r<rows;++r){
            cout<<r%10<<": ";
            for(int c=0;c<cols;++c){
                if(revealed[r][c]){
                    if(board[r][c]==-1) cout<<"* ";
                    else if(board[r][c]==0) cout<<"  ";
                    else cout<<board[r][c]<<" ";
                }else if(flagged[r][c]) cout<<"F ";
                else cout<<"# ";
            }
            cout<<"\n";
        }
        if(gameOver){
            if(gameWon) cout<<"You Win! Press restart to play again.\n";
            else cout<<"Game Over! You hit a mine. Press restart to try again.\n";
        }
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Game g(9,9,10);
    string cmd;
    while(true){
        g.draw();
        cout<<"> ";
        if(!(cin>>cmd)) break;
        if(cmd=="quit" || cmd=="q") break;
        else if(cmd=="restart") { g.reset(); continue; }
        else if(cmd=="r"){
            int x,y; if(!(cin>>x>>y)) break; g.reveal(x,y);
        } else if(cmd=="f"){
            int x,y; if(!(cin>>x>>y)) break; g.toggleFlag(x,y);
        } else {
            cout<<"Unknown command\n";
            string rest; getline(cin, rest);
        }
    }
    return 0;
}
