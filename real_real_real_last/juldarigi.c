#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void jul_init(void);
void jul_manual(key_t key);
void jul_tail(int i, int nx, int ny);
void key_dialog(char []);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기

int mid, mid2;
int end = 0, d_sec = 2;
bool x = false, x2 = false;
int leftstr = 0, rightstr = 0, str = 0;

void jul_init(void) {
	map_init(3, n_player * 5);
	int x = 1, y;
	int n1 = 1;
	mid = (n_player * 5) / 2;
	mid2 = (n_player * 5) / 2;
	back_buf[0][mid] = ' ';
	back_buf[2][mid] = ' ';
	back_buf[1][mid - 1] = '-';
	back_buf[1][mid] = '-';
	back_buf[1][mid + 1] = '-';
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player2[i];
		p->is_alive = true;
		if (i % 2 == 0) {
			leftstr += p->str;
			n1++;
			y = mid - n1;
		}
		else {
			rightstr += p->str;
			y = mid + n1;
		}
		px[i] = x;
		py[i] = y;
		period[i] = randint(100, 500);

		back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
	}
	if (leftstr > 0) {
		leftstr *= -1;
	}
	str = leftstr + rightstr;

	tick = 0;
}

// back_buf[][]에 기록
void jul_tail(int player, int nx, int ny) {
	int p = player;  // 이름이 길어서...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void jul_manual(key_t key) {
	switch (key) {
	case K_Z: str -= 1; break;
	case K_SLASH: str += 1; break;
	case K_X: x = true, str = -1, key_dialog("left"); break;
	case K_DOT: x2 = true, str = 1, key_dialog("right"); break;
	default: return;
	}
}

void jul_move() {
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 움직일 방향(0~3)

	if (str < 0) {
		dir = dy[2];
	}
	else {
		dir = dy[3];
	}

	int x = 1, y;
	int n1 = 1;
	mid += dir;

	back_buf[1][mid - 1] = '-';
	back_buf[1][mid] = '-';
	back_buf[1][mid + 1] = '-';
	if (dir < 0) {
		jul_tail(n_player - 1, x, py[n_player - 1]);
	}
	else {
		jul_tail(n_player - 2, x, py[n_player - 2]);
	}
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player2[i];
		if (i % 2 == 0) {
			n1++;
			y = mid - n1;
		}
		else {
			y = mid + n1;
		}
		px[i] = x;
		py[i] = y;
		period[i] = randint(100, 500);

		if (p->is_alive == true) {
			back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
		}
		else {
			back_buf[px[i]][py[i]] = '-';
		}
	}
	goto_cell(6, 0);
	printf("str: %2d", str);
}

void print_status2(void) {
	printf("no. of players left: %d\n", n_alive);
	printf("			intel	str	stm\n");
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player2[i];
		printf("player %2d: %5s", i, p->is_alive ? "alive" : "DEAD");

		printf("	%2d(%s%d)	%2d(%s%d)	%3d%%\n", p->intel,
			p->item.intel_buf >= 0 ? ("+") : (""), p->item.intel_buf,
			p->str, p->item.str_buf >= 0 ? ("+") : (""), p->item.str_buf, p->stamina);
	}
}

void display2(void) {
	draw();
	goto_cell(N_ROW + 4, 0);  // 추가로 표시할 정보가 있으면 맵과 상태창 사이의 빈 공간에 출력
	print_status2();
}

void jul_dialog(int p, char message[]) {
	int messageLength = strlen(message);

	for (int i = d_sec; i >= 0; i--) {
		goto_cell(3, 1);

		for (int j = 0; j < 16; j++) {
			printf("*");
		}

		goto_cell(4, 0);
		printf(" *%d player %s* ", p, message);

		goto_cell(5, 1);
		for (int j = 0; j < 16; j++) {
			printf("*");
		}

		Sleep(500);
	}
}

void key_dialog(char message[]) {
	int messageLength = strlen(message);

	goto_cell(6, 8);
	printf("%s team str x2 !!!!!", message);

	if (tick % 500 == 0) {

		printf("                              ");
	}
}

void juldarigi(void) {
	int n = 0, n2 = 0;
	int count[10] = { 0 };
	jul_init();

	system("cls");
	display2();
	while (1) {
		
		// player 0만 손으로 움직임(4방향)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			jul_manual(key);
		}
		
		for (int i = 0; i < n_player; i++) {
			PLAYER* p = &player2[i];
			if (i % 2 == 0 && py[i] >= mid2) {
				p->is_alive = false;
				back_buf[1][py[i]] = ' ';
			}
			else if (i % 2 == 1 && py[i] <= mid2) {
				p->is_alive = false;
				back_buf[1][py[i]] = ' ';
			}
			if (i % 2 == 0 && p->is_alive == false) {
				n++;
			}
			if (i % 2 == 1 && p->is_alive == false) {
				n2++;
			}
		}
		for (int i = 0; i < n_player; i++) {
			PLAYER* p = &player2[i];
			if (p->is_alive == false && count[i] == 0) {
				jul_dialog(i, "탈락!");
				count[i] = 1;
				n_alive--;
			}
		}
		if (x && str < 0) {
			jul_move();
			jul_move();
		}
		if (x2 && str > 0) {
			jul_move();
			jul_move();
		}
		if (tick % 1000 == 0 && !x && !x2) {
			jul_move();
		}
		x = false, x2 = false;
		if (n >= (n_player / 2)) break;
		if (n2 >= (n_player / 2)) break;
		n = 0;
		n2 = 0;
		

		display2();
		Sleep(10);
		tick += 10;
	}
}
