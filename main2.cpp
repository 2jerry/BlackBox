#include "BlackBox.hpp"


int main(int argc, char *argv[])
{

	BlackBox *blackbox = new BlackBox(argv[1]);
	blackbox->playVideo(GRAY);

	return 0;


}