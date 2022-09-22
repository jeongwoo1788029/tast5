#include <windows.h>//커서
#include <iostream>//입출력
#include <cstdlib>//랜덤
#include <conio.h>//키 입력
using namespace std;

#define left 75
#define right 77
#define down 80
#define up 72
#define space 32

//블럭의 좌표 구조체(시작 지점으로 초기화)
struct block {
	int x = 5;
	int y = 2;
};

//블럭 모음집
block block_shape[19][4] = {/*ㅁ*/
						{{0,-1},{0,0},{-1,-1},{-1,0}},

						/*ㅡ,ㅣ*/
						{{-1,0},{0,0},{1,0},{2,0}}, {{0,-1}, {0,0},{0,1},{0,2}},
						/*S*/
						{{0,1},{0,0},{1,0},{1,-1}}, {{-1,-1},{0,-1},{0,0},{1,0}},
						/*Z*/
						{{-1,-1},{-1,0},{0,0},{0,1}}, {{-1,0},{0,0},{0,-1},{1,-1}},

						/*반ㄱㄴ*/
						{{0,-1},{0,0},{0,1},{1,1}}, {{0,0},{0,-1},{1,-1},{2,-1}}, {{-1,-1},{0,1},{0,0},{0,-1}}, {{-1,0},{0,0},{1,0},{1,-1}},
						/*ㄱㄴ*/
						{{-1,1},{0,-1},{0,0},{0,1}}, {{-1,-1},{-1,0},{0,0},{1,0}}, {{0,-1},{0,0},{0,1},{1,-1}}, {{0,0},{0,-1},{-1,-1},{-2,-1}},
						/*ㅓ*/
						{{0,-1},{0,0},{0,1},{1,0}},{{-1,0},{0,0},{0,1},{1,0}}, {{-1,0},{0,0},{0,1},{0,-1}}, {{-1,0},{0,-1},{0,0},{1,0}}
};

//커서 위치
void gotoXY(int x, int y) {
	COORD Pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//커서 제거
void Cursor(bool flag)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = flag;//true : 깜빡임, false : 안 깜빡임
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//데이터 백업 10 * 20
int backup_Map[12][22] = { 0 };


/* 1. 테두리 생성 */
void Create_Map() {
	for (int x = 1; x < 11; x++) {//가로 : 맨 위,맨 밑 8개(사이)  
		gotoXY(x * 2 + 2, 0);
		cout << "▼";
		backup_Map[x][0] = 1;

		gotoXY(x * 2 + 2, 21);
		cout << "▲";
		backup_Map[x][21] = 1;
	}
	for (int y = 0; y < 22; y++) {//세로 : 22개 양쪽 
		gotoXY(2, y);
		cout << "●";
		backup_Map[0][y] = 1;

		gotoXY(24, y);
		cout << "●";
		backup_Map[11][y] = 1;
	}
}

/* 2. 블럭 생성, 이동, 체인지 가능 판별 */
bool Check_Block(int block_select, block blockxy) {

	block temp;

	for (int i = 0; i < 4; i++) {
		temp.x = block_shape[block_select][i].x + blockxy.x;
		temp.y = block_shape[block_select][i].y + blockxy.y;
		if (temp.y > 0) {
			if (backup_Map[temp.x][temp.y] == 1) {
				return false;
			}
		}
	}
	return true;
}

/* 3. 블럭 칸 채우기 */
void fill_Block(int block_select, block blockxy) {

	block temp;

	for (int i = 0; i < 4; i++) {
		temp.x = block_shape[block_select][i].x + blockxy.x;
		temp.y = block_shape[block_select][i].y + blockxy.y;
		if (temp.y >= 1) {
			backup_Map[temp.x][temp.y] = 2;
		}
	}
}

/* 4. 블럭 생성 가능 여부 판단 후 생성, 게임오버 처리 */
void create_Block(int block_select, block blockxy) {

	bool check = Check_Block(block_select, blockxy);//2번

	if (check == false) {//게임 오버
		system("cls");
		cout << "\n\n    Game Over!!    \n\n\n\n";
		exit(0);
	}
	else {
		fill_Block(block_select, blockxy);//3번
	}
}

/* 5. 블럭 칸 비우기(블럭 이동에 이용) */
void Empty_Block(int block_select, block blockxy) {

	block temp;

	for (int i = 0; i < 4; i++) {
		temp.x = block_shape[block_select][i].x + blockxy.x;
		temp.y = block_shape[block_select][i].y + blockxy.y;
		backup_Map[temp.x][temp.y] = 0;
	}

}

/* 6. 줄 제거 */
void Clear_BlockLine() {

	int check;

	for (int i = 20; i > 0; i--) {
		for (int j = 1; j < 11; j++) {
			check = backup_Map[j][i];
			if (check == 0) {
				break;
			}
		}

		if (check == 1) {//한 줄이 꽉 찼다면!!
			for (int n = 1; n < 11; n++) {
				for (int m = i; m > 1; m--) {
					backup_Map[n][m] = backup_Map[n][m - 1];//윗 줄이랑 체인지
				}
			}
			i++;//여러줄이 한번에 채워졌을 때 검사 및 처리
		}
	}
}

/* 7. 블럭 멈추고 백업 데이터에 대입 */
void Stop_Block(int block_select, block blockxy) {

	block temp;

	for (int i = 0; i < 4; i++) {
		temp.x = block_shape[block_select][i].x + blockxy.x;
		temp.y = block_shape[block_select][i].y + blockxy.y;
		if (backup_Map[temp.x][temp.y] == 1) {//멈추고 맨 윗 줄에 다았을 때
			exit(0);
		}
		backup_Map[temp.x][temp.y] = 1;
	}

	Clear_BlockLine();//6번 줄 제거
}

/* 8. 맵 렌더링 */
void Redering_Map() {
	for (int i = 20; i > 0; i--) {
		for (int j = 1; j < 11; j++) {
			if (backup_Map[j][i] == 1) {//멈췄을 때 블럭
				gotoXY(j * 2 + 2, i);
				cout << "■";
			}
			else if (backup_Map[j][i] == 2) {//움직이는 블럭
				gotoXY(j * 2 + 2, i);
				cout << "■";
			}
			else {
				gotoXY(j * 2 + 2, i);
				cout << "  ";
			}
		}
	}
}

/* 9. 블럭 움직임 */
void Move_Left(int block_select, block blockxy) {

	blockxy.x++;
	Empty_Block(block_select, blockxy);
	blockxy.x--;
	fill_Block(block_select, blockxy);

}
void Move_Right(int block_select, block blockxy) {

	blockxy.x--;
	Empty_Block(block_select, blockxy);
	blockxy.x++;
	fill_Block(block_select, blockxy);

}
void Move_Down(int block_select, block blockxy) {

	blockxy.y--;
	Empty_Block(block_select, blockxy);
	blockxy.y++;
	fill_Block(block_select, blockxy);

}

/* 10. 블럭 회전 */
void Rotate_Block(int* block_select, block blockxy) {

	bool check_rotate = false;//변형 가능 여부

	//2번 변하는 블럭일 때
	if (*block_select >= 1 && *block_select <= 6) {
		if (*block_select % 2 == 0) {
			*block_select -= 1;
			check_rotate = Check_Block(*block_select, blockxy);//2번
			if (check_rotate == 0) {
				*block_select += 1;
			}
			else {
				*block_select += 1;
				Empty_Block(*block_select, blockxy);//5번
				*block_select -= 1;
				fill_Block(*block_select, blockxy);//3번
			}
		}
		else {
			*block_select += 1;
			check_rotate = Check_Block(*block_select, blockxy);//2번
			if (check_rotate == 0) {
				*block_select -= 1;
			}
			else {
				*block_select -= 1;
				Empty_Block(*block_select, blockxy);//5번
				*block_select += 1;
				fill_Block(*block_select, blockxy);//3번
			}
		}
	}
	//4번 변하는 블럭일 때
	else if (*block_select >= 7 && *block_select <= 18) {
		if ((*block_select - 7) % 4 == 3) {//배열 끝에 있는 블럭에서 처음 블럭으로 순환 
			*block_select -= 3;
			check_rotate = Check_Block(*block_select, blockxy);//2번
			if (check_rotate == 0) {
				*block_select += 3;
			}
			else {
				*block_select += 3;
				Empty_Block(*block_select, blockxy);//5번
				*block_select -= 3;
				fill_Block(*block_select, blockxy);//3번
			}
		}
		else {
			*block_select += 1;
			check_rotate = Check_Block(*block_select, blockxy);//2번
			if (check_rotate == 0) {
				*block_select -= 1;
			}
			else {
				*block_select -= 1;
				Empty_Block(*block_select, blockxy);//5번
				*block_select += 1;
				fill_Block(*block_select, blockxy);//3번
			}
		}
	}
}

/* 11. 랜덤 값 설정 함수 */
int Random_value() {

	srand((static_cast<unsigned int>(time(NULL))));
	int value = rand() % 19;//0부터 18까지

	return value;
}

/* main * main * main * main * main * main * main * main * main */
int main() {
	Cursor(false);//커서 off

	block blockxy;//블럭 좌표 저장
	int block_select = 0;//선택된 블럭
	int input = 0;//키 입력
	bool check = 0;//이동 가능 여부
	int level = 0;//난이도 설정

	int* _block_select = &block_select;//block_select의 주소값을 갖는 포인터 변수(블럭 회전에 이용)

	//난이도 입력
	cout << "난이도를 입력해 주세요 1 ~ 2 >> ";
	cin >> level;
	if (level >= 2)
		level = 2;

	//콘솔 창 비우기
	system("cls");

	//1번
	Create_Map();


	block_select = Random_value();//11번
	create_Block(block_select, blockxy);//4번

	unsigned __int64 Timer = GetTickCount64();//시스템이 시작한 시점부터 이 함수를 호출한 시점까지 흘러간 시간을 1000분의 1초 단위의 시간을 알려주는 함수
	while (true) {

		/*Input - Input - Input - Input - Input - Input */
		if (_kbhit())
			input = _getch();
		else
			input = 0;


		/* Update - Update - Update - Update - Update */
		if (GetTickCount64() >= Timer + 1000 / level) //속도 지정 1000 <- 입력받기
		{
			Empty_Block(block_select, blockxy);//5번
			blockxy.y++;
			check = Check_Block(block_select, blockxy);//2번

			if (check == false) {
				blockxy.y--;
				Stop_Block(block_select, blockxy);//7번

				//커서를 생성 좌표로 이동
				blockxy.x = 5;
				blockxy.y = 2;
				//블럭 랜덤으로 생성
				block_select = Random_value();//11번
				create_Block(block_select, blockxy);//4번
			}
			else {
				Move_Down(block_select, blockxy);//9번
			}
			Timer = GetTickCount64();
		}

		//입력받은 키 Update
		switch (input) {
			//왼쪽 키
		case left:
			blockxy.x--;
			if (Check_Block(block_select, blockxy) == false)//2번
				blockxy.x++;
			else
				Move_Left(block_select, blockxy);//9번
			break;

			//오른쪽 키
		case right:
			blockxy.x++;
			if (Check_Block(block_select, blockxy) == false)//2번
				blockxy.x--;
			else
				Move_Right(block_select, blockxy);//9번
			break;

			//아래쪽 키
		case down:
			blockxy.y++;
			if (Check_Block(block_select, blockxy) == false)//2번
				blockxy.y--;
			else
				Move_Down(block_select, blockxy);//9번
			break;

			//위쪽 키 (도형 변형)
		case up:
			Rotate_Block(_block_select, blockxy);//10번
			break;

			//스페이스바 (하드 드롭)
		case space:
			while (true) {
				blockxy.y++;
				check = Check_Block(block_select, blockxy);//2번
				if (check == 0) {
					blockxy.y--;
					break;
				}
				else {
					Move_Down(block_select, blockxy);//9번
				}
			}
			break;
		}

		/*rendering - rendering - rendering - rendering - rendering*/
		Redering_Map();//8번
	}
}
