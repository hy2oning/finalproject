// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));
	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1; // -1 리턴하면 메인함수에서 처리하고 종료
	}
	// 플레이어 데이터 load
	fscanf_s(fp, "%d", &n_player);
	for (int i = 0; i < n_player; i++) {
		// 아직 안 배운 문법(구조체 포인터, 간접참조연산자)
		PLAYER* p = &player[i];

		p->id = i;
		// 파일에서 각 스탯 최댓값 읽기
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name), &(p->intel), &(p->str));
		p->stamina = 100; // 100%
		// 현재 상태
		p->is_alive = true;
		p->hasitem = false;
	}
	// 아이템 데이터 load
	fscanf_s(fp, "%d", &n_item);
	for (int i = 0; i < n_item; i++) {
		fscanf_s(fp, "%s%d%d%d",
			item[i].name, (unsigned int)sizeof(item[i].name),
			&(item[i].intel_buf),
			&(item[i].str_buf),
			&(item[i].stamina_buf));
	}
	fclose(fp);


	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player2[i];
		p->is_alive = true;
		player[i] = true;
	}
	return 0;
}

void printLine(const char* line) {
	printf("%s\n", line);
}

void intro() {
	const char* art[] = {
		"=============================================================================",
		"=============================================================================",
		"==   ####                                                                  ==",
		"==     ##      ####                                                        ==",
		"==     ##        ##   ##   ##                                              ==",
		"==#   ###        ##   ##   ##   ######                                     ==",
		"== #####   #    ###   ##   ##  ##        ######                            ==",
		"==           #####    ##   ##  ##  ###  ##       ##   ##                   ==",
		"==                     #####   ##   ##  ##  ###  ##   ##   ##   ##         ==",
		"==                              #####   ##   ##  ##   ##   ### ###   ######==",
		"==                                       #####   ##   ##   ## # ##     ##  ==",
		"==                                                #####    ##   ##     ##  ==",
		"==                                                         ##   ##     ##  ==",
		"==                   ######                                          ######==",
		"==                 ##           #####                                      ==",
		"==                 ##    ###  ##     ##  ##   ##                           ==",
		"==                 ##     ##  ## ### ##  ### ###  #######                  ==",
		"==                   #####    ##     ##  ## # ##  ##                       ==",
		"==                            ##     ##  ##   ##  #######                  ==",
		"==                                       ##   ##  ##                       ==",
		"==                                                #######                  ==",
		"=============================================================================",
		"============================================================================="
	};

	for (int i = 0; i < sizeof(art) / sizeof(art[0]); i++) {
		printLine(art[i]);
#ifdef _WIN32
		Sleep(100);  
#else
		usleep(100000);  
#endif
	}
	system("cls");

}


void ending() {
	system("cls");
	printf("===============================================\n");
	if (n_alive == 1) {
		int winner = -1;
		for (int i = 0; i < n_player; i++) {
			if (player[i]) {
				winner = i;
				break;
			}
		}
		printf("Player %d won!\n", winner);
	}
	else {
		printf("우승자를 가리지 못했습니다.\n");
		printf("살아남은 플레이어: ");
		if (n_alive == 0) {
			printf("없음");
		}
		else {
			bool is_first = true;
			for (int i = 0; i < n_player; i++) {
				if (player[i]) {
					if (!is_first) {
						printf(", ");
					}
					printf("%d", i);
					is_first = false;
				}
			}
		}
		printf("\n");
	}
	printf("===============================================\n");
}

void ending2() {
	system("cls");
	printf("===============================================\n");
	if (n_alive == 1) {
		int winner = -1;
		for (int i = 0; i < n_player; i++) {
			PLAYER* p = &player2[i];
			if (p->is_alive == true) {
				winner = i;
				break;
			}
		}
		printf("Player %d won!\n", winner);
	}
}

int main(void) {
	intro();
	jjuggumi_init();
	//sample();
	mugunghwa();
	nightgame();
	juldarigi();
	system("cls");
	jebi();
	ending();
	ending2();
	return 0;
}
