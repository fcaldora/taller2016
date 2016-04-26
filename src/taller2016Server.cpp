
#include "GameManager.h"

using namespace std;

int main(int argc, char* argv[]) {
	GameManager *gameManager = new GameManager();

	int returnSuccess = gameManager->initGameWithArguments(argc, argv);

	delete gameManager;

	return returnSuccess;
}
