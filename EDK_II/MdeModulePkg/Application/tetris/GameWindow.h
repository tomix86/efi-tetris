#pragma once

#include "Board.h"

typedef struct _GameWindow GameWindow;

typedef void( *DRAW_GAME_WINDOW )(IN GameWindow* this);

struct _GameWindow {
	Board* board;
	int level;
	int lines;
	int score;
	BOOLEAN paused;

	DRAW_GAME_WINDOW drawGameWindow;
};

void ConstructGameWindow( OUT GameWindow** this );
void DestructGameWindow( IN GameWindow* this );