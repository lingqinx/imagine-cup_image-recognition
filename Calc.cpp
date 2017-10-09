#include "Calc.h"

DWORD WINAPI func( LPVOID lpParam) 
{
    int *tmp=(int *)lpParam;
	printf("id=%d group=%d\n",tmp[0],tmp[1]);
    PlaySoundA((LPCSTR)tone[tmp[1]][tmp[0]],NULL,SND_FILENAME | SND_ASYNC);
    return 0; 
} 

Calc::Calc(char Recordergroup)
{
	//judge[0]=-15,judge[1]=-16.5,judge[2]=-15,judge[3]=-13,judge[4]=-18;
	//judge[0]=-10,judge[1]=-12.5,judge[2]=-12,judge[3]=-17,judge[4]=-15;
	//judge[0]=-7,judge[1]=-10,judge[2]=-10,judge[3]=-10,judge[4]=-10;
	//judge[0]=2,judge[1]=-10,judge[2]=-11,judge[3]=-10.5,judge[4]=-10;
	//judge[0]=11.5,judge[1]=6,judge[2]=6.5,judge[3]=6,judge[4]=8.5;
	//judge[0]=18,judge[1]=9,judge[2]=8.5,judge[3]=6.5,judge[4]=8.5;
//	judge[0]=-3,judge[1]=-3,judge[2]=-3,judge[3]=-2,judge[4]=-3;
	judge[0]=0,judge[1]=-2,judge[2]=-5,judge[3]=-6,judge[4]=-5;
	//{-11,-10,-10,-10,-10};
	area[0][1]=-320,area[0][0]=-480,
	area[1][1]=-300,area[1][0]=-610,
	area[2][1]=-200,area[2][0]=-770;
	group=Recordergroup;
	memset(st,0,sizeof(st));
}


Calc::~Calc(void)
{
}


double Calc::calc(int p,double ul,double vl,double ur,double vr){
    
    
    double A_data[] = {ml31,ml32,ml33,  ml31,ml32,ml33, mr31,mr32,mr33, mr31,mr32,mr33};
    double UV_data[] = {ul,ul,ul,   vl,vl,vl,   ur,ur,ur,   vr,vr,vr};
    double SM_data[] = {ml11,ml12,ml13, ml21,ml22,ml23, mr11,mr12,mr13, mr21,mr22,mr23};
    double b1_data[]=   {ml34,ml34,mr34,mr34};
    double b_data[]=    {ml14,ml24,mr14,mr24};
    double uv_data[]=    {ul,vl,ur,vr};
    
        
    CvMat* UV=cvCreateMatHeader(4,3,CV_64F);
    CvMat* A=cvCreateMatHeader(4,3,CV_64F);
    CvMat* SM=cvCreateMatHeader(4,3,CV_64F);
    
    CvMat* b=cvCreateMatHeader(4,1,CV_64F);
    CvMat* b1=cvCreateMatHeader(4,1,CV_64F);
    CvMat* uv=cvCreateMatHeader(4,1,CV_64F);
    
    CvMat* TMP=cvCreateMat(3,3,CV_64F);
    CvMat* TMP1=cvCreateMat(3,3,CV_64F);
    CvMat* TMP2=cvCreateMat(3,4,CV_64F);
    CvMat* AT=cvCreateMat(3,4,CV_64F);
    CvMat* P=cvCreateMat(3,1,CV_64F);
    
    cvSetData(A,&A_data,24);
    cvSetData(UV,&UV_data,24);
    cvSetData(SM,&SM_data,24);
    
    cvSetData(b,&b_data,8);
    cvSetData(b1,&b1_data,8);
    cvSetData(uv,&uv_data,8);
    
    cvMul(A,UV,A);
    cvSub(A,SM,A);
    
    cvMul(b1,uv,b1);
    cvSub(b,b1,b);
    
    cvTranspose(A,AT);
    cvMulTransposed(A,TMP,1);
    cvInvert(TMP,TMP1);
    
    
    cvGEMM(TMP1,AT,1.0,NULL,0,TMP2);
    cvGEMM(TMP2,b,1.0,NULL,0,P);
    double T[3][3]={    0.988645840637552      ,  0.0216279417191418       , -0.129718003657719,
       -0.0922373640360381    ,     0.634294841134257   ,     -0.793303717018697,
       -0.0498042099536848     ,   -0.782964822149094  ,      -0.635827448484825	};
    double w[3]={   122.23173597865,
          475.767709709807,
          457.682644766977};
    for(int i=0;i<3;i++){
        v[p][i]=w[i];
        for(int j=0;j<3;j++)
            v[p][i]+=T[i][j]*cvGetReal1D(P,j);
    }
    /*printf("new:\n");
    for(int i=0;i<3;i++)
        printf("%lf ",v[p][i]);
    printf("\n");*/
	//printf("%lf ",v[p][2]);
	return v[p][2];
}

int Calc::transfer(Solve &l,Solve &r){
	double x=0,y=0;
	int k=0,ans=-1,i;
	
	if(!l.p && !r.p){
		for(int i=0;i<5;i++){
			calc(i,(double)l.ans[i][1],600.0-l.ans[i][0],(double)r.ans[i][1],600.0-r.ans[i][0]);  //1:横坐标 0:纵坐标
			x+=v[i][1],y+=v[i][0];
		}
		for(int i=0;i<3;i++){
		//printf("%lf ",abs(x-area[i][1])+abs(y-area[i][0]));
		if((x-area[i][1])*(x-area[i][1])+(y-area[i][0])*(y-area[i][0])<(x-area[k][1])*(x-area[k][1])+(y-area[k][0])*(y-area[k][0]))
			k=i;
	}
	//printf("k=%d\n",k);
	for(i=0;i<5;i++){
		if(v[i][2]<judge[i]){
			if(clicked(i+k))
				ans=i+k;
		}else
			st[i+k]=0;
	}
	for(i=5;i<7;i++)
		st[(i+k)%7]=0;
		/*if(l.point_count==2 && r.point_count==2)
			for(int i=0;i<3;i++)
				calc(i+5,l.point[i][1],600.0-l.point[i][0],(double)r.point[i][1],600.0-r.point[i][0]);*/
	}
	
	//printf("x=%lf y=%lf\n",x,y);
	
	

	return ans;
}

bool Calc::clicked(int i){
	//printf("id=%d group=%d\n",i,group);
	int *tmp=new int(2);
    tmp[0]=i,tmp[1]=group;
	//printf("group=%d\n",group);

    if(!st[tmp[0]]){
        CreateThread(NULL,0,func,tmp,0,NULL);
        st[i]=1;     //若某一个键被按下，则把其状态置为1 
		return true;
    }
	return false;
}

