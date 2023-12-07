#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

#define MAP_SIZE_HEIGHT 30
#define MAP_SIZE_WIDTH 15 

bool iplacable(int row, int col);
void night_init(void);
void night_manual(key_t key);
void night_random(int i, int dir);
void night_tail(int i, int nx, int ny);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기
int ix[ITEM_MAX], iy[ITEM_MAX];
bool ignoreitem[PLAYER_MAX][ITEM_MAX] = { 0 };

bool iplacable(int row, int col) {
	if (row < 0 + 1 || row >= N_ROW - 1 ||
		col < 0 + 1 || col >= N_COL - 1) {
		return false;
	}
	return true;
}

bool iplacable2(int row, int col) {
	for (int i = 1; i < n_player; i++) {
		if (back_buf[px[i]][py[i]] == back_buf[row][col]) {
			return false;
		}
	}
	return true;
}

void night_init(void) {
	map_init(MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT);
	srand((unsigned int)time(NULL));
	// 플레이어
	for (int i = 0; i < n_player; i++) {
		int x, y;
		PLAYER* p = &player2[i];

		if (p->is_alive == true) {
			do {
				x = rand() % (MAP_SIZE_HEIGHT - 2) + 1;
				y = rand() % (MAP_SIZE_WIDTH - 2) + 1;
			} while (back_buf[x][y] != ' ' || !placable(x, y)); // placable 함수가 정의되어 있다고 가정합니다.

			back_buf[x][y] = '0' + i; // 플레이어 번호 출력 (0~3//

			px[i] = x;
			py[i] = y;
			period[i] = rand() % 401 + 100; // 100에서 500 사이의 난수 생성
			p->hasitem = false;
		}
	}
	//아이템

	for (int i = 0; i < n_item; ++i) {
		int x, y;
		do {
			x = rand() % (MAP_SIZE_HEIGHT - 2) + 1;
			y = rand() % (MAP_SIZE_WIDTH - 2) + 1;
		} while (!placable(x, y));
		back_buf[x][y] = 'I'; // 아이템을 나타내는 문//
		ix[i] = x;
		iy[i] = y;
	}
}

// back_buf[][]에 기록
void night_tail(int player, int nx, int ny) {
	int p = player;  // 이름이 길어서...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void night_manual(key_t key) {
	char change;
	int try;
	// 각 방향으로 움직일 때 x, y값 delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 움직일 방향(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// 움직여서 놓일 자리
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];

	if (!iplacable2(nx, ny)) {
		for (int i = 1; i < n_player; i++) {
			if (back_buf[nx][ny] == back_buf[px[i]][py[i]]) {
				goto_cell(2, MAP_SIZE_HEIGHT + 2);
				printf("1) 강탈 시도");
				goto_cell(4, MAP_SIZE_HEIGHT + 2);
				printf("2) 회유 시도");
				goto_cell(6, MAP_SIZE_HEIGHT + 2);
				printf("3) 무시");
				goto_cell(7, MAP_SIZE_HEIGHT + 2);
				printf("---->");
				scanf_s("%d", &try);
				goto_cell(2, MAP_SIZE_HEIGHT + 2);
				printf("                   ");
				goto_cell(4, MAP_SIZE_HEIGHT + 2);
				printf("                   ");
				goto_cell(6, MAP_SIZE_HEIGHT + 2);
				printf("                   ");
				goto_cell(7, MAP_SIZE_HEIGHT + 2);
				printf("                   ");
			}
		}
		return;
	}
	if (!iplacable(nx, ny)) {
		return;
	}

	

	for (int i = 0; i < n_item; i++) {
		if (nx == ix[i] && ny == iy[i]) {
			goto_cell(MAP_SIZE_WIDTH + 1, 0);
			printf("아이템을 바꾸시겠습니까? y / n : ");
			scanf_s("%c", &change);
			if (change == 'y') {
				player2[0].intel = 0;
				player2[0].str = 0;
				player2[0].stamina = 0;
				player2[0].item = item[i];
				ix[i] = 0;
				iy[i] = 0;
				player2[0].hasitem = TRUE;
				player2[0].intel += item[i].intel_buf;
				player2[0].str += item[i].str_buf;
				player2[0].stamina += item[i].stamina_buf;
			}
			else {
				return;
			}
		}
		
		goto_cell(MAP_SIZE_WIDTH + 1, 0);
		printf("                                                 ");
	}
	change = ' ';
	night_tail(0, nx, ny);
}

// 0 <= dir < 4가 아니면 랜덤
void night_random(int player, int dir) {
	int p = player;  // 이름이 길어서...
	int nx, ny;  // 움직여서 다음에 놓일 자리

	// 움직일 공간이 없는 경우는 없다고 가정(무한 루프에 빠짐)	

	do {
		nx = px[p] + randint(-1, 1);
		ny = py[p] + randint(-1, 1);
	} while (!iplacable(nx, ny) || !iplacable2(nx, ny));

	for (int i = 0; i < n_item; i++) {
		if (nx == ix[i] && ny == iy[i]) {
			if (!player2[p].hasitem) {
				player2[p].intel = 0;
				player2[p].str = 0;
				player2[p].stamina = 0;
				player2[p].item = item[i];
				ix[i] = 0;
				iy[i] = 0;
				player2[p].hasitem = TRUE;
				player2[p].intel += item[i].intel_buf;
				player2[p].str += item[i].str_buf;
				player2[p].stamina += item[i].stamina_buf;
			}
			else {
				return;
			}
		}
	}

	night_tail(p, nx, ny);
}

void nightgame(void) {
	night_init();

	system("cls");
	display();
	while (1) {
		// player 0만 손으로 움직임(4방향)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			night_manual(key);
		}

		// player 1 부터는 랜덤으로 움직임(8방향)
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				night_random(i, -1);
			}
		}

		display();
		Sleep(10);
		tick += 10;
	}
}
