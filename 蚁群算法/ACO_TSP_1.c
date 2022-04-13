#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//相关可修改参数
#define END  1000//迭代次数
#define groupsize  48 //蚂蚁种群大小应大于等于城市数
#define InitialPheromones 0.1
#define a 1//信息素重要因子
#define b 2//期望重要因子
#define p 0.99//信息素挥发因子
#define Q 1
#define TABLENULL 0xffffffff

double ACO_TSP(int *TestSetPointor, unsigned int *ResultPointor)
{
		//后面会对以下定义的变量初始化
	struct antStruct{
		unsigned int AllowedCity[48];
		unsigned int currentCity;
		unsigned int tabu[48];
		double 		 L;
	}__attribute__((aligned (4)));
	struct antStruct	ant[groupsize];
	unsigned int i,j,k,v,count,t;
	double Attraction;
	unsigned int table[groupsize];
	unsigned int initialPlance[groupsize];
	double x,y;
	double probability[48]; //第i个城市对蚂蚁的吸引力比为probability[i]
	
	double PheromoneConcentration[48][48];
	double PheromoneRecords[48][48];
	double Distance[48][48];
	

		srand((unsigned int)time(NULL));
		for(i=0;i<48;i++)//初始化各个城市之间信息素浓度表和距离表
			for(j=0;j<48;j++)
			{
				PheromoneConcentration[i][j]=InitialPheromones;
				PheromoneConcentration[j][i]=InitialPheromones;
				x =pow((double)(*(TestSetPointor +i*2  ) - *(TestSetPointor + j*2  )),2);
				y =pow((double)(*(TestSetPointor +i*2+1) - *(TestSetPointor + j*2+1)),2);
				y=sqrt(x + y);
				Distance[i][j]=y;
				Distance[j][i]=y;
			}
		

		//初始化蚂蚁的tabu[0],也即初始位置
		for(i=0;i<groupsize;i++)//得到groupsize个顺序机排序的蚂蚁
		{
				table[i]=i;
//				ant[i].tabu[0]=TABLENULL;
		}
		for(i=0;i<5000;i++)//随机交换5000次得到groupsize个随机排序的蚂蚁
			{
				j=rand()%groupsize;
				v=rand()%groupsize;
				k=table[j];
				table[j]=table[v];
				table[v]=k;
			}
		/* for(i=0;i<groupsize;i++)
		{
			printf("tabble[%d]=%d  ",i,table[i]);
			printf("\n");
		} */
		//tabu[0]会一直保存蚂蚁的初始城市
		for(i=0;i<48;i++)//城市选蚂蚁，保证每个城市都有蚂蚁
		{
			v = table[i];
			ant[v].tabu[0]=i;
		}
		for(i=48;i<groupsize;i++)//未被城市选上的蚂蚁选城市选蚂蚁，保证每个蚂蚁都有城市
		{
			v = table[i];
			ant[v].tabu[0] = rand() % 48;
		}
		/* for(i=0;i<groupsize;i++)
		{
			printf("ant[%d].tabu[0]=%d  ", i , ant[i].tabu[0]);
			printf("\n");
		} */
	/*****************迭代开始*************************/
	for(t=0;t<END;t++)
	{	
		//将蚂蚁放回初始位置
		for(i=0;i<groupsize;i++)
			ant[i].currentCity=ant[i].tabu[0];
		//初始化蚂蚁的AllowedCity和L两个参数，currentCity参数在执行这里之前已经设置好，tabu参数除tabu[0]的其他tabu可以覆盖就不用反复清零
		for(i=0;i<groupsize;i++)
		{
			for(j=0;j<48;j++)
				if(j==ant[i].currentCity)
					ant[i].AllowedCity[j]=TABLENULL;
				else 
					ant[i].AllowedCity[j]=j;
			ant[i].L=0;
		}
		//清空每次迭代对总的气味浓度表的影响记录表
		for(i=0;i<48;i++)
			for(j=0;j<48;j++)
				PheromoneRecords[i][j]=0;
			
		///循环48 次完成一次城市遍历，即一次迭代
		for(count=1;count<48;count++)//起点绕路到终点，跳47次
		{
			//求概率并移动
			for(i=0;i<groupsize;i++)//将groupsize个蚂蚁从当前城市以各自allowedCity对其的吸引力比，移动到下一个城市
			{	
				k=0;
				Attraction=0;
				for(j=0;j<48;j++)//每次换蚂蚁时需要清零下一个城市选择吸引力比
				{
					probability[j]=0;
				}
				for(j=0;j<48;j++)//将allowedCit中城市不为空的城市提出并序放入table
				{
					if(ant[i].AllowedCity[j]!=TABLENULL)
					{
						table[k]=ant[i].AllowedCity[j];
						k++;
					}
				}
				
				for(j=0;j<k;j++)//计算AllowedCity中table[k]这个城市（记作A）对当前城市位置蚂蚁的吸引力大小，并累加所有A对当前城市位置蚂蚁的吸引力
				{
					y=pow(1000/Distance[ ant[i].currentCity ][table[j]] ,b);
					x=pow(PheromoneConcentration[ ant[i].currentCity ][ table[j] ] ,a);
					if(x==0)return 0;
					probability[ table[j] ]=x*y;
					Attraction+=probability[ table[j] ];
					/* if(i==0)
					{
						printf("t[%d]=%d,dis=%f,%f,exp=%f,Ph=%f,prob=%f\n" ,j,table[j],Distance[ ant[i].currentCity ][table[j]],1000/Distance[ ant[i].currentCity ][table[j]],y,x,probability[ table[j] ]);
						//printf("exp=%f\n",pow(1000/Distance[ ant[i].currentCity ][table[j]] ,b));
						
					} */
				}
				//得到选中概率阈值
				
				probability[0]=probability[0] /  Attraction;
				for(j=1;j<48;j++)
					probability[j]=probability[j-1] +probability[j]/  Attraction;
				probability[ table[k-1] ]=1;//allowedCity中最后一个元素累加阈值时置一优化，防止浮点运算不精准所有比列累加后不为1
				
				
				
				// if(t<50&&count==1&&i==0)
					// for(j=0;j<48;j++)
						// printf("probability[%d]=%f\n", j , probability[j]);
					
					
					
					
					
				y = (double)rand() / RAND_MAX;
				for(j=0;;j++)
				{
					if(y<=probability[j])//随机选择
					{//选择第ant[i].AllowedCity[table[j]]个城市//由于probability[table[k-1]]=1，所以随着i增长这里一定会触发
					 //往所选择的城市移动
						v = ant[i].currentCity;
						ant[i].L += Distance[v][j];
						x = Q / ant[i].L;
						PheromoneRecords[v][j] +=x;
						PheromoneRecords[j][v] +=x;
						ant[i].currentCity=j;
						ant[i].tabu[count]=j;
						for(v=0;;v++)
							if(ant[i].AllowedCity[v]==j)
							{
								ant[i].AllowedCity[v]=TABLENULL;
								break;
							}
						
						break;
					}
				}
			}
		}
		//终点直接跳回起点
		
		for(i=0;i<groupsize;i++)
		{
			v = ant[i].tabu[47];
			j = ant[i].tabu[0];
			ant[i].L += Distance[v][j];
			x = Q / ant[i].L;
			PheromoneRecords[v][j] +=x;
			PheromoneRecords[j][v] +=x;
		}
		//更新气味浓度表，以便下次迭代使用
		for(i=0;i<48;i++)
			for(j=0;j<48;j++) 
				PheromoneConcentration[i][j] = PheromoneConcentration[i][j]*p + PheromoneRecords[i][j];
		
		
		x = ant[0].L;
		j = 0;
		Attraction=0;
		for(i=0;i<groupsize;i++)//比较得到最小L
		{
			Attraction+=ant[i].L;
			if(ant[i].L < x)
			{
				x = ant[i].L;
				j = i;
			}
			
		}
		printf("t=%d,distance=%f,average=%f\n" ,t,ant[j].L,Attraction/groupsize);
		/* for(i=0;i<48;i++)
		{
			for(j=0;j<48;j++)
				printf("Ph[%d][%d]=%f  ",i,j,PheromoneConcentration[i][j]);
			printf("\n");
		} */
			
	}
	x = ant[0].L;
	j = 0;
	for(i=0;i<groupsize;i++)//比较得到最小L
		if(ant[i].L < x)
		{
			x = ant[i].L;
			j = i;
		}
	for(i=0;i<48;i++)
		*ResultPointor++ = ant[j].tabu[i];
	return	x;
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
	unsigned int result[48]={0};
	double distance=0;
	char i=0;
	
//	printf("okokokok\n");
	
	distance=ACO_TSP((int *)TestSet,result);
	
	printf("The order of ACO_TSP is\n");
	while(i<48)
	{
		printf("%d " , result[i] );
		i++;
	}
	printf("\nThe distance of ACO_TSP is\n");
	printf("%f\n",distance);
	
	return 0;
}
