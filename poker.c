#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define SIZE 52		//ī�� ����
#define PLAYER 4	//�÷��̾�
#define NUM 7		//�÷��̾ �޴� ī�� �� 

struct card {	//ī�� ���� EX)���, ���� ��
	int num;
	char* pattern;
}typedef Card;

struct hand_ranks {
	int rank; //1:�����,2:�����,3:Ʈ����,4:��Ʈ����Ʈ,5:�÷���,6:Ǯ�Ͽ콺,7:��ī�� 
	int buf1; //�ϳ��� ī�� ���ڸ� ����ص� �ɶ�
	int buf2; //�ΰ� �ʿ��Ҷ�
	char* flush; //�÷��� ��� ���
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
			printf("%d ����� �Դϴ�. \n", point[i].buf1);
			break;
		case 2:
			printf("%d %d ����� �Դϴ�.\n", point[i].buf1, point[i].buf2);
			break;
		case 3:
			printf("%d Ʈ���� �Դϴ�. \n", point[i].buf1);
			break;
		case 4:
			printf("%d %d %d %d %d ��Ʈ����Ʈ �Դϴ�. \n", point[i].buf1, point[i].buf1 + 1, point[i].buf1 + 2, point[i].buf1 + 3, point[i].buf1 + 4);
			break;
		case 5:
			printf("%s �÷��� �Դϴ�.\n", point[i].flush);
			break;
		case 6:
			printf("%d %d Ǯ�Ͽ콺 �Դϴ�. \n", point[i].buf1, point[i].buf2);
			break;
		case 7:
			printf("%d ��ī���Դϴ�.\n", point[i].buf1);
			break;
		default:
			printf("������ �����ϴ�. \n");
		}
	}
	printf("\n");
	determine_rank(point);
	return 0;
}
void init_card(Card* c) {			//ī�� �ʱ�ȭ
	int i, j = 0;
	static char* p[] = { "Spades","Hearts","Diamonds","Clubs" };
	for (i = 0; i < SIZE; i++) {
		if (i % 13 == 0)
			j++;
		c[i].pattern = p[j - 1];
		c[i].num = (i % 13) + 1;
	}
}
void shuffle_card(Card* c) {		//ī�� ����

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

	printf("%d���� ī�尡 ������ ���� �������ϴ�. \n", SIZE);
	printf("-------------------------------------\n");

	for (i = 0; i < SIZE; i++)
		printf("Card%3d:\t%8s,%5d\n", i + 1, c[i].pattern, c[i].num);
	printf("\n");
}

void spray_card(Card* c) {
	int i, j;
	printf("%d���� PLAYER���� %d�徿 Card ���\nCard No.\t", PLAYER, NUM);
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


//Turn ���� ������ �ִ� �Լ� ���ڰ� Ŀ������ ���� ���� ȹ��

void scoring(Card* c, int result[]) {
	int i, j, count = 0, max = 0, max_index;
	int temp[52];

	for (i = 0; i < SIZE; i++) {                                             //ī���� ������ ���� 14�� ����� ���Ѵ�, ���� �񱳴� 14�� ���� �������� ������ ���� �װ� ���� ��� �������� ���Ѵ�.
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
			max = 0;														//1���� Turn�� ���� �� ���� count ���� ����, ��� �� ���� �� ���� �ʱ�ȭ 
			result[max_index] = PLAYER - 1 - count;
			temp[max_index] = -1;
			count++;
		}
		count = 0;
	}
}

//�־��� 7���� ī�尡 ����ִ� ī������ ������ ���� hand_rank�� �ο��ϴ� �Լ�
//ũ�� 3���� ���� ���� 1: ����� ���� ��� 2: ���ڰ� ���� ��� 3: ���ڰ� ���ӵǴ°��

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
		sort_card(cards);			// �з��ϱ� ���ϰ� �־��� 7���� ī�带 ������ ũ���� ����
		for (j = 0; j < 7; j++) {	//	1. ����� ���� ���
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
		for (j = 0; j < 7; j++) {	//	2. ���ڰ� ���� ���
			if (cards[j].num == cards[j + 1].num) {	//������ ���� ���
				buf = cards[j].num;
				if (pair != 0) {						//�����
					if (point[i].rank < 2) {
						point[i].rank = 2;
						if (two_pair != 0)
							point[i].buf1 = point[i].buf2;
						point[i].buf2 = buf;
						two_pair = buf;
					}
				}
				else if (triple != 0) {           //Ǯ�Ͽ콺
					if (point[i].rank < 6) {
						point[i].rank = 6;
						point[i].buf1 = buf;
					}
				}
				else
					if (point[i].rank < 1) {       //�����
						point[i].rank = 1;
						point[i].buf1 = buf;
					}
				if (cards[j + 1].num == cards[j + 2].num) {    //������ �������
					triple = cards[j].num;			
					if (pair != 0) {				//Ǯ�Ͽ콺
						if (point[i].rank < 6) {
							point[i].rank = 6;
							point[i].buf2 = buf;
						}
					}
					if (cards[j + 2].num == cards[j + 3].num) {	//4���� �������
						point[i].rank = 7;
						point[i].buf1 = buf;
					}
					else
						if (point[i].rank < 3) {	//Ʈ����
							point[i].rank = 3;
							point[i].buf1 = buf;
						}
				}
			}
			pair = buf;
		}
		//3. ���ڰ� ���� �Ǵ� ���
		for (j = 0; j < 3; j++) { //��Ʈ����Ʈ�� ī�尡 7���̴� 3�������� Ȯ���ϸ��
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

//	�־��� 7���� ī�带 ũ�� ������ �����ϴ� �Լ�(����� �����ϰ� ���ڷθ� ����)
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


//	misson1 �� ������� ���
void print_scores(int scores[]) {
	int i, j;
	int buf[PLAYER] = { 0 };
	printf("���� ����\n");
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

//�߰� ���� ����� ���� ������ �����ϴ� �Լ�
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
			else if (max.rank == point[j].rank) { //  (.rank �� ����� ���ڰ� )2: ����� 5 : �÷��� : 6 Ǯ�Ͽ콺
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
		if (max.rank == 0) {	//	������ ������� ������ ó��
			if (min == -1)
				min = i + 1;
			printf("%d�� : Player %d\n", min, max_index + 1);
		}
		else {
			printf("%d�� : Player %d\n", i + 1, max_index + 1);
		}
		point[max_index].rank = -1;
	}
}