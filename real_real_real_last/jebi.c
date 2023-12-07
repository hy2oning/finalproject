#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define DIR_UP     0
#define DIR_DOWN   1
#define DIR_LEFT   2
#define DIR_RIGHT  3

void jebi_init();
void jebi_manual(key_t key);
void jebi_tail(int i, int nx, int ny);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기
int dailog_sec = 1; // dailog 초 (1 = 0.5초)
int control = 0;
int round;
int round_cnt = 1;



void init(void) {
    for (int i = 0; i < n_player; i++) {
        if (player2[i].is_alive) {
            round = i;
            break;
        }
    }
}


void jebi_init(void) {
    map_init(5, (n_alive * 3) + 2);
    int alive_count = 0;
    int place = 0;

    for (int i = 0; i < n_player; i++) {
        place = 2 + (alive_count * 3);
        if (player2[i].is_alive) {
            px[i] = 2; 
            py[i] = place; 
            back_buf[px[i]][py[i]] = '?';
            alive_count++;

            if (i == round && player2[i].is_alive) {
                back_buf[px[round]][py[round]] = '@';
            }
        }
    }
    tick = 0;
}

bool jebi_placable(int row, int col) {
    if (row < 0 || row >= N_ROW ||
        col < 0 || col >= N_COL ||
        back_buf[row][col] == ' '){
        return false;
    }
    return true;
}


void jebi_tail(int player1, int nx, int ny) {
    int p = player1;  
    back_buf[nx][ny] = back_buf[px[p]][py[p]];
    back_buf[px[p]][py[p]] = '?';
    px[p] = nx;
    py[p] = ny;
}

void jebi_manual(key_t key) {
    static int dx[4] = { -1, 1, 0, 0 };
    static int dy[4] = { 0, 0, -1, 1 };
    int dir;
    switch (key) {
    case K_LEFT:
        dir = DIR_LEFT;
        break;
    case K_RIGHT:
        dir = DIR_RIGHT;
        break;
    default:
        return;
    }

    int nx, ny;
    nx = px[round];
    ny = py[round] + (dy[dir] * 3);

    if (!jebi_placable(nx, ny)) {
        return;
    }

    for (int i = 0; i < n_player; i++) {
        if (player2[i].is_alive && ny == py[i]) {
            py[i] = py[round];
        }
    }

    jebi_tail(round, nx, ny);
}

void jebi_dialog(int p, char message[]) {
    for (int i = dailog_sec; i >= 0; i--) {
        goto_cell(5, 1);

        for (int j = 0; j < 16; j++) {
            printf("*");
        }

        goto_cell(6, 0);
        printf(" *%d player %s* ", p, message);

        goto_cell(7, 1);
        for (int j = 0; j < 16; j++) {
            printf("*");
        }

        Sleep(250);
    }
}

void jebi(void) {
    int jebi[PLAYER_MAX] = { 0 };
    int y, s = 0, n = 0;
    int fail = 0;

    init();
    jebi_init();
    system("cls");
    display();

    while (1) {
        display();
        goto_cell(8, 0);
        printf("round: %d", round_cnt);
        if (round > n_player) {
            for (int i = 0; i < n_player; i++) {
                if (player2[i].is_alive) {
                    round = i;
                    s = 0;
                    break;
                }
            }
        }
        if (s == 0) {
            fail = randint(0, n_alive - 1);
            for (int i = 0; i < n_alive; i++) {
                y = 2 + (i * 3);
                jebi[i] = y;
                s = 1;
            }
            jebi_init();
        }
        key_t key = get_key();
        if (key == K_QUIT) {
            break;
        }
        else if (key != K_UNDEFINED) {
            jebi_manual(key);
        }
        if (key == K_SPACE) {
            if (player2[round].is_alive && py[round] == jebi[fail]) {
                player2[round].is_alive = false;
                jebi_dialog(round, "탈락!");
                n_alive--;
                round_cnt++;
                s = 0;
                for (int i = 0; i < n_player; i++) {
                    if (player2[i].is_alive) {
                        round = i;
                        control == 1;
                        break;
                    }
                }
            }
            else if (player2[round].is_alive) {
                jebi_dialog(round, "통과!");
            }

            round++;
            if (player2[round].is_alive == false) {
                while (player2[round].is_alive == false) {
                    round++;
                }
            }
            if (control) {
                for (int i = 0; i < n_player; i++) {
                    if (player2[i].is_alive) {
                        round = i;
                        break;
                    }
                }
            }
            jebi_init();
            system("cls");
            control == 0;
        }

        display();
        Sleep(100);
        tick += 10;

        if (n_alive <= 1) {
            break;
        }
    }
}

