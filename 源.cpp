#include "Solve.h"
#include "Calc.h"
#include "Display.h"
#include "Recorder.h"
#include <GL/glut.h>
#include <Windows.h>
using namespace std;
using namespace cv;



DWORD WINAPI proc( LPVOID lpParam) 
{
	Display d;
	d.display();
    return 0; 
} 


Solve s1,s2;   
Calc c(0);
Recorder r;

const int mouse_left[5][2]={27,21,141,21,27,130,141,130,486,284};
const int mouse_right[5][2]={127,114,241,114,127,223,241,223,599,329};
bool isStart;
int song;
IplImage* Main[100],*choose;

void restart(){
	cvDestroyWindow("Main");
	cvDestroyWindow("image1");
	cvDestroyWindow("image2");
	glutShowWindow();
	
	memset(c.st,0,sizeof(c.st));
	r.init(song);
	c.group=r.group;
	
	mess.pos=r.pos;
	mess.start=r.start;
	mess.p=r.p;
	mess.step=10;
}
void correct1(){
	int k=0,count=0;
	double tmp[5][100],tmp1;
	while(1){
		s1.getimage();
		s2.getimage();
		s1.work();
		s2.work();
		//c.transfer(s2,s1);
		memset(c.judge,0,sizeof(c.judge));
		for(int i=0;i<5;i++)
			tmp[i][count]=c.v[i][2];
		count++;
		if(count==100){
			count=0,tmp1=0;
			for(int i=0;i<5;i++){
				sort(tmp[i],tmp[i]+100);
				for(int j=30;j<70;j++)
					tmp1+=tmp[i][j];
				tmp1/=80;
				c.judge[i]+=tmp1;
			}
			break;
		}
		cvWaitKey(10);
	}
	s1.closewindow();
	s2.closewindow();
}
void correct2(){
	int k=0,count=0;
	double tmp[5][100],tmp1;
	while(1){
		s1.getimage();
		s2.getimage();
		s1.work();
		s2.work();
		//c.transfer(s2,s1);
		//memset(c.judge,0,sizeof(c.judge));
		for(int i=0;i<5;i++)
			tmp[i][count]=c.v[i][2];
		count++;
		if(count==100){
			count=0,tmp1=0;
			for(int i=0;i<5;i++){
				sort(tmp[i],tmp[i]+100);
				for(int j=30;j<70;j++)
					tmp1+=tmp[i][j];
				tmp1/=80;
				c.judge[i]+=tmp1;
			}
			break;
		}
		cvWaitKey(10);
	}
	s1.closewindow();
	s2.closewindow();
}
void message(int event, int x, int y,int flags, void* param){
	if(event==CV_EVENT_LBUTTONDOWN){
		if( mouse_left[0][0]<x && x<mouse_right[0][0] && mouse_left[0][1]<y && y<mouse_right[0][1]){
			song=0;
			restart();
		}else if( mouse_left[1][0]<x && x<mouse_right[1][0] && mouse_left[1][1]<y && y<mouse_right[1][1]){
			song=1;
			restart();
		}else if( mouse_left[2][0]<x && x<mouse_right[2][0] && mouse_left[2][1]<y && y<mouse_right[2][1]){
			song=2;
			restart();
		}else if( mouse_left[3][0]<x && x<mouse_right[3][0] && mouse_left[3][1]<y && y<mouse_right[3][1]){
			song=3;
			restart();
		}else if(mouse_left[4][0]<x && x<mouse_right[4][0] && mouse_left[4][1]<y && y<mouse_right[4][1]){
			choose=cvLoadImage("1.png");
			cvShowImage("Main",choose);
			cvWaitKey(5000);
			
			choose=cvLoadImage("2.png");
			cvShowImage("Main",choose);
			correct1();
			cvWaitKey(5000);

			choose=cvLoadImage("3.png");
			cvShowImage("Main",choose);
			correct2();
			cvWaitKey(5000);

			choose=cvLoadImage("4.png");
			cvShowImage("Main",choose);
			cvWaitKey(5000);
			
			choose=cvLoadImage("choose.jpg");
			cvShowImage("Main",choose);
		}
	}
}

int main(int argc,char ** argv){

	/*s1.init(1,31,300,42,300,121,300); //right
    s2.init(2,52,300,158,300,31,212); //left
	while(1){
		s1.getimage();
        s2.getimage();
        s1.work();
        s2.work();
		c.transfer(s2,s1);
		mess.xs0=c.v[3][0],mess.ys0=c.v[3][1],mess.zs0=c.v[3][2],
		mess.xz0=c.v[2][0],mess.yz0=c.v[2][1],mess.zz0=c.v[2][2],
		mess.xw0=c.v[1][0],mess.yw0=c.v[1][1],mess.zw0=c.v[1][2],
		mess.xs1=c.v[7][0],mess.ys1=c.v[7][1],mess.zs1=c.v[7][2],
		mess.xz1=c.v[6][0],mess.yz1=c.v[6][1],mess.zz1=c.v[6][2],
		mess.xw1=c.v[5][0],mess.yw1=c.v[5][1],mess.zw1=c.v[5][2];
//
		//r.clicked(c.transfer(s2,s1));
		cvWaitKey(10);
	}*/
	


	/*s1.init(1,31,300,42,300,121,300); //right
    s2.init(2,52,300,158,300,31,212); //left
	while(1){
		s1.getimage();
        s2.getimage();
        s1.work();
        s2.work();
		c.transfer(s2,s1);
		printf("%lf %lf %lf %lf %lf\n",c.v[0][2],c.v[1][2],c.v[2][2],c.v[3][2],c.v[4][2]);
		//r.clicked(c.transfer(s2,s1));
		cvWaitKey(10);
	}*/



	
	//opengl ui
	

	//开场动画
	cvNamedWindow("Main");
	Main[0]=cvLoadImage("Main.jpg");
	for(int i=0;i<99;i++){
		Main[i+1]=cvCreateImage(cvGetSize(Main[0]),Main[0]->depth,Main[0]->nChannels);
		cvSmooth(Main[i],Main[i+1]);
		cvShowImage("Main",Main[i]);
		cvWaitKey(10);
	}
	for(int i=99;i>=0;i--){
		cvShowImage("Main",Main[i]);
		cvWaitKey(10);
	}
	cvWaitKey(500);


	HWND hwnd=(HWND)cvGetWindowHandle("Main");
	int mess_result=MessageBoxA(hwnd,"请确认你装载了双目摄像头\n","提示",1);
	if(mess_result==2)
		return -1;



	glutInit(&argc, argv);
	CreateThread(NULL,0,proc,NULL,0,NULL);

	s1.init(1,31,300,42,300,121,300); //right
    s2.init(2,52,300,158,300,31,212); //left

	cvSetMouseCallback("Main",message);
	choose=cvLoadImage("choose.jpg");
	cvShowImage("Main",choose);
	cvWaitKey(0);

	


	//r.over=true;
	while(1){
		//printf("i'm here");
		s1.getimage();
        s2.getimage();
        s1.work();
        s2.work();
		r.clicked(c.transfer(s2,s1));
		
		//printf("%lf %lf %lf %lf %lf\n",c.v[0][2],c.v[1][2],c.v[2][2],c.v[3][2],c.v[4][2]);
		if(r.over){
			//结束
			glutHideWindow();
			cvNamedWindow("Main");
			cvSetMouseCallback("Main",message);
			cvShowImage("Main",choose);
			//printf("why!");
			cvWaitKey(0);
		}
		c.group=r.group;

		mess.dis=false;
		if(!s2.p && !s1.p){
			for(int i=0;i<5;i++)
				if(c.v[i][2]<c.judge[i])
					mess.z[i]=1;
				else mess.z[i]=0;
			mess.dis=true;
		}

		for(int i=0;i<7;i++)
			mess.st[i]=c.st[i];
		for(int i=0;i<5;i++)
			mess.v[i][2]=c.v[i][2];

		mess.start=r.start;
		mess.pos=r.pos;
		mess.ani=255;
		//glutPostRedisplay();

		cvWaitKey(10);
	}
//	printf("the end\n");

	return 0;
    system("pause");
}
