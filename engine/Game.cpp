#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		return false;
	}

	return true;
}

void Game::Run()
{
}