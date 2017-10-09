#pragma once
#include <cv.h>
#include "Solve.h"
#include <string.h>
using namespace cv;

DWORD WINAPI func(LPVOID lpParam); 
const double ml11= 1012.43460,ml12=0,ml13=591.34764,ml14=0,
       ml21=0,ml22=	1011.19337 ,ml23=352.59097,ml24=0,
       ml31=0,ml32=0,ml33=1,ml34=0;                  
const double mr11=  1128.18290834138 ,mr12= -21.82563346788,mr13=320.83134534766,mr14= 129528.732504853,
       mr21=  128.48031639736      ,mr22= 1022.16713265336,mr23=383.56732643537,mr24=-6925.5375743761,
       mr31=0.242816,mr32=0.016572 ,mr33=0.969931,mr34=7.61599;
const char tone[3][7][10]={"1do.wav","1re.wav","1mi.wav","1fa.wav","1sol.wav","1la.wav","1xi.wav",
					"2do.wav","2re.wav","2mi.wav","2fa.wav","2sol.wav","2la.wav","2xi.wav",
					"3do.wav","3re.wav","3mi.wav","3fa.wav","3sol.wav","3la.wav","3xi.wav"};



class Calc
{
public:
	double v[8][3],judge[5],area[3][2];
	bool st[7];
	char group;
	Calc(char);
	~Calc(void);
	bool clicked(int i);
	double calc(int p,double ul,double vl,double ur,double vr);			
	int transfer(Solve &l,Solve &r); //¼ÆËã³ö
	
};

