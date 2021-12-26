#include "core/included.h"

int main()
{
	Game game;
	if (game.Construct(1024, 512, 1, 1))
		game.Start();

	return 0;
}