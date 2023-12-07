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

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�
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
	// �÷��̾�
	for (int i = 0; i < n_player; i++) {
		int x, y;
		PLAYER* p = &player2[i];

		if (p->is_alive == true) {
			do {
				x = rand() % (MAP_SIZE_HEIGHT - 2) + 1;
				y = rand() % (MAP_SIZE_WIDTH - 2) + 1;
			} while (back_buf[x][y] != ' ' || !placable(x, y)); // placable �Լ��� ���ǵǾ� �ִٰ� �����մϴ�.

			back_buf[x][y] = '0' + i; // �÷��̾� ��ȣ ��� (0~3//

			px[i] = x;
			py[i] = y;
			period[i] = rand() % 401 + 100; // 100���� 500 ������ ���� ����
			p->hasitem = false;
		}
	}
	//������

	for (int i = 0; i < n_item; ++i) {
		int x, y;
		do {
			x = rand() % (MAP_SIZE_HEIGHT - 2) + 1;
			y = rand() % (MAP_SIZE_WIDTH - 2) + 1;
		} while (!placable(x, y));
		back_buf[x][y] = 'I'; // �������� ��Ÿ���� ��//
		ix[i] = x;
		iy[i] = y;
	}
}

// back_buf[][]�� ���
void night_tail(int player, int nx, int ny) {
	int p = player;  // �̸��� ��...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void night_manual(key_t key) {
	char change;
	int try;
	// �� �������� ������ �� x, y�� delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // ������ ����(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// �������� ���� �ڸ�
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];

	if (!iplacable2(nx, ny)) {
		for (int i = 1; i < n_player; i++) {
			if (back_buf[nx][ny] == back_buf[px[i]][py[i]]) {
				goto_cell(2, MAP_SIZE_HEIGHT + 2);
				printf("1) ��Ż �õ�");
				goto_cell(4, MAP_SIZE_HEIGHT + 2);
				printf("2) ȸ�� �õ�");
				goto_cell(6, MAP_SIZE_HEIGHT + 2);
				printf("3) ����");
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
			printf("�������� �ٲٽðڽ��ϱ�? y / n : ");
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

// 0 <= dir < 4�� �ƴϸ� ����
void night_random(int player, int dir) {
	int p = player;  // �̸��� ��...
	int nx, ny;  // �������� ������ ���� �ڸ�

	// ������ ������ ���� ���� ���ٰ� ����(���� ������ ����)	

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
		// player 0�� ������ ������(4����)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			night_manual(key);
		}

		// player 1 ���ʹ� �������� ������(8����)
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
