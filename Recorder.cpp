#include "Recorder.h"
#include <stdio.h>
#include <string>
Recorder::Recorder(){

}
void Recorder::init(int i)
{
	id=i,pos=0,start=0,over=false,p=(char *)str[i];
	if(low[id][pos]=='`')
		group=0;
	else if(high[id][pos]=='.')
		group=2;
	else group=1;
}


Recorder::~Recorder(void)
{
}

void Recorder::clicked(int i){
	
	if(str[id][pos]==i+'1'){
		do{
			pos++;
		}while(str[id][pos]<'1' || str[id][pos]>'7');
		error=0;
		if(start+step<=pos)
			start+=step;
	}else
		error=1;

	if(pos>=strlen(str[id])){
		over=true;
		//printf("over");
	}
	if(low[id][pos]=='`')
		group=0;
	else if(high[id][pos]=='.')
		group=2;
	else group=1;
	//for(int c=start;c<start+step;c++)
		//printf("%c",str[id][c]);
	//printf("\npos=%d str=%c i+'1'=%c\n",pos,str[id][pos],i+'1');

}

bool Recorder::isover(){
	return true;
}