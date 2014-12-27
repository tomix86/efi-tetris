#pragma once

#include "Board.h"

// periodic timer initializing constant (1mS = 10000)
#define TIMER_PERIOD        100000 // sprawdzic dokladnie te czasy
#define GAME_REFRESH_PERIOD 35
#define MAX_LEVEL 10

#if MAX_LEVEL < 1 || MAX_LEVEL > 10 
#error Max level can't be lower than 1 or higher than 10!
#endif // max level constraint

typedef enum {
	GAME_STATE_WAITING_FOR_START,
	GAME_STATE_RUNNING,
	GAME_STATE_PAUSED,
	GAME_STATE_OVER,
	GAME_STATE_EXIT
} GAME_STATE;

typedef struct _Core Core;

typedef void( *HANDLE_INPUT )( IN Core* this );
typedef void( *DRAW_WINDOW )( IN Core* this );

struct _Core {
	GAME_STATE gameState;
	Board* board;
	EFI_EVENT timerEvent;
	int tickCounter;

	HANDLE_INPUT handleInput;
	DRAW_WINDOW drawWindow;
};

void ConstructCore( OUT Core** this );
void DestructCore( IN Core* this );