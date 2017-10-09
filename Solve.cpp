#include "Solve.h"

const int N=25000;

Solve::Solve(void)
{
}


Solve::~Solve(void)
{
	//printf("~Solve\n");
	cvReleaseCapture(&capture);
	cvReleaseImage(&image);
	cvReleaseImage(&binary);
	cvReleaseImage(&dst);
	cvReleaseImage(&hsv);
	cvReleaseImage(&binary1);
}

void Solve::control(){
	char *p=(char *)s_track.data();
	cvNamedWindow(p,CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Y_MIN",p,&Y_min,300,NULL);
	cvCreateTrackbar("Y_MAX",p,&Y_max,300,NULL);
	cvCreateTrackbar("Cr_MIN",p,&Cr_min,300,NULL);
	cvCreateTrackbar("Cr_MAX",p,&Cr_max,300,NULL);
	cvCreateTrackbar("Cb_MIN",p,&Cb_min,300,NULL);
	cvCreateTrackbar("Cb_MAX",p,&Cb_max,300,NULL);
}

void Solve::init(int num,int Y_min,int Y_max,int Cr_min,int Cr_max,int Cb_min,int Cb_max){
    this->Y_min=Y_min;
    this->Y_max=Y_max;
    this->Cr_min=Cr_min;
    this->Cr_max=Cr_max;
    this->Cb_min=Cb_min;
    this->Cb_max=Cb_max;
        
    id=num;
    s_image="image",s_track="trackBar";
        
    capture=cvCaptureFromCAM(num); 
	//printf("%d in function\n",capture);
    cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,weight);
		
    image=cvQueryFrame(capture);
    //创建图像 
    dst=cvCreateImage(cvGetSize(image),8,1);
    binary=cvCreateImage(cvGetSize(image),8,1);
    binary1=cvCreateImage(cvGetSize(image),8,1);
    hsv=cvCreateImage(cvGetSize(image),8,image->nChannels);
        
    data=(unsigned char (*)[weight])binary1->imageData;
    dst->imageData=(char *)processing;
    
    Y_min=0,Y_max=125,Cr_min=0,Cr_max=147,Cb_min=121,Cb_max=255,step=40,count_threshold=3;
    cos_threshold=0.5;
        
    if(num==1){
        s_track+='1';
        s_image+='1';
    }else{
        s_track+='2';
        s_image+='2';
    }
    //创建一个滑动条滤出手指 
   // control();
}
void Solve::getimage(){
        image=cvQueryFrame(capture);
}
    
void Solve::imageprocess(){
    //以rgb为滤波空间，最终图像保存在binary1   
    IplConvKernel	*kernel = cvCreateStructuringElementEx(9, 9, 4, 4, CV_SHAPE_RECT,
						NULL);
						   
    cvCvtColor(image,hsv,CV_RGB2HSV);              
    cvInRangeS(hsv,cvScalar(Y_min,Cr_min,Cb_min),cvScalar(Y_max,Cr_max,Cb_max),binary);
        
    cvMorphologyEx(binary,binary1, NULL, kernel,
		    CV_MOP_OPEN, 1);
	cvSmooth(binary1, binary1, CV_GAUSSIAN, 3, 3, 0, 0);  
	    
    cvFlip(binary1,NULL,0); 
    // 直接将图像采集卡采集的图像cvShowImage出来的是反着的图像
    // 也就是图像采集卡采集的图像是以左下角为原点的，而窗口显示的图像原点是左上角 
}
void Solve::work(){
        
    //init
    memset(processing,0,sizeof(processing));
    //imageprocess   图像处理    
    imageprocess();
    //calc
    p=5; 
    calc(0,cos_threshold,step,count_threshold);
    //在摄像头画面中绘制提取出的手指指尖 
    for(int i=0;i<=4;i++)
        cvCircle(dst,cvPoint(ans[i][1],ans[i][0]),10,cvScalar(255,255,255));
            
    char *p=(char *)s_image.data();

    // cvShowImage(p,image);
   // if(id==1)
   //     cvShowImage("binary11",binary1);
   // else cvShowImage("binary12",binary1);
     cvShowImage(p,dst);
        
}
void Solve::closewindow(){
	cvDestroyWindow(s_image.data());
}
void Solve::calc(int start,double cos_threshold,int step,int count_threshold){
  int I[8]={0,-1,-1,-1,0,1,1,1},J[8]={-1,-1,0,1,1,1,0,-1},i,j,c,prei,prej;
        const int JJ[4]={0,0,-1,1},II[4]={-1,1,0,0};
        unsigned char threshold=100;
        bool vt[height][weight];
        int buff[2*40+1][2],vi,vj,ui,uj,k,i1,i2,j1,j2,M=2*step+1,cnt=0,count=0;
        int tmp1,tmp2,pre_work=0;
        
        memset(buff,0,sizeof(buff));
        memset(vt,0,sizeof(vt));
        for(i=height/3*2,j=start;j<weight;j++)
            if(data[i][j]>threshold)
                break;
        if(j==weight)
            return;
            
        vi=ui=i,vj=uj=j,buff[k=0][0]=vi,buff[k][1]=vj;
        vt[ui][uj]=1;
        while(1){
            cnt++,pre_work++;
            if(!p)
                break;
            if(cnt>1500)
                break;
            if(vi-ui>5)
                break;
                
            for(c=0;c<8;c++)
                if(data[vi+I[c]][vj+J[c]]<threshold && data[vi+I[(c+1)%8]][vj+J[(c+1)%8]]>threshold && !vt[vi+I[(c+1)%8]][vj+J[(c+1)%8]])
                    break;
        
           if(c==8){
                if(!vi || !vj || vi>=height || vj>=weight)  break;
                data[vi][vj]=0;
                k=(k-1+M)%M;
                vi=buff[k][0],vj=buff[k][1];
            }else{
                k=(k+1)%M;
                vt[vi+I[(c+1)%8]][vj+J[(c+1)%8]]=1;
                processing[vi+I[(c+1)%8]][vj+J[(c+1)%8]]=255;
                
              //  for(int i=0;i<4;i++)
                //    processing[vi+II[i]][vj+JJ[i]]=255;
                    
                if(vi+I[(c+1)%8]>0)
                    buff[k][0]=vi=vi+I[(c+1)%8];
                else break;
                if(vi+I[(c+1)%8]>0)
                    buff[k][1]=vj=vj+J[(c+1)%8];
                else break;
                
                if(pre_work>step+step){
                   
                    i1=buff[k][0]-buff[(k-step+M)%M][0],i2=buff[(k-step-step+M)%M][0]-buff[(k-step+M)%M][0];
                    j1=buff[k][1]-buff[(k-step+M)%M][1],j2=buff[(k-step-step+M)%M][1]-buff[(k-step+M)%M][1];
                    
                    
                    if((tmp1=i1*i1+j1*j1) && (tmp2=i2*i2+j2*j2) &&(i1*j2-i2*j1<0) && (i1*i2+j1*j2)/sqrt((double)tmp1)/sqrt((double)tmp2)>cos_threshold){
                        //printf("%d %d %d %d\n",(k-step+M)%M,(k-step+M)%M,buff[(k-step+M)%M][0],buff[(k-step+M)%M][1]);
                        count++;
                    }else{
                        if(count>count_threshold && count<step){
                            ans[--p][0]=buff[(k-step-count/2+M+M+M)%M][0]; 
                            ans[p][1]=buff[(k-step-count/2+M+M+M)%M][1];
                            //ans[++p][0]=buff[(M-(k+count/2-step+M+M)%M)%M][0],
                            //ans[p][1]=buff[(M-(k+count/2-step+M+M)%M)%M][1];
                          
                        }
                        count=0;
                    }
                }
            }
            
        }    
        //get three points
        /*if(p==4){
            cvLine(dst,cvPoint(uj,ui),cvPoint(vj+20,vi-10),cvScalar(255,255,255),3);
            getpoint(vi-30,vj-30,vt);
        }*/
}

bool Solve::getpoint(int start_i,int start_j,bool vt[height][weight]){
    const int I[4]={-1,0,0,1},J[4]={0,-1,1,0};
        
    int hd,tl,vi,vj,ui,uj;
    node dui[N],u;
    point_count=-1;

	u.i=start_i,u.j=start_j;
    dui[hd=tl=1]=u;
    while(hd<=tl){
        if(tl>24000 || point_count==2)
            break;
        ui=dui[hd].i,uj=dui[hd++].j;
        //processing[ui][uj]=255;
        for(int i=0;i<4;i++){
            vi=ui+I[i],vj=uj+J[i];
            if(processing[vi][vj]!=255 && !vt[vi][vj]){
                vt[vi][vj]=1;
                if(!data[vi][vj])
                    ranse(vi,vj,vt);

				u.i=vi,u.j=vj;
                dui[++tl]=u;
            }  
        }
    }
    if(tl>24000)
        return false;
        
    //if(id==1)
      //  printf("id=%d count=%d\n",id,point_count);
    if(point_count==2){
        // printf("num:%d\n",id);
    for(int i=0;i<=point_count;i++)
        cvCircle(dst,cvPoint(point[i][1],point[i][0]),10,cvScalar(255,255,255));
        //printf("%lf %lf\n", point[i][1],point[i][0]);
    }
    return true;
}
void Solve::ranse(int ui,int uj,bool vt[height][weight]){
    const int I[4]={1,-1,0,0},J[4]={0,0,-1,1};
        
    node dui[120],u;
    int hd,tl,vi,vj;
    double data_i=0,data_j=0;
	
	u.i=ui,u.j=uj;
    dui[hd=tl=1]=u;
    while(hd<=tl){
        if(tl>100)
            break;
        ui=dui[hd].i,uj=dui[hd++].j,data_i+=ui,data_j+=uj;
        processing[ui][uj]=255;
        //   printf("%d %d\n",ui,uj);
        for(int i=0;i<4;i++){
            vi=ui+I[i],vj=uj+J[i];
            if(!vt[vi][vj] && !data[vi][vj]){
                vt[vi][vj]=1;

				u.i=vi,u.j=vj;
                dui[++tl]=u;
            }
        }
    }
    if(tl>100 || hd<10)
        return;
    // printf("\n");
    point[++point_count][1]=(double)data_j/tl, point[point_count][0]=(double)data_i/tl;
}