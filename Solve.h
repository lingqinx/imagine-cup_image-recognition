#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <iostream>
#include <algorithm>
#include <cv.h>
#include <highgui.h> 
#include <WinBase.h>
#include <time.h>
using namespace std;
using namespace cv;

const int weight=800 ,height=600;

struct node{
    int i,j;
};

class Solve
{
public:
	IplImage* image,*binary,*dst,*hsv,*binary1;
    unsigned char (*data)[weight];
    unsigned char processing[height][weight];
    CvCapture* capture;
    int id;  //…„œÒÕ∑±‡∫≈ 
    int Y_min,Y_max,Cr_min,Cr_max,Cb_min,Cb_max,count_threshold,step;
    double cos_threshold;
    int ans[200][2],p,point_count;
    double point[10][2];
    string s_track,s_image;
	Solve(void);
	~Solve(void);
	void closewindow();
	void control();
	void calc(int start,double cos_threshold,int step,int count_threshold);
	void work();
	void imageprocess();
	void getimage();
	void init(int num,int Y_min,int Y_max,int Cr_min,int Cr_max,int Cb_min,int Cb_max);
	bool getpoint(int start_i,int start_j,bool vt[height][weight]);
	void ranse(int ui,int uj,bool vt[height][weight]);
};

