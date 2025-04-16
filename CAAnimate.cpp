/*
    Author: Francisco Arenas

    Project takes heavy inspiration from Wolfram Alpha's Grayscale CA concepts and implementation
    https://www.wolframscience.com/nks/p158--continuous-cellular-automata/

*/


#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "Empirical/include/emp/math/Random.hpp"
#include <cmath>

emp::web::Document doc{"target"};
emp::Random random_generator(1);

class CAAnimator : public emp::web::Animate {

// CA grid width and height
const int num_h_boxes = 60;
const int num_w_boxes = 80;
const double RECT_SIDE = 10;
const double width{num_w_boxes * RECT_SIDE};
const double height{num_h_boxes * RECT_SIDE};

int StepCounter = 0;


// Some vectors to hold information about the CA
std::vector<std::vector<float> > cells;
std::vector<std::vector<float> > next_cells;


// Canvas to draw on
emp::web::Canvas canvas{width, height, "canvas"};

public:

    CAAnimator() {
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        //fill the vectors with 0 to start
        cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));
        next_cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));

        Set180Kickback();
        DrawCells();
    }

    /*
        Input: None
        Output: None
        Purpose: Updates canvas grid with every step and adds step counter to the bottom right.
    */
    void DoFrame() override {
        if (StepCounter > 0){
            canvas.Clear();
            UpdateCells();
        }

        emp::Point pos(width - 30, height - 30);
        canvas.Text(pos, std::to_string(StepCounter), "green");
        StepCounter++;
    }

    /*
        Input: emp::Random type
        Output: None
        Purpose: Sets a defined number of randomly located living cells onto the grid
    */
    void SetRandomLiveCells(emp::Random random) {
        int num_of_starting_cells = 40;

        for (int i = 0; i < num_of_starting_cells; i++){
            int random_x = random.GetInt(0, (num_w_boxes - 1));
            int random_y = random.GetInt(0, (num_h_boxes - 1));

            cells[random_x][random_y] = 1;
        }
    }

    /*
        Input: None
        Output: None
        Purpose: Sets initial grid of live cells to start the 180-degree Kickback pattern acquired from Stephen Silver's Life Lexicon
                https://conwaylife.com/ref/lexicon/lex_home.htm
    */
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

    /*
        Input: None
        Output: None
        Purpose: Sets single cell around center of grid to live state
    */
    void SetOneCell() {
        cells[20][20] = 1.0f;
    }

    /*
        Input: None
        Output: None
        Purpose: Draws "cells" (Canvas Rectangles) onto grid where color depicts state of life. 
                 Utilizes ColorHSV to showcase continous values
    */
    void DrawCells() {
        for (int x = 0; x < num_w_boxes; x++){
            for (int y = 0; y < num_h_boxes; y++) {

               if (cells[x][y] > 0) {
                   //Draw a rectangle on the canvas filled with varying hue and outlined in black
                   canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, emp::ColorHSV(0, 0, cells[x][y]));
               } else {
                   //Draw a rectangle filled with white and outlined in black
                   canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "white", "black");
               }
               
           }
       }
    }   

    /*
        Input: None
        Output: None
        Purpose: Updates cells value by taking average grey-scale of neighborhood, adding a constant to average
                 and taking fractional part of result as influenced by Wolfram Alpha
    */
    void UpdateCells() {
        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                float avg = GetNeighborhoodAverage(x, y);

                if (avg > 0){
                    float new_value = fmodf(avg + 0.05f, 1.0f);
                    next_cells[x][y] = new_value;
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

    /*
        Input: Two arguments of type int
        Output: Float sum
        Purpose: Retrieves average gray-scale value cells around an individual cell using the Von Neumann Neighborhood configuration
                 Includes center cell in average.
    */
    float GetNeighborhoodAverage(int x, int y){
        float sum = 0.0f;

        for(int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                int x_neighbor = x + i;
                int y_neighbor = y + j;

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

                sum += cells[x_neighbor][y_neighbor];
                
            }
        }

        return sum / 9.0f;
    }


};

CAAnimator animator;

int main() {
    //Have animator call DoFrame once to start
    animator.Step();
}