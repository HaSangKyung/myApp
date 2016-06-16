/*
	���� ��� ���α׷��� Genetic �˰��� ���.

	�ۼ���: 2008�� 1�� 12��.
	1��������: 2008�� 1�� 13��.
	2��������: 2008�� 1�� 18��. -------> ����ȣ�� Int ������ Char[] ������ ����.

	���: PHP���� �Ķ���͸� ���Ϸ� ����صθ�, �װ��� �о�� �˰����� �����ϰ�, OUTPUT�� ���Ϸ� ���.
	�Ķ���� ���ϸ�: argument.txt
	��� ���ϸ�: result.txt

		argument.txt ���Ͽ����� �Ķ���� ����.
		�� �Ķ���ʹ� \n ���� ���еǾ� �ִ�.
	
		1��°: Target Cost.
		2��°: ������ �� ��.
		3��°: CrossOver Rate. (������)
		4��°: Mutation Rate.	(����������)
		5��°: Population Size. (��ü��)
		6��°: Max Generation.	(�ִ� �����)
		7��°: a�� ��ȣ.(.., -1, -2, 1..)
		8��°: a�� ���̸�		
		9��°: a�� � �ǿ� ������ ������Ÿ�� ����.
		10��°: a�� �濡 ������ ����.(õ��, ����, �ٴ� ���)
		11, 12��°: a�� � �ǿ� ������ ������Ÿ�� �ڵ� �� ����.
		13, 14��°: a�� � �ǿ� ������ ������Ÿ�� �ڵ� �� ����.
		....
		*��°: n�� ��ȣ.
		*+1��°: a�� ���̸�		
		*+2��°: n�� � �ǿ� ������ ������Ÿ�� ����.
		*+3��°: a�� �濡 ������ ����.(õ��, ����, �ٴ� ���)
		*+4, *+5��°: n�� � �ǿ� ������ ������Ÿ�� �ڵ� �� ����.
		*+6, *+7��°: n�� � �ǿ� ������ ������Ÿ�� �ڵ� �� ����.

		
		result.txt ������ ����.

		��� ������ ������ �˰����� ���� ���, �����̶�� �������� ������Ÿ�Ե��� ������ ��ϵǾ� �ִ�.
		
		���� ����.

		(����ȣ ���̸� ������Ÿ���ڵ� �ܰ� �Ѱ��� ������Ÿ����ü���� ���յ�) **�ݺ�.

		������ ��ϵǾ� �ִµ�, Population * maxGeneration ��ŭ�� �����̴�.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define CODEMAX 20		// �ڵ��� �ִ� ���ڼ�.
#define PROTOMAX 50		// ������Ÿ�� �׷��� ���� �� �ִ� �ִ��� ������Ÿ�� ����.
#define FLOORMAX 50		// ���� �ִ� ����.
#define INITMAX 100		// �ʱ������� �ִ� ����.
#define ROOMMAX 40		// ���̸� �ִ� ���ڼ�.
#define RESULTMAX 1000	// ����� �ִ� ����.
#define FLOORCNT 10		// ���̸� ���ڼ��� �ִ� ����.

#define EVAL(BUDGET, GOLD) (1 / fabs(BUDGET-GOLD))	// ���Լ� ��ũ��. 
													// ���ϰ��� �Ǽ��̹Ƿ� fabs ��ũ�θ� ���.


// ������Ÿ�� ����ü. (������ ����ü)
typedef struct _Proto
{
	char code[CODEMAX];		// ������Ÿ�� �ڵ�.
	int unitgold;			// ������Ÿ�� �ܰ�.
	int totalgold;			// �ܰ� * ���� 
} Proto;

// ��, �� ����ü.
typedef struct _ProtoGroup
{
	int cnt;					// ���밡���� ������Ÿ���� ����.
	char floor[FLOORCNT];					// ����(����ȣ).
	int area;					// ����.
	char room[ROOMMAX];			// ���̸�.
	Proto Prototype[PROTOMAX];	// ���밡���� ������Ÿ�Ե�.
} Floor;

// ����ü �ϳ��� ����.
typedef struct _Gene
{
	char floor[FLOORCNT];			// ��.
	char room[ROOMMAX];		// ���̸�.
	char code[CODEMAX];		// �ڵ�.
	int unitgold;
	int totalgold;			// area * �ܰ�.
} Gene;

// �ʱ����� ����ü.
typedef struct _GenePopul
{
	Gene gene[FLOORMAX];
	int populgold;			// ������ Ÿ�Ե����� ����.
	double prop;			// ���յ�.
	double selectProb;		// ���õ� ����Ȯ��.
} GenePopul;

Floor flo[FLOORMAX];		// ��ü ����.(������)
GenePopul popul[INITMAX];	// �ʱ�����.

int budget;					// ����.
int floorCnt;				// ������ ����.(������ ������ ����)
int crossRate;				// ������.
int mutationRate;			// ����������.
int populationSize;			// ����ũ��.
int maxGeneration;			// �ִ� ����.

void init_gene();
void prop_eval();
void rulletselect();
void crossover();
void mutation();

void print();

int main()
{
	int i, j;		
	int cnt = 0;
	int total_cnt;
	FILE* fp;
	FILE* fp2;

	fp = fopen("argument.txt", "r");	// �Ķ���� ���� Read ������.
	fp2 = fopen("result.txt", "w");		// ��� ���� Write ������.

	if(fp == NULL)
	{
		printf("������ �������� �ʽ��ϴ�.\n");
		return 0;
	}

	fscanf(fp, "%ld", &budget);			// ����.
	fscanf(fp, "%d", &floorCnt);		// ������ �� ��.
	fscanf(fp, "%d", &crossRate);		// ������.
	fscanf(fp, "%d", &mutationRate);	// ����������.
	fscanf(fp, "%d", &populationSize);	// ���� ũ��.
	fscanf(fp, "%d", &maxGeneration);	// �ִ� ����.
		
	// ���ڸ� ����ü�� ����.
	for(i = 0; i < floorCnt; i++)
	{
		fscanf(fp, "%s", flo[i].floor);						// ����ȣ
		fscanf(fp, "%s", flo[i].room);							// ���̸�
		fscanf(fp, "%d", &(flo[i].cnt));						// ������Ÿ�� ����
		fscanf(fp, "%d", &(flo[i].area));						// ����
		for(j = 0; j < flo[i].cnt; j++)
		{			
			fscanf(fp, "%s", flo[i].Prototype[j].code);			// ������Ÿ���ڵ�
			fscanf(fp, "%d", &(flo[i].Prototype[j].unitgold));	// ��������.
			flo[i].Prototype[j].totalgold = flo[i].area * flo[i].Prototype[j].unitgold;	// ���� * ��������.
		}
	}

	// ������ �ִ뼼����� �ݺ�.
	fprintf(fp2, "%d\n", floorCnt);		// ���������� ���� ���� ���Ͽ� ���.
	for(total_cnt = 0; total_cnt < maxGeneration; total_cnt++)
	{
		init_gene();	// �ʱ� ���� ���� ����.
		prop_eval();	// ���յ� ��.

		while(cnt < 1000)		// ���������� ���� �ָ��ѵ�.. �ϴ� 1000�� ����.
		{
			rulletselect();		// ������ ��ü ����.(�귿 ���)
			crossover();		// ���� ����.
			mutation();			// �������� ����.
			prop_eval();		// ���յ� ��.
		//	print();
			cnt++;
		}			

		// �ߺ������͸� �����ϸ鼭 ���Ͽ� ����.						
		for(i = 0; i < populationSize; i++)
		{		
			for(j = 0; j < floorCnt; j++)
			{				
				fprintf(fp2, "%s ", popul[i].gene[j].floor);
				fprintf(fp2, "%s ", popul[i].gene[j].room);
				fprintf(fp2, "%s ", popul[i].gene[j].code);
				fprintf(fp2, "%d ", popul[i].gene[j].unitgold);
				fprintf(fp2, "%d ", popul[i].gene[j].totalgold);				
				fprintf(fp2, "%d ", popul[i].populgold);
				fprintf(fp2, "%.10f\n", popul[i].prop);
			}
		}	

	
		cnt = 0;
	}	

	printf("result.txt ���� �Ϸ�\n");

	fclose(fp);
	fclose(fp2);
	return 1;
}

/*
	�ʱ����� ���� �Լ�.
*/
void init_gene()
{
	int i, j, k;
	
	srand(time(NULL));
	for(i = 0; i < populationSize; i++)
	{
		for(j = 0; j < floorCnt; j++)
		{						
			k = rand() % flo[j].cnt;									// ���� ���� ����.
			strcpy(popul[i].gene[j].floor, flo[j].floor);						// �� ���� �Է�.
			strcpy(popul[i].gene[j].room, flo[j].room);					// �� �̸� �Է�.
			strcpy(popul[i].gene[j].code, flo[j].Prototype[k].code);	// ������Ÿ�� �����ϰ� ������.
			popul[i].gene[j].unitgold = flo[j].Prototype[k].unitgold;
			popul[i].gene[j].totalgold = flo[j].Prototype[k].totalgold;	// ������Ÿ�� �ݾ׵� ������.
			popul[i].populgold += popul[i].gene[j].totalgold;			// �� �ݾ׵� ������.
		}
	}
}

/*
	���յ� ���� �Լ�.
*/
void prop_eval()
{
	int i, j;
	double t_total = 0;	// ��ü������ �� ���յ�.

	// �� �ݾ� �� ���.
	for(i = 0; i < populationSize; i++)
	{
		popul[i].populgold = 0;
		for(j = 0; j < floorCnt; j++)
		{
			popul[i].populgold += popul[i].gene[j].totalgold;
		}
	}

	// �� ����ü�� ���յ� ��� �� �� ���յ� ���.
	for(i = 0; i < populationSize; i++)
	{
		popul[i].prop = EVAL(budget, popul[i].populgold);
		t_total += popul[i].prop;
	}

	// �귿 ������ �ϱ����� ����Ȯ�� ���.
	for(i = 0; i < populationSize; i++)
	{
		if(i == 0)
			popul[i].selectProb = popul[i].prop / t_total;
		else
			popul[i].selectProb = popul[i - 1].selectProb + (popul[i].prop / t_total);
	}
}

/*
	�귿 ���� ���� �Լ�.
*/
void rulletselect()
{
	int i, j;
	int k = 0;
	double prob;
	GenePopul select[INITMAX];	// �귿�� ���� ���õ� ��ü���� ������ ����.

	srand(time(NULL));

	// select pool �� �ϴ� ����.
	for(i = 0; i < populationSize; i++)
	{
		prob = (double)rand() / ((double)RAND_MAX + 1); 
	
		for(j = 0; j < populationSize; j++)
		{
			// �����ϰ� �߻��� �Ǽ������� ���� �귿 ����.
			if(popul[j].selectProb <= prob && popul[j + 1].selectProb >= prob)
			{
				select[k] = popul[j + 1];
			}
			else if(popul[0].selectProb >= prob)
			{
				select[k] = popul[0];
			}
		}
		k++;
	}


	// ���õ� �����ڵ��� �ٽ� ����.
	for(i = 0; i < populationSize; i++)
	{
		popul[i] = select[i];
	}
}

/*
	���迬�� �Լ�.
*/
void crossover()
{
	int i;
	int rate;
	int cut;
	int crossCnt = 0;
	int cross1 = 0, cross2 = 0;	
	char bitmap[INITMAX];

	// ���迡 ���� �ӽ� ������.
	char tempcode[CODEMAX];
	char tempfloor[FLOORCNT];
	int temptotalgold;
	int tempunitgold;
	char temproom[ROOMMAX];

	// ���踦 �������� ���� ����ü�� �������� ���� ��Ʈ�� ����.
	for(i = 0; i < populationSize; i++)
	{
		bitmap[i] = 0;	// 0�̸� �̽���, 1�̸� ����.
	}
	
	srand(time(NULL));
	rate = rand() % 100;

	if(rate < crossRate)	// �־��� ����Ȯ�� ������ ���� ����.
	{				
		do
		{
			cut = rand() % floorCnt;	// Ŀ�� ����Ʈ ����.
		} while(cut == 0);	// 0�� �� �� ������.					

		while(crossCnt < (populationSize / 2))	// ���� ���� �� ��ŭ �������. Ȧ���� 1�� ��������.
												// �ϴ� ���⼱ ������� ����.
		{
			
			// ���� �ٸ� �� �ε����� ������ ����.
			do
			{
				cross1 = rand() % populationSize;
				cross2 = rand() % populationSize;
			} while(cross1 == cross2 || bitmap[cross1] || bitmap[cross2]);							
				
			bitmap[cross1] = 1;		// �����ߴٰ� ǥ��.
			bitmap[cross2] = 1;			

			// ������ Ŀ������Ʈ ���� ���� �ǽ�.
			for(i = 0; i < cut; i++)
			{
				
				// �ڵ� ����.
				strcpy(tempcode, popul[cross1].gene[i].code); 
				strcpy(popul[cross1].gene[i].code, popul[cross2].gene[i].code);
				strcpy(popul[cross2].gene[i].code, tempcode);

				// ���̸� ����.
				strcpy(temproom, popul[cross1].gene[i].room); 
				strcpy(popul[cross1].gene[i].room, popul[cross2].gene[i].room);
				strcpy(popul[cross2].gene[i].room, temproom);

				// ���� ����.
				strcpy(tempfloor, popul[cross1].gene[i].floor); 
				strcpy(popul[cross1].gene[i].floor, popul[cross2].gene[i].floor);
				strcpy(popul[cross2].gene[i].floor, tempfloor);

				// ������Ÿ�� �ܰ� ����.
				tempunitgold = popul[cross1].gene[i].unitgold; 
				popul[cross1].gene[i].unitgold = popul[cross2].gene[i].unitgold;
				popul[cross2].gene[i].unitgold = tempunitgold;

				// ������Ÿ�� ��ü ���� ����.
				temptotalgold = popul[cross1].gene[i].totalgold; 
				popul[cross1].gene[i].totalgold = popul[cross2].gene[i].totalgold;
				popul[cross2].gene[i].totalgold = temptotalgold;
				
			}			
			crossCnt++;
		}
	}				
}

/*
	�������� �Լ�.
*/
void mutation()
{
	int i;
	int rate;
	int dst, src;

	srand(time(NULL));
	// ��� ���뿡 �������� ���� �õ�.
	for(i = 0; i < populationSize; i++)
	{		
		rate = rand() % 100;
		if(rate < mutationRate)		// �־��� �������� Ȯ�� ������ �������� ����.
		{
			do
			{
				dst = rand() % floorCnt;		// ����� ��ü ����.
				src = rand() % flo[dst].cnt;	// ������ ��ü ����.
			} while(strcmp(popul[i].gene[dst].code, flo[dst].Prototype[src].code) == 0);

			strcpy(popul[i].gene[dst].room, flo[dst].room);
			strcpy(popul[i].gene[dst].code, flo[dst].Prototype[src].code);
			popul[i].gene[i].unitgold = flo[dst].Prototype[src].unitgold;
			popul[i].gene[i].totalgold = flo[dst].area * flo[dst].Prototype[src].unitgold;
		}
	}
	
}

// ����� ����Ʈ.
void print()
{
	int i, j;
	for(i = 0; i < populationSize; i++)
	{	
		for(j = 0; j < floorCnt; j++)
		{
			printf("%d  ", popul[i].gene[j].totalgold);
		}
		printf("%d\n", popul[i].populgold);
	}
}

