#include<iostream>
#include<GL/glut.h>

#define ROWS 40
#define COLUMNS 40

extern std::vector<int> x_radar;
extern std::vector<int> y_radar;
extern std::vector<int> xy_grid;
extern std::vector<double> x_path;
extern std::vector<double> y_path;

extern std::vector<double> x1_path;
extern std::vector<double> y1_path;

extern std::vector<double> x2_path;
extern std::vector<double> y2_path;

extern std::vector<double> x3_path;
extern std::vector<double> y3_path;

extern std::vector<double> x4_path;
extern std::vector<double> y4_path;

void unit(int x, int y);
void drawGrid();
void init();
void display_callback();
void reshape_callback(int w, int h);
