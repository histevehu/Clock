#include "iostream"
#include "time.h"
#include "windows.h"
#include "GL/glut.h"

/*Color Index*/
#define INDEX_BLACK 1
#define INDEX_WHITE 2
#define INDEX_GREY 3
#define INDEX_RED 4
#define INDEX_ORANGE 5
#define INDEX_YELLOW 6
#define INDEX_GREEN 7
#define INDEX_CYAN 8
#define INDEX_BLUE 9
#define INDEX_PURPLE 10

/*Color Config*/
#define BLACK 0,0,0
#define WHITE 1,1,1
#define LIGHTGREY 0.7, 0.7, 0.7
#define GREY 0.5,0.5,0.5
#define RED 1,0,0
#define ORANGE 1,0.5,0.2
#define YELLOW 1,1,0
#define GREEN 0,1,0
#define CYAN 0,1,1
#define BLUE 0,0,1
#define PURPLE 0.5,0.5,1

/*OpenGL Draw Size*/
#define SIZE_WINDOW 500
#define RADIUS_CLOCK 0.9
#define RADIUS_PLATE_DOT 0.03

#define LEN_HOUR 0.4
#define LEN_MIN 0.7
#define LEN_SEC 0.9

#define WIDTH_HOUR 8.0f
#define WIDTH_MIN 6.0f
#define WIDTH_SEC 2.0f

/*OpenGL Draw Color*/
#define COLOR_BACKGROUND 0, 0, 0
#define COLOR_PLATE_DOT 0.5, 0.5, 0.5

#define COLOR_INITIAL_HOUR WHITE
#define COLOR_INITIAL_MIN LIGHTGREY
#define COLOR_INITIAL_SEC ORANGE

/*OpenGL Option*/
#define TIME_DRAW_PLATE_DOT 100
#define AA 1
#define DIS_AA 2
#define EXIT 3

#define HAND_HOUR 1
#define HAND_MIN 2
#define HAND_SEC 3

/*Const*/
#define PI 3.1416

using namespace std;
int radius = 100;
bool useAA = true;
int FPS = 120;
int menu_color_hour, menu_color_bg, menu_fps, menu_main;
SYSTEMTIME sys;

struct Color {
	double R, G, B, Alp;
	Color(double iR, double iG, double iB, double iAlp = 1) :R(iR), G(iG), B(iB), Alp(iAlp) {}
	void set(double iR, double iG, double iB, double iAlp = 1)
	{
		R = iR, G = iG, B = iB, Alp = iAlp;
	}
}COLOR_HOUR(COLOR_INITIAL_HOUR), COLOR_BG(COLOR_BACKGROUND);

void enableAA() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
}
void disableAA() {
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
}
void GLInit(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(SIZE_WINDOW, SIZE_WINDOW);
	glutCreateWindow("CG_CLOCK(OPENGL)");
	glClearColor(COLOR_BG.R, COLOR_BG.G, COLOR_BG.B, COLOR_BG.Alp);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	enableAA();
	glutSwapBuffers();
}
void drawLine(float width, double x1, double y1, double x2 = 0, double y2 = 0) {
	glLineWidth(width);
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}
void drawDot(double x, double y) {
	glBegin(GL_POINTS);
	glLineWidth(5.0f);
	glVertex2f(x, y);
	glEnd();
}
void drawSolidCircle(double x, double y) {
	GLfloat TWOPI = 2.0f * PI;
	glColor3f(COLOR_PLATE_DOT);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (int count = 0; count <= TIME_DRAW_PLATE_DOT; count++)
	{
		glVertex2f(x + RADIUS_PLATE_DOT * cos(count * TWOPI / TIME_DRAW_PLATE_DOT), y + RADIUS_PLATE_DOT * sin(count * TWOPI / TIME_DRAW_PLATE_DOT));
	}
	glEnd();
}
void drawHand(int handIndex, SYSTEMTIME t) {
	double radian, hour, min, sec;
	switch (handIndex)
	{
	case HAND_HOUR:
		hour = (t.wHour > 12 ? t.wHour - 12 : t.wHour);
		min = t.wMinute;
		radian = (360 - (((hour / 12) * 360) + (min / 60) * 30 - 90)) * PI / 180;
		glColor4f(COLOR_HOUR.R, COLOR_HOUR.G, COLOR_HOUR.B, 1);
		drawLine(WIDTH_HOUR, cos(radian) * LEN_HOUR, sin(radian) * LEN_HOUR);
		break;

	case HAND_MIN:
		min = t.wMinute;
		sec = t.wSecond;
		radian = (360 - (((min / 60) * 360) + (sec / 60) * 6 - 90)) * PI / 180;
		glColor3f(COLOR_INITIAL_MIN);
		drawLine(WIDTH_MIN, cos(radian) * LEN_MIN, sin(radian) * LEN_MIN);
		break;

	case HAND_SEC:
		sec = t.wSecond * 1000 + t.wMilliseconds;
		radian = (360 - ((sec / 60000) * 360 - 90)) * PI / 180;
		glColor3f(COLOR_INITIAL_SEC);
		drawLine(WIDTH_SEC, cos(radian) * LEN_SEC, sin(radian) * LEN_SEC);
		break;

	}
}
void drawHands() {
	GetLocalTime(&sys);
	drawHand(HAND_HOUR, sys);
	drawHand(HAND_MIN, sys);
	drawHand(HAND_SEC, sys);
}
void drawPlate() {
	float radian;
	for (int i = 1; i <= 12; i++) {
		radian = i * 30 * PI / 180;
		drawSolidCircle(cos(radian) * RADIUS_CLOCK, sin(radian) * RADIUS_CLOCK);
	}
}
void draw()
{
	glClearColor(COLOR_BG.R, COLOR_BG.G, COLOR_BG.B, COLOR_BG.Alp);
	glClear(GL_COLOR_BUFFER_BIT);
	drawPlate();
	drawHands();
	drawSolidCircle(0, 0);
	glutSwapBuffers();
}
void timerDraw(int i)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timerDraw, 1);
}
void setColor(int colorIndex, Color& c)
{
	switch (colorIndex)
	{
	case INDEX_BLACK:
		c.set(BLACK);
		break;
	case INDEX_WHITE:
		c.set(WHITE);
		break;
	case INDEX_GREY:
		c.set(GREY);
		break;
	case INDEX_RED:
		c.set(RED);
		break;
	case INDEX_ORANGE:
		c.set(ORANGE);
		break;
	case INDEX_YELLOW:
		c.set(YELLOW);
		break;
	case INDEX_GREEN:
		c.set(GREEN);
		break;
	case INDEX_CYAN:
		c.set(CYAN);
		break;
	case INDEX_BLUE:
		c.set(BLUE);
		break;
	case INDEX_PURPLE:
		c.set(PURPLE);
		break;
	}
	draw();
}
void event_menu_main(int option) {
	switch (option) {
	case AA:
		enableAA();
		draw();
		break;
	case DIS_AA:
		disableAA();
		draw();
		break;
	case EXIT:
		exit(0);
		break;
	}
}
void event_menu_color_hour(int value)
{
	setColor(value, COLOR_HOUR);
}
void event_menu_color_bg(int value)
{
	setColor(value, COLOR_BG);
}
void event_menu_fps(int value)
{
	FPS = value;
}
void createGLUTMenus() {
	menu_color_hour = glutCreateMenu(event_menu_color_hour);
	glutAddMenuEntry("Black", INDEX_BLACK);
	glutAddMenuEntry("White", INDEX_WHITE);
	glutAddMenuEntry("Grey", INDEX_GREY);
	glutAddMenuEntry("Red", INDEX_RED);
	glutAddMenuEntry("Orange", INDEX_ORANGE);
	glutAddMenuEntry("Yellow", INDEX_YELLOW);
	glutAddMenuEntry("Green", INDEX_GREEN);
	glutAddMenuEntry("Cyan", INDEX_CYAN);
	glutAddMenuEntry("Blue", INDEX_BLUE);
	glutAddMenuEntry("Purple", INDEX_PURPLE);

	menu_color_bg = glutCreateMenu(event_menu_color_bg);
	glutAddMenuEntry("Black", INDEX_BLACK);
	glutAddMenuEntry("White", INDEX_WHITE);
	glutAddMenuEntry("Grey", INDEX_GREY);
	glutAddMenuEntry("Red", INDEX_RED);
	glutAddMenuEntry("Orange", INDEX_ORANGE);
	glutAddMenuEntry("Yellow", INDEX_YELLOW);
	glutAddMenuEntry("Green", INDEX_GREEN);
	glutAddMenuEntry("Cyan", INDEX_CYAN);
	glutAddMenuEntry("Blue", INDEX_BLUE);
	glutAddMenuEntry("Purple", INDEX_PURPLE);

	menu_fps = glutCreateMenu(event_menu_fps);
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("30", 30);
	glutAddMenuEntry("60", 60);
	glutAddMenuEntry("120", 120);

	menu_main = glutCreateMenu(event_menu_main);
	glutAddSubMenu("Hour Hand Color", menu_color_hour);
	glutAddSubMenu("Background Color", menu_color_bg);
	glutAddMenuEntry("Enable Anti-Aliasing", AA);
	glutAddMenuEntry("Disable Anti-Aliasing", DIS_AA);
	glutAddSubMenu("FPS", menu_fps);
	glutAddMenuEntry("Exit", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char* argv[])
{
	GLInit(argc, argv);
	glutTimerFunc(1000 / FPS, timerDraw, 1);
	glutDisplayFunc(draw);
	createGLUTMenus();
	glutMainLoop();
}
