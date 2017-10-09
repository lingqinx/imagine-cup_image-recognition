#pragma once
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>


struct mess_data{
	char *p;
	int pos,start,step;
	bool st[8],z[5];
	double v[5][3];
	bool exi,dis;
	unsigned char ani;
	double xs0,ys0,zs0,
		xs1,ys1,zs1,
	xz0,yz0,zz0,
	xz1,yz1,zz1,
	xw0,yw0,zw0,
	xw1,yw1,zw1;
};
extern struct mess_data mess;
void myDisplay();
void myReshape(int w,int h);
class Display
{
public:
	
	void display();
	Display(void);
	~Display(void);
};

