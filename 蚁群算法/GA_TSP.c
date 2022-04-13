#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//相关可修改参数
#define END  5000//迭代次数
#define groupsize  200 //种群大小，偶数,4n~6n,种群大小即路径集合大小为200
#define pcross 0.9
#define pMutation 0.01


double GA_TSP(int *TestSetPointor, unsigned char *ResultPointor)
{
	unsigned char *flag=NULL;
	unsigned int  i=0,j=0,k=0,w=0,m=0,exchangeBuffer=0,returnIndex=0,count=0,citycount=0,flag1=0;
	unsigned char vector[groupsize][48];//种群存放的地方
	unsigned char vector_a[groupsize][48]={0};//可替代存放地方
	unsigned char table[groupsize]={0};
	
	unsigned char *pointer=(unsigned char *)vector;
	unsigned char *pointer_a=(unsigned char *)vector_a; 
	unsigned char *Index1=NULL;
	unsigned char *Index2=NULL;
	unsigned int crossPoint[2]={0};

	double distance[groupsize]={0};
	double fitness[groupsize]={0};
	double sum=0,x=0,y=0;
	double p[groupsize];
	double select;//随机产生的0~1随机数


	
	
	srand((unsigned int)time(NULL));
/***************************产生初始种群也即随机初始化vector连接各个城市每个城市只连一次**************************/
	
	for(i=0;i<groupsize;i++)//得到200个随机个体
	{
		k = 48*i;
		for(j=0;j<48;j++)//每个个体内基因全部升序排列
			vector[i][j] = j;

		for(j=0;j<1000;j++)//随机交换每个个体中的基因序列，每个个体中交换100次
		{
			Index1= pointer + k + rand()%48;
			Index2= pointer + k + rand()%48;
			
			exchangeBuffer = *Index1;
			*Index1 =*Index2;
			*Index2 =exchangeBuffer;
		}
	}
	/***************************迭代循环直到满足次数停止**************************/
	for(;;)
	{
	/**********获取适应度以及生存下去的概率***********/

		for(i=0;i<groupsize;i++)
		{
			//distance不会溢出
			Index1 =pointer+ 48*i;
			
			//累加索引城市顺序vector[i]中vector[i][47]到vector[i][0]的距离
			x =pow((double)(*(TestSetPointor +(*Index1)*2  ) - *(TestSetPointor + (*(Index1+47))*2  )),2);
			y =pow((double)(*(TestSetPointor +(*Index1)*2+1) - *(TestSetPointor + (*(Index1+47))*2+1)),2);
			distance[i] = sqrt(x + y);
			//累加索引城市顺序vector[i]中vector[i][0]到vector[i][1]到...到vector[i][47]的距离
			for(j=0;j<47;j++)
			{
				x =pow((double)(*(TestSetPointor +(*(Index1+j))*2  ) - *(TestSetPointor + (*(Index1+j+1))*2  )),2);
				y =pow((double)(*(TestSetPointor +(*(Index1+j))*2+1) - *(TestSetPointor + (*(Index1+j+1))*2+1)),2);
				distance[i] +=sqrt(x +y );
			}
		}
		
		
		if(count==END)//判断是否达到迭代终止条件,循环迭代0 ~ END-1共END次
		{
			for(i=0;i<groupsize;i++)//获取最小distance对应在pointer所指buffer的行指针
			{
				if(distance[i] < distance[0])
				{
					distance[0] = distance[i];
					returnIndex =i;
				}
			}
			Index1 =pointer + returnIndex*48;
			for(j=0;j<48;j++)
			*ResultPointor++ = *Index1++;//输出结果
			return distance[0];
		}
		count++;
		
		
		
		for(i=0;i<groupsize;i++)
			fitness[i] =1/distance[i];//总距离与适应度成反比
		
		for(i=0;i<groupsize;i++)
			sum += fitness[i];
		
		//得到选中概率阈值
		p[0]=fitness[0]/sum;
		for(i=1;i<groupsize-1;i++)
			p[i]=p[i-1]+fitness[i]/sum;
		p[groupsize-1]=1;
	/*******200次自然选择得出下一代,将上一代从pointer所指buffer工作区选择到pointer_a所指buffer工作区********/	
	//第一次进入for(count=0;count<END;count++)循环时pointer指向vecter,	pointer_a指向vecter_a
		
		for(k=0;k<groupsize;k++)
		{
			select=0;
			while(select==0)
				select = (double)rand() / RAND_MAX;    /* 产生 1 个 (0.0  ,  1.0] 之间的随机浮点数 */
			i=0;
			for(;;)
			{
				if(select<=p[i])//随机选择
				{
					Index1 = pointer + 48*i;//选中个体索引
					for(j=0;j<48;j++)
					{
						*pointer_a++ = *Index1++;//复制一个选中个体到pointer_a所指buffer
					}
					break;
				}
				i++;
			}
		}
		pointer_a=(unsigned char *)vector_a;
	/**********************在pointer_a所指buffer工作区对下一代进行交叉交叉结果放回pointer所指buffer工作区*********************/
		 
		
		k=0;//第二次迭代以及第二次以后的迭代需要先初始化k计数
		while(k==0)
			for(i=0;i<groupsize;i++)//确认哪些选择出来的个体参与交叉
			{
				select = 0;
				while(select==0)
					select = (double)rand() / RAND_MAX;
				if(select<pcross)
				{
					table[k]=i;//放入需要进行交叉的集合中
					k++;//统计个数
				}
			}
		
		for(i=0;i<groupsize/2;i++)//每个循环得到两个交叉结果放入vecter
		{
			Index1= pointer_a +48*table[rand() % k];//随机从table[k]中获得交叉对象1
			Index2= pointer_a +48*table[rand() % k];//随机从table[k]中获得交叉对象2
			
			
			//随机获得两个不同的交叉点，并升序排列单点交叉
			//crossPoint[0] = crossPoint[1];
			//while((crossPoint[0] == crossPoint[1])||((crossPoint[1]-crossPoint[0])==1))
			//{
				crossPoint[0]=(unsigned int)(rand() % 48);
				crossPoint[1]=(unsigned int)(rand() % 48);
				if(crossPoint[0]>crossPoint[1])
				{
					exchangeBuffer=crossPoint[0];
					crossPoint[0]=crossPoint[1];
					crossPoint[1]=exchangeBuffer;
				}
			//}
			exchangeBuffer=crossPoint[1]+1;
			if(flag1==0)
			{
				for(w=0;w<48;w++)
				{
					if(w==crossPoint[0])printf("\n");
					if(w==exchangeBuffer)printf("\n");
					printf("%d ",*(Index1+w));
					
				}
				printf("\n\n");
				for(w=0;w<48;w++)
				{	
					if(w==crossPoint[0])printf("\n");
					if(w==exchangeBuffer)printf("\n");
					printf("%d ",*(Index2+w));
					
				}
				printf("\n ");
				}
			flag1=1;
			//子代1
			citycount=0;

//			flag=(unsigned char *)vector+48*2*i+48;
			pointer=(unsigned char *)vector+48*2*i+exchangeBuffer;
			
			j=exchangeBuffer;
			while(j<48)
			{
				for(w=crossPoint[0];w<exchangeBuffer;w++)//判断是否有重复
				{
					if(*(Index2+j)==*(Index1+w))break;
				}
				if(w==exchangeBuffer)//无重复就复制
				{

					
					if(citycount==48-exchangeBuffer)
						pointer=(unsigned char *)vector+48*2*i;
					citycount++;
					*pointer++ = *(Index2+j);
				}
				j++;
			}
			j=0;
			while(j<exchangeBuffer)
			{
				for(w=crossPoint[0];w<exchangeBuffer;w++)//判断是否有重复
				{
					if(*(Index2+j)==*(Index1+w))break;
				}
				if(w==exchangeBuffer)//无重复就复制
				{
					
					if(citycount==48-exchangeBuffer)
						pointer=(unsigned char *)vector+48*2*i;
					citycount++;
					*pointer++ = *(Index2+j);
				}
				j++;
			}
			for(w=crossPoint[0];w<exchangeBuffer;w++)
				*pointer++ = *(Index1+w);
/*//////////////////////////////////////////////判断是否有重复
			if(pointer!=flag)
			{
				printf("zidai1  cp0=%d,cp1=%d,count=%d,i=%d\n",crossPoint[0],crossPoint[1],count,i);
				return 0;
			}
///////////////////////////////////////////*/
			//子代2
			citycount=0;
			pointer=(unsigned char *)vector+48*2*i+48+exchangeBuffer;
//			flag=(unsigned char *)vector+48*2*i+48*2;
			j=exchangeBuffer;
			while(j<48)
			{
				for(w=crossPoint[0];w<exchangeBuffer;w++)//判断是否有重复
				{
					if(*(Index1+j)==*(Index2+w))break;
				}
				if(w==exchangeBuffer)//无重复就复制
				{
					
					if(citycount==48-exchangeBuffer)
						pointer=(unsigned char *)vector+48*2*i+48;
					citycount++;
					*pointer++ = *(Index1+j);
				}
				j++;
			}
			j=0;
			while(j<exchangeBuffer)
			{
				for(w=crossPoint[0];w<exchangeBuffer;w++)//判断是否有重复
				{
					if(*(Index1+j)==*(Index2+w))break;
				}
				if(w==exchangeBuffer)//无重复就复制
				{
					
					if(citycount==48-exchangeBuffer)
						pointer=(unsigned char *)vector+48*2*i+48;
					citycount++;
					*pointer++ = *(Index1+j);
				}
				j++;
			}
			for(w=crossPoint[0];w<exchangeBuffer;w++)
				*pointer++ = *(Index2+w);
/*//////////////////////////////////////////////判断是否有重复
			if(pointer!=flag)
			{
				printf("zidai2  cp0=%d,cp1=%d,count=%d\n",crossPoint[0],crossPoint[1],count);
				return 0;
			}
///////////////////////////////////////////*/
		}
		pointer=(unsigned char *)vector;
		
	/*****************以一定概率在vecter中进行变异，结果(下一代种群)放入vecter****************************/	
		
		for(i=0;i<groupsize;i++)
		{
			select = 0;
			while(select==0)
				select = ((double)rand()) / RAND_MAX;
			if(select<pMutation)//根据变异率决定是否突变
			{
				//随机获得两个不同的交叉点，并升序排列
				crossPoint[0] = crossPoint[1];
				while(crossPoint[0] == crossPoint[1])
				{
					crossPoint[0]=(unsigned int)(rand() % 48);
					crossPoint[1]=(unsigned int)(rand() % 48);
				}
				if(crossPoint[0]>crossPoint[1])
				{
					exchangeBuffer=crossPoint[0];
					crossPoint[0]=crossPoint[1];
					crossPoint[1]=exchangeBuffer;
				}
				exchangeBuffer=crossPoint[1]+1;
				Index1= pointer+48*i;
				Index2= pointer_a;
				//在某个个体vector[i][0]~vector[i][47]中交换两个城市的顺序，即产生一次突变
				for(j=crossPoint[0];j<exchangeBuffer;j++)
				*Index2++ = *(Index1+j);
				for(j=crossPoint[0];j<exchangeBuffer;j++)
				*(Index1+j) = *(--Index2);
			}
		}
	}
}

int main()
{
	int TestSet[48][2]={             //0~47共48个城市坐标
		{ 6734 ,1453},
		{ 2233 ,10  },  
		{ 5530 ,1424},
		{ 401  ,841 },
		{ 3082 ,1644},
		{ 7608 ,4458},
		{ 7573 ,3716},
		{ 7265 ,1268},
		{ 6898 ,1885},
		{ 1112 ,2049},
		{ 5468 ,2606},
		{ 5989 ,2873},
		{ 4706 ,2674},
		{ 4612 ,2035},
		{ 6347 ,2683},
		{ 6107 ,669 },
		{ 7611 ,5184},
		{ 7462 ,3590},
		{ 7732 ,4723},
		{ 5900 ,3561},
		{ 4483 ,3369},
		{ 6101 ,1110},
		{ 5199 ,2182},
		{ 1633 ,2809},
		{ 4307 ,2322},
		{ 675  ,1006},
		{ 7555 ,4819},
		{ 7541 ,3981},
		{ 3177 ,756 },
		{ 7352 ,4506},
		{ 7545 ,2801},
		{ 3245 ,3305},
		{ 6426 ,3173},
		{ 4608 ,1198},
		{ 23   ,2216},
		{ 7248 ,3779},
		{ 7762 ,4595},
		{ 7392 ,2244},
		{ 3484 ,2829},
		{ 6271 ,2135},
		{ 4985 ,140 },
		{ 1916 ,1569},
		{ 7280 ,4899},
		{ 7509 ,3239},
		{ 10   ,2676},
		{ 6807 ,2993},
		{ 5185 ,3258},
		{ 3023 ,1942}
	};
	unsigned char result[48]={0};
	char i=0;
	double distance=0;
	
	distance=GA_TSP((int *)TestSet,result);
	
	printf("The order of GA_TSP is\n");
	while(i<48)
	{
		printf("%d " , result[i] );
		i++;
	}
	printf("\nThe distance of GA_TSP is\n");
	printf("%f\n",distance);
	
	return 0;
}
