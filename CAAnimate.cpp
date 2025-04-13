/*
    Author: Francisco Arenas

    Project imitates The Game of Life cellular automata.
    The current disaplyed pattern is the "180-degree kickback" acquired from Stephen Silver's Life Lexicon
    https://conwaylife.com/ref/lexicon/lex_home.htm
*/


#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "Empirical/include/emp/math/Random.hpp"

emp::web::Document doc{"target"};
emp::Random random_generator(1);

class CAAnimator : public emp::web::Animate {

// CA grid width and height
const int num_h_boxes = 30;
const int num_w_boxes = 40;
const double RECT_SIDE = 20;
const double width{num_w_boxes * RECT_SIDE};
const double height{num_h_boxes * RECT_SIDE};

int StepCounter = 0;


// Some vectors to hold information about the CA
std::vector<std::vector<int> > cells;
std::vector<std::vector<int> > next_cells;
        

// Canvas to draw on
emp::web::Canvas canvas{width, height, "canvas"};

public:

    CAAnimator() {
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        //fill the vectors with 0 to start
        cells.resize(num_w_boxes, std::vector<int>(num_h_boxes, 0));
        next_cells.resize(num_w_boxes, std::vector<int>(num_h_boxes, 0));

        Set180Kickback();
        DrawCells();
    }

    // Draws CA grid and updates each cell according to Game of Life rules
    void DoFrame() override {
        if (StepCounter > 0){
            canvas.Clear();
            UpdateCells();
        }

        canvas.Text(*new emp::Point(width - 30, height - 30), std::to_string(StepCounter), "green");
        StepCounter++;
    }

    // Sets a set number of randomly located living cells onto the grid
    void SetRandomLiveCells(emp::Random random) {
        int num_of_starting_cells = 20;

        for (int i = 0; i < num_of_starting_cells; i++){
            int random_x = random.GetInt(0, (num_w_boxes - 1));
            int random_y = random.GetInt(0, (num_h_boxes - 1));

            cells[random_x][random_y] = 1;
        }
    }

    // Sets the initial grid of live cells to start the 180-degree Kickback pattern
    void Set180Kickback() {
        cells[7][2] = 1;
        cells[6][3] = 1;
        cells[6][4] = 1;
        cells[7][2] = 1;
        cells[7][4] = 1;
        cells[8][4] = 1;

        cells[7][7] = 1;
        cells[8][7] = 1;
        cells[6][8] = 1;
        cells[8][8] = 1;
        cells[8][9] = 1;
    }

    // Function draws cells onto the canvas where pink cells are dead and green are alive
    void DrawCells() {
        for (int x = 0; x < num_w_boxes; x++){
            for (int y = 0; y < num_h_boxes; y++) {

               if (cells[x][y] == 0) {
                   //Draw a rectangle on the canvas filled with pink and outlined in black
                   canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "pink", "black");
                   canvas.CenterText((x * RECT_SIDE) + 10, (y * RECT_SIDE) + 10, std::to_string(x)+","+std::to_string(y), "black", "black");
               } else {
                   //Draw a rectangle filled with green and outlined in black
                   canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "green", "black");
                   canvas.CenterText((x * RECT_SIDE) + 10, (y * RECT_SIDE) + 10, std::to_string(x)+","+std::to_string(y), "black", "black");
               }
               
           }
       }
    }   

    // Function updates individual cell based on Game of Life rules
    void UpdateCells() {
        for (int x = 0; x < num_w_boxes; x++){
            for (int y = 0; y < num_h_boxes; y++) {
                int NumLivingNeighbors = 0;
                NumLivingNeighbors = GetNeighborhoodStatus(x, y);
                if (cells[x][y] == 0) {
                    // Any dead cell with exactly three live neighbors becomes a live cell
                    if (NumLivingNeighbors == 3){
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "green", "black");
                        canvas.CenterText((x * RECT_SIDE) + 10, (y * RECT_SIDE) + 10, std::to_string(x)+","+std::to_string(y), "black", "black");
                        next_cells[x][y] = 1;
                    }
                    // Else, Remains dead
                    else {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "pink", "black");
                        canvas.CenterText((x * RECT_SIDE) + 10, (y * RECT_SIDE) + 10, std::to_string(x)+","+std::to_string(y), "black", "black");
                        next_cells[x][y] = 0;
                    }
                }
                
                else {
                    // Any live cell with fewer than two live neighbors dies
                    if (NumLivingNeighbors < 2){
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "pink", "black");
                        canvas.CenterText((x * RECT_SIDE) + 10, (y * RECT_SIDE) + 10, std::to_string(x)+","+std::to_string(y), "black", "black");
                        next_cells[x][y] = 0;
                    }
                    // Any live cell with two or three live neighbors lives on
                    if (NumLivingNeighbors == 2 || NumLivingNeighbors == 3){
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "green", "black");
                        canvas.CenterText((x * RECT_SIDE) + 10, (y * RECT_SIDE) + 10, std::to_string(x)+","+std::to_string(y), "black", "black");
                        next_cells[x][y] = 1;
                    }
                    // Cell has more than 3 live neighbors and dies
                    else {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "pink", "black");
                        canvas.CenterText((x * RECT_SIDE) + 10, (y * RECT_SIDE) + 10, std::to_string(x)+","+std::to_string(y), "black", "black");
                        next_cells[x][y] = 0;
                    }
               }
               
           }
       }

       for (int x = 0; x < num_w_boxes; x++){
        for (int y = 0; y < num_h_boxes; y++) {
            cells[x][y] = next_cells[x][y];
            }
        }
        DrawCells();
    }

    // Retrieves the total number of "living" cells around an individual cell using the Von Neumann Neighborhood configuration
    int GetNeighborhoodStatus(int x, int y){
        int NumLivingNeighbors = 0;

        for(int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                int x_neighbor = x + i;
                int y_neighbor = y + j;
                if (!(x_neighbor == x && y_neighbor == y)){

                    if (y_neighbor < 0) {
                        y_neighbor = (num_h_boxes - 1);
                    }
                    if (y_neighbor > (num_h_boxes - 1)) {
                        y_neighbor = 0;
                    }
                    if (x_neighbor < 0) {
                        x_neighbor = (num_w_boxes - 1);
                    }
                    if (x_neighbor > (num_w_boxes - 1)){
                        x_neighbor = 0;
                    }
    
                    if (cells[x_neighbor][y_neighbor] == 1){
                        NumLivingNeighbors++;
                    }
                }
            }
        }

        return NumLivingNeighbors;
    }


};

CAAnimator animator;

int main() {
    //Have animator call DoFrame once to start
    animator.Step();
}