#ifndef _JJUGGUMI_H_
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define PLAYER_MAX		10
#define ITEM_MAX		10

bool player[PLAYER_MAX];  // 기본값 true, 탈락하면 false
int n_player, n_alive, n_item;
int tick;  // 시계

// 아이템을 장착하면 스탯 증가
typedef struct {
	char name[100];
	int intel_buf, 
		str_buf, 
		stamina_buf;
} ITEM;
ITEM item[ITEM_MAX];

typedef struct {
	int id;
	char name[100];
	// 능력치: 지능, 힘, 스태미나
	int intel, str, stamina;
	// 현재 상태
	bool is_alive; // 탈락했으면 false
	bool hasitem; // 아이템이 있으면 true
	ITEM item; // 아이템 1개 장착 가능
} PLAYER;
PLAYER player2[PLAYER_MAX];

// 미니게임
void sample(void);
void mugunghwa(void);
void nightgame(void);
void juldarigi(void);
void jebi(void);

int randint(int low, int high);

#endif

