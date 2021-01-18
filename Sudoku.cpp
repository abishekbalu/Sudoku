#include<iostream>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;

#define UNASSIGNED = 0

class Sudoku {
    private:
    int grid[9][9];
    int solutionGrid[9][9];
    int gridPosition[9];
    int guessNumber[9];
    int difficultyLevel;
    bool grid_status;

    public:
    Sudoku();
    Sudoku(string, bool row_major = true);
    void CreateSeed();
    void printGrid();
    bool solveGrid();
    string getGrid();
    void countSolution(int &number);
    void genPuzzle();
    bool verifyGridStatus();
    void printSVG(string);
    void calculateDifficultyLevel();
    int branchDifficultyLevel();
};

// Get Grid as a string in row major order

string Sudoku::getGrid() 
{
    string s=" ";
    for(int row_num = 0; row_num<9; ++row_num) {
        for(int col_num=0; col_num<9; ++col_num) {
            s = s + to_string(grid[row_num][col_num]);
        }
    }
    return s;
}

// Generate Random Number

int genRandomNumber(int maxLimit) 
{
    return rand()%maxLimit;
}

// Create Seed Grid
void Sudoku :: CreateSeed() 
{
    this->solveGrid();

    //Saving the Solution Grid

    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            this->solutionGrid[i][j] = this->grid[i][j];
        }
    }
}


//Initializing

Sudoku :: Sudoku()
{
    // Initialize the difficulty level
    this->difficultyLevel = 0;

    // Randomly shuffling the array of removing grid positions
    for(int i=0; i<81; i++) {
        this->gridPosition[i] = i;
    }

    random_shuffle(this->gridPosition, (this->gridPosition) + 81, genRandomNumber);

    //Randomly shuffling the guessing number array

    for(int i=0; i<9; i++) {
        this->guessNumber[i] = i+1;
    }

    random_suffle(this->guessNumber, (this->guessNumber) + 9, genRandomNumber);

    //Initializing the grid

    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            this->grid[i][j] = 0;
        }
    }

    grid_status = true;
}

// Custom Initializing with grid passed as arguments

Sudoku::Sudoku(string grid_str, bool row_major) 
{
    if(grid_str.length() != 81) {
        grid_status = false;
        return;
    }   

    // First Pass: Check if all the cells are valid

    for(int i=0; i<81; ++i) {
        int curr_num = grid_str[i] - '0';
        if(!((curr_num == UNASSIGNED) || (curr_num > 0 && curr_num < 10))) {
            grid_status = false;
            return;
        }

        if(row_major) 
            grid[i/9][i%9] = curr_num;
        else 
            grid[i%9][i/9] = curr_num;
    }

    // Second Pass: Check if all columns are valid

    for(int col_num=0; col_num<9; ++col_num) {
        bool nums[10] = {false};
        for(int row_num=0; row_num<9; ++row_num) {
            int curr_num = grid[row_num][col_num];
            if(curr_num != UNASSIGNED && nums[curr_num] == true) {
                grid_status = false;
                return;
            }
            nums[curr_num] = true;   
        }
    }

    // Third Pass: Check if all rows are valid

    for(int row_num=0; row_num<9; ++row_num) {
        bool nums[10] = {false};
        for(int col_num=0; col_num<9; ++col_num) {
            int curr_num = grid[row_num][col_num];
            if(curr_num != UNASSIGNED && nums[curr_num] == true) {
                grid_status = false;
                return;
            }

            nums[curr_num] = true;
        }
    }

    // Fourth Pass: Check if all blocks are valid

    for(int block_num=0; block_num<9; ++block_num) {
        bool nums[10] = {false};
        for(int cell_num=0; cell_num<9; ++cell_num) {
            int curr_num = grid[((int)(block_num/3)*3) + (cell_num/3)][(int)(block_num%3)*3 + (cell_num%3)];
            if(curr_num != UNASSIGNED && nums[curr_num] == true) {
                grid_status = false;
                return;
            }

            nums[curr_num] = true;
        }
    }

    // Randomly shuffling the guessing number array 

    for(int i=0; i<9; i++) {
        this->guessNumber[i] = i+1;
    } 

    random_shuffle(this->guessNumber, (this->guessNumber) + 9, genRandomNumber);

    grid_status = true;
}

// Verification status of custom grid 

bool Sudoku::verifyGridStatus() 
{
    return grid_status;
}

// Printing the grid

void Sudoku::printGrid()
{
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

    cout << "Difficulty of current Sudoku (o being the easiest): " << this->difficultyLevel;
    cout << endl; 
}


// Helper functions for solving the grid 

bool findUnassignedLocation(int grid[9][9], int &row, int &col) 
{
    for(row=0; row<9; row++) {
        for(col=0; col<9; col++) {
            if(grid[row][col] == UNASSIGNED) {
                return true;
            }
        }
    }

    return false;
}

bool UsedInRow(int grid[9][9], int row, int num) 
{
    for(int col=0; col<9; col++) {
        if(grid[row][col] == num) {
            return true;
        }
    }
    
    return false;
}

bool UsedInCol(int grid[9][9], int col, int num) 
{
    for(int row=0; row<9; row++) {
        if(grid[row][col] == num) {
            return true;
        }
    }

    return false;
}

bool UsedInBox(int grid[9][9], int boxStartRow, int boxStartCol, int num)
{
    for(int row=0; row<3; row++) {
        for(int col=0; col<3; col++) {
            if(grid[row+boxStartCol][col+boxStartCol] == num) {
                return true;
            }
        }
    }

    return false;
}

bool isSafe(int grid[9][9], int row, int col, int num)
{
    return !UsedInRow(grid, row, num) && !UsedInCol(grid, col, num) && !UsedInBox(grid, row-row%3, col-col%3, num);
}

// Solve Grid

bool Sudoku::solveGrid()
{
    int row, col;

    // Check if there are any unassigned locations
    if(!findUnassignedLocation(this->grid, row, col)) {
        // if there are no unassigned locations, the grid is solved. SUCCESS!!!
        return true;
    }

    // Consider digits from 1 to 9
    for(int num=0; num<9; num++) {
        // If looks promising
        if(isSafe(this->grid, row, col, this->guessNumber[num])) {
            // make a tentative assignment
            this->grid[row][col] = this->guessNumber[num];

            // If we are able to solve the grid
            if(solveGrid())
                return true;
            
            // Replace the grid to zero and start once again
            this->grid[row][col] = UNASSIGNED;
        }
    }
    // triggers backtracking
    return false;
}

