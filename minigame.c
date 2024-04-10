//	<보드 게임 프로그램>

/*
규칙 1 - 0~RANDOM_NUM 숫자가 적혀있는 5x5 보드에서 진행
규칙 2 - 보드의 가장 왼쪽 위 에서 시작하며 적힌 숫자만큼 위,아래,오른쪽,왼쪽으로 이동
규칙 3 - 동전이 보드 밖으로 나가는 경우 게임 종료
규칙 4 - 동전이 있는 칸의 숫자가 0 일 경우 게임 종료
규칙 5 - 이동 경로에 장애물(-1)이 있는 경우 이동 불가
규칙 3,4 의 경우 이동 후 게임 끝나는 판정 즉, 점수는 늘어남(시작지점의 숫자가 0일 경우 이동이 없었기 때문에 바로 종료)
*/

/* +무한루프 처리
무한루프를 검증하는 방법 -> 왔던좌표로 돌아오기

크기가 5x5인 보드 안에서 가능한 좌표의 인덱스는 x[0~4][0~4] 즉 25개임
따라서 중복되지 않고 새로운 좌표에 이동하는 최대 횟수 역시 25번임 (장애물은 생각하지 않음)
즉, 이동 횟수가 25번을 넘어가면 최소 한번 이상 방문했던 좌표에 돌아온 것이며 이는 무한루프임을 의미함*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 5
#define DEPTH 5
#define MAX_MOVEMENT 25
#define RANDOM_NUMBER 7

// 전역변수 선언
int max_movement_count = 0;

// 함수 원형
void setBoard(int* square_board, int max_width, int max_depth);
void printBoard(int* square_board, int max_width, int max_depth);
void playBoard(int* square_board, int width, int depth, int movement_count);

// 과제1. 구현(4) 보드 게임 실행을 위한 main문 작성

int main(void)
{
	int square_board[WIDTH][DEPTH];

	srand((unsigned)time(NULL));
	setBoard(square_board, WIDTH, DEPTH);
	printBoard(square_board, WIDTH, DEPTH);
	playBoard(square_board, 0, 0, max_movement_count);

	printf("최대 이동 횟수: %d\n\n", max_movement_count);
	return 0;
}

// 과제1. 구현(1) setBoard 함수 구현하기

void setBoard(int* square_board, int max_width, int max_depth)
{
	int obx1, obx2, oby1, oby2 = 0;
	obx1 = rand() % max_width;
	obx2 = rand() % max_width;
	oby1 = rand() % max_depth;
	oby2 = rand() % max_depth;
	while (obx1 == obx2 && oby1 == oby2) // 장애물이 중복된 경우 처리
		oby2 = rand() % max_depth;



	*(square_board + (DEPTH * obx1) + oby1) = -1;
	*(square_board + (DEPTH * obx2) + oby2) = -1;

	for (int i = 0; i < max_depth * max_width; i++) {
		if (*(square_board + i) != -1) {
			*(square_board + i) = rand() % (RANDOM_NUMBER+1); // 0~RANDOM_NUMBER 사이의 난수 
		}
	}
}

// 과제1. 구현(2) printBoard 함수 구현하기

void printBoard(int* square_board, int max_width, int max_depth)
{
	for (int i = 0; i < max_depth; i++) {
		for (int j = 0; j < max_width; j++) {
			printf("%2d ", *(square_board + j * DEPTH + i));
		}
		printf("\n");
	}
}

  // 과제1. 구현(3) playBoard 함수 구현하기
void playBoard(int* square_board, int width, int depth, int movement_count) {
	int *current = (square_board + width * DEPTH + depth); // 함수의 인수로 입력받은 width, 와 depth가 가리키는 data값을 가리키는 포인터를 저장하는 변수
	int isDuplicate = 1; // 조건을 나타내는 변수 -> False 일 경우 성공적인 이동을 하지 못함
	int overBoard = 0; // 조건을 나타내는 변수 -> 동전이 보드 밖으로 나갈경우 True
	
	if (max_movement_count == -1) // 무한루프일 경우 함수 종료
		return;

	// 입력받은 좌표의 data가 0인지 확인 시작지점이 아닐 경우 -1은 나올 수 없음
	if (*current == 0 || *current == -1) {   
		if (max_movement_count < movement_count)
			max_movement_count = movement_count;
		return;
	}

	// 아래쪽으로 이동

	if (depth + *current >= DEPTH) { // 5x5 보드를 넘어가능 경우
		overBoard = 1;
		isDuplicate = 0;
	}

	for (int i = 1; i <= *current; i++) { // 진행 방향과 목적지 사이에 -1 이 있는 경우
		if (*(current + i) == -1) {
			isDuplicate = 0;
		}
	}

	if (movement_count >= MAX_MOVEMENT) { // 무한루프일 경우
		max_movement_count = -1;
		return;
	}

	if (isDuplicate) { // 정상적으로 목적지에 도착한 경우
		playBoard(square_board, width, depth + *current, movement_count+1);
		if (max_movement_count == -1) // 무한루프일 경우 함수 종료
			return;
	}
	isDuplicate = 1;

	// 위쪽으로 이동

	if (depth - *current < 0) { // 5x5 보드를 넘어가능 경우
		overBoard = 1;
		isDuplicate = 0;
	}

	for (int i = 1; i <= *current; i++) { // 진행 방향과 목적지 사이에 -1 이 있는 경우
		if ((*current - i) == -1) {
			isDuplicate = 0;
		}
	}

	if (movement_count >= MAX_MOVEMENT) { // 무한루프일 경우
		max_movement_count = -1;
		return;
	}

	if (isDuplicate) { // 정상적으로 목적지에 도착한 경우
		playBoard(square_board, width, depth - *current, movement_count+1);
		if (max_movement_count == -1) // 무한루프일 경우 함수 종료
			return;
	}
	isDuplicate = 1;

	// 오른쪽으로 이동

	if (width + *current >= WIDTH) { // 5x5 보드를 넘어가능 경우
		overBoard = 1;
		isDuplicate = 0;
	}

	for (int i = 1; i <= *current; i++) { // 진행 방향과 목적지 사이에 -1 이 있는 경우
		if (*(current + i * DEPTH) == -1) {
			isDuplicate = 0;
		}
	}

	if (movement_count >= MAX_MOVEMENT) { // 무한루프일 경우
		max_movement_count = -1;
		return;
	}

	if (isDuplicate) { // 정상적으로 목적지에 도착한 경우
		playBoard(square_board, width + *current, depth, movement_count+1);
		if (movement_count == -1) // 무한루프일 경우 함수 종료
			return;
	}
	isDuplicate = 1;

	// 왼쪽으로 이동

	if (width - *current < 0) { // 5x5 보드를 넘어가능 경우
		overBoard = 1;
		isDuplicate = 0;
	}

	for (int i = 1; i <= *current; i++) { // 진행 방향과 목적지 사이에 -1 이 있는 경우
		if (*(current - i * DEPTH) == -1) {
			isDuplicate = 0;
		}
	}

	if (movement_count >= MAX_MOVEMENT) { // 무한루프일 경우
		max_movement_count = -1;
		return;
	}

	if (isDuplicate) { // 정상적으로 목적지에 도착한 경우
		playBoard(square_board, width - *current, depth, movement_count+1);
		if (max_movement_count == -1) // 무한루프일 경우 함수 종료
			return;
	}

	if (overBoard == 1)
		movement_count++;

	if (max_movement_count < movement_count && max_movement_count != -1) 
		max_movement_count = movement_count;
}
