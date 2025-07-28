#include "myglwidget.h"
#include<cmath>
#include<windows.h>
#include <iostream>
#include "DrawFunction.h"
using namespace std;
/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

QImage tex1, tex2, tex3;
double win1 = 1, win2 = 1, jump1 = 0, jump2 = 0;
double pref1 = 0, pref2 = 0;
double posx = 0, posy = 0, posz = 0;

double walk1 = 0, attack_move_melon = 0, boomerang1 = 0, moon1 = 0, fly1 = 0;
double moons = 0, moone = 0, moonlast = 0;
double walk2 = 0, attack_move_milk = 0, boomerang2 = 0, moon2 = 0, fly2 = 0;
double moons2 = 0, moone2 = 0, moonlast2 = 0;

double fhurt1 = 0;
double fhurt2 = 0;
int fightlast = 0;
int fightlast2 = 0;
int flagmoon = 0;
int flagmoon2 = 0;
bool move1 = 0, f1 = 0, bo1 = 0;
bool move2 = 0, f2 = 0, bo2 = 0;
double foot_direction_melon = 1, flagfight = 0, flagbo = -6, flagb = 0, fly_direction_melon = 1;
double foot_direction_milk = 1, flagfight2 = 0, flagbo2 = -6, flagb2 = 0, fly_direction_milk = 1;

int flagdead = 1;
double gdead = 0;
bool dead1 = 0;
int flagdead2 = 1;
double gdead2 = 0;
bool dead2 = 0;

double view = 1;
double high = 0;
double nowx = 0, nowz = 0;

double sum = 0;

bool isup, isdown, isleft, isright;
bool isw, iss, isa, isd;

double melon_x = 1200, melon_z = 1200, melon_face = -90;
double gbx = -11, gbz = 0, gby = 3, gb;
bool bfly1 = 0, bfly2 = 0;
double milk_x = 300, milk_z = 300, milk_face = -90;
double mbx = -8, mbz = 0, mby = 3, mb;
double n = 0, f = 0;

bool start = 0;
bool w[1601][1601] = { 0 };

MyGLWidget::MyGLWidget(QWidget* parent)
	:QOpenGLWidget(parent)
{
	timer = new QTimer(this); // 实例化一个定时器
	timer->start(16); // 时间间隔设置为16ms，可以根据需要调整
	connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图
}

MyGLWidget::~MyGLWidget()
{
	delete this->timer;
}

void MyGLWidget::onMessageReceived(const QByteArray& data)
{
	
	QString msg = QString::fromUtf8(data);
	QStringList lines = msg.split("\n");
	for (QString line : lines) {
		QStringList parts = line.split(" ");
		if (parts.size() != 1 && parts.size() != 5)
			continue;
		int key = parts[0].toInt();
		qDebug() << "key:" << key;
		if (parts.size() == 5) {
			melon_x = parts[1].toDouble();
			melon_z = parts[2].toDouble();
			milk_x = parts[3].toDouble();
			milk_z = parts[4].toDouble();
		}
		QKeyEvent e(
			(parts.size() == 1) ? QEvent::KeyPress : QEvent::KeyRelease,
			key,
			Qt::NoModifier
		);
		handleRemoteKey(&e);
	}
}

void MyGLWidget::setNetworkManager(NetworkManager* manager)
{
	this->network = manager;
	// 示例：连接网络接收到消息的信号
	connect(manager, SIGNAL(messageReceived(QByteArray)),
		this, SLOT(onMessageReceived(QByteArray)));
}

void makew() {
	for (int i = 0; i < 150; i++) {
		for (int j = 0; j < 750; j++)
			w[i][j] = 1;
	}
	for (int i = 1350; i < 1501; i++) {
		for (int j = 850; j < 1501; j++)
			w[i][j] = 1;
	}
	for (int i = 0; i < 1501; i++) {
		for (int j = 0; j <= 4; j++) {
			w[i][j] = 1;
			w[j][i] = 1;
			w[1500 + j][i] = 1;
			w[i][1500 + j] = 1;
			w[1500 - j][i] = 1;
			w[i][1500 - j] = 1;
		}
	}

	for (int i = 200; i <= 400; i++) {
		for (int j = 1100; j <= 1300; j++)
			w[i][j] = 1;
	}
	for (int i = 1100; i <= 1340; i++) {
		for (int j = 200; j <= 400; j++)
			w[i][j] = 1;
	}
}

void MyGLWidget::keyPressEvent(QKeyEvent* e)
{
	if (network && !e->isAutoRepeat()) {
		QByteArray msg;
		msg.append(QByteArray::number(e->key()));
		msg.append(QString("\n"));
		network->sendMessage(msg);
	}
	if (e->key() == Qt::Key_H) {
		start = 1;
	}
	if (e->key() == Qt::Key_G) {
		dead1 = !dead1;
		flagdead = 1;
	}
	if (e->key() == Qt::Key_J && f2 == 0 && moon2 == 0) {
		f2 = 1;
		moon2 = 1;
	}
	if (e->key() == Qt::Key_1 && f1 == 0 && moon1 == 0) {
		f1 = 1;
		moon1 = 1;
	}
	if (e->key() == Qt::Key_K && start) {
		bo2 = 1;
	}
	if (e->key() == Qt::Key_2 && start) {
		bo1 = 1;
	}
	if (!e->isAutoRepeat() && start) {
		switch (e->key()) {
		case Qt::Key_Down: isdown = 1; break;
		case Qt::Key_Up: isup = 1; break;
		case Qt::Key_Left: isleft = 1; break;
		case Qt::Key_Right: isright = 1; break;
		case Qt::Key_W: isw = 1; break;
		case Qt::Key_S: iss = 1; break;
		case Qt::Key_A: isa = 1; break;
		case Qt::Key_D: isd = 1; break;
		}
	}
}
void MyGLWidget::keyReleaseEvent(QKeyEvent* e) {

	if (network && !e->isAutoRepeat()) {
		QByteArray msg;
		msg.append(QByteArray::number(e->key()));
		msg.append(QString(" %1 %2 %3 %4\n").arg(melon_x).arg(melon_z).arg(milk_x).arg(milk_z));
		network->sendMessage(msg);
	}

	if (!e->isAutoRepeat() && start) {
		switch (e->key()) {
		case Qt::Key_Down: isdown = 0; break;
		case Qt::Key_Up: isup = 0; break;
		case Qt::Key_Left: isleft = 0; break;
		case Qt::Key_Right: isright = 0; break;
		case Qt::Key_W: isw = 0; break;
		case Qt::Key_S: iss = 0; break;
		case Qt::Key_A: isa = 0; break;
		case Qt::Key_D: isd = 0; break;
		}
	}
}
void MyGLWidget::handleRemoteKey(QKeyEvent* e)
{
	if (e->type() == QEvent::KeyPress) {
		if (e->key() == Qt::Key_H) {
			start = 1;
		}
		if (e->key() == Qt::Key_J && f2 == 0 && moon2 == 0) {
			f2 = 1;
			moon2 = 1;
		}
		if (e->key() == Qt::Key_1 && f1 == 0 && moon1 == 0) {
			f1 = 1;
			moon1 = 1;
		}
		if (e->key() == Qt::Key_K && start)
			bo2 = 1;
		if (e->key() == Qt::Key_2 && start)
			bo1 = 1;

		if (!e->isAutoRepeat() && start) {
			switch (e->key()) {
			case Qt::Key_Down: isdown = 1; break;
			case Qt::Key_Up: isup = 1; break;
			case Qt::Key_Left: isleft = 1; break;
			case Qt::Key_Right: isright = 1; break;
			case Qt::Key_W: isw = 1; break;
			case Qt::Key_S: iss = 1; break;
			case Qt::Key_A: isa = 1; break;
			case Qt::Key_D: isd = 1; break;
			}
		}
	}
	else if (e->type() == QEvent::KeyRelease) {
		if (!e->isAutoRepeat() && start) {
			switch (e->key()) {
			case Qt::Key_Down: isdown = 0; break;
			case Qt::Key_Up: isup = 0; break;
			case Qt::Key_Left: isleft = 0; break;
			case Qt::Key_Right: isright = 0; break;
			case Qt::Key_W: isw = 0; break;
			case Qt::Key_S: iss = 0; break;
			case Qt::Key_A: isa = 0; break;
			case Qt::Key_D: isd = 0; break;
			}
		}
	}
}

void init() {
	QImage buf;
	buf.load("1.jpg");
	tex1 = QGLWidget::convertToGLFormat(buf);
	buf.load("2.jpg");
	tex2 = QGLWidget::convertToGLFormat(buf);
	buf.load("1.png");
	tex3 = QGLWidget::convertToGLFormat(buf);
}

void table1(int flag) {
	glColor3ub(195, 140, 95);
	
	glPushMatrix();
	
	glScalef(1, 1, 1.5);
	glPushMatrix();
	glTranslatef(0, 11, 0);
	glScalef(20.2, 2.2, 20.2);
	if (flag) cube(); 
	else {
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_TEXTURE_2D);//允许采用2D纹理技术
		glShadeModel(GL_SMOOTH);//设置阴影平滑模式
		glTexImage2D(GL_TEXTURE_2D, 0, 3, tex1.width(), tex1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex1.bits());//开始真正创建纹理数据
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//当所显示的纹理比加载进来的纹理小时，采用GL_LINEAR的方法来处理
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//当所显示的纹理比加载进来的纹理大时，采用GL_LINEAR的方法来处理
		glScalef(0.5, 0.5, 0.5);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, 1, -1);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(1, 1, -1);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-1, -1, -1);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(1, -1, -1);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(1, -1, 1);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(1, 1, -1);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(1, 1, 1);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-1, 1, -1);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, 1, 1);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-1, -1, -1);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-1, -1, 1);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(1, -1, 1);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, 1, 1);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(1, 1, 1);
		glEnd();
		glTexImage2D(GL_TEXTURE_2D, 0, 3, tex2.width(), tex2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2.bits());//开始真正创建纹理数据
	}
	
	glPopMatrix();
	


	glColor3ub(205, 170, 125);
	glPushMatrix();
	glTranslatef(9, 6, 9);
	glScalef(2, 12, 2);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-9, 6, 9);
	glScalef(2, 12, 2);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(9, 6, -9);
	glScalef(2, 12, 2);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-9, 6, -9);
	glScalef(2, 12, 2);
	cube();
	glPopMatrix();
	glPopMatrix();
}
void table2(int flag) {
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(0, 12, 0);
	glScalef(30, 1, 20);
	if (flag) cube();
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, 3, tex3.width(), tex3.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex3.bits());//开始真正创建纹理数据
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//当所显示的纹理比加载进来的纹理小时，采用GL_LINEAR的方法来处理
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//当所显示的纹理比加载进来的纹理大时，采用GL_LINEAR的方法来处理
		glScalef(0.5, 0.5, 0.5);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, 1, -1);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(1, 1, -1);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-1, -1, -1);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(1, -1, -1);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(1, -1, 1);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(1, 1, -1);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(1, 1, 1);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-1, 1, -1);
	
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, 1, 1);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-1, -1, -1);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-1, -1, 1);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(1, -1, 1);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1, 1, 1);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(1, 1, 1);
		glEnd();
		glTexImage2D(GL_TEXTURE_2D, 0, 3, tex2.width(), tex2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2.bits());//开始真正创建纹理数据
	}
	
	
	


	glPopMatrix();
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(6, 6, 0);
	glScalef(3, 12, 3);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-6, 6, 0);
	glScalef(3, 12, 3);
	cube();
	glPopMatrix();
	glPushMatrix();
	glScalef(22, 1, 16);
	cube();
	glPopMatrix();
	glPopMatrix();
}
void td(int mode) {
	glScalef(0.9, 1, 0.8);
	glPushMatrix();
	glPushMatrix();
	glPushMatrix();
	table1(mode);
	glPushMatrix();
	glTranslatef(-16, 0, -7.5);
	glRotatef(90, 0, 1, 0);
	desk();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-16, 0, 7.5);
	glRotatef(90, 0, 1, 0);
	desk();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16, 0, -7.5);
	glRotatef(-90, 0, 1, 0);
	desk();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16, 0, 7.5);
	glRotatef(-90, 0, 1, 0);
	desk();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void world() {
	glPushMatrix();
	floor();
	double down = 154.0;
	for (double i = 1; i < 6; i += 1.0) {
		glPushMatrix();
		glTranslatef(15 + i * 20, 0, 0.0 + down);
		desk();
		glPopMatrix();
	}
	glPushMatrix();
	glTranslatef(25, 0, 13.0 + down);
	glRotatef(-10, 0, 1, 0);
	table2(0);
	glPopMatrix();

	for (double i = 1; i < 3; i += 1.0) {
		glPushMatrix();
		glTranslatef(25 + i * 40, 0, 14.0 + down);
		table2(1);
		glPopMatrix();
	}
	for (double i = 0; i < 3; i += 1.0) {
		glPushMatrix();
		glTranslatef(25 + i * 40, 0, 32.0 + down);
		sofa();
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(75, 0, -60);
	bwall(300, 10, 60, 60, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(90, 0, 0);
	bigmall(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(152, 0, 60);
	e();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-9, 6, 140);
	glRotatef(-8, 0, 1, 0);
	glRotatef(-85, 1, 0, 0);
	desk();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-7, 0, 110);
	glRotatef(-7, 0, 1, 0);
	e();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20, 11, 95);
	glRotatef(45, 0, 1, 0);
	glRotatef(180, 1, 0, 0);
	table1(0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 20);
	glRotatef(90, 0, 1, 0);
	bigmall(2);
	glPopMatrix();

	glColor3ub(113, 99, 96);
	glPushMatrix();
	glTranslatef(165, 30, -12);
	glScalef(50, 60, 4);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(150 + 18, 0, 5);
	td(0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(150 + 75, 0, 0);
	td(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(150 + 75, 0, -35);
	td(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(150 + 75, 0, -60);
	bwall(60, 10, 50, 50, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-6, 0, -75);
	bwall(32, 90, 50, 50, 2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(150, 0, 170);
	bwall(20, 20, 50, 50, 2);
	glPopMatrix();

	for (double i = 0; i < 1; i++) {
		glPushMatrix();
		glTranslatef(150 + 8 + 50 * i, 0, 105);
		td(0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(150 + 8 + 50 * i, 0, 135);
		td(0);
		glPopMatrix();
	}
	glPushMatrix();
	glTranslatef(45, 0, -40);
	bwall(10, 46, 40, 40, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(135, 0, -40);
	bwall(10, 46, 40, 40, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 0, 118.5);
	glScalef(1, 0.8, 1);
	glRotatef(-135, 0, 1, 0);
	glTranslatef(0, 4, 7);
	glRotatef(-85, 1, 0, 0);
	glTranslatef(4 / 4, 7 / 6, 1);
	desk();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(120, 0, 30);
	glRotatef(-45, 0, 1, 0);
	car();
	glPopMatrix();
}

void foot(int mode) {
	glPushMatrix();
	double w, bw;
	if (mode == 1) {
		w = 2.8;
		bw = 0.82;
		glColor3ub(42, 58, 49);
	}
	else {
		w = 1.8;
		bw = 1.5;
		glColor3ub(75, 119, 179);
	}
	glPushMatrix();
	if (move1 && mode == 1)
		glTranslatef((walk1)*bw, 0, 0);
	if (move2 && mode == 2)
		glTranslatef((walk2)*bw, 0, 0);
	glScalef(0.8, 0.7, 1);
	glPushMatrix();
	glTranslatef(0, 1.5, w);
	glScalef(2, 3, 2);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1, 1.5, w);
	glScalef(0.4, 1, 1);
	zhu(1, 1, 3, 16, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1, 1.5, w);
	glScalef(0.4, 1, -1);
	zhu(1, 1, 3, 16, 2);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	if (move1 && mode == 1)
		glTranslatef((-walk1) * bw, 0, 0);
	if (move2 && mode == 2)
		glTranslatef((-walk2) * bw, 0, 0);
	glScalef(0.8, 0.7, 1);
	glPushMatrix();
	glTranslatef(0, 1.5, -w);
	glScalef(2, 3, 2);
	cube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1, 1.5, -w);
	glScalef(0.4, 1, 1);
	zhu(1, 1, 3, 16, 2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1, 1.5, -w);
	glScalef(0.4, 1, -1);
	zhu(1, 1, 3, 16, 2);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void lhand(int mode) {
	glPushMatrix();
	double w, h;
	if (mode == 1) {
		w = 8;
		h = 2.6;
		glColor3ub(42, 58, 49);
	}
	else {
		w = 4.8;
		h = 2.5;
		glColor3ub(75, 119, 179);
	}

	glPushMatrix();
	if (move1 && !bo1 && mode == 1)
		glRotatef(-walk1 * 12, 0, 1, 0);
	if (move2 && !bo2 && mode == 2)
		glRotatef(-walk2 * 12, 0, 1, 0);
	if (bo1 && mode == 1)
		glRotatef(boomerang1 * 1.2, 0, 1, 0);
	if (bo2 && mode == 2)
		glRotatef(boomerang2 * 1.2, 0, 1, 0);
	glTranslatef(0, h, -w);
	ball(1);
	glPopMatrix();
	glPopMatrix();
}
void b1() {
	glPushMatrix();
	double o = 0.017453293;
	glPushMatrix();
	glTranslatef(0, 0, -2 * cos(8 * o));
	glScalef(0.8, 0.2, 4 * cos(8 * o));
	cube();
	glPopMatrix();

	glPushMatrix();
	glRotatef(8, 0, 1, 0);
	glTranslatef(0, 0, -2);
	glScalef(0.8, 0.2, 4);
	cube();
	glPopMatrix();

	zhu(0.4, 0.4, 0.2, 16, 2);
	glPopMatrix();
}
void biao(int mode) {
	if (mode == 1) {
		glColor3ub(255, 73, 55);
		//glColor3ub(251, 189, 198);
	}
	else {
		glColor3ub(119, 185, 254);
		//glColor3ub(188, 220, 251);
	}
	glPushMatrix();

	glTranslatef(0.5, 0, 0.5);
	glScalef(-1, 1, -1);
	glPushMatrix();
	glTranslatef(-0.36, 0, -4);
	zhu(0.6, 0.6, 0.2, 16, 2);
	glPopMatrix();
	b1();
	glPushMatrix();
	glTranslatef(-0.36, 0, -4);
	glRotatef(-40, 0, 1, 0);
	glTranslatef(0.36, 0, -4);
	glScalef(1, 1, -1);
	b1();
	glPopMatrix();

	glPopMatrix();
}

void b(int mode) {
	glPushMatrix();
	double w, h, bei;
	if (mode == 1) {
		w = 8;
		h = 2.6;
		bei = 1;
	}
	else {
		w = 4.8;
		h = 2.5;
		bei = 1.1;
	}

	glPushMatrix();
	if (f1 == 1 && mode == 1) {
		glTranslatef(attack_move_melon * bei / 130, 0, 0);
		glTranslatef(0, 0, -attack_move_melon * bei / 100);
		glTranslatef(0, -1.10115, 1);
		glRotatef(attack_move_melon * bei, 0, 3.7321, 1);
		glTranslatef(0, 1.10115, -1);
	}
	if (f2 == 1 && mode != 1) {
		glTranslatef(attack_move_milk * bei / 30, 0, -attack_move_milk / 150);
		glTranslatef(0, 0, -attack_move_milk * bei / 100);
		glTranslatef(0, -1.10115, 1);
		glRotatef(attack_move_milk * bei, 0, 3.7321, 1);
		glTranslatef(0, 1.10115, -1);
	}
	if (bo1 && !flagb && mode == 1) {
		glRotatef(boomerang1 * 2, 0, 1, 0);
		glTranslatef(0, boomerang1 / 15, 0);
	}
	if (bo2 && !flagb2 && mode == 2) {
		glRotatef(boomerang2 * 2.3, 0, 1, 0);
		glTranslatef(0, boomerang2 / 15, 0);
	}
	if (move1 && mode == 1)
		glRotatef(-walk1 * 12, 0, 1, 0);
	if (move2 && mode == 2)
		glRotatef(-walk2 * 12, 0, 1, 0);
	glTranslatef(0, h, w);
	if (!bo1 && mode == 1)
		glRotatef(15, 1, 0, 0);
	if (!bo2 && mode == 2)
		glRotatef(15, 1, 0, 0);
	biao(mode);
	glPopMatrix();
	glPopMatrix();
}
void rhand(int mode) {
	glPushMatrix();
	double w, h, bei;
	if (mode == 1) {
		w = 8;
		h = 2.6;
		bei = 1;
		glColor3ub(42, 58, 49);
	}
	else {
		w = 4.8;
		h = 2.5;
		bei = 1.1;
		glColor3ub(75, 119, 179);
	}

	glPushMatrix();
	if (f1 == 1 && mode == 1) {
		glTranslatef(attack_move_melon * bei / 130, 0, 0);
		glTranslatef(0, 0, -attack_move_melon * bei / 100);
		glTranslatef(0, -1.10115, 1);
		glRotatef(attack_move_melon * bei, 0, 3.7321, 1);
		glTranslatef(0, 1.10115, -1);
	}
	if (f2 == 1 && mode == 2) {
		glTranslatef(attack_move_milk * bei / 30, 0, -attack_move_milk / 150);
		glTranslatef(0, 0, -attack_move_milk * bei / 100);
		glTranslatef(0, -1.10115, 1);
		glRotatef(attack_move_milk * bei, 0, 3.7321, 1);
		glTranslatef(0, 1.10115, -1);
	}
	if (bo1 && mode == 1) {
		glRotatef(boomerang1 * 2, 0, 1, 0);
		glTranslatef(0, boomerang1 / 15, 0);
	}
	if (bo2 && mode == 2) {
		glRotatef(boomerang2 * 2.3, 0, 1, 0);
		glTranslatef(0, boomerang2 / 15, 0);
	}
	if (move1 && mode == 1)
		glRotatef(-walk1 * 12, 0, 1, 0);
	if (move2 && mode == 2)
		glRotatef(-walk2 * 12, 0, 1, 0);
	glTranslatef(0, h, w);
	ball(1);
	glPopMatrix();
	glPopMatrix();
}
void bmoon(int mode) {
	double w, h;
	if (mode == 1) {
		w = 8 + 2.4;
		h = 2.6;
		glColor3ub(251, 189, 198);
	}
	else {
		w = 4.8 + 2.4 + 4;
		h = 2.5;
		glColor3ub(188, 220, 251);
	}
	glPushMatrix();
	if (mode == 1) {
		glTranslatef(0, h + 0.8, 0);
		/*else
			glTranslatef(0, h + 0.5, 0);*/
		glRotatef(15, 1, 0, 0);
		for (double i = moons; i < moone * 0.98; i += 0.5) {
			glPushMatrix();
			glRotatef(i - 10, 0, 1, 0);
			glBegin(GL_QUAD_STRIP);
			glVertex3f(-0.08, 0, w + i / 50);
			glVertex3f(-0.04, 0, w + i / 50);
			glVertex3f(-0.08, 0, w);
			glVertex3f(1.8, 0, w);
			glVertex3f(-0.08, 0, w - i / 50);
			glVertex3f(0.4, 0, w - i / 50);
			glEnd();
			glPopMatrix();
		}
	}
	else {
		glTranslatef(0, h + 0.5, 0);
		glRotatef(15, 1, 0, 0);
		for (double i = moons2; i < moone2 * 0.98; i += 0.5) {
			glPushMatrix();
			glRotatef(i - 10, 0, 1, 0);
			glBegin(GL_QUAD_STRIP);
			glVertex3f(-0.08, 0, w + i / 50);
			glVertex3f(-0.04, 0, w + i / 50);
			glVertex3f(-0.08, 0, w);
			glVertex3f(1.8, 0, w);
			glVertex3f(-0.08, 0, w - i / 50);
			glVertex3f(0.4, 0, w - i / 50);
			glEnd();
			glPopMatrix();
		}
	}
	glPopMatrix();
}
void mmmm() {
	glPushMatrix();
	glScalef(1.1, 1.1, 0.8);
	glColor3f(0, 0, 0);
	glPushMatrix();
	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 1.414 / 2, -0.1);
	glScalef(0.2, 1.414, 0.2);
	cube();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45, 1, 0, 0);
	glTranslatef(0, 1.414 / 2, 0.1);
	glScalef(0.2, 1.414, 0.2);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1 + 0.1 / 1.414, 0);
	glScalef(0.2, 0.2 / 1.414, 2);
	cube();
	glPopMatrix();

	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0.1, 0);
	glVertex3f(0, 1.02, -0.98);
	glVertex3f(0, 1.02, 0.98);
	glEnd();
	glPopMatrix();
}
void mouth(int mode) {
	double t, h;
	if (mode == 1) {
		t = 2.1;
		h = 2.6;
	}
	else {
		t = 3.35;
		h = 1.5;
	}
	if (mode == 1) {
		if (!f1 && !bo1 && !fly1 && !dead2) {
			double l1, l0;
			double h1, h0;
			h1 = 2 - 1.8;
			h0 = 2 - 2;
			double o = 0.017453293;
			l1 = tan(o) * 1.8;
			l0 = tan(o) * 2;
			glColor3f(0, 0, 0);
			glPushMatrix();
			glTranslatef(t, h, 0);
			glScalef(0.8, 1, 1);
			if (dead1)
				glScalef(1, -1, 1);
			for (double a = -25.0; a <= 25; a += 1) {
				glPushMatrix();
				glTranslatef(0, 2, 0);
				glRotatef(a, 2, 0, 0);
				glTranslatef(0, -2, 0);
				glBegin(GL_QUAD_STRIP);
				glVertex3f(0.17, h1, l1);
				glVertex3f(0.17, h1, -l1);
				glVertex3f(0.17, h0, l0);
				glVertex3f(0.17, h0, -l0);
				glVertex3f(0, h0, l0);
				glVertex3f(0, h0, -l0);
				glVertex3f(0, h1, l1);
				glVertex3f(0, h1, -l1);
				glVertex3f(0.18, h1, l1);
				glVertex3f(0.18, h1, -l1);
				glEnd();
				glPopMatrix();
			}
		}
		else {
			glPushMatrix();
			glTranslatef(t, h - 0.6, 0);
			mmmm();
			glPopMatrix();
		}
	}
	if (mode == 2) {
		if (!f2 && !bo2 && !fly2 && !dead1) {
			double l1, l0;
			double h1, h0;
			h1 = 2 - 1.8;
			h0 = 2 - 2;
			double o = 0.017453293;
			l1 = tan(o) * 1.8;
			l0 = tan(o) * 2;
			glColor3f(0, 0, 0);
			glPushMatrix();
			glTranslatef(t, h, 0);
			glScalef(0.8, 1, 1);
			if (dead2)
				glScalef(1, -1, 1);
			for (double a = -25.0; a <= 25; a += 1) {
				glPushMatrix();
				glTranslatef(0, 2, 0);
				glRotatef(a, 2, 0, 0);
				glTranslatef(0, -2, 0);
				glBegin(GL_QUAD_STRIP);
				glVertex3f(0.17, h1, l1);
				glVertex3f(0.17, h1, -l1);
				glVertex3f(0.17, h0, l0);
				glVertex3f(0.17, h0, -l0);
				glVertex3f(0, h0, l0);
				glVertex3f(0, h0, -l0);
				glVertex3f(0, h1, l1);
				glVertex3f(0, h1, -l1);
				glVertex3f(0.18, h1, l1);
				glVertex3f(0.18, h1, -l1);
				glEnd();
				glPopMatrix();
			}
		}
		else {
			glPushMatrix();
			glTranslatef(t, h - 0.6, 0);
			mmmm();
			glPopMatrix();
		}
	}
	glPopMatrix();
}
void eye(int mode) {
	double w, h, t;
	if (mode == 1) {
		w = 2.5;
		h = 4;
		t = 2.1;
	}
	else {
		w = 2;
		h = 3.5;
		t = 3.35;
	}
	if (!dead1 && mode == 1) {
		glColor3f(0, 0, 0);
		glPushMatrix();
		glTranslatef(t, h, w);
		glRotatef(90, 0, 0, 1);
		zhu(0.8, 0.8, 0.18, 16, 2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(t, h, -w);
		glRotatef(90, 0, 0, 1);
		zhu(0.8, 0.8, 0.18, 16, 2);
		glPopMatrix();
		glColor3f(1, 1, 1);
		glPushMatrix();
		glTranslatef(t + 0.05, h + 0.4, w + 0.2);
		glRotatef(90, 0, 0, 1);
		zhu(0.25, 0.25, 0.18, 16, 2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(t + 0.05, h + 0.4, -w + 0.2);
		glRotatef(90, 0, 0, 1);
		zhu(0.25, 0.25, 0.18, 16, 2);
		glPopMatrix();
	}
	else {
		glColor3f(0, 0, 0);
		glPushMatrix();
		glTranslatef(t, h, w);
		glRotatef(45, 1, 0, 0);
		glScalef(0.18, 1.6, 0.3);
		cube();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(t, h, w);
		glRotatef(45, 1, 0, 0);
		glScalef(0.18, 0.3, 1.6);
		cube();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(t, h, -w);
		glRotatef(45, 1, 0, 0);
		glScalef(0.18, 1.6, 0.3);
		cube();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(t, h, -w);
		glRotatef(45, 1, 0, 0);
		glScalef(0.18, 0.3, 1.6);
		cube();
		glPopMatrix();
	}
	if (!dead2 && mode == 2) {
		glColor3f(0, 0, 0);
		glPushMatrix();
		glTranslatef(t, h, w);
		glRotatef(90, 0, 0, 1);
		zhu(0.8, 0.8, 0.18, 16, 2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(t, h, -w);
		glRotatef(90, 0, 0, 1);
		zhu(0.8, 0.8, 0.18, 16, 2);
		glPopMatrix();
		glColor3f(1, 1, 1);
		glPushMatrix();
		glTranslatef(t + 0.05, h + 0.4, w + 0.2);
		glRotatef(90, 0, 0, 1);
		zhu(0.25, 0.25, 0.18, 16, 2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(t + 0.05, h + 0.4, -w + 0.2);
		glRotatef(90, 0, 0, 1);
		zhu(0.25, 0.25, 0.18, 16, 2);
		glPopMatrix();
	}
	else {
		glColor3f(0, 0, 0);
		glPushMatrix();
		glTranslatef(t, h, w);
		glRotatef(45, 1, 0, 0);
		glScalef(0.18, 1.6, 0.3);
		cube();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(t, h, w);
		glRotatef(45, 1, 0, 0);
		glScalef(0.18, 0.3, 1.6);
		cube();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(t, h, -w);
		glRotatef(45, 1, 0, 0);
		glScalef(0.18, 1.6, 0.3);
		cube();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(t, h, -w);
		glRotatef(45, 1, 0, 0);
		glScalef(0.18, 0.3, 1.6);
		cube();
		glPopMatrix();
	}
}

void wm1() {
	glPushMatrix();
	glTranslatef(0, -10, 0);
	double l1, l2, l3, l0;
	double h1, h2, h3, h0;
	h3 = 10 - 6;
	h2 = 10 - 8;
	h1 = 10 - 8.5;
	h0 = 10 - 10;
	double o = 0.017453293;
	l3 = tan(o) * 6;
	l2 = tan(o) * 8;
	l1 = tan(o) * 8.5;
	l0 = tan(o) * 10;
	glColor3ub(57, 128, 102);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(3, h1, l1);
	glVertex3f(3, h1, -l1);
	glVertex3f(3, h0, l0);
	glVertex3f(3, h0, -l0);
	glVertex3f(-3, h0, l0);
	glVertex3f(-3, h0, -l0);
	glVertex3f(-3, h1, l1);
	glVertex3f(-3, h1, -l1);
	glEnd();
	glColor3ub(89, 209, 79);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(3, h2, l2);
	glVertex3f(3, h2, -l2);
	glVertex3f(3, h1, l1);
	glVertex3f(3, h1, -l1);
	glVertex3f(-3, h1, l1);
	glVertex3f(-3, h1, -l1);
	glVertex3f(-3, h2, l2);
	glVertex3f(-3, h2, -l2);
	glEnd();
	glColor3ub(195, 83, 88);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(3, h3, l3);
	glVertex3f(3, h3, -l3);
	glVertex3f(3, h2, l2);
	glVertex3f(3, h2, -l2);
	glVertex3f(-3, h2, l2);
	glVertex3f(-3, h2, -l2);
	glVertex3f(-3, h3, l3);
	glVertex3f(-3, h3, -l3);
	glEnd();
	/*glColor3ub(255, 73, 55);
	glBegin(GL_TRIANGLES);
	glVertex3f(3, 10, 0);
	glVertex3f(3, h3, l3);
	glVertex3f(3, h3, -l3);
	glVertex3f(-3, 10, 0);
	glVertex3f(-3, h3, l3);
	glVertex3f(-3, h3, -l3);
	glEnd();*/
	glPopMatrix();
}
void wm2() {
	glPushMatrix();
	glTranslatef(0, -10, 0);
	double h3, l3, o;
	o = 0.017453293;
	h3 = 10 - 6;
	l3 = tan(o) * 6;
	glColor3ub(255, 73, 55);
	glBegin(GL_TRIANGLES);
	glVertex3f(3, 10, 0);
	glVertex3f(3, h3, l3);
	glVertex3f(3, h3, -l3);
	glVertex3f(-3, 10, 0);
	glVertex3f(-3, h3, l3);
	glVertex3f(-3, h3, -l3);
	glVertex3f(3, h3, l3);
	glVertex3f(3, h3, -l3);
	glVertex3f(-3, h3, l3);
	glVertex3f(-3, h3, l3);
	glVertex3f(-3, h3, -l3);
	glVertex3f(3, h3, -l3);
	glEnd();
	glPopMatrix();
}
void gua1() {
	glPushMatrix();
	glScalef(0.6666667, 1, 1);

	glPushMatrix();
	glTranslatef(0, 10, 0);
	for (double a = -40.0; a <= 40; a += 1.0) {
		glPushMatrix();
		glRotatef(a, 1, 0, 0);
		//wm1();
		wm2();
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 10, 0);
	glRotatef(-41, 1, 0, 0);
	glTranslatef(0, -10, 0);
	double h1, h2, h3, h0;
	h3 = 10 - 6;
	h2 = 10 - 8;
	h1 = 10 - 8.5;
	h0 = 10 - 10;
	glBegin(GL_QUADS);
	glColor3ub(255, 73, 55);
	glVertex3f(3, h3, 0);
	glVertex3f(-3, h3, 0);
	glVertex3f(-3, 10, 0);
	glVertex3f(3, 10, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 10, 0);
	glRotatef(41, 1, 0, 0);
	glTranslatef(0, -10, 0);
	glBegin(GL_QUADS);
	glColor3ub(255, 73, 55);
	glVertex3f(3, h3, 0);
	glVertex3f(-3, h3, 0);
	glVertex3f(-3, 10, 0);
	glVertex3f(3, 10, 0);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}
void gua2() {
	glPushMatrix();
	glScalef(0.6666667, 1, 1);

	glPushMatrix();
	glTranslatef(0, 10, 0);
	for (double a = -40.0; a <= 40; a += 1.0) {
		glPushMatrix();
		glRotatef(a, 1, 0, 0);
		wm1();
		//wm2();
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 10, 0);
	glRotatef(-41, 1, 0, 0);
	glTranslatef(0, -10, 0);
	double h1, h2, h3, h0;
	h3 = 10 - 6;
	h2 = 10 - 8;
	h1 = 10 - 8.5;
	h0 = 10 - 10;
	glBegin(GL_QUADS);
	glColor3ub(57, 128, 102);
	glVertex3f(3, h0, 0);
	glVertex3f(-3, h0, 0);
	glVertex3f(-3, h1, 0);
	glVertex3f(3, h1, 0);
	glColor3ub(89, 209, 79);
	glVertex3f(3, h2, 0);
	glVertex3f(-3, h2, 0);
	glVertex3f(-3, h1, 0);
	glVertex3f(3, h1, 0);
	glColor3ub(195, 83, 88);
	glVertex3f(3, h2, 0);
	glVertex3f(-3, h2, 0);
	glVertex3f(-3, h3, 0);
	glVertex3f(3, h3, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 10, 0);
	glRotatef(41, 1, 0, 0);
	glTranslatef(0, -10, 0);
	glBegin(GL_QUADS);
	glColor3ub(57, 128, 102);
	glVertex3f(3, h0, 0);
	glVertex3f(-3, h0, 0);
	glVertex3f(-3, h1, 0);
	glVertex3f(3, h1, 0);
	glColor3ub(89, 209, 79);
	glVertex3f(3, h2, 0);
	glVertex3f(-3, h2, 0);
	glVertex3f(-3, h1, 0);
	glVertex3f(3, h1, 0);
	glColor3ub(195, 83, 88);
	glVertex3f(3, h2, 0);
	glVertex3f(-3, h2, 0);
	glVertex3f(-3, h3, 0);
	glVertex3f(3, h3, 0);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}
void gua() {
	glPushMatrix();
	glTranslatef(melon_x / 10, 0, melon_z / 10);
	glRotatef(melon_face, 0, 1, 0);
	glScalef(0.8, 0.9, 0.9);
	glPushMatrix();

	glPushMatrix();		//上半块
	if (dead1) {
		if (gdead <= 45)
			glTranslatef(-gdead / 10, 1.3 * (gdead / (45 / 4)) + 0.02, 0);
		else
			glTranslatef(-gdead / 10, 1.3 * (-gdead * 2 / 45 + 6) + 0.02, 0);
		glRotatef(gdead / 9, 0, 1, 0);
		glRotatef(gdead, 0, 0, 1);
	}
	if (move1 && !bo1 && !f1) {
		glRotatef(walk1 * 6, 0, 1, 0);
		glRotatef(walk1 * 6, 1, 0, 0);
	}
	if (bo1) {
		glRotatef(boomerang1, 0, 1, 0);
	}
	if (f1)
		glRotatef(attack_move_melon / 8, 0, 1, 0);
	glTranslatef(0, 0.9, 0);
	gua1();
	glPopMatrix();

	glPushMatrix();
	if (dead1) {
		if (gdead <= 45)
			glTranslatef(-gdead / 30, gdead / (45 / 4) + 0.02, 0);
		else
			glTranslatef(-gdead / 30, -gdead * 2 / 45 + 6 + 0.02, 0);
		glRotatef(-gdead / 9, 0, 1, 0);
		glRotatef(gdead, 0, 0, 1);
	}
	glPushMatrix();
	if (move1 && !bo1 && !f1) {
		glRotatef(walk1 * 6, 0, 1, 0);
		glRotatef(walk1 * 6, 1, 0, 0);
	}
	if (bo1) {
		glRotatef(boomerang1, 0, 1, 0);
	}
	foot(1);
	if (f1)
		glRotatef(attack_move_melon / 8, 0, 1, 0);
	glTranslatef(0, 0.9, 0);
	eye(1);
	gua2();
	mouth(1);
	glPopMatrix();

	glPushMatrix();
	if (gdead2 >= 70)
		glTranslatef(0, jump2 / 3, -1 * jump2 / 10);
	lhand(1);
	glPopMatrix();
	rhand(1);
	if (!bfly1)
		b(1);
	bmoon(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void m1() {
	glPushMatrix();
	glTranslatef(0, 6, 0);
	glPushMatrix();

	glColor3ub(119, 185, 254);
	glPushMatrix();
	glTranslatef(3, 0, 0);
	glScalef(0.5, 4, 6.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3, 0, 0);
	glScalef(0.5, 4, 6.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 3);
	glScalef(6.5, 4, 0.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -3);
	glScalef(6.5, 4, 0.5);
	cube();
	glPopMatrix();

	double h = 6 / 1.414;
	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glScalef(0.5, h, 6.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3, 2 - 0.1, 0);
	glRotatef(-45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glScalef(0.5, h, 6.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1 + h, 0);
	glScalef(0.5, 1, 6.5);
	cube();
	glPopMatrix();

	glColor3ub(88, 112, 141);
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	glVertex3f(3, 2, 3);
	glVertex3f(0, 5, 0.5);
	glVertex3f(-3, 2, 3);
	glVertex3f(3, 2, -3);
	glVertex3f(0, 5, -0.5);
	glVertex3f(-3, 2, -3);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1 + h, 1.625);
	glScalef(0.52, 1.02, 0.4);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1 + h, -1.625);
	glScalef(0.52, 1.02, 0.4);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1 + h, 0);
	glScalef(0.52, 1.02, 0.4);
	cube();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(2.25, 1, 3.27);
	glVertex3f(-2.25, 1, 3.27);
	glVertex3f(2.25, 0.1, 3.27);
	glVertex3f(-2.25, 0.1, 3.27);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glVertex3f(2.25, -0.4, 3.27);
	glVertex3f(-2.25, -0.4, 3.27);
	glVertex3f(2.25, -1.3, 3.27);
	glVertex3f(-2.25, -1.3, 3.27);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glVertex3f(2, 1, -3.27);
	glVertex3f(-2, 1, -3.27);
	glVertex3f(2, -1.5, -3.27);
	glVertex3f(-2, -1.5, -3.27);
	glEnd();
	glPopMatrix();

	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, h / 2 - 0.2, 3.25);
	glScalef(1, 0.8, 1.2);
	shan(0, h / 2 - 0.2, 0, 90);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, h / 2 - 0.2, -0.2);
	glScalef(1, 0.6, 1);
	shan(0, h / 2 - 0.4, 270, 90);
	shan(0, h / 2 - 0.4, 0, 90);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, h / 2 - 0.2, -3.25);
	glScalef(1, 0.9, 1);
	shan(0, h / 2 - 0.4, 270, 90);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, h / 2 - 1, -1);
	glScalef(1, 0.9, 1);
	shan(0, h / 2 - 1, 270, 180 + 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, 0.2, 1);
	glScalef(1, 0.9, 1);
	shan(0, h / 8, 0, 360);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, -0.3, 2.3);
	glScalef(1, 0.9, 1);
	shan(0, h / 18, 0, 360);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUAD_STRIP);
	//glNormal3f();
	glVertex3f(3.26, 1.65, 2.9);
	glVertex3f(3.26, 1.65, -2.9);
	glVertex3f(3.26, -2, 2.9);
	glVertex3f(3.26, -2, -2.9);
	glEnd();
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(3.26, 1.65, 2.9);
	glVertex3f(3.26, 1.65, -2.9);
	glVertex3f(3.26, -2, 2.9);
	glVertex3f(3.26, -2, -2.9);
	glEnd();
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(3.26, 1.65, 2.9);
	glVertex3f(3.26, 1.65, -2.9);
	glVertex3f(3.26, -2, 2.9);
	glVertex3f(3.26, -2, -2.9);
	glEnd();
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(3.26, 1.65, 2.9);
	glVertex3f(3.26, 1.65, -2.9);
	glVertex3f(3.26, -2, 2.9);
	glVertex3f(3.26, -2, -2.9);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}
void m2() {
	glPushMatrix();
	glTranslatef(0, 2, 0);
	glPushMatrix();
	for (int i = 0; i < 5; i++) {
		glRotatef(90, 0, 1, 0);
		glPushMatrix();
		glColor3f(1, 1, 1);
		glBegin(GL_QUAD_STRIP);
		glVertex3f(3.26, 2, 2.9);
		glVertex3f(3.26, 2, -2.9);
		glVertex3f(3.26, -1.65, 2.9);
		glVertex3f(3.26, -1.65, -2.9);
		glEnd();
		glPushMatrix();
		glColor3ub(119, 185, 254);
		glTranslatef(0, 0, 3);
		glScalef(6.5, 4, 0.5);
		cube();
		glPopMatrix();
		glPopMatrix();
	}
	glRotatef(-90, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, -1.75, 0);
	glScalef(6, 0.5, 6);
	cube();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(2, 1.5, -3.27);
	glVertex3f(-2, 1.5, -3.27);
	glVertex3f(2, 0.6, -3.27);
	glVertex3f(-2, 0.6, -3.27);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(2, -0.4, -3.27);
	glVertex3f(-2, -0.4, -3.27);
	glVertex3f(2, -1.3, -3.27);
	glVertex3f(-2, -1.3, -3.27);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}
void m11() {
	glPushMatrix();
	glTranslatef(0, 6, 0);
	glPushMatrix();

	glColor3ub(119, 185, 254);
	double h = 6 / 1.414;
	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glScalef(0.5, h, 6.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3, 2 - 0.1, 0);
	glRotatef(-45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glScalef(0.5, h, 6.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1 + h, 0);
	glScalef(0.5, 1, 6.5);
	cube();
	glPopMatrix();

	glColor3ub(88, 112, 141);
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	glVertex3f(3, 2, 3);
	glVertex3f(0, 5, 0.5);
	glVertex3f(-3, 2, 3);
	glVertex3f(3, 2, -3);
	glVertex3f(0, 5, -0.5);
	glVertex3f(-3, 2, -3);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1 + h, 1.625);
	glScalef(0.52, 1.02, 0.4);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1 + h, -1.625);
	glScalef(0.52, 1.02, 0.4);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1 + h, 0);
	glScalef(0.52, 1.02, 0.4);
	cube();
	glPopMatrix();

	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, h / 2 - 0.2, 3.25);
	glScalef(1, 0.8, 1.2);
	shan(0, h / 2 - 0.2, 0, 90);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, h / 2 - 0.2, -0.2);
	glScalef(1, 0.6, 1);
	shan(0, h / 2 - 0.4, 270, 90);
	shan(0, h / 2 - 0.4, 0, 90);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, h / 2 - 0.2, -3.25);
	glScalef(1, 0.9, 1);
	shan(0, h / 2 - 0.4, 270, 90);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, h / 2 - 1, -1);
	glScalef(1, 0.9, 1);
	shan(0, h / 2 - 1, 270, 180 + 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, 0.2, 1);
	glScalef(1, 0.9, 1);
	shan(0, h / 8, 0, 360);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3, 2 - 0.1, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, h / 2, 0);
	glTranslatef(0.26, -0.3, 2.3);
	glScalef(1, 0.9, 1);
	shan(0, h / 18, 0, 360);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}
void m12() {
	glPushMatrix();
	glTranslatef(0, 6, 0);
	//glRotatef(180, 1, 0, 0);
	glPushMatrix();
	for (int i = 0; i < 5; i++) {
		glRotatef(90, 0, 1, 0);
		glPushMatrix();
		glColor3f(1, 1, 1);
		glBegin(GL_QUAD_STRIP);
		glVertex3f(3.26, 1.65, 2.9);
		glVertex3f(3.26, 1.65, -2.9);
		glVertex3f(3.26, 0.45 - 6, 2.9);
		glVertex3f(3.26, 0.45 - 6, -2.9);
		glEnd();
		glPushMatrix();
		glColor3ub(119, 185, 254);
		glTranslatef(0, -2, 3);
		glScalef(6.5, 8, 0.5);
		cube();
		glPopMatrix();
		glPopMatrix();
	}
	glRotatef(-90, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, -1.75, 0);
	glScalef(6, 0.5, 6);
	cube();
	glPopMatrix();

	glColor3ub(88, 112, 141);
	glPushMatrix();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(2.25, 1, 3.27);
	glVertex3f(-2.25, 1, 3.27);
	glVertex3f(2.25, 0.1, 3.27);
	glVertex3f(-2.25, 0.1, 3.27);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glVertex3f(2.25, -0.4, 3.27);
	glVertex3f(-2.25, -0.4, 3.27);
	glVertex3f(2.25, -1.3, 3.27);
	glVertex3f(-2.25, -1.3, 3.27);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glVertex3f(2, 1, -3.27);
	glVertex3f(-2, 1, -3.27);
	glVertex3f(2, -1.5, -3.27);
	glVertex3f(-2, -1.5, -3.27);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -4, 0);
	glColor3ub(119, 185, 254);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(2, 1.5, -3.27);
	glVertex3f(-2, 1.5, -3.27);
	glVertex3f(2, 0.6, -3.27);
	glVertex3f(-2, 0.6, -3.27);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(2, -0.4, -3.27);
	glVertex3f(-2, -0.4, -3.27);
	glVertex3f(2, -1.3, -3.27);
	glVertex3f(-2, -1.3, -3.27);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void milk1() {
	glPushMatrix();
	glTranslatef(milk_x / 10, 0, milk_z / 10);
	glRotatef(milk_face, 0, 1, 0);
	glScalef(0.9, 0.9, 0.9);
	m12();
	m11();
	glPopMatrix();
}
void milk() {
	glPushMatrix();
	glTranslatef(milk_x / 10, 0, milk_z / 10);
	glRotatef(milk_face, 0, 1, 0);
	glScalef(0.9, 0.9, 0.9);
	glPushMatrix();

	glPushMatrix();
	if (dead2) {
		if (gdead2 <= 45)
			glTranslatef(-gdead2 / 30, 1 * (gdead2 / (45 / 6.5)) + 0.02, 0);
		else
			glTranslatef(-gdead2 / 30, 1 * (-gdead2 * 3.25 / 45 + 9.75) + 0.02, 0);
		glRotatef(-gdead2 / 9, 0, 1, 0);
		glRotatef(gdead2, 0, 0, 1);
	}
	glPushMatrix();
	if (move2 && !bo1 && !f1) {
		glRotatef(walk2 * 6, 0, 1, 0);
		glRotatef(walk2 * 5, 1, 0, 0);
	}
	if (bo2) {
		glRotatef(boomerang2, 0, 1, 0);
	}
	foot(2);
	if (f2)
		glRotatef(attack_move_milk / 8, 0, 1, 0);
	glTranslatef(0, 1.2, 0);
	eye(2);
	if (!dead2) {
		m12();
		m11();
	}
	else {
		m2();
	}
	mouth(2);
	glPopMatrix();

	glPushMatrix();
	if (gdead >= 70)
		glTranslatef(0, jump1 / 3, -1 * jump1 / 10);
	lhand(2);
	glPopMatrix();
	rhand(2);
	if (!bfly2)
		b(2);

	glPushMatrix();
	glScalef(0.85, 1, 1);
	bmoon(2);
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();
	if (dead2) {
		if (gdead2 <= 45)
			glTranslatef(-gdead2 / 10, 1.3 * (gdead2 / (45 / 6.5)) + 0.02, 0);
		else
			glTranslatef(-gdead2 / 10, 1.3 * (-gdead2 * 3.25 / 45 + 9.75) + 0.02, 0);
		glRotatef(-gdead2 / 9, 0, 1, 0);
		glRotatef(gdead2, 0, 0, 1);
		glTranslatef(0, 1.2, 0);
		m1();
	}
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	particles_gua = new Particles("ParticleRunner.vert", "ParticleRunner.geom", "ParticleRender.vert", "ParticleRender.frag");
	particles_milk = new Particles("ParticleRunner.vert", "ParticleRunner.geom", "ParticleRender.vert", "ParticleRender.frag");
	particles_gua_dao = new Particles("ParticleRunner.vert", "ParticleRunner.geom", "ParticleRender.vert", "ParticleRender.frag");
	particles_milk_dao = new Particles("ParticleRunner.vert", "ParticleRunner.geom", "ParticleRender.vert", "ParticleRender.frag");
	light = new LightSpot();
	light->init("lightcube.vs", "lightcube.fs", "basiclight.vs", "basiclight.fs");
	glViewport(0, 0, width(), height());
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	makew();
	glDisable(GL_DEPTH_TEST);
}


QVector3D lightd;
bool down = false;
float flag1 = 1;
float flag2 = 1;
double Rad_to_deg = 45.0 / atan(1.0);
float theta = 17.5f / Rad_to_deg;
double pregx = 0, pregz = 0, premx = 0, premz = 0;
float sgn(float d) { return d < 0 ? -1 : d>0; }
bool incircle(double x, double y) {
	double r = (double)lightd.y() * tan(theta);
	if (sqrt(pow(x / 10 - lightd.x(), 2) + pow(y / 10 - lightd.z(), 2)) <= r - 5)
		return 1;
	return 0;
}
bool check(float x, float y){
	int a[] = { floor(x), ceil(x) };
	int b[] = { floor(y), ceil(y) };
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			if (a[i] < 0 || b[j] < 0 || a[i] > 1500 || b[j] > 1500 )return 0;
			if (down && !incircle(x, y))return 0;
			if (w[a[i]][b[j]] == 1)return 0;
		}
	}
	return 1;
}

void selfcheck(double& x, double& y) {    //x, y为玩家坐标，当玩家还在灯时将玩家往圈内推
	double sintheta = (y / 10 - lightd.z()) / 100, costheta = (x / 10 - lightd.x()) / 100;
	if(!incircle(x, y) && !dead1 && !dead2){
		y -= sintheta * 30;
		x -= costheta * 30;
	}
}
void MyGLWidget::paintGL()
{
	// Your Implementation
	down = light->down;
	lightd = QVector3D(light->lightxpos, light->lightypos, light->lightzpos);
	selfcheck(milk_x, milk_z);
	selfcheck(melon_x, melon_z);
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(1.0);
	glEnable(GL_LINE_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glLoadIdentity();
	gluPerspective(60 + posy * 1.2, 1.18, 5, 420);
	double xx, yy, zz, e2 = 0.436332;
	xx = 76 + posx;
	yy = 55 + posy;
	zz = 167 + posz;
	if (dead1) {
		gluLookAt(cos(e2 - gdead / 300) * cos(1.5708) * 50 + gdead * (milk_x / 10 - xx) / 90 + xx, 25 + sin(e2 - gdead / 300) * 50 + yy - gdead / 3, sin(1.5708) * cos(e2 - gdead / 300) * 50 + gdead * (milk_z / 10 - zz) / 90 + zz,
			gdead * (milk_x / 10 - xx) / 90 + xx, yy - gdead / 3, gdead * (milk_z / 10 - zz) / 90 + zz, 0, 1, 0);
	}
	else if (dead2) {
		gluLookAt(cos(e2 - gdead2 / 300) * cos(1.5708) * 50 + gdead2 * (melon_x / 10 - xx) / 90 + xx, 25 + sin(e2 - gdead2 / 300) * 50 + yy - gdead2 / 3, sin(1.5708) * cos(e2 - gdead2 / 300) * 50 + gdead2 * (melon_z / 10 - zz) / 90 + zz,
			gdead2 * (melon_x / 10 - xx) / 90 + xx, yy - gdead2 / 3, gdead2 * (melon_z / 10 - zz) / 90 + zz, 0, 1, 0);
	}
	else
		gluLookAt((cos(0.436332) * cos(1.5708) * 50 + 76 + posx), (25 + sin(0.436332) * 50 + 55 + posy), (sin(1.5708) * cos(0.436332) * 50 + 167 + posz),
			76 + posx, 55 + posy, 167 + posz, 0, 1, 0);
	posx = (melon_x + milk_x - 1500) / 150;
	posz = (melon_z + milk_z - 1500) / 300;
	posy = ((melon_z / 10 - milk_z / 10) * (melon_z / 10 - milk_z / 10) + (melon_x / 10 - milk_x / 10) * (melon_x / 10 - milk_x / 10) - 90 * 90 * 2) / 5000;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

	double o = 0.01745329;
	if (melon_face < 0)
		melon_face += 360;
	gbx = 0;
	gbz = 11;
	double gbx1 = gbx * cos(melon_face * o) + gbz * sin(melon_face * o);
	double gbz1 = -gbx * sin(melon_face * o) + gbz * cos(melon_face * o);
	double gbx2 = gbx1, gbz2 = gbz1;
	if (f1) {
		gbx2 = gbx1 * cos(attack_move_melon * o) + gbz1 * sin(attack_move_melon * o);
		gbz2 = -gbx1 * sin(attack_move_melon * o) + gbz1 * cos(attack_move_melon * o);
	}
	if (bfly1) {
		fly1 += double(fly_direction_melon) * 5;
		fhurt1 = 1.5;
		if (fly1 >= 100) {
			fly_direction_melon = -1;
		}
		if (fly1 < 0) {
			fly1 = 0;
			fly_direction_melon = 1;
			fhurt1 = 0;
			bfly1 = 0;
		}
		double ggx = fly1, ggz = 0, ggx1, ggz1;
		ggx1 = ggx * cos(melon_face * o) + ggz * sin(melon_face * o);
		ggz1 = -ggx * sin(melon_face * o) + ggz * cos(melon_face * o);
		gbx2 = ggx1 * 0.8;
		gbz2 = ggz1 * 0.8;

		double nowgbx = (gbx2 + melon_x / 10) * 10, nowgbz = (gbz2 + melon_z / 10) * 10;
		if (!check(nowgbx, nowgbz)) fly_direction_melon = -1;
	}

	if (milk_face < 0)
		milk_face += 360;
	mbx = 0;
	mbz = 8;
	double mbx1 = mbx * cos(milk_face * o) + mbz * sin(milk_face * o);
	double mbz1 = -mbx * sin(milk_face * o) + mbz * cos(milk_face * o);
	double mbx2 = mbx1, mbz2 = mbz1;
	if (f2) {
		mbx2 = mbx1 * cos(attack_move_milk * o) + mbz1 * sin(attack_move_milk * o);
		mbz2 = -mbx1 * sin(attack_move_milk * o) + mbz1 * cos(attack_move_milk * o);
	}
	if (bfly2) {
		fhurt2 = 1.5;
		fly2 += double(fly_direction_milk) * 5;
		if (fly2 >= 100) {
			fly_direction_milk = -1;
		}
		if (fly2 < 0) {
			fly2 = 0;
			fly_direction_milk = 1;
			fhurt2 = 0;
			bfly2 = 0;
		}
		double mmx = fly2, mmz = 0, mmx1, mmz1;
		mmx1 = mmx * cos(milk_face * o) + mmz * sin(milk_face * o);
		mmz1 = -mmx * sin(milk_face * o) + mmz * cos(milk_face * o);
		mbx2 = mmx1 * 0.8;
		mbz2 = mmz1 * 0.8;

		double nowmbx = (mbx2 + milk_x / 10) * 10, nowmbz = (mbz2 + milk_z / 10) * 10;
		if (!check(nowmbx, nowmbz)) fly_direction_milk = -1;
	}


	if (isup == 0 && isdown == 0 && isleft == 0 && isright == 0)
		move1 = 0;
	else
		move1 = 1;
	if (isw == 0 && iss == 0 && isa == 0 && isd == 0)
		move2 = 0;
	else
		move2 = 1;
	if (move1 && !bfly1 && !dead2) {
		walk1 += double(foot_direction_melon) * 0.1;
		if (walk1 >= 1.0) {
			foot_direction_melon = -1;
		}
		else if (walk1 <= -1.0) {
			foot_direction_melon = 1;
		}
		int now = isup * 8 + isdown * 4 + isleft * 2 + isright * 1;
		if (now == 10) {
			melon_face = -225;
			melon_x -= 4;
			melon_z -= 4;
			if ((melon_x <= 5 || melon_x >= 1495) || !check(melon_x, melon_z)) {
				melon_x += 4;
				melon_z += 4;
			}
		}
		if (now == 5) {
			melon_face = -45;
			melon_x += 4;
			melon_z += 4;
			if ((melon_x <= 5 || melon_x >= 1495) || !check(melon_x, melon_z)) {
				melon_x -= 4;
				melon_z -= 4;
			}
		}
		if (now == 6) {
			melon_face = -135;
			melon_x -= 4;
			melon_z += 4;
			if ((melon_x <= 5 || melon_x >= 1495) || !check(melon_x, melon_z)) {
				melon_x += 4;
				melon_z -= 4;
			}
		}
		if (now == 9) {
			melon_face = 45;
			melon_x += 4;
			melon_z -= 4;
			if ((melon_x <= 5 || melon_x >= 1495) || !check(melon_x, melon_z)) {
				melon_x -= 4;
				melon_z += 4;
			}
		}
		if (now == 2) {
			melon_face = -180;
			melon_x -= 5;
			melon_z += 0;
			if ((melon_x <= 5 || melon_x >= 1495) || !check(melon_x, melon_z)) {
				melon_x += 5;
			}
		}
		if (now == 8) {
			melon_face = 90;
			melon_x += 0;
			melon_z -= 5;
			if ((melon_z <= 5 || melon_z >= 1495) || !check(melon_x, melon_z)) {
				melon_z += 5;
			}
		}
		if (now == 4) {
			melon_face = -90;
			melon_x -= 0;
			melon_z += 5;
			if ((melon_z <= 5 || melon_z >= 1495) || !check(melon_x, melon_z)) {
				melon_z -= 5;
			}
		}
		if (now == 1) {
			melon_face = 0;
			melon_x += 5;
			melon_z += 0;
			if (melon_x <= 5 || melon_x >= 1495 || !check(melon_x, melon_z)) {
				melon_x -= 5;
			}
		}
	}
	if (move2 && !bfly2 && !dead1) {
		walk2 += foot_direction_milk * 0.1;
		if (walk2 >= 1.0) {
			foot_direction_milk = -1;
		}
		else if (walk2 <= -1.0) {
			foot_direction_milk = 1;
		}
		int now = isw * 8 + iss * 4 + isa * 2 + isd * 1;
		if (now == 10) {
			milk_face = -225;
			milk_x -= 4;
			milk_z -= 4;
			if ((milk_x <= 5 || milk_x >= 1495) || !check(milk_x, milk_z)) {
				milk_x += 4;
				milk_z += 4;

			}
		}
		if (now == 5) {
			milk_face = -45;
			milk_x += 4;
			milk_z += 4;
			if ((milk_x <= 5 || milk_x >= 1495) || !check(milk_x, milk_z)) {
				milk_x -= 4;
				milk_z -= 4;

			}
		}
		if (now == 6) {
			milk_face = -135;
			milk_x -= 4;
			milk_z += 4;
			if ((milk_x <= 5 || milk_x >= 1495) || !check(milk_x, milk_z)) {
				milk_x += 4;
				milk_z -= 4;

			}
		}
		if (now == 9) {
			milk_face = 45;
			milk_x += 4;
			milk_z -= 4;
			if ((milk_x <= 5 || milk_x >= 1495) || !check(milk_x, milk_z)) {
				milk_x -= 4;
				milk_z += 4;

			}
		}
		if (now == 2) {
			milk_face = -180;
			milk_x -= 5;
			milk_z += 0;
			if ((milk_x <= 5 || milk_x >= 1495) || !check(milk_x, milk_z)) {
				milk_x += 5;
				milk_z -= 0;

			}
		}
		if (now == 8) {
			milk_face = 90;
			milk_x += 0;
			milk_z -= 5;
			if ((milk_x <= 5 || milk_x >= 1495) || !check(milk_x, milk_z)) {
				milk_x -= 0;
				milk_z += 5;

			}
		}
		if (now == 4) {
			milk_face = -90;
			milk_x -= 0;
			milk_z += 5;
			if ((milk_x <= 5 || milk_x >= 1495) || !check(milk_x, milk_z)) {
				milk_x += 0;
				milk_z -= 5;

			}
		}
		if (now == 1) {
			milk_face = 0;
			milk_x += 5;
			milk_z += 0;
			if ((milk_x <= 5 || milk_x >= 1495) || !check(milk_x, milk_z)) {
				milk_x -= 5;
				milk_z += 0;

			}
		}
	}
	if (f1) {
		if (flagfight == 0) {
			attack_move_melon += 15;
			fhurt1 = 1;
		}
		if (flagfight == 0 && attack_move_melon >= 160 + 15) {
			fightlast += 1;
			fhurt1 = 0;
			attack_move_melon -= 15;
		}
		if (fightlast == 14) {
			flagfight = 1;
			fightlast = 0;
		}
		if (flagfight == 1) {
			attack_move_melon -= 7.5;
		}
		if (flagfight == 1 && attack_move_melon == 0) {
			flagfight = 0;
			f1 = 0;
		}
	}
	if (f2) {
		if (flagfight2 == 0) {
			attack_move_milk += 15;
			fhurt2 = 1;
		}
		if (flagfight2 == 0 && attack_move_milk >= 160 + 15) {
			fightlast2 += 1;
			fhurt2 = 0;
			attack_move_milk -= 15;
		}
		if (fightlast2 == 14) {
			flagfight2 = 1;
			fightlast2 = 0;
		}
		if (flagfight2 == 1) {
			attack_move_milk -= 7.5;
		}
		if (flagfight2 == 1 && attack_move_milk == 0) {
			flagfight2 = 0;
			f2 = 0;
		}
	}
	if (moon1) {
		if (flagmoon == 0) {
			moone += 15;
		}
		if (flagmoon == 0 && moone >= 160 + 15) {
			moonlast += 1;
			moone -= 15;
		}
		if (moonlast == 8) {
			flagmoon = 1;
			moonlast = 0;
		}
		if (flagmoon == 1) {
			moons += 165 / 6;
			//moone -= 7.5;
		}
		if (flagmoon == 1 && moons >= moone) {
			flagmoon = 0;
			moone = 0;
			moons = 0;
			moon1 = 0;
		}
	}
	if (moon2) {
		if (flagmoon2 == 0) {
			moone2 += 15;
		}
		if (flagmoon2 == 0 && moone2 >= 160 + 15) {
			moonlast2 += 1;
			moone2 -= 15;
		}
		if (moonlast2 == 8) {
			flagmoon2 = 1;
			moonlast2 = 0;
		}
		if (flagmoon2 == 1) {
			moons2 += 165 / 6;
			//moone -= 7.5;
		}
		if (flagmoon2 == 1 && moons2 >= moone2) {
			flagmoon2 = 0;
			moone2 = 0;
			moons2 = 0;
			moon2 = 0;
		}
	}
	if (bo1) {
		boomerang1 += double(flagbo) * 0.5;
		if (boomerang1 <= -30) {
			flagbo = 10;
		}
		else if (boomerang1 >= 40) {
			flagbo = -2;
			flagb = 1;
			bfly1 = 1;
		}
		else if (flagb == 1 && boomerang1 <= 0) {
			flagb = 0;
			flagbo = -6;
			bo1 = 0;
		}
	}
	if (bo2) {
		boomerang2 += double(flagbo2) * 0.5;
		if (boomerang2 <= -30) {
			flagbo2 = 10;
		}
		else if (boomerang2 >= 40) {
			flagbo2 = -2;
			flagb2 = 1;
			bfly2 = 1;
		}
		else if (flagb2 == 1 && boomerang2 <= 0) {
			flagb2 = 0;
			flagbo2 = -6;
			bo2 = 0;
		}
	}

	double nowgbx = gbx2 + melon_x / 10, nowgbz = gbz2 + melon_z / 10;
	if (bfly1) {
		glPushMatrix();
		glTranslatef(nowgbx, 6, nowgbz);
		glRotatef(fly1 * 50, 0, 1, 0);
		glPushMatrix();
		glColor3ub(251, 189, 198);
		glTranslatef(0, -1, 3);
		glRotatef(90, 0, 0, 1);
		shan(0, 5, 270, 90);
		glPopMatrix();
		biao(1);
		glPopMatrix();
	}
	if (fhurt1 > 0) {
		double d = (milk_x / 10 - nowgbx) * (milk_x / 10 - nowgbx) + (milk_z / 10 - nowgbz) * (milk_z / 10 - nowgbz);
		if (d <= 3.5 * 3.5 * fhurt1 * fhurt1) {
			dead2 = 1;
			fhurt2 = 0;
		}
	}

	double nowmbx = mbx2 + milk_x / 10, nowmbz = mbz2 + milk_z / 10;
	if (bfly2) {
		glPushMatrix();
		glTranslatef(nowmbx, 6, nowmbz);
		glRotatef(fly2 * 50, 0, 1, 0);
		glPushMatrix();
		glColor3ub(188, 220, 251);
		glTranslatef(0, -1, 3);
		glRotatef(90, 0, 0, 1);
		shan(0, 5, 270, 90);
		glPopMatrix();
		biao(2);
		glPopMatrix();
	}
	if (fhurt2 > 0) {
		double d = (melon_x / 10 - nowmbx) * (melon_x / 10 - nowmbx) + (melon_z / 10 - nowmbz) * (melon_z / 10 - nowmbz);
		if (d <= 3.5 * 3.5 * fhurt2 * fhurt2) {
			dead1 = 1;
			fhurt1 = 0;
		}
	}
	if (dead1) {
		f1 = 0;
		bo1 = 0;
		move1 = 0;
		moon1 = 0;
		if (gdead == 0)
			pref1 = milk_face - 180;
		melon_face = pref1;
		walk1 = 0, attack_move_melon = 0, boomerang1 = 0, moon1 = 0;
		moons = 0, moone = 0, moonlast = 0;
		gdead += double(flagdead) * 1;
		if (gdead >= 90)
			flagdead = 0;
		
	}
	if (dead2) {
		f2 = 0;
		bo2 = 0;
		move2 = 0;
		moon2 = 0;
		if (gdead2 == 0)
			pref2 = melon_face - 180;
		milk_face = pref2;
		walk2 = 0, attack_move_milk = 0, boomerang2 = 0, moon2 = 0;
		moons2 = 0, moone2 = 0, moonlast2 = 0;
		gdead2 += double(flagdead2) * 1;
		if (gdead2 >= 90)
			flagdead2 = 0;
		
	}
	glPushMatrix();
	if (gdead >= 70) {
		milk_face = -90;
		jump1 += win1 * 0.4;
		if (jump1 >= 10)
			win1 = -1;
		if (jump1 <= 0)
			win1 = 1;
		glTranslatef(milk_x / 10, jump1, milk_z / 10);
		glRotatef(-jump1*2, 1, 0, 0);
		glRotatef(jump1, 0, 0, 1);
		glTranslatef(-milk_x / 10, 0, -milk_z / 10);
	}
	milk();
	glPopMatrix();

	if (gdead2 >= 70) {
		melon_face = -90;
		jump2 += win2 * 0.4;
		if (jump2 >= 10)
			win2 = -1;
		if (jump2 <= 0)
			win2 = 1;
		glTranslatef(melon_x / 10, jump2, melon_z / 10);
		glRotatef(-jump2 * 2, 1, 0, 0);
		glRotatef(jump2, 0, 0, 1);
		glTranslatef(-melon_x / 10, 0, -melon_z / 10);
	}
	gua();
	glPopMatrix();
	init();
	world();
	

	QVector3D viewpos = QVector3D((cos(0.436332) * cos(1.5708) * 50 + 76), (25 + sin(0.436332) * 50 + 55), (sin(1.5708) * cos(0.436332) * 50 + 167));


	QMatrix4x4 m1, m2, m3;
	m1.perspective(60 + posy * 1.2, 1.18, 5, 420);
	
	if (dead2) {
		m2.lookAt(QVector3D(cos(e2 - gdead2 / 300)* cos(1.5708) * 50 + gdead2 * (melon_x / 10 - xx) / 90 + xx, 25 + sin(e2 - gdead2 / 300) * 50 + yy - gdead2 / 3, sin(1.5708)* cos(e2 - gdead2 / 300) * 50 + gdead2 * (melon_z / 10 - zz) / 90 + zz),
			QVector3D(gdead2* (melon_x / 10 - xx) / 90 + xx, yy - gdead2 / 3, gdead2* (melon_z / 10 - zz) / 90 + zz), QVector3D(0, 1, 0));
		double nowgbx = gbx2 + melon_x / 10, nowgbz = gbz2 + melon_z / 10;
		m3.translate(float(nowgbx), 1 + jump2, float(nowgbz));

		QVector3D milk_color = QVector3D(249.0 / 255, 247.0 / 255, 236.0 / 255);
		particles_gua_dao->use(m1 * m2 * m3, milk_color, QVector3D(((rand() % 1000) / 1000.0 - 0.5) * 0.2, flag2 * 0.03, ((rand() % 1000) / 1000.0 - 0.5) * 0.2));
		if (!(nowgbx != pregx || nowgbz != pregz)) flag2 = -1;
		m3.setToIdentity();
		m3.translate(milk_x / 10, 1, milk_z / 10);
		m3.rotate(milk_face - 180, 0, 1, 0);
		m3.translate(10.0, 0, 0);
		particles_milk->use(m1 * m2 * m3, milk_color, QVector3D(((rand() % 1000) / 1000.0 - 0.5) * 0.2, 0.01, ((rand() % 1000) / 1000.0 - 0.5) * 0.2));
		m3.setToIdentity();
		light->down = false;
		light->lightypos = 50;
		light->lightxpos = melon_x / 10;
		light->lightzpos = melon_z / 10;
	}

	
	if (dead1) {
		m2.lookAt(QVector3D(cos(e2 - gdead / 300)* cos(1.5708) * 50 + gdead * (milk_x / 10 - xx) / 90 + xx, 25 + sin(e2 - gdead / 300) * 50 + yy - gdead / 3, sin(1.5708)* cos(e2 - gdead / 300) * 50 + gdead * (milk_z / 10 - zz) / 90 + zz),
			QVector3D(gdead* (milk_x / 10 - xx) / 90 + xx, yy - gdead / 3, gdead* (milk_z / 10 - zz) / 90 + zz), QVector3D(0, 1, 0));
		m3.translate(float(nowmbx), 1 + jump1, float(nowmbz));
		QVector3D gua_color = QVector3D(255.0 / 255, 73.0 / 255, 55.0 / 255);
		particles_milk_dao->use(m1* m2* m3, gua_color, QVector3D(((rand() % 1000) / 1000.0 - 0.5) * 0.2, flag1 * 0.03, ((rand() % 1000) / 1000.0 - 0.5) * 0.2));
		if (nowmbx == premx && nowmbz == premz) flag1 = -1;
		m3.setToIdentity();
		m3.translate(melon_x / 10, 1, melon_z / 10);
		m3.rotate(melon_face - 180, 0, 1, 0);
		m3.translate(3.0, 0, 0);
		particles_gua->use(m1 * m2 * m3, gua_color, QVector3D(((rand() % 1000) / 1000.0 - 0.5) * 0.2, 0.01, ((rand() % 1000) / 1000.0 - 0.5) * 0.2));
		m3.setToIdentity();
		light->down = false;
		light->lightypos = 50;
		light->lightxpos = milk_x / 10;
		light->lightzpos = milk_z / 10;
	}
	premx = mbx2 + milk_x / 10;
	premz = mbz2 + milk_z / 10;
	pregx = gbx2 + melon_x / 10;
	pregz = gbz2 + melon_z / 10;
	light->use(m1, m2, viewpos, start);
}


/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}
