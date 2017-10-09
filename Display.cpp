#include "Display.h"
#include <windows.h>
#include <GL/glut.h>
#include <GL/GLAUX.H>
#include <GL/GLU.h>
#include <GL/GL.H>
#include <GL/freeglut.h>
#include <time.h>  
#include <stdio.h>					// 标准输入输出库头文件
#define n_pi 57.2957805
#define MAX_CHAR 128
#define WindowWidth 800 
#define WindowHeight 800 
#define WindowTitle "OpenGL纹理测试"
GLuint	texture[1];		// Storage for 1 textures
#define BMP_Header_Length 54

const double Correct_Line=30;
struct mess_data mess;
int len[3]={26,45,46};
struct  thetaa{
double theta1;
double theta2;
double theta3;
};
struct thetaa theta[3][46];
struct  table{
 double x;
 double z;      
};
struct table Table[3][46];




typedef struct _IMAGE
{
    GLuint x;
    GLuint y;
    unsigned char *data;
}IMAGE;
void selectFont(int size, int charset, const char* face) {
    HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
        charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
    HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
    DeleteObject(hOldFont);
}

GLUquadricObj *quadObj1;
GLfloat light_ambient[] = {1.0,1.0,1.0,0.0};
GLfloat light_diffuse[] = {1.0,1.0,1.0,1.0};
GLfloat light_position[] = {0.0,0.0,1.0,1.0};
GLfloat back_mat_ambient[] = {0.01,0.01,0.01,1.0};
GLfloat back_mat_diffuse[] = {0.7,0.7,0.7,1.0};
GLfloat back_mat_specular[] = {0.5,0.5,0.5,0.5};
GLfloat back_mat_ambient2[] = {0.01,0.01,0.01,1.0};
GLfloat back_mat_diffuse2[] = {0.86,1.0,0.0,1.0};
GLfloat back_mat_specular2[] = {0.5,0.0,0.0,0.5};
GLfloat Imodel_ambient[] = {0.0,0.0,0.0,1.0};
IMAGE *Image[1];
GLuint Texture[1];
unsigned char light=0;
double  shizhi_theta1,shizhi_theta2,shizhi_theta3;
double  zhongzhi_theta1,zhongzhi_theta2,zhongzhi_theta3;
double  wumingzhi_theta1,wumingzhi_theta2,wumingzhi_theta3;
int list_str = 0; //用来创建字符串的显示列表
int list_image=0; //用来创建手指，光球，按键的显示列表
bool LoadGLTextures()
{
     FILE *myf;//指向位图的指针
      if(!(myf = fopen("Star.bmp","rb")))//尝试打开位图
      {
               //MessageBox("位图无法打开","错误！",WB_OK);
      }
      else
     {
             Image[0] = (IMAGE *)auxDIBImageLoadA("Star.bmp");//装载位图
     }
	 glGenTextures(1,&texture[0]);
     glBindTexture(GL_TEXTURE_2D,texture[0]);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER ,GL_NEAREST);
     glTexImage2D(GL_TEXTURE_2D,
		 0,
		 3,
		 Image[0]->x,
		 Image[0]->y,
		 0,
		 GL_RGB,
		 GL_UNSIGNED_BYTE,
		 Image[0]->data
		 );
     //GL_LINEAR 滤波方式的纹理，该滤波方式的放走样效果最好
    //释放位图所占的内存
    if(Image[0])
     {
           if(Image[0]->data)
                   free(Image[0]->data);
           free(Image[0]);
     }
 return true;
}

void  InitZhiData()
{
  //这里我们假定食指的编号是0号
  FILE *f0 = fopen("D:\\shizhi.txt","r");
  int i;
  double a,b;
  for (i = 0;i<len[0];i++)
  {
          fscanf(f0, "%lf", &theta[0][i].theta1);
          fscanf(f0, "%lf", &theta[0][i].theta2);
          fscanf(f0, "%lf", &theta[0][i].theta3);
          fscanf(f0, "%lf", &Table[0][i].x);
		  Table[0][i].x*=10;
          fscanf(f0, "%lf", &Table[0][i].z);
		  Table[0][i].z*=10;
          fscanf(f0, "%lf", &a);
          fscanf(f0, "%lf", &b);
  }
  fclose(f0);
  FILE *f1 = fopen("D:\\zhongzhi.txt","r");
  for (i = 0;i<len[1];i++)
  {
          fscanf(f1, "%lf", &theta[1][i].theta1);
          fscanf(f1, "%lf", &theta[1][i].theta2);
          fscanf(f1, "%lf", &theta[1][i].theta3);
          fscanf(f1, "%lf", &Table[1][i].x);
		  Table[1][i].x*=10;
          fscanf(f1, "%lf", &Table[1][i].z);
		  Table[1][i].z*=10;
          fscanf(f1, "%lf", &a);
          fscanf(f1, "%lf", &b);
  }
  fclose(f1);
  FILE *f2 = fopen("D:\\wumingzhi.txt","r");
  for (i = 0;i<len[2];i++)
  {
          fscanf(f1, "%lf", &theta[2][i].theta1);
          fscanf(f1, "%lf", &theta[2][i].theta2);
          fscanf(f1, "%lf", &theta[2][i].theta3);
          fscanf(f1, "%lf", &Table[2][i].x);
		  Table[2][i].x*=10;
          fscanf(f1, "%lf", &Table[2][i].z);
		  Table[2][i].z*=10;
          fscanf(f1, "%lf", &a);
          fscanf(f1, "%lf", &b);
  }
  fclose(f2);
  return ;
}

void myInit(void)
{
/*设置背景色*/
//InitZhiData();
LoadGLTextures();				// Load The Texture(s)
glClearDepth(1.0);
glClearColor(0,0,0,0);
/*设置光照*/
glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
glLightfv(GL_LIGHT0,GL_POSITION,light_position);
/*设置光照模型参数*/
glLightModelfv(GL_LIGHT_MODEL_AMBIENT,Imodel_ambient);
glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,FALSE);
glShadeModel (GL_SMOOTH); 
}




int find(int k,double x,double y,double z)
{
	/*if(k==2)
		printf("wuming: %lf %lf %lf\n",x,y,z);
	else if(k==0)
		printf("shizhi: %lf %lf %lf\n",x,y,z);*/
       double vz=0-z;
       double vx=sqrt(pow(x,2)+pow(y,2));
       int i;
       if(vz<=Table[k][0].z) 
       {
            return 0;
       }
        for(i=1;i<len[k];i++)
        {
           if(Table[k][i-1].z<=vz&&vz<=Table[k][i].z)
           {
              //  现在看与谁的向量的X更接近 
              if(fabs(Table[k][i-1].x-vx)>=fabs(Table[k][i].x-vx)) 
              {
                  return i;
              }
              else
              {
                   return i-1;
              }
              break;
           }                         
        }
        return len[k]-1;
}

void find_theta()
{
double xs0=mess.xs0,ys0=mess.ys0,zs0=mess.zs0,
xs1=mess.xs1,ys1=mess.ys1,zs1=mess.zs1,
xz0=mess.xz0,yz0=mess.yz0,zz0=mess.zz0,
xz1=mess.xz1,yz1=mess.yz1,zz1=mess.zz1,
xw0=mess.xw0,yw0=mess.yw0,zw0=mess.zw0,
xw1=mess.xw1,yw1=mess.yw1,zw1=mess.zw1;
double vx1=xz1-xs1,vy1=yz1-ys1,vz1=zz1-zs1;
double vx2=xw1-xz1,vy2=yw1-yz1,vz2=zw1-zz1;
//printf("%lf  %lf  %lf \n",vx1,vy1,vz1);
//printf("%lf  %lf  %lf \n",vx2,vy2,vz2);
double n_vx=vy2*vz1-vy1*vz2;
double n_vy=vx1*vz2-vx2*vz1;
double n_vz=vx2*vy1-vx1*vy2;
double cos_theta=n_vz/sqrt(pow(n_vx,2)+pow(n_vy,2)+pow(n_vz,2));
cos_theta=n_pi*acos(fabs(cos_theta));//取反三角函数余弦 
int index=find(0,xs0-xs1,ys0-ys1,zs0-zs1);
//printf("%lf  %lf  %lf ",n_vx,n_vy,n_vz);
//printf("%d ",index);
shizhi_theta1=cos_theta-theta[0][index].theta1;
shizhi_theta2=theta[0][index].theta2;
shizhi_theta3=theta[0][index].theta3;
index=find(1,xz0-xz1,yz0-yz1,zz0-zz1);
//printf("%d ",index);
zhongzhi_theta1=cos_theta-theta[1][index].theta1;
zhongzhi_theta2=theta[1][index].theta2;
zhongzhi_theta3=theta[1][index].theta3;
index=find(2,xw0-xw1,yw0-yw1,zw0-zw1);
//printf("%d ",index);
wumingzhi_theta1=cos_theta-theta[2][index].theta1;
wumingzhi_theta2=theta[2][index].theta2;
wumingzhi_theta3=theta[2][index].theta3;
return ;
}

void drawString(int start,int play,int step)
{
	if( list_str == 0 ) 
	{
     //如果显示列表不存在，则创建
     list_str = glGenLists(MAX_CHAR); //说明：创建一个显示列表
	 wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, list_str);
	}
    for(int i=start,j=0;j<step;i++,j++)
	{	
		if(i==play)
        {  
			glColor3f(1.0f,1.0f,0.0f);
		}
		else
		{
			glColor3f(1.0f,0.0f, 0.0f);
		}
		glRasterPos2f(-14.0+j,13.0f);
		glCallList(list_str+*(mess.p+i));
	}
}

void drawString2(char str[])
{   
	char* p=str;
    for(int i=0;*(p+i)!='\0';i++)
	{	
		glCallList(list_str+*(p+i));
	}
}


GLfloat left=0,up=0;

void myDisplay(void)
{
	if(list_image == 0)
		{  
		     //如果显示列表不存在，则创建
			 list_image = glGenLists(12);  //目前包括查看的方式，手指的三种位置，加上光球这5种
			 //创建显示列表0，用来表示视角
			 glNewList(list_image,GL_COMPILE);
			 glMatrixMode(GL_PROJECTION);
			 glLoadIdentity();
			 gluPerspective(45.0,1.0,1.0,60.0);
			 glMatrixMode(GL_MODELVIEW);
			 glLoadIdentity();
			 glTranslatef(0.0,0.0,-50.0);
			
			 glEndList();

glNewList(list_image+1,GL_COMPILE);   //画大拇指翘起
glEnable(GL_DEPTH_TEST);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glPushMatrix();
glTranslatef(-3.0,-3,-3.0);
glRotatef(-30,1.0,0.0,0.0);
glRotatef(-30,0.0,1.0,0.0);
quadObj1 = gluNewQuadric();
glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
//手指的第一个关节
gluQuadricDrawStyle(quadObj1,GLU_FILL);
gluQuadricNormals(quadObj1,GL_FLAT);
gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
gluQuadricTexture(quadObj1,GL_TRUE);
gluCylinder(quadObj1,1.0,1.0,4.0,20.0,8.0);
gluDeleteQuadric(quadObj1);
//绘制第一个关节球
glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
glTranslatef(0,0,4.2);
glutSolidSphere(1.0,40,32);
//绘制第二个关节
glTranslatef(0,0,0.2);
glRotatef(15,0.0,1.0,0.0);
quadObj1 = gluNewQuadric();
glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
gluQuadricDrawStyle(quadObj1,GLU_FILL);
gluQuadricNormals(quadObj1,GL_FLAT);
gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
gluQuadricTexture(quadObj1,GL_TRUE);
gluCylinder(quadObj1,1.0,1.0,3.5,20.0,8.0);
glTranslatef(0,0,3.5);
glutSolidSphere(1.0,40,32);
glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
glEndList();

           


	  glNewList(list_image+2,GL_COMPILE);  // 画大拇指按下去
          glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
glPushMatrix();
glTranslatef(-3.0,-3,-3.0);
glRotatef(-30,0.0,1.0,0.0);
quadObj1 = gluNewQuadric();
glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
//手指的第一个关节
gluQuadricDrawStyle(quadObj1,GLU_FILL);
gluQuadricNormals(quadObj1,GL_FLAT);
gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
gluQuadricTexture(quadObj1,GL_TRUE);
gluCylinder(quadObj1,1.0,1.0,4.0,20.0,8.0);
gluDeleteQuadric(quadObj1);
//绘制第一个关节球
glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
glTranslatef(0,0,4.2);
glutSolidSphere(1.0,40,32);
//绘制第二个关节
glTranslatef(0,0,0.2);
glRotatef(15,0.0,1.0,0.0);
quadObj1 = gluNewQuadric();
glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
gluQuadricDrawStyle(quadObj1,GLU_FILL);
gluQuadricNormals(quadObj1,GL_FLAT);
gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
gluQuadricTexture(quadObj1,GL_TRUE);
gluCylinder(quadObj1,1.0,1.0,3.5,20.0,8.0);
glTranslatef(0,0,3.5);
glutSolidSphere(1.0,40,32);
glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
glEndList();


	  glNewList(list_image+3,GL_COMPILE);   // 画食指翘起来
          glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
glPushMatrix();
          glRotatef(9,1.0,0.0,0.0);
	  quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,5.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,5.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-25,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,4.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,4.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-9,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,3.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
          glTranslatef(0,0,3.0);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  glutSolidSphere(1.0,40,32);
          glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	  glEndList();

           glNewList(list_image+4,GL_COMPILE);   // 画食按下去
          glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
glPushMatrix();
          glRotatef(-31,1.0,0.0,0.0);
	  quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,5.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,5.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-43,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,4.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,4.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-12,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,3.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
          glTranslatef(0,0,3.0);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  glutSolidSphere(1.0,40,32);
          glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	  glEndList();

          glNewList(list_image+5,GL_COMPILE);   // 画中指翘起来
          glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
glPushMatrix();
          glTranslatef(3.0,0.0,0.0);
          glRotatef(14,1.0,0.0,0.0);
	  quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,6.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,6.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-42,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,5.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,5.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-23,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,4.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
          glTranslatef(0,0,4.0);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  glutSolidSphere(1.0,40,32);
          glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	  glEndList();

          glNewList(list_image+6,GL_COMPILE);   // 画中指按下去
          glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
glPushMatrix();
          glTranslatef(3.0,0.0,0.0);
          glRotatef(-40,1.0,0.0,0.0);
	  quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,6.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,6.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-35,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,5.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,5.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-17,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,4.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
          glTranslatef(0,0,4.0);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  glutSolidSphere(1.0,40,32);
          glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	  glEndList();

          glNewList(list_image+7,GL_COMPILE);   // 画无名指翘起来
          glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
glPushMatrix();
          glTranslatef(6.0,0.0,0.0);
          glRotatef(12,1.0,0.0,0.0);
	  quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,5.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,5.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-40,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,4.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,4.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-12,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,3.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
          glTranslatef(0,0,3.0);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  glutSolidSphere(1.0,40,32);
          glPopMatrix();
          
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	  glEndList();
          
          glNewList(list_image+8,GL_COMPILE);   // 画无名指按下去
          glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
glPushMatrix();
          glTranslatef(6.0,0.0,0.0);
          glRotatef(-29,1.0,0.0,0.0);
	  quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,5.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,5.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-56,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,4.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,4.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-18,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,3.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
          glTranslatef(0,0,3.0);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  glutSolidSphere(1.0,40,32);
          glPopMatrix();
          
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	  glEndList();
        
          glNewList(list_image+9,GL_COMPILE);   // 画小母指翘起来
          glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
          glPushMatrix();
          glTranslatef(9.0,0.0,0.0);
          glRotatef(10,1.0,0.0,0.0);
	  quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,4.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,4.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-26,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,3.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,3.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-11,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,3.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
          glTranslatef(0,0,3.0);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  glutSolidSphere(1.0,40,32);
          glPopMatrix();
          
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	  glEndList();

	  glNewList(list_image+10,GL_COMPILE);   // 画小姆指按下去
          glEnable(GL_DEPTH_TEST);
	  glEnable(GL_LIGHTING);
	  glEnable(GL_LIGHT0);
          glPushMatrix();
          glTranslatef(9.0,0.0,0.0);
          glRotatef(-23,1.0,0.0,0.0);
	  quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,4.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,4.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-40,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,3.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
	  glTranslatef(0,0,3.2);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient2);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse2);
	  glutSolidSphere(1.0,40,32);
	  glTranslatef(0,0,0.2);
          glRotatef(-16,1.0,0.0,0.0);
          quadObj1 = gluNewQuadric();
	  glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  gluQuadricDrawStyle(quadObj1,GLU_FILL);
	  gluQuadricNormals(quadObj1,GL_FLAT);
	  gluQuadricOrientation(quadObj1,GLU_OUTSIDE);
	  gluQuadricTexture(quadObj1,GL_TRUE);
	  gluCylinder(quadObj1,1.0,1.0,3.0,20.0,8.0);
	  gluDeleteQuadric(quadObj1);
          glTranslatef(0,0,3.0);
          glMaterialfv(GL_FRONT,GL_AMBIENT,back_mat_ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,back_mat_diffuse);
	  glutSolidSphere(1.0,40,32);
          glPopMatrix();
         
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	  glEndList();
 
			 glNewList(list_image+11,GL_COMPILE);
			 glEnable(GL_TEXTURE_2D);
			 glEnable(GL_BLEND);
			 glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			 glBindTexture(GL_TEXTURE_2D,texture[0]);
			 glBegin(GL_QUADS);
			 glTexCoord2f(0.0f, 0.0f); glVertex2f(-2.0f,-2.0f);
			 glTexCoord2f(1.0f, 0.0f); glVertex2f( 2.0f,-2.0f);
			 glTexCoord2f(1.0f, 1.0f); glVertex2f( 2.0f,2.0f);
			 glTexCoord2f(0.0f, 1.0f); glVertex2f(-2.0f,2.0f);
			 glEnd();
			 glDisable(GL_BLEND);
			 glDisable(GL_TEXTURE_2D);
			 glEndList();
		}
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glCallList(list_image);
	  glPushMatrix();
	  glRotatef(-90,1.0,0.0,0.0);
	  glRotatef(180,0.0,1.0,0.0);
	  glTranslatef(-4,0,-8.0);
      //画光球，先要开启纹理，然后开启颜色混合
	  char warn[1000]="Your finger ";
	  int error=0,warn_pos;
	  selectFont(64,ANSI_CHARSET,"Times New Roman");
	  for(int i=0;i<5;i++)
	  {
		  if(!mess.z[i])
		  {
			  if(mess.v[i][2]>=Correct_Line)
			  {  
				 warn_pos=2*error+12;
				 warn[warn_pos]=i+'1';
				 warn[warn_pos+1]=' ';
				 error++;
			  }
              glCallList(list_image+2*i+1);
		  }
		  else
		  {
			  glCallList(list_image+2*i+2);
		  }
	  }
	  glPopMatrix();
	  if(error>0 && mess.dis)
	  {     
		    warn_pos=2*error+12;
			warn[warn_pos]='i';
			warn[warn_pos+1]='s';
			warn[warn_pos+2]=' ';
			warn[warn_pos+3]='t';
			warn[warn_pos+4]='o';
			warn[warn_pos+5]='o';
			warn[warn_pos+6]=' ';
			warn[warn_pos+7]='h';
			warn[warn_pos+8]='i';
			warn[warn_pos+9]='g';
			warn[warn_pos+10]='h';
			warn[warn_pos+11]='\0';
			glRasterPos2f(-3.0f,15.0f);
			drawString2(warn);
	  } 
	  
	  drawString(mess.start,mess.pos,mess.step);

      glColor4ub(22,193,178,mess.ani);
	  
	  glTranslatef(-10,-10,0);
	  for(int i=0;i<7;i++)
	  {
		  if(mess.st[i])
		  {
			glCallList(list_image+11);
		  }
		  else
		  {
		  }
		  glTranslatef(4,0,0);
	  }
	  
      light+=10;
	  if(light>=255)
	  {
		  light=0;
	  } 
	  
      glFlush();
      glutSwapBuffers();

}

int state=0;
int kk;
int count=0;
void myIdle(void)
{
    //此处以添加手指识别的函数
	if(mess.ani){
		mess.ani-=5;
		Sleep(1);
		myDisplay();
	}
	/*printf("idle\n");
	if(state>=0&&count<=27)
    {
        left=2*state-5;
		for(kk=0;kk<=1000000;kk++)
		{
		}
		myDisplay();
		count++;
		if(count==27)
		{
			state=-1;
			count=0;
			light=0;
		}
	}
	glutIdleFunc(NULL);*/
}
void myReshape(int w,int h)
{
glViewport(0,0,(GLsizei)w,(GLsizei)h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,(GLfloat)w/(GLfloat)h,1.0,50.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0.0,0.0,-40.0);
}
void SpecialKeys(int key, int x, int y)
{   
         switch(key) { 
                 case GLUT_KEY_DOWN:  
                         up-=2.0;
						 state=1;
						 break; 
                  case GLUT_KEY_UP:  
                         up+=2.0;
						 state=1;
						 break;
				 case GLUT_KEY_LEFT:  
                         left-=2.0;
						 state=1;
						 break;
				 case GLUT_KEY_RIGHT:  
                         left+=2.0;
						 state=1;
						 break;
         } 
} 


void Display::display(){
	mess.exi=false;
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(100,100);
	/*创建窗口*/
	glutCreateWindow("DRAW QUADRIC OBJECTS");
	/*绘制与显示*/
	myInit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);
	glutSpecialFunc(SpecialKeys);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutHideWindow();
	glutMainLoop();
	printf("Bye!");
	mess.exi=true;
}

Display::Display()
{
}


Display::~Display(void)
{
}

