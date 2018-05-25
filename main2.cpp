#include "BlackBox.hpp"


int main(int argc, char *argv[])
{

	BlackBox *blackbox = new BlackBox(argv[1],VIDEO);
	blackbox->processedVideo(GRAY);
	//blackbox->processedImg();

	return 0;
}