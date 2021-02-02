#include<iostream>
#include<string>
#include<cmath>
#include<ctime>
#include<algorithm>

using namespace std;

class Sudoku {
    private:
    int grid[9][9];
    int solnGrid[9][9];
    int guessNum[9];

    public:
    Sudoku();
    void createSeed();
    void printGrid();
    bool solveGrid();
    void countSolution(int &number);
    void genPuzzle();
    void removeDigits(int K);
};

string getGrid(int grid[9][9]) {
    string s="";
    for(int row=0; row<9; row++) {
        for(int col=0; col<9; col++) {
            s = s + to_string(grid[row][col]);
        }
    }
    return s;
}

int genRandomNumber(int maxLimit) {
    return rand()%maxLimit + 1;
}

void Sudoku :: createSeed() {
    this->solveGrid();

    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            this->solnGrid[i][j] = this->grid[i][j];
        }
    } 
}

Sudoku :: Sudoku() {

    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            this->guessNum[i] = i+1;
        }
    }
    
    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            this->grid[i][j] = 0;
        }
    }
}

void Sudoku :: printGrid() {
    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            if(grid[i][j] == 0) {
                cout << ".";
            }
            else {
                cout << grid[i][j];
            }
            cout << "|";
        }
        cout << endl;
    }
}

bool FindUnassignedLocations(int grid[9][9], int &row, int &col) {
    for(row=0; row<9; row++) {
        for(col=0; col<9; col++) {
            if(grid[row][col] == 0){
                return true;
            }
        }
    }
    return false;
}

bool UsedinRow(int grid[9][9], int row, int num) {
    for(int col=0; col<9; col++) {
        if(grid[row][col] == num) {
            return true;
        }
    }


    return false;
}

bool UsedinCol(int grid[9][9], int col, int num) {
    for(int row=0; row<9; row++) {
        if(grid[row][col] == num) {
            return true;
        }
    }
    return false;
}

bool UsedinBox(int grid[9][9], int rowStart, int colStart, int num) {
    for(int row=0; row<3; row++) {
        for(int col=0; col<3; col++) {
            if(grid[row+rowStart][col+colStart] == num) {
                return true;
            }
        }
    }
    return false;
}

bool isSafe(int grid[9][9], int row, int col, int num) {
    return !UsedinRow(grid, row, num) && !UsedinCol(grid, col, num) && !UsedinBox(grid, row-row%3, col-col%3, num);
}

bool Sudoku :: solveGrid() {
    int row, col;
    if(!FindUnassignedLocations(this->grid, row, col)) {
        return true;
    }

    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            int num = genRandomNumber(9);
            if(isSafe(this->grid, i, j, num)) {
                this->grid[i][j] = num;
                if(solveGrid()) {
                    return true;
                }
                this->grid[i][j] = 0;
            }
        }
    }
/*
    for(int num=0; num<9; num++) {
        if(isSafe(this->grid, row, col, this->guessNum[num])) {
            this->grid[row][col] = this->guessNum[num];
            if(solveGrid()) {
                return true;
            }
            this->grid[row][col] = 0;
        }
    } */
    return false;
}

void Sudoku :: countSolution(int &number) {
    int row, col;
    if(!FindUnassignedLocations(this->grid, row, col)) {
        number++;
        return;
    }

    while(number<2) {
        int num = genRandomNumber(9);
        if(isSafe(this->grid, row, col, num)) {
            this->grid[row][col] = num;
            countSolution(number);
        }
        this->grid[row][col] = 0;
    }

    /*for(int i=0; i<9 && number<2; i++) {
        if(isSafe(this->grid, row, col, this->guessNum[i])) {
            this->grid[row][col] = this->guessNum[i];
            countSolution(number);
        }

        this->grid[row][col] = 0;
    }*/
}

void Sudoku :: genPuzzle() {
    for(int i=0; i<81; i++) {
        int x = (i/9);
        int y = (i%9);
        int temp = this->grid[x][y];
        this->grid[x][y] = 0;

        int check=0;
        countSolution(check);
        if(check!=1) {
            this->grid[x][y] = temp;
        }
    }
}

void Sudoku :: removeDigits(int K) {
    int count = K;
    while (count != 0) {
        int cellNumber = genRandomNumber(81);

        int i = (cellNumber/9);
        int j = (cellNumber%9);
        if(j != 0) {
            j = j - 1;
        }
        if(this->grid[i][j] != 0) {
            count--;
            this->grid[i][j] = 0;
        }    
    }
}

int main(int argc, char const *argv[]) {
    srand(time(0));

    int difficultyLevel;
    cout << "Plese enter the number of blank boxes: ";
    cin >> difficultyLevel;

    Sudoku *puzzle = new Sudoku();

    puzzle->createSeed();

    puzzle->genPuzzle();

    puzzle->printGrid();

    puzzle->removeDigits(difficultyLevel);

    puzzle->printGrid();

    delete puzzle;

    return 0;
}