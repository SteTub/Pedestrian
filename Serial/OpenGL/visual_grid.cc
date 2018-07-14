#include "Agent.h"

//---global variables for graphic program
std::vector<int> x_radar;
std::vector<int> y_radar;

std::vector<int> xy_grid;

std::vector<double> x_path;
std::vector<double> y_path;
std::vector<double> x1_path;
std::vector<double> y1_path;

std::vector<double> x2_path;
std::vector<double> y2_path;

std::vector<double> x3_path;
std::vector<double> y3_path;

std::vector<double> x4_path;
std::vector<double> y4_path;

void unit(double x, double y)
{
	glColor3f(1.0,1.0,1.0);		//set colour to white
	glLineWidth(1.0);		//set width of line

	glBegin(GL_LINES);		//draw the lines
		glVertex2d(x,y);	glVertex2d(x+1,y);
		glVertex2d(x+1,y);	glVertex2d(x+1,y+1);
		glVertex2d(x+1,y+1);	glVertex2d(x,y+1);
		glVertex2d(x,y+1);	glVertex2d(x,y);
	glEnd();
}

void drawGrid()
{
	for(double x=0;x<COLUMNS;x+=BOX_SIZE)		//make a grid with lines
		for(double y=0;y<ROWS;y+=BOX_SIZE)
			unit(x,y);			//draw the grid points
}

//---COLOUR BOUNDARIES GREY---//
void fillGrid()
{
	glColor3f(0.8,0.8,0.8);	//GREY

	for(int i=0;i<COLUMNS;i++){
		for(int j=0;j<COLUMNS;j++){

			if(xy_grid[j*COLUMNS + i] == 1)		//if position is a boundary/wall
				glRectd(i*BOX_SIZE+2,j*BOX_SIZE+2,i*BOX_SIZE+BOX_SIZE+2,j*BOX_SIZE+BOX_SIZE+2);
		}
	}

}

void init()
{
	glClearColor(0.0,0.0,0.0,0.0);	//clear colours
}

void display_callback()
{
	int const w =glutGet(GLUT_WINDOW_WIDTH);
	int const h =glutGet(GLUT_WINDOW_WIDTH);

	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0,0,(GLsizei)(1.5*w),(GLsizei)(1.5*h));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,COLUMNS,0.0,ROWS,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawGrid();
	fillGrid();

/* ---- PATH ----*/
//draw path taken

	//---AGENTS START POINT---//
	glColor3f(1.0,0.6,0.8);		//set to pink
	glRectd((double)x_path[0]*BOX_SIZE+1.7,(double)y_path[0]*BOX_SIZE+1.7,(double)x_path[0]*BOX_SIZE+BOX_SIZE+1.7,(double)y_path[0]*BOX_SIZE+BOX_SIZE+1.7);

	//---AGENTS ENDPOINT---//
	glColor3f(0.2,0.6,0.8);		//set to blue
	glRectd((double)x_path[x_path.size()-1]*BOX_SIZE+1.7,(double)y_path[y_path.size()-1]*BOX_SIZE+1.7,(double)x_path[x_path.size()-1]*BOX_SIZE+BOX_SIZE+1.7,(double)y_path[y_path.size()-1]*BOX_SIZE+BOX_SIZE+1.7);
	glColor3f(1.0,0,0);		//set to red
	glLineWidth(3.0);
	glBegin(GL_LINES);

	for(int i=1;i<x_path.size();i++){
		glVertex2d(x_path[i-1]*BOX_SIZE+2, y_path[i-1]*BOX_SIZE+2);
		glVertex2d(x_path[i]*BOX_SIZE+2, y_path[i]*BOX_SIZE+2);
	}
	glEnd();

//	drawGrid();

	//---ADD PATHS OF ADDITIONAL PEOPLE---//
/*
	//PERSON 2
	glColor3f(0,1.0,0);		//change colour
	glLineWidth(3.0);
	glBegin(GL_LINES);
	
	for(int i=1;i<x1_path.size();i++){
		glVertex2d(x1_path[i-1]*BOX_SIZE+2, y1_path[i-1]*BOX_SIZE+2);
		glVertex2d(x1_path[i]*BOX_SIZE+2, y1_path[i]*BOX_SIZE+2);
	}
	glEnd();
	//PERSON 3
	glColor3f(0,0,1.0);		//set to blue
	glLineWidth(3.0);
	glBegin(GL_LINES);
	
	for(int i=1;i<x2_path.size();i++){
		glVertex2d(x2_path[i-1]*BOX_SIZE+2, y2_path[i-1]*BOX_SIZE+2);
		glVertex2d(x2_path[i]*BOX_SIZE+2, y2_path[i]*BOX_SIZE+2);
	}
	glEnd();
	//PERSON 4
	glColor3f(0.5,0.5,0);		
	glLineWidth(3.0);
	glBegin(GL_LINES);
	
	for(int i=1;i<x3_path.size();i++){
		glVertex2d(x3_path[i-1]*BOX_SIZE+2, y3_path[i-1]*BOX_SIZE+2);
		glVertex2d(x3_path[i]*BOX_SIZE+2, y3_path[i]*BOX_SIZE+2);
	}
	glEnd();
	//PERSON 5
	glColor3f(0,0.5,0.5);	
	glLineWidth(3.0);
	glBegin(GL_LINES);
	
	for(int i=1;i<x4_path.size();i++){
		glVertex2d(x4_path[i-1]*BOX_SIZE+2, y4_path[i-1]*BOX_SIZE+2);
		glVertex2d(x4_path[i]*BOX_SIZE+2, y4_path[i]*BOX_SIZE+2);
	}
	glEnd();
*/		
		

// ---- RADAR ----
/*
//uncomment to visualise radar scan

	glColor3f(0.2,0.6,0.8);		//set to blue
	
	//fill in radar
	for(int i=1;i<x_radar.size();i++){
		glRectd(((double)x_radar[i]*BOX_SIZE)+2,(double)y_radar[i]*BOX_SIZE+2,(double)x_radar[i]*BOX_SIZE+BOX_SIZE+2,(double)y_radar[i]*BOX_SIZE+BOX_SIZE+2);
		//glRectd(((double)x_radar[i]*BOX_SIZE)+20,(double)y_radar[i]*BOX_SIZE+10,(double)x_radar[i]*BOX_SIZE+20+BOX_SIZE,(double)y_radar[i]*BOX_SIZE+10+BOX_SIZE);
	}
	

	glColor3f(1.0,0.6,0.8);		//set to pink
	
	//fill in agents location
	glRectd((double)x_radar[0]*BOX_SIZE+2,(double)y_radar[0]*BOX_SIZE+2,(double)x_radar[0]*BOX_SIZE+BOX_SIZE+2,(double)y_radar[0]*BOX_SIZE+BOX_SIZE+2);
	//glRectd(((double)x_radar[0]*BOX_SIZE)+20,(double)y_radar[0]*BOX_SIZE+10,(double)x_radar[0]*BOX_SIZE+20+BOX_SIZE,(double)y_radar[0]*BOX_SIZE+10+BOX_SIZE);

	drawGrid();
	
	glColor3f(1.0,0.0,0.0);		//set to red
	glLineWidth(2);		//set width of line
*/
/*
	glBegin(GL_LINES);		//draw the lines
//	glVertex2d(x_radar[0]*BOX_SIZE+(BOX_SIZE/2),y_radar[0]*BOX_SIZE+(BOX_SIZE/2));
//	glVertex2d(x_radar[x_radar.size()-1]*BOX_SIZE+(BOX_SIZE/2),y_radar[y_radar.size()-1]*BOX_SIZE+(BOX_SIZE/2));

	glVertex2d(x_radar[0]*BOX_SIZE,y_radar[0]*BOX_SIZE);
	glVertex2d(x_radar[x_radar.size()-1]*BOX_SIZE,y_radar[y_radar.size()-1]*BOX_SIZE);
	glEnd();
*/
	
	glutSwapBuffers();
}

void reshape_callback(int w, int h)
{
//	glViewport(0,0,(GLsizei)(1.5*w),(GLsizei)(1.5*h));
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0.0,COLUMNS,0.0,ROWS,-1.0,1.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
}
