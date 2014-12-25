#pragma once

#include "GameWindow.h"

// periodic timer initializing constant (1mS = 10000)
#define TIMER_PERIOD        100000
#define GAME_REFRESH_PERIOD 10
#define MAX_LEVEL 5

typedef enum {
	GAME_STATE_MENU,
	GAME_STATE_RUNNING,
	GAME_STATE_EXIT
} GAME_STATE;

typedef struct _Core Core;

typedef void( *HANDLE_INPUT )( IN Core* this );
typedef void( *DRAW_WINDOW )( IN Core* this );

struct _Core {
	GAME_STATE gameState;
	GameWindow* gameWindow;
	EFI_EVENT timerEvent;

	HANDLE_INPUT handleInput;
	DRAW_WINDOW drawWindow;
};

void ConstructCore( OUT Core** this );
void DestructCore( IN Core* this );