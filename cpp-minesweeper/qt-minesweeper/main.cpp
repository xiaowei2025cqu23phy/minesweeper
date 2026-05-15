#include <QApplication>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

// ---------- Game logic (no Qt dependency) ----------
struct GameLogic {
    int rows, cols, mines;
    vector<vector<int>> board; // -1 mine, >=0 number
    vector<vector<bool>> revealed;
    vector<vector<bool>> flagged;
    int revealedCount = 0;
    bool gameOver = false;
    bool gameWon = false;
    chrono::steady_clock::time_point startTime;

    GameLogic(int r=9,int c=9,int m=10):rows(r),cols(c),mines(m){ reset(); }

    void reset(){
        board.assign(rows, vector<int>(cols, 0));
        revealed.assign(rows, vector<bool>(cols,false));
        flagged.assign(rows, vector<bool>(cols,false));
        revealedCount = 0; gameOver=false; gameWon=false;
        startTime = chrono::steady_clock::now();
        placeMines(); calcNumbers();
    }

    void placeMines(){
        vector<int> cells(rows*cols);
        iota(cells.begin(), cells.end(), 0);
        random_device rd; mt19937 g(rd());
        shuffle(cells.begin(), cells.end(), g);
        for(int i=0;i<mines && i<(int)cells.size();++i){
            int id=cells[i]; int r=id/cols, c=id%cols; board[r][c]=-1;
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

    bool inBounds(int r,int c) const { return r>=0 && r<rows && c>=0 && c<cols; }

    void floodFill(int r,int c){
        for(int dr=-1;dr<=1;++dr) for(int dc=-1;dc<=1;++dc){
            int nr=r+dr, nc=c+dc;
            if(inBounds(nr,nc) && !revealed[nr][nc] && !flagged[nr][nc]){
                revealed[nr][nc]=true; revealedCount++;
                if(board[nr][nc]==0) floodFill(nr,nc);
            }
        }
    }

    void reveal(int r,int c){
        if(gameOver || !inBounds(r,c) || revealed[r][c] || flagged[r][c]) return;
        revealed[r][c]=true; revealedCount++;
        if(board[r][c]==-1){ gameOver=true; revealAllMines(); return; }
        if(board[r][c]==0) floodFill(r,c);
        checkWin();
    }

    void toggleFlag(int r,int c){ if(gameOver || !inBounds(r,c) || revealed[r][c]) return; flagged[r][c]=!flagged[r][c]; }
    void revealAllMines(){ for(int r=0;r<rows;++r) for(int c=0;c<cols;++c) if(board[r][c]==-1) revealed[r][c]=true; }
    void checkWin(){ int total=rows*cols; if(revealedCount==total-mines){ gameWon=true; gameOver=true; } }
};

// ---------- Qt GUI (no Q_OBJECT — no moc needed) ----------
class MinesweeperWidget : public QWidget {
public:
    MinesweeperWidget(int rows=9, int cols=9, int mines=10, QWidget* parent=nullptr)
        : QWidget(parent), logic(rows, cols, mines)
    {
        QVBoxLayout *vl = new QVBoxLayout(this);
        QHBoxLayout *top = new QHBoxLayout();
        timerLabel = new QLabel("Time: 0s");
        restartBtn = new QPushButton("Restart");
        top->addWidget(timerLabel);
        top->addStretch();
        top->addWidget(restartBtn);
        vl->addLayout(top);

        grid = new QGridLayout();
        grid->setSpacing(2);
        buttons.resize(logic.rows, vector<QPushButton*>(logic.cols, nullptr));
        for(int i=0;i<logic.rows;++i){
            for(int j=0;j<logic.cols;++j){
                QPushButton* b = new QPushButton(this);
                b->setFixedSize(32,32);
                b->setProperty("row", i);
                b->setProperty("col", j);
                buttons[i][j] = b;
                grid->addWidget(b,i,j);
                b->installEventFilter(this);
                connect(b, &QPushButton::clicked, this, [this, b](){
                    int r = b->property("row").toInt();
                    int c = b->property("col").toInt();
                    logic.reveal(r,c);
                    refresh();
                    if(logic.gameOver){
                        QMessageBox::information(this, "Result",
                            logic.gameWon ? "You Win!" : "Game Over!");
                    }
                });
            }
        }
        vl->addLayout(grid);

        connect(restartBtn, &QPushButton::clicked, this, [this](){ logic.reset(); refresh(); });
        startTimer(1000);
        refresh();
    }

protected:
    void timerEvent(QTimerEvent*) override {
        auto now = chrono::steady_clock::now();
        auto sec = chrono::duration_cast<chrono::seconds>(now - logic.startTime).count();
        timerLabel->setText(QString("Time: %1s").arg((long long)sec));
    }

    bool eventFilter(QObject* obj, QEvent* ev) override {
        QPushButton* btn = qobject_cast<QPushButton*>(obj);
        if(btn && ev->type() == QEvent::MouseButtonPress) {
            QMouseEvent* me = static_cast<QMouseEvent*>(ev);
            if(me->button() == Qt::RightButton) {
                int r = btn->property("row").toInt();
                int c = btn->property("col").toInt();
                logic.toggleFlag(r,c);
                refresh();
                return true;
            }
        }
        return QWidget::eventFilter(obj, ev);
    }

private:
    GameLogic logic;
    QGridLayout *grid;
    vector<vector<QPushButton*>> buttons;
    QLabel *timerLabel;
    QPushButton *restartBtn;

    void refresh(){
        for(int i=0;i<logic.rows;++i){
            for(int j=0;j<logic.cols;++j){
                QPushButton* b = buttons[i][j];
                if(logic.revealed[i][j]){
                    if(logic.board[i][j]==-1){ b->setText("*"); b->setStyleSheet("background: #ff8080"); }
                    else if(logic.board[i][j]==0){ b->setText(""); b->setStyleSheet("background: #ddd"); }
                    else { b->setText(QString::number(logic.board[i][j])); b->setStyleSheet("background: #ddd; font-weight:bold"); }
                    b->setEnabled(false);
                }else{
                    b->setEnabled(true);
                    if(logic.flagged[i][j]){ b->setText("F"); b->setStyleSheet("background: #f0e68c"); }
                    else { b->setText(""); b->setStyleSheet(""); }
                }
            }
        }
    }
};

int main(int argc, char** argv){
    QApplication app(argc, argv);
    MinesweeperWidget w(9,9,10);
    w.setWindowTitle("Minesweeper (Qt)");
    w.show();
    return app.exec();
}
