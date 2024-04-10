#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define SIZE 52		//카드 개수
#define PLAYER 4	//플레이어
#define NUM 7		//플레이어가 받는 카드 수 

struct card {	//카드 정보 EX)모양, 숫자 등
	int num;
	char* pattern;
}typedef Card;

struct hand_ranks {
	int rank; //1:원페어,2:투페어,3:트리플,4:스트레이트,5:플러쉬,6:풀하우스,7:포카드 
	int buf1; //하나의 카드 숫자만 기억해도 될때
	int buf2; //두개 필요할때
	char* flush; //플러쉬 모양 기억
}typedef Point;

void init_card(Card*);
void shuffle_card(Card*);
void spray_card(Card*);
void scoring(Card*,int[]);
void print_scores(int[]);
void sort_card(Card*);
void get_hand_ranks(Card*,Point*);
void determine_rank(Point*);

int main() {
	Card pack[SIZE];
	int i,scores[SIZE];
	Point point[PLAYER];

	srand((unsigned)time(NULL));
	
	init_card(pack);
	shuffle_card(pack);
	spray_card(pack);
	scoring(pack,scores);
	print_scores(scores);
	get_hand_ranks(pack,point);


	for (i = 0; i < PLAYER; i++) {
		printf("Player%d:\t",i+1);
		switch (point[i].rank) {
		case 1:
			printf("%d 원페어 입니다. \n", point[i].buf1);
			break;
		case 2:
			printf("%d %d 투페어 입니다.\n", point[i].buf1, point[i].buf2);
			break;
		case 3:
			printf("%d 트리플 입니다. \n", point[i].buf1);
			break;
		case 4:
			printf("%d %d %d %d %d 스트레이트 입니다. \n", point[i].buf1, point[i].buf1 + 1, point[i].buf1 + 2, point[i].buf1 + 3, point[i].buf1 + 4);
			break;
		case 5:
			printf("%s 플러쉬 입니다.\n", point[i].flush);
			break;
		case 6:
			printf("%d %d 풀하우스 입니다. \n", point[i].buf1, point[i].buf2);
			break;
		case 7:
			printf("%d 포카드입니다.\n", point[i].buf1);
			break;
		default:
			printf("점수가 없습니다. \n");
		}
	}
	printf("\n");
	determine_rank(point);
	return 0;
}
void init_card(Card* c) {			//카드 초기화
	int i, j = 0;
	static char* p[] = { "Spades","Hearts","Diamonds","Clubs" };
	for (i = 0; i < SIZE; i++) {
		if (i % 13 == 0)
			j++;
		c[i].pattern = p[j - 1];
		c[i].num = (i % 13) + 1;
	}
}
void shuffle_card(Card* c) {		//카드 섞기

	struct card tmp[SIZE];
	int check[SIZE] = { 0 };
	int i, buf;

	for (i = 0; i < SIZE; i++)
		while (1) {

			buf = rand() % SIZE;

			if (check[buf] == 0) {
				tmp[buf] = c[i];
				check[buf] = 1;
				break;
			}
		}

	for (i = 0; i < SIZE; i++) {
		c[i] = tmp[i];
	}

	printf("%d장의 카드가 다음과 같이 섞였습니다. \n", SIZE);
	printf("-------------------------------------\n");

	for (i = 0; i < SIZE; i++)
		printf("Card%3d:\t%8s,%5d\n", i + 1, c[i].pattern, c[i].num);
	printf("\n");
}

void spray_card(Card* c) {
	int i, j;
	printf("%d명의 PLAYER에게 %d장씩 Card 배분\nCard No.\t", PLAYER, NUM);
	for (i = 0; i < PLAYER; i++)
		printf("  PLAYER%3d\t", i + 1);
	printf("\n");
	for (i = 0; i < PLAYER; i++)
		printf("-------------------");
	printf("\n");
	for (i = 0; i < NUM; i++) {
		printf("Card%3d:\t", i + 1);
		for (j = 0; j < PLAYER; j++)
			printf("%8s,%5d\t", c[NUM * j + i].pattern, c[NUM * j + i].num);
		printf("\n");
	}
	printf("\n");
}


//Turn 별로 점수를 주는 함수 숫자가 커질수록 높은 점수 획득

void scoring(Card* c, int result[]) {
	int i, j, count = 0, max = 0, max_index;
	int temp[52];

	for (i = 0; i < SIZE; i++) {                                             //카드의 문양을 보고 14의 배수를 곱한다, 숫자 비교는 14로 나눈 나머지로 하지만 만약 그가 같을 경우 원본으로 비교한다.
		if (!strcmp("Spades", c[i].pattern))
			temp[i] = (c[i].num + 14 * 3);
		else if (!strcmp("Hearts", c[i].pattern))
			temp[i] = (c[i].num + 14 * 2);
		else if (!strcmp("Diamonds", c[i].pattern))
			temp[i] = (c[i].num + 14 * 1);
		else
			temp[i] = c[i].num;
	}

	for (i = 0; i < NUM; i++) {
		while (count != PLAYER) {
			for (j = 0; j < PLAYER; j++) {
				if (temp[j + (PLAYER * i)] % 14 > max % 14) {
					max = temp[j + (PLAYER * i)];
					max_index = j + (PLAYER * i);
				}
				else if (temp[j + (PLAYER * i)] % 14 == max % 14)
					if (temp[j + (PLAYER * i) - 1] > max) {
						max = temp[j + (PLAYER * i)];
						max_index = j + (PLAYER * i);
					}
			}
			max = 0;														//1번의 Turn이 끝날 때 마다 count 변수 변경, 결과 값 저장 및 변수 초기화 
			result[max_index] = PLAYER - 1 - count;
			temp[max_index] = -1;
			count++;
		}
		count = 0;
	}
}

//주어진 7개의 카드가 들어있는 카드팩을 족보에 따라 hand_rank를 부여하는 함수
//크게 3가지 경우로 나눔 1: 모양이 같은 경우 2: 숫자가 같은 경우 3: 숫자가 연속되는경우

void get_hand_ranks(Card* c,Point *point) {
	int i, j, flush[4] = { 0 };
	int buf = 0, pair = 0, two_pair = 0, triple = 0;
	Card cards[7];
	for (i = 0; i < PLAYER; i++) {
		buf = 0;
		pair = 0;
		two_pair = 0;
		triple = 0;
		point[i].rank = 0;
		for (j = 0; j < 4; j++) {
			flush[j] = 0;
		}
		for (j = 0; j < 7; j++) {
			cards[j] = c[j + i * 7];
		}
		sort_card(cards);			// 분류하기 편하게 주어진 7개의 카드를 숫자의 크기대로 정렬
		for (j = 0; j < 7; j++) {	//	1. 모양이 같은 경우
			if (!strcmp("Spades", cards[j].pattern)) {
				flush[0]++;
				if (flush[0] == 5) {
					if (point[i].rank < 5) {
						point[i].rank = 5;
						point[i].flush = "Spades";
					}
				}
			}
			else if (!strcmp("Hearts", cards[j].pattern)) {
				flush[1]++;
				if (flush[1] == 5) {
					if (point[i].rank < 5) {
						point[i].rank = 5;
						point[i].flush = "Hearts";
					}
				}
			}
			else if (!strcmp("Diamonds", cards[j].pattern)) {
				flush[2]++;
				if (flush[2] == 5) {
					if (point[i].rank < 5) {
						point[i].rank = 5;
						point[i].flush = "Diamonds";
					}
				}
			}
			else {
				flush[3]++;
				if (flush[3] == 5) {
					if (point[i].rank < 5) {
						point[i].rank = 5;
						point[i].flush = "Clubs";
					}
				}
			}

		}
		for (j = 0; j < 7; j++) {	//	2. 숫자가 같은 경우
			if (cards[j].num == cards[j + 1].num) {	//두장이 같을 경우
				buf = cards[j].num;
				if (pair != 0) {						//투페어
					if (point[i].rank < 2) {
						point[i].rank = 2;
						if (two_pair != 0)
							point[i].buf1 = point[i].buf2;
						point[i].buf2 = buf;
						two_pair = buf;
					}
				}
				else if (triple != 0) {           //풀하우스
					if (point[i].rank < 6) {
						point[i].rank = 6;
						point[i].buf1 = buf;
					}
				}
				else
					if (point[i].rank < 1) {       //원페어
						point[i].rank = 1;
						point[i].buf1 = buf;
					}
				if (cards[j + 1].num == cards[j + 2].num) {    //세장이 같을경우
					triple = cards[j].num;			
					if (pair != 0) {				//풀하우스
						if (point[i].rank < 6) {
							point[i].rank = 6;
							point[i].buf2 = buf;
						}
					}
					if (cards[j + 2].num == cards[j + 3].num) {	//4장이 같을경우
						point[i].rank = 7;
						point[i].buf1 = buf;
					}
					else
						if (point[i].rank < 3) {	//트리플
							point[i].rank = 3;
							point[i].buf1 = buf;
						}
				}
			}
			pair = buf;
		}
		//3. 숫자가 연속 되는 경우
		for (j = 0; j < 3; j++) { //스트레이트는 카드가 7장이니 3번까지만 확인하면됨
			if (cards[j + 1].num - cards[j].num == 1)
				if (cards[j + 2].num - cards[j + 1].num == 1)
					if (cards[j + 3].num - cards[j + 2].num == 1)
						if (cards[j + 4].num == cards[j + 5].num) {
							if (point[i].rank < 4) {
								point[i].rank = 4;
								point[i].buf1 = cards[j].num;
							}
						}
		}
	}
}

//	주어진 7장의 카드를 크기 순으로 정렬하는 함수(모양은 무시하고 숫자로만 정렬)
void sort_card(Card* c) {
	int i, j;
	Card tmp;
	for (j = 6; j > 0; j--) {
		for (i = 0; i < j; i++) {
			if (c[i].num > c[i + 1].num) {
				tmp = c[i];
				c[i] = c[i + 1];
				c[i + 1] = tmp;
			}
		}
	}
}


//	misson1 의 결과값을 출력
void print_scores(int scores[]) {
	int i, j;
	int buf[PLAYER] = { 0 };
	printf("게임 점수\n");
	printf("Turn No.\t");
	for (i = 0; i < PLAYER; i++) printf("  PLAYER%3d\t", i + 1);
	printf("\n");
	for (i = 0; i < PLAYER; i++) printf("--------------------");
	printf("\n");

	for (i = 0; i < NUM; i++) {

		printf("Turn%3d:\t", i + 1);

		for (j = 0; j < PLAYER; j++) {
			buf[j % PLAYER] += scores[j+PLAYER*i];
			printf("%13d\t", buf[j]);
		}
		printf("\n");
	}
	printf("\n");
}

//추가 과제 결과에 따라 순위를 선정하는 함수
void determine_rank(Point* point) {
	int i, j, max_index;
	int min=-1;
	Point max;
	for (i = 0; i < PLAYER; i++) {
		max = point[0];
		max_index = 0;
		for (j = 1; j < PLAYER; j++) {
			if (max.rank < point[j].rank) {
				max_index = j;
				max = point[j];
			}
			else if (max.rank == point[j].rank) { //  (.rank 에 저장된 숫자가 )2: 투페어 5 : 플러쉬 : 6 풀하우스
				if (max.rank == 2 || max.rank == 6) {
					if (max.buf2 < point[j].buf2) {
						max_index = j;
						max = point[j];
					}
					else if (max.buf2 == point[j].buf2) {
						if (max.buf1 < point[j].buf1) {
							max_index = j;
							max = point[j];
						}
					}
				}
				else if (max.rank == 5) {
					if (!strcmp(point[j].flush, "Spades")) {
						max_index = j;
						max = point[j];
					}
					else if (!strcmp(point[j].flush, "Heart")) {
						if (strcmp(max.flush, "Spades")) {
							max_index = j;
							max = point[j];
						}
					}
					else if (!strcmp(point[j].flush, "Diamonds")) {
						if (!strcmp(max.flush, "Clubs")) {
							max_index = j;
							max = point[j];
						}
					}
				}
				else 
					if (max.buf1 < point[j].buf1) {
						max_index = j;
						max = point[j];
					}
			}
			
		}
		if (max.rank == 0) {	//	점수가 없을경우 동점자 처리
			if (min == -1)
				min = i + 1;
			printf("%d등 : Player %d\n", min, max_index + 1);
		}
		else {
			printf("%d등 : Player %d\n", i + 1, max_index + 1);
		}
		point[max_index].rank = -1;
	}
}