#pragma once
#include "Common.h"

class Game {
public:
	Game();
	virtual ~Game();

	virtual bool Init();
	void Run();
};