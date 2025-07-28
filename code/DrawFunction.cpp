#include "myglwidget.h"

void cube() {
	glScalef(0.5, 0.5, 0.5);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-1, 1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, 1, -1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(-1, 1, 1);
	glVertex3f(1, 1, 1);
	glEnd();
}
GLvoid DrawCircleArea(float cx, float cy, float cz, float r, int num_segments)
{
	GLfloat vertex[4];

	const GLfloat delta_angle = 2.0 * M_PI / num_segments;
	glBegin(GL_TRIANGLE_FAN);

	vertex[0] = cx;
	vertex[1] = cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);

	//draw the vertex on the contour of the circle
	for (int i = 0; i < num_segments; i++)
	{
		vertex[0] = std::cos(delta_angle * i) * r + cx;
		vertex[1] = std::sin(delta_angle * i) * r + cy;
		vertex[2] = cz;
		vertex[3] = 1.0;
		glVertex4fv(vertex);
	}

	vertex[0] = 1.0 * r + cx;
	vertex[1] = 0.0 * r + cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);
	glEnd();
}
void zhu(GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks) {
	GLUquadricObj* quad;
	quad = gluNewQuadric();
	glPushMatrix();
	glTranslatef(0, height / 2, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(quad, base, top, height, slices, stacks);
	//top
	DrawCircleArea(0.0, 0.0, height, top, slices);
	//base
	DrawCircleArea(0.0, 0.0, 0.0, base, slices);
	glPopMatrix();
}
void shan(double ir, double or , double start, double s) {
	GLUquadricObj* quad;
	quad = gluNewQuadric();
	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(90, 0, 1, 0);
	gluPartialDisk(quad, ir, or , 16, 2, start, s);
	glPopMatrix();
}
void ball(double r1) {
	int du = 50;
	int dv = 50;
	float gu = 3.1415926535 / du, gv = 2.0 * 3.1415926535 / (dv - 1);
	float x, y, z, r;
	glPushMatrix();
	glScalef(r1, r1, r1);
#define MAKE_R(z2) sqrt(1.0 - (z2))
#define MAKE_Z(z_step) cos((z_step) *gu)
#define MAKE_Y(x_step,r) (r)*sin((x_step)* gv)
#define MAKE_X(x_step,r) (r)*cos((x_step)* gv)
	// ÇòÌå
	glBegin(GL_QUADS);
	for (int z_step = 0; z_step < du; ++z_step) {
		for (int x_step = 0; x_step < dv; ++x_step) {
			z = MAKE_Z(z_step);
			r = MAKE_R(z * z);
			x = MAKE_X(x_step, r);
			y = MAKE_Y(x_step, r);
			glVertex3f(x, y, z);

			z = MAKE_Z(z_step + 1);
			r = MAKE_R(z * z);
			x = MAKE_X(x_step, r);
			y = MAKE_Y(x_step, r);
			glVertex3f(x, y, z);

			z = MAKE_Z(z_step + 1);
			r = MAKE_R(z * z);
			x = MAKE_X(x_step + 1, r);
			y = MAKE_Y(x_step + 1, r);
			glVertex3f(x, y, z);

			z = MAKE_Z(z_step);
			r = MAKE_R(z * z);
			x = MAKE_X(x_step + 1, r);
			y = MAKE_Y(x_step + 1, r);
			glVertex3f(x, y, z);
		}
	}
	glEnd();
#undef MAKE_Y
#undef MAKE_Z
#undef MAKE_X
#undef MAKE_R
	glPopMatrix();
}
void circle(double R) {
	glBegin(GL_POLYGON); //»­Õýn±ßÐÎ
	double n = 100, Pi = 3.1415926;
	for (int i = 0; i < 100; ++i)
		glVertex2f(R * cos(2 * Pi / n * i), R * sin(2 * Pi / n * i));
	glEnd();
}
void sofa() {
	glPushMatrix();
	glScalef(0.98, 1, 1);
	glPushMatrix();
	glScalef(0.99, 1, 1);
	glColor3ub(222, 184, 135);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(20, 0, 6);
	glVertex3f(-20, 0, 6);
	glVertex3f(20, 20, 6);
	glVertex3f(-20, 20, 6);
	glVertex3f(20, 20, 4);
	glVertex3f(-20, 20, 4);
	glVertex3f(20, 6, 2);
	glVertex3f(-20, 6, 2);
	glVertex3f(20, 6, -6);
	glVertex3f(-20, 6, -6);
	glVertex3f(20, 0, -6);
	glVertex3f(-20, 0, -6);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(20, 20, 6);
	glVertex3f(20, 20, 4);
	glVertex3f(20, 0, 6);
	glVertex3f(20, 6, 2);
	glVertex3f(20, 0, -6);
	glVertex3f(20, 6, -6);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(-20, 20, 6);
	glVertex3f(-20, 20, 4);
	glVertex3f(-20, 0, 6);
	glVertex3f(-20, 6, 2);
	glVertex3f(-20, 0, -6);
	glVertex3f(-20, 6, -6);
	glEnd();
	glPopMatrix();
	glColor3ub(51, 82, 100);
	glBegin(GL_QUAD_STRIP);
	double g = 27.0 / 7.0;
	glVertex3f(20, 19, g - 0.5);
	glVertex3f(-20, 19, g - 0.5);
	glVertex3f(20, 6.2, 1.5);
	glVertex3f(-20, 6.2, 1.5);
	glVertex3f(20, 6.2, -8);
	glVertex3f(-20, 6.2, -8);
	glVertex3f(20, 2, -8);
	glVertex3f(-20, 2, -8);
	glVertex3f(20, 2, 2.5);
	glVertex3f(-20, 2, 2.5);
	glVertex3f(20, 6.2, 2.5);
	glVertex3f(-20, 6.2, 2.5);
	glVertex3f(20, 19, g + 0.5);
	glVertex3f(-20, 19, g + 0.5);
	glVertex3f(20, 19, g - 0.5);
	glVertex3f(-20, 19, g - 0.5);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(20, 19, g + 0.5);
	glVertex3f(20, 19, g - 0.5);
	glVertex3f(20, 6.2, 2.5);
	glVertex3f(20, 6.2, 1.5);
	glVertex3f(20, 2, 2.5);
	glVertex3f(20, 6.2, 1.5);
	glVertex3f(20, 2, -8);
	glVertex3f(20, 6.2, -8);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(-20, 19, g + 0.5);
	glVertex3f(-20, 19, g - 0.5);
	glVertex3f(-20, 6.2, 2.5);
	glVertex3f(-20, 6.2, 1.5);
	glVertex3f(-20, 2, 2.5);
	glVertex3f(-20, 6.2, 1.5);
	glVertex3f(-20, 2, -8);
	glVertex3f(-20, 6.2, -8);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(20, 2, 2.5);
	glVertex3f(-20, 2, 2.5);
	glVertex3f(20, 2, -8);
	glVertex3f(-20, 2, -8);
	glEnd();
	glPopMatrix();
}
void dback() {
	glPushMatrix();
	glColor3ub(84, 65, 50);
	double x = 6.0, y = 0.0, z = 0.0;
	double x1 = 6.0, y1 = 1.0, z1 = 0.0;
	double dx = 6.0 / 50.0, dy = 3.0 / 50, dz = 2.0 / 50;
	double ddk = 1.0 / 50.0;
	for (double i = 0.0; i < 100; i += 1.0) {
		glBegin(GL_QUAD_STRIP);
		glVertex3f(x, y, z);
		glVertex3f(x - dx, y, z - dz * (50 - i) * ddk);
		glVertex3f(x1, y1, z1);
		glVertex3f(x - dx, y1 + dy * (50 - i) * ddk, z1 - dz * (50 - i) * ddk);
		glVertex3f(x1, y1, z1 - 1);
		glVertex3f(x - dx, y1 + dy * (50 - i) * ddk, z1 - dz * (50 - i) * ddk - 1);
		glVertex3f(x, y, z - 1);
		glVertex3f(x - dx, y, z - dz * (50 - i) * ddk - 1);
		x -= dx; x1 -= dx;
		y1 += dy * (50 - i) * ddk;
		z -= dz * (50 - i) * ddk;
		z1 -= dz * (50 - i) * ddk;
		glEnd();
	}
	glPopMatrix();
}
void desk() {
	glPushMatrix();
	glColor3ub(84, 65, 50);
	glPushMatrix();
	glTranslatef(0, 4.5, 0);
	glScalef(10, 1, 8);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5, 4.5, 4);
	glRotatef(-5, 1, 0, 0);
	zhu(0.7, 0.5, 9, 16, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-5, 4.5, 4);
	glRotatef(-5, 1, 0, 0);
	zhu(0.7, 0.5, 9, 16, 2);
	glPopMatrix();
	glTranslatef(-5, 4.5, -4);
	glRotatef(5, 1, 0, 0);
	zhu(0.7, 0.5, 9, 16, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5, 4.5, -4);
	glRotatef(5, 1, 0, 0);
	zhu(0.7, 0.5, 9, 16, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5, 9.5, 0);
	glScalef(2, 1, 10);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-5, 9.5, 0);
	glScalef(2, 1, 10);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 9, -4);
	dback();
	glPopMatrix();
	glColor3ub(0, 87, 82);
	glPushMatrix();
	glTranslatef(0, 5, 3);
	glRotatef(90, 0, 0, 1);
	zhu(0.5, 0.5, 9, 16, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 5, -3);
	glRotatef(90, 0, 0, 1);
	zhu(0.5, 0.5, 9, 16, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 5, 0);
	glScalef(9, 1, 6);
	cube();
	glPopMatrix();
	glPopMatrix();
}
void floor1(int mode) {

	glPushMatrix();
	glColor3ub(215, 1999, 179);
	glBegin(GL_LINE_STRIP);
	glVertex3f(5, 0, 15);
	glVertex3f(5, 0, -15);
	glVertex3f(-5, 0, -15);
	glVertex3f(-5, 0, 15);
	glVertex3f(5, 0, 15);
	glEnd();
	if (mode == 0)
		glColor3ub(255, 239, 219);
	else
		glColor3ub(240, 224, 205);
	glTranslatef(0, -0.5, 0);
	glScalef(10, 1, 30);
	cube();
	glPopMatrix();

}
void col(int a) {
	double init;
	if (a == 1)
		init = -135.0;
	else
		init = -120.0;
	for (double a = 0; a < 12; a++) {
		glPushMatrix();
		glTranslatef(0, 0, init + a * 30.0);
		floor1((int)a % 2);
		glPopMatrix();
	}
}
void floor() {
	glPushMatrix();
	glTranslatef(-160, 0, -15);
	double init = 0;
	for (int a = 0; a < 48; a++) {
		glPushMatrix();
		glTranslatef(init + a * 10, 0, 0);
		col(a % 2);
		glPopMatrix();
	}
	glPopMatrix();
}
void bwall(double l, double w, double h1, double h2, int mode) {
	glPushMatrix();
	glTranslatef(0, h2 / 2, 0);
	if (mode == 1)
		glColor3f(0.1, 0.1, 0.1);
	else
		glColor3f(0.98, 0.98, 0.98);
	glPushMatrix();
	glTranslatef(0, (h1 - h2) / 2, 0);
	glScalef(l, h1, w);
	cube();
	glPopMatrix();

	if (mode == 1)
		glColor3ub(125, 101, 88);
	else
		glColor3ub(211, 211, 211);
	glPushMatrix();
	glTranslatef(l / 2, 0, w / 2);
	glScalef(1, h2, 1);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-l / 2, 0, w / 2);
	glScalef(1, h2, 1);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(l / 2, 0, -w / 2);
	glScalef(1, h2, 1);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-l / 2, 0, -w / 2);
	glScalef(1, h2, 1);
	cube();
	glPopMatrix();

	glPopMatrix();
}
void kuang(double l, double h, double t, double s) {
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0, h / 2, 0);
	glScalef(l, t, s);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -h / 2, 0);
	glScalef(l, t, s);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(l / 2, 0, 0);
	glScalef(t, h + t, s);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-l / 2, 0, 0);
	glScalef(t, h + t, s);
	cube();
	glPopMatrix();
	glPopMatrix();
}
void mall(double tv1, double tv2, double tv3) {
	glPushMatrix();

	glColor3ub(125, 101, 88);
	glPushMatrix();
	glTranslatef(0, 2.5 + 4.5 / 2, 0);
	kuang(20, 4.5, 1, 5.9);
	glPopMatrix();

	glPushMatrix();
	glScalef(39.8, 3, 5.5);
	cube();
	glPopMatrix();

	for (double i = 0; i < 3; i++) {
		glPushMatrix();
		glTranslatef(-40 / 3 + 40 / 3 * i, 25, -5);
		kuang(40 / 3, 10, 0.8, 4);
		glPopMatrix();
	}
	for (double i = 0; i < 8; i++) {
		glPushMatrix();
		glTranslatef(-17.5 + 5 * i, 32.5, -2);
		kuang(5, 5, 0.6, 10);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-17.5 + 5 * i, 37.5, -2);
		kuang(5, 5, 0.6, 10);
		glPopMatrix();
	}

	glColor3ub(100, 80, 68);
	glPushMatrix();
	glTranslatef(0, 4, -3);
	glScalef(30, 8, 2);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 35, -7);
	glScalef(39.8, 9.8, 0.2);
	cube();
	glPopMatrix();

	glColor3ub(205, 205, 193);
	glPushMatrix();
	glTranslatef(0, 25, -7);
	glScalef(39.8, 9.8, 0.2);
	cube();
	glPopMatrix();

	glColor3f(0.15, 0.15, 0.15);
	if (tv1 == 1) {
		glPushMatrix();
		glTranslatef(-40 / 3, 20, -4.5);
		glRotatef(10, 1, 0, 0);
		glTranslatef(0, 4.5, 0);
		glScalef(40 / 3 - 2, 8.5, 0.8);
		cube();
		glPopMatrix();
	}
	if (tv2 == 1) {
		glPushMatrix();
		glTranslatef(0, 20, -4.5);
		glRotatef(10, 1, 0, 0);
		glTranslatef(0, 4.5, 0);
		glScalef(40 / 3 - 2, 8.5, 0.8);
		cube();
		glPopMatrix();
	}
	if (tv3 == 1) {
		glPushMatrix();
		glTranslatef(40 / 3, 20, -4.5);
		glRotatef(10, 1, 0, 0);
		glTranslatef(0, 4.5, 0);
		glScalef(40 / 3 - 2, 8.5, 0.8);
		cube();
		glPopMatrix();
	}

	glColor3ub(205, 170, 125);
	glPushMatrix();
	glTranslatef(0, 1.9, -1);
	glScalef(40, 2, 8);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 8, -1);
	glScalef(40, 2.9, 8);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-(20 - 10.45 / 2), 5, -1);
	glScalef(10.48, 5, 8);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20 - 10.45 / 2, 5, -1);
	glScalef(10.48, 5, 8);
	cube();
	glPopMatrix();

	glColor3ub(248, 248, 255);
	glPushMatrix();
	glTranslatef(0, 9.75, -2);
	glScalef(40, 0.5, 10);
	cube();
	glPopMatrix();

	glColor3ub(248, 248, 255);
	glPushMatrix();
	glTranslatef(0, 8, 3);
	glScalef(4, 1.8, 0.2);
	cube();
	glPopMatrix();

	glColor3ub(255, 165, 79);
	glPushMatrix();
	glTranslatef(-1.3, 7.3, 3.11);
	glScalef(1.4, 0.4, 0.02);
	cube();
	glPopMatrix();

	glColor3ub(34, 139, 34);
	glPushMatrix();
	glTranslatef(0.8, 7.3, 3.11);
	glScalef(2.4, 0.4, 0.02);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, 2.5, 0.7);
	glScalef(0.8, 0.7, 0.8);
	glColor3ub(248, 248, 255);

	glPushMatrix();
	glTranslatef(0, 8, 3);
	glScalef(4, 1.8, 0.2);
	cube();
	glPopMatrix();

	glColor3ub(255, 165, 79);
	glPushMatrix();
	glTranslatef(-1.3, 7.3, 3.11);
	glScalef(1.4, 0.4, 0.02);
	cube();
	glPopMatrix();

	glColor3ub(34, 139, 34);
	glPushMatrix();
	glTranslatef(0.8, 7.3, 3.11);
	glScalef(2.4, 0.4, 0.02);
	cube();
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}
void bigmall(int mode) {
	glPushMatrix();

	glTranslatef(-80, 0, -10);
	glPushMatrix();
	glTranslatef(60, 0, 7);
	mall(0, 1, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(100, 0, 7);
	mall(1, 1, 1);
	glPopMatrix();

	double x = 0;
	if (mode == 2)
		x = -9;
	glPushMatrix();
	glTranslatef(35, 0, 3 + x);
	bwall(10, 16 * mode, 45, 40, mode);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(125, 0, 3 + x);
	bwall(10, 16 * mode, 45, 40, mode);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(80, 40, 3 + x);
	bwall(100, 16 * mode, 8, 8, mode);
	glPopMatrix();

	glPopMatrix();
}
void car() {
	glColor3ub(211, 211, 211);
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0, 6, 0);
	glScalef(16, 5, 10);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(9, 6, -3);
	glScalef(1.6, 0.6, 0.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(9, 6, 3);
	glScalef(1.6, 0.6, 0.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-9, 6, -3);
	glScalef(1.6, 0.6, 0.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-9, 6, 3);
	glScalef(1.6, 0.6, 0.5);
	cube();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-10, 6, 0);
	glRotatef(90, 1, 0, 0);
	zhu(0.3, 0.3, 6.2, 16, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 6, 0);
	glRotatef(90, 1, 0, 0);
	zhu(0.3, 0.3, 6.2, 16, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7, 2.5, 4);
	glScalef(1, 3, 0.6);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(7, 2.5, -4);
	glScalef(1, 3, 0.6);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-7, 2.5, 4);
	glScalef(1, 3, 0.6);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-7, 2.5, -4);
	glScalef(1, 3, 0.6);
	cube();
	glPopMatrix();

	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glTranslatef(7, 0.5, 4);
	glRotatef(90, 1, 0, 0);
	zhu(0.5, 0.5, 0.6, 16, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(7, 0.5, -4);
	glRotatef(90, 1, 0, 0);
	zhu(0.5, 0.5, 0.6, 16, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-7, 0.5, 4);
	glRotatef(90, 1, 0, 0);
	zhu(0.5, 0.5, 0.6, 16, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-7, 0.5, -4);
	glRotatef(90, 1, 0, 0);
	zhu(0.5, 0.5, 0.6, 16, 2);
	glPopMatrix();

	glPopMatrix();
}
void e() {
	glPushMatrix();
	glColor3ub(205, 38, 38);
	for (double i = -30; i < 30; i += 0.5) {
		glPushMatrix();
		glTranslatef(0, 50 * 1.73205 / 2 + 6, 0);
		glRotatef(i, 1, 0, 0);
		glTranslatef(0, -50 * 1.73205 / 2, 0);
		glRotatef(90, 1, 0, 0);
		zhu(0.4, 0.4, 1, 16, 2);
		glPopMatrix();
	}
	glColor3ub(205, 205, 180);
	glPushMatrix();
	glTranslatef(0, 6, -24);
	zhu(0.9, 0.9, 12, 16, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 6, 24);
	zhu(0.9, 0.9, 12, 16, 2);
	glPopMatrix();

	glColor3ub(205, 205, 180);
	glPushMatrix();
	glTranslatef(0, 0.5, -24);
	zhu(1.5, 1.5, 1, 16, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.5, 24);
	zhu(1.5, 1.5, 1, 16, 2);
	glPopMatrix();

	glColor3ub(205, 205, 180);
	glPushMatrix();
	glTranslatef(0, 12.5, -24);
	zhu(1.5, 1.5, 1, 16, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 12.5, 24);
	zhu(1.5, 1.5, 1, 16, 2);
	glPopMatrix();

	glPopMatrix();
}

