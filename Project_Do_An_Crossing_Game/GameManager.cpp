#include"GameManager.h"
GameManager::GameManager()
{
	mCurrentState = new PlayState(this);
	mCurrentState->Init();
}

GameManager::~GameManager()
{
	if (mCurrentState != NULL)
	{
		delete mCurrentState;
	}
}
IGameState* GameManager::getState()
{
	return mCurrentState;
}
void GameManager::setState(IGameState* state)
{
	if (mCurrentState != NULL)
	{
		delete mCurrentState;
	}
	mCurrentState = state;
	if (mCurrentState != NULL)
	{
		mCurrentState->Init();
	}
}
void GameManager::Update(float deltime, sf::RenderWindow& window)
{
	if (mCurrentState != NULL) mCurrentState->Update(deltime, window);
}
void GameManager::Render(sf::RenderWindow& window)
{
	if (mCurrentState != NULL) mCurrentState->Render(window);
}