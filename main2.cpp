#include "BlackBox.hpp"
/* 클래스 분리
* 1. 번호판 추출을 위한 영상처리, 컨투어를 찾아주는 클래스
* 2. 영상처리된 영상을 기반으로 영상 속 번호판을 찾는 알고리즘 클래스
*    => 여기서 동영상을 플레이 한다.
* 1번 클래스는 2번 클래스에서 영상의 정보를 받아와야함

*/

int main(int argc, char *argv[])
{

	BlackBox *blackbox = new BlackBox(argv[1], VIDEO); // 비디오 불러온다.


	blackbox->playVideo(GRAY);
	//blackbox->processedImg();

	return 0;
}