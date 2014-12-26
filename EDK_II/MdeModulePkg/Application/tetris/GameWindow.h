#pragma once

#include "Board.h"

#define STATIC_TEXT L"\
     CONTROLS\r\n\
      P - Pause game\r\n\
     <- - Move left\r\n\
     -> - Move right\r\n\
   Down - Move down\r\n\
     Up - Rotate\r\n\
  Space - Drop\r\n\
  Pg Up - level up\r\n\
Pg Down - level down\r\n\
    ESC - Exit game\r\n"

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