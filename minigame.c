//	<���� ���� ���α׷�>

/*
��Ģ 1 - 0~RANDOM_NUM ���ڰ� �����ִ� 5x5 ���忡�� ����
��Ģ 2 - ������ ���� ���� �� ���� �����ϸ� ���� ���ڸ�ŭ ��,�Ʒ�,������,�������� �̵�
��Ģ 3 - ������ ���� ������ ������ ��� ���� ����
��Ģ 4 - ������ �ִ� ĭ�� ���ڰ� 0 �� ��� ���� ����
��Ģ 5 - �̵� ��ο� ��ֹ�(-1)�� �ִ� ��� �̵� �Ұ�
��Ģ 3,4 �� ��� �̵� �� ���� ������ ���� ��, ������ �þ(���������� ���ڰ� 0�� ��� �̵��� ������ ������ �ٷ� ����)
*/

/* +���ѷ��� ó��
���ѷ����� �����ϴ� ��� -> �Դ���ǥ�� ���ƿ���

ũ�Ⱑ 5x5�� ���� �ȿ��� ������ ��ǥ�� �ε����� x[0~4][0~4] �� 25����
���� �ߺ����� �ʰ� ���ο� ��ǥ�� �̵��ϴ� �ִ� Ƚ�� ���� 25���� (��ֹ��� �������� ����)
��, �̵� Ƚ���� 25���� �Ѿ�� �ּ� �ѹ� �̻� �湮�ߴ� ��ǥ�� ���ƿ� ���̸� �̴� ���ѷ������� �ǹ���*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 5
#define DEPTH 5
#define MAX_MOVEMENT 25
#define RANDOM_NUMBER 7

// �������� ����
int max_movement_count = 0;

// �Լ� ����
void setBoard(int* square_board, int max_width, int max_depth);
void printBoard(int* square_board, int max_width, int max_depth);
void playBoard(int* square_board, int width, int depth, int movement_count);

// ����1. ����(4) ���� ���� ������ ���� main�� �ۼ�

int main(void)
{
	int square_board[WIDTH][DEPTH];

	srand((unsigned)time(NULL));
	setBoard(square_board, WIDTH, DEPTH);
	printBoard(square_board, WIDTH, DEPTH);
	playBoard(square_board, 0, 0, max_movement_count);

	printf("�ִ� �̵� Ƚ��: %d\n\n", max_movement_count);
	return 0;
}

// ����1. ����(1) setBoard �Լ� �����ϱ�

void setBoard(int* square_board, int max_width, int max_depth)
{
	int obx1, obx2, oby1, oby2 = 0;
	obx1 = rand() % max_width;
	obx2 = rand() % max_width;
	oby1 = rand() % max_depth;
	oby2 = rand() % max_depth;
	while (obx1 == obx2 && oby1 == oby2) // ��ֹ��� �ߺ��� ��� ó��
		oby2 = rand() % max_depth;



	*(square_board + (DEPTH * obx1) + oby1) = -1;
	*(square_board + (DEPTH * obx2) + oby2) = -1;

	for (int i = 0; i < max_depth * max_width; i++) {
		if (*(square_board + i) != -1) {
			*(square_board + i) = rand() % (RANDOM_NUMBER+1); // 0~RANDOM_NUMBER ������ ���� 
		}
	}
}

// ����1. ����(2) printBoard �Լ� �����ϱ�

void printBoard(int* square_board, int max_width, int max_depth)
{
	for (int i = 0; i < max_depth; i++) {
		for (int j = 0; j < max_width; j++) {
			printf("%2d ", *(square_board + j * DEPTH + i));
		}
		printf("\n");
	}
}

  // ����1. ����(3) playBoard �Լ� �����ϱ�
void playBoard(int* square_board, int width, int depth, int movement_count) {
	int *current = (square_board + width * DEPTH + depth); // �Լ��� �μ��� �Է¹��� width, �� depth�� ����Ű�� data���� ����Ű�� �����͸� �����ϴ� ����
	int isDuplicate = 1; // ������ ��Ÿ���� ���� -> False �� ��� �������� �̵��� ���� ����
	int overBoard = 0; // ������ ��Ÿ���� ���� -> ������ ���� ������ ������� True
	
	if (max_movement_count == -1) // ���ѷ����� ��� �Լ� ����
		return;

	// �Է¹��� ��ǥ�� data�� 0���� Ȯ�� ���������� �ƴ� ��� -1�� ���� �� ����
	if (*current == 0 || *current == -1) {   
		if (max_movement_count < movement_count)
			max_movement_count = movement_count;
		return;
	}

	// �Ʒ������� �̵�

	if (depth + *current >= DEPTH) { // 5x5 ���带 �Ѿ�� ���
		overBoard = 1;
		isDuplicate = 0;
	}

	for (int i = 1; i <= *current; i++) { // ���� ����� ������ ���̿� -1 �� �ִ� ���
		if (*(current + i) == -1) {
			isDuplicate = 0;
		}
	}

	if (movement_count >= MAX_MOVEMENT) { // ���ѷ����� ���
		max_movement_count = -1;
		return;
	}

	if (isDuplicate) { // ���������� �������� ������ ���
		playBoard(square_board, width, depth + *current, movement_count+1);
		if (max_movement_count == -1) // ���ѷ����� ��� �Լ� ����
			return;
	}
	isDuplicate = 1;

	// �������� �̵�

	if (depth - *current < 0) { // 5x5 ���带 �Ѿ�� ���
		overBoard = 1;
		isDuplicate = 0;
	}

	for (int i = 1; i <= *current; i++) { // ���� ����� ������ ���̿� -1 �� �ִ� ���
		if ((*current - i) == -1) {
			isDuplicate = 0;
		}
	}

	if (movement_count >= MAX_MOVEMENT) { // ���ѷ����� ���
		max_movement_count = -1;
		return;
	}

	if (isDuplicate) { // ���������� �������� ������ ���
		playBoard(square_board, width, depth - *current, movement_count+1);
		if (max_movement_count == -1) // ���ѷ����� ��� �Լ� ����
			return;
	}
	isDuplicate = 1;

	// ���������� �̵�

	if (width + *current >= WIDTH) { // 5x5 ���带 �Ѿ�� ���
		overBoard = 1;
		isDuplicate = 0;
	}

	for (int i = 1; i <= *current; i++) { // ���� ����� ������ ���̿� -1 �� �ִ� ���
		if (*(current + i * DEPTH) == -1) {
			isDuplicate = 0;
		}
	}

	if (movement_count >= MAX_MOVEMENT) { // ���ѷ����� ���
		max_movement_count = -1;
		return;
	}

	if (isDuplicate) { // ���������� �������� ������ ���
		playBoard(square_board, width + *current, depth, movement_count+1);
		if (movement_count == -1) // ���ѷ����� ��� �Լ� ����
			return;
	}
	isDuplicate = 1;

	// �������� �̵�

	if (width - *current < 0) { // 5x5 ���带 �Ѿ�� ���
		overBoard = 1;
		isDuplicate = 0;
	}

	for (int i = 1; i <= *current; i++) { // ���� ����� ������ ���̿� -1 �� �ִ� ���
		if (*(current - i * DEPTH) == -1) {
			isDuplicate = 0;
		}
	}

	if (movement_count >= MAX_MOVEMENT) { // ���ѷ����� ���
		max_movement_count = -1;
		return;
	}

	if (isDuplicate) { // ���������� �������� ������ ���
		playBoard(square_board, width - *current, depth, movement_count+1);
		if (max_movement_count == -1) // ���ѷ����� ��� �Լ� ����
			return;
	}

	if (overBoard == 1)
		movement_count++;

	if (max_movement_count < movement_count && max_movement_count != -1) 
		max_movement_count = movement_count;
}
