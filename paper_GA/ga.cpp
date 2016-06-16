/*
	견적 계산 프로그램의 Genetic 알고리즘 모듈.

	작성일: 2008년 1월 12일.
	1차수정일: 2008년 1월 13일.
	2차수정일: 2008년 1월 18일. -------> 층번호를 Int 형에서 Char[] 형으로 변경.

	기능: PHP에서 파라미터를 파일로 기록해두면, 그것을 읽어와 알고리즘을 적용하고, OUTPUT을 파일로 기록.
	파라미터 파일명: argument.txt
	결과 파일명: result.txt

		argument.txt 파일에서의 파라미터 순서.
		각 파라미터는 \n 으로 구분되어 있다.
	
		1번째: Target Cost.
		2번째: 적용할 층 수.
		3번째: CrossOver Rate. (교배율)
		4번째: Mutation Rate.	(돌연변이율)
		5번째: Population Size. (개체수)
		6번째: Max Generation.	(최대 세대수)
		7번째: a층 번호.(.., -1, -2, 1..)
		8번째: a층 방이름		
		9번째: a층 어떤 실에 적당한 프로토타입 갯수.
		10번째: a층 방에 적용할 면적.(천정, 벽면, 바닥 등등)
		11, 12번째: a층 어떤 실에 적당한 프로토타입 코드 및 가격.
		13, 14번째: a층 어떤 실에 적당한 프로토타입 코드 및 가격.
		....
		*번째: n층 번호.
		*+1번째: a층 방이름		
		*+2번째: n층 어떤 실에 적당한 프로토타입 갯수.
		*+3번째: a층 방에 적용할 면적.(천정, 벽면, 바닥 등등)
		*+4, *+5번째: n층 어떤 실에 적당한 프로토타입 코드 및 가격.
		*+6, *+7번째: n층 어떤 실에 적당한 프로토타입 코드 및 가격.

		
		result.txt 파일의 구성.

		결과 파일은 유전자 알고리즘의 적용 결과, 최적이라고 여겨지는 프로토타입들의 조합이 기록되어 있다.
		
		층의 갯수.

		(층번호 방이름 프로토타입코드 단가 총가격 프로토타입전체가격 적합도) **반복.

		순으로 기록되어 있는데, Population * maxGeneration 만큼의 갯수이다.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define CODEMAX 20		// 코드의 최대 글자수.
#define PROTOMAX 50		// 프로토타입 그룹이 가질 수 있는 최대의 프로토타입 개수.
#define FLOORMAX 50		// 층의 최대 개수.
#define INITMAX 100		// 초기집단의 최대 개수.
#define ROOMMAX 40		// 방이름 최대 글자수.
#define RESULTMAX 1000	// 결과물 최대 갯수.
#define FLOORCNT 10		// 층이름 글자수의 최대 갯수.

#define EVAL(BUDGET, GOLD) (1 / fabs(BUDGET-GOLD))	// 평가함수 매크로. 
													// 리턴값이 실수이므로 fabs 매크로를 사용.


// 프로토타입 구조체. (개개의 염색체)
typedef struct _Proto
{
	char code[CODEMAX];		// 프로토타입 코드.
	int unitgold;			// 프로토타입 단가.
	int totalgold;			// 단가 * 면적 
} Proto;

// 층, 실 구조체.
typedef struct _ProtoGroup
{
	int cnt;					// 적용가능한 프로토타입의 갯수.
	char floor[FLOORCNT];					// 층수(층번호).
	int area;					// 면적.
	char room[ROOMMAX];			// 방이름.
	Proto Prototype[PROTOMAX];	// 적용가능한 프로토타입들.
} Floor;

// 염색체 하나의 정보.
typedef struct _Gene
{
	char floor[FLOORCNT];			// 층.
	char room[ROOMMAX];		// 방이름.
	char code[CODEMAX];		// 코드.
	int unitgold;
	int totalgold;			// area * 단가.
} Gene;

// 초기집단 구조체.
typedef struct _GenePopul
{
	Gene gene[FLOORMAX];
	int populgold;			// 프로토 타입들의총 가격.
	double prop;			// 적합도.
	double selectProb;		// 선택될 누적확률.
} GenePopul;

Floor flo[FLOORMAX];		// 전체 정보.(모집단)
GenePopul popul[INITMAX];	// 초기집단.

int budget;					// 예산.
int floorCnt;				// 적용할 층수.(유전자 집단의 갯수)
int crossRate;				// 교배율.
int mutationRate;			// 돌연변이율.
int populationSize;			// 집단크기.
int maxGeneration;			// 최대 세대.

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

	fp = fopen("argument.txt", "r");	// 파라미터 파일 Read 포인터.
	fp2 = fopen("result.txt", "w");		// 결과 파일 Write 포인터.

	if(fp == NULL)
	{
		printf("파일이 존재하지 않습니다.\n");
		return 0;
	}

	fscanf(fp, "%ld", &budget);			// 예산.
	fscanf(fp, "%d", &floorCnt);		// 적용할 층 수.
	fscanf(fp, "%d", &crossRate);		// 교배율.
	fscanf(fp, "%d", &mutationRate);	// 돌연변이율.
	fscanf(fp, "%d", &populationSize);	// 세대 크기.
	fscanf(fp, "%d", &maxGeneration);	// 최대 세대.
		
	// 인자를 구조체에 저장.
	for(i = 0; i < floorCnt; i++)
	{
		fscanf(fp, "%s", flo[i].floor);						// 층번호
		fscanf(fp, "%s", flo[i].room);							// 방이름
		fscanf(fp, "%d", &(flo[i].cnt));						// 프로토타입 갯수
		fscanf(fp, "%d", &(flo[i].area));						// 면적
		for(j = 0; j < flo[i].cnt; j++)
		{			
			fscanf(fp, "%s", flo[i].Prototype[j].code);			// 프로토타입코드
			fscanf(fp, "%d", &(flo[i].Prototype[j].unitgold));	// 단위가격.
			flo[i].Prototype[j].totalgold = flo[i].area * flo[i].Prototype[j].unitgold;	// 면적 * 단위가격.
		}
	}

	// 설정된 최대세대까지 반복.
	fprintf(fp2, "%d\n", floorCnt);		// 적용할층의 갯수 부터 파일에 기록.
	for(total_cnt = 0; total_cnt < maxGeneration; total_cnt++)
	{
		init_gene();	// 초기 랜덤 집단 생성.
		prop_eval();	// 적합도 평가.

		while(cnt < 1000)		// 종료조건이 조금 애매한데.. 일단 1000번 시행.
		{
			rulletselect();		// 재생산될 개체 선택.(룰렛 방식)
			crossover();		// 교배 연산.
			mutation();			// 돌연변이 연산.
			prop_eval();		// 적합도 평가.
		//	print();
			cnt++;
		}			

		// 중복데이터를 제거하면서 파일에 저장.						
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

	printf("result.txt 생성 완료\n");

	fclose(fp);
	fclose(fp2);
	return 1;
}

/*
	초기집단 생성 함수.
*/
void init_gene()
{
	int i, j, k;
	
	srand(time(NULL));
	for(i = 0; i < populationSize; i++)
	{
		for(j = 0; j < floorCnt; j++)
		{						
			k = rand() % flo[j].cnt;									// 랜덤 변수 생성.
			strcpy(popul[i].gene[j].floor, flo[j].floor);						// 층 정보 입력.
			strcpy(popul[i].gene[j].room, flo[j].room);					// 방 이름 입력.
			strcpy(popul[i].gene[j].code, flo[j].Prototype[k].code);	// 프로토타입 랜덤하게 가져옴.
			popul[i].gene[j].unitgold = flo[j].Prototype[k].unitgold;
			popul[i].gene[j].totalgold = flo[j].Prototype[k].totalgold;	// 프로토타입 금액도 가져옴.
			popul[i].populgold += popul[i].gene[j].totalgold;			// 총 금액도 가져옴.
		}
	}
}

/*
	적합도 판정 함수.
*/
void prop_eval()
{
	int i, j;
	double t_total = 0;	// 개체집단의 총 적합도.

	// 총 금액 재 계산.
	for(i = 0; i < populationSize; i++)
	{
		popul[i].populgold = 0;
		for(j = 0; j < floorCnt; j++)
		{
			popul[i].populgold += popul[i].gene[j].totalgold;
		}
	}

	// 각 염색체의 적합도 계산 및 총 접합도 계산.
	for(i = 0; i < populationSize; i++)
	{
		popul[i].prop = EVAL(budget, popul[i].populgold);
		t_total += popul[i].prop;
	}

	// 룰렛 선택을 하기위한 누적확률 계산.
	for(i = 0; i < populationSize; i++)
	{
		if(i == 0)
			popul[i].selectProb = popul[i].prop / t_total;
		else
			popul[i].selectProb = popul[i - 1].selectProb + (popul[i].prop / t_total);
	}
}

/*
	룰렛 선택 연산 함수.
*/
void rulletselect()
{
	int i, j;
	int k = 0;
	double prob;
	GenePopul select[INITMAX];	// 룰렛에 의해 선택된 개체들을 저장할 변수.

	srand(time(NULL));

	// select pool 에 일단 복사.
	for(i = 0; i < populationSize; i++)
	{
		prob = (double)rand() / ((double)RAND_MAX + 1); 
	
		for(j = 0; j < populationSize; j++)
		{
			// 랜덤하게 발생된 실수변수에 의해 룰렛 선택.
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


	// 선택된 유전자들을 다시 복사.
	for(i = 0; i < populationSize; i++)
	{
		popul[i] = select[i];
	}
}

/*
	교배연산 함수.
*/
void crossover()
{
	int i;
	int rate;
	int cut;
	int crossCnt = 0;
	int cross1 = 0, cross2 = 0;	
	char bitmap[INITMAX];

	// 교배에 쓰일 임시 변수들.
	char tempcode[CODEMAX];
	char tempfloor[FLOORCNT];
	int temptotalgold;
	int tempunitgold;
	char temproom[ROOMMAX];

	// 교배를 시행하지 않은 염색체를 가져오기 위한 비트맵 설정.
	for(i = 0; i < populationSize; i++)
	{
		bitmap[i] = 0;	// 0이면 미시행, 1이면 시행.
	}
	
	srand(time(NULL));
	rate = rand() % 100;

	if(rate < crossRate)	// 주어진 교배확률 충족시 교배 시행.
	{				
		do
		{
			cut = rand() % floorCnt;	// 커팅 포인트 설정.
		} while(cut == 0);	// 0이 될 수 없도록.					

		while(crossCnt < (populationSize / 2))	// 세대 수의 반 만큼 교배시행. 홀수면 1이 남겠지만.
												// 일단 여기선 고려하지 않음.
		{
			
			// 서로 다른 두 인덱스를 빼오기 위해.
			do
			{
				cross1 = rand() % populationSize;
				cross2 = rand() % populationSize;
			} while(cross1 == cross2 || bitmap[cross1] || bitmap[cross2]);							
				
			bitmap[cross1] = 1;		// 교배했다고 표시.
			bitmap[cross2] = 1;			

			// 설정한 커팅포인트 까지 교배 실시.
			for(i = 0; i < cut; i++)
			{
				
				// 코드 교배.
				strcpy(tempcode, popul[cross1].gene[i].code); 
				strcpy(popul[cross1].gene[i].code, popul[cross2].gene[i].code);
				strcpy(popul[cross2].gene[i].code, tempcode);

				// 방이름 교배.
				strcpy(temproom, popul[cross1].gene[i].room); 
				strcpy(popul[cross1].gene[i].room, popul[cross2].gene[i].room);
				strcpy(popul[cross2].gene[i].room, temproom);

				// 층수 교배.
				strcpy(tempfloor, popul[cross1].gene[i].floor); 
				strcpy(popul[cross1].gene[i].floor, popul[cross2].gene[i].floor);
				strcpy(popul[cross2].gene[i].floor, tempfloor);

				// 프로토타입 단가 교배.
				tempunitgold = popul[cross1].gene[i].unitgold; 
				popul[cross1].gene[i].unitgold = popul[cross2].gene[i].unitgold;
				popul[cross2].gene[i].unitgold = tempunitgold;

				// 프로토타입 전체 가격 교배.
				temptotalgold = popul[cross1].gene[i].totalgold; 
				popul[cross1].gene[i].totalgold = popul[cross2].gene[i].totalgold;
				popul[cross2].gene[i].totalgold = temptotalgold;
				
			}			
			crossCnt++;
		}
	}				
}

/*
	돌연변이 함수.
*/
void mutation()
{
	int i;
	int rate;
	int dst, src;

	srand(time(NULL));
	// 모든 세대에 돌연변이 적용 시도.
	for(i = 0; i < populationSize; i++)
	{		
		rate = rand() % 100;
		if(rate < mutationRate)		// 주어진 돌연변이 확률 충족시 돌연변이 시행.
		{
			do
			{
				dst = rand() % floorCnt;		// 사라질 개체 선정.
				src = rand() % flo[dst].cnt;	// 생성될 개체 선정.
			} while(strcmp(popul[i].gene[dst].code, flo[dst].Prototype[src].code) == 0);

			strcpy(popul[i].gene[dst].room, flo[dst].room);
			strcpy(popul[i].gene[dst].code, flo[dst].Prototype[src].code);
			popul[i].gene[i].unitgold = flo[dst].Prototype[src].unitgold;
			popul[i].gene[i].totalgold = flo[dst].area * flo[dst].Prototype[src].unitgold;
		}
	}
	
}

// 시험용 프린트.
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

