// 2023-2 ������α׷��� ����: �޲ٹ� ����
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low �̻� high ���� ������ �߻���Ű�� �Լ�
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));
	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1; // -1 �����ϸ� �����Լ����� ó���ϰ� ����
	}
	// �÷��̾� ������ load
	fscanf_s(fp, "%d", &n_player);
	for (int i = 0; i < n_player; i++) {
		// ���� �� ��� ����(����ü ������, ��������������)
		PLAYER* p = &player[i];

		p->id = i;
		// ���Ͽ��� �� ���� �ִ� �б�
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name), &(p->intel), &(p->str));
		p->stamina = 100; // 100%
		// ���� ����
		p->is_alive = true;
		p->hasitem = false;
	}
	// ������ ������ load
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
		printf("����ڸ� ������ ���߽��ϴ�.\n");
		printf("��Ƴ��� �÷��̾�: ");
		if (n_alive == 0) {
			printf("����");
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
