#include <windows.h>//Ŀ��
#include <iostream>//�����
#include <cstdlib>//����
#include <conio.h>//Ű �Է�
using namespace std;

#define left 75
#define right 77
#define down 80
#define up 72
#define space 32

//���� ��ǥ ����ü(���� �������� �ʱ�ȭ)
struct block {
	int x = 5;
	int y = 2;
};

//�� ������
block block_shape[19][4] = {/*��*/
						{{0,-1},{0,0},{-1,-1},{-1,0}},

						/*��,��*/
						{{-1,0},{0,0},{1,0},{2,0}}, {{0,-1}, {0,0},{0,1},{0,2}},
						/*S*/
						{{0,1},{0,0},{1,0},{1,-1}}, {{-1,-1},{0,-1},{0,0},{1,0}},
						/*Z*/
						{{-1,-1},{-1,0},{0,0},{0,1}}, {{-1,0},{0,0},{0,-1},{1,-1}},

						/*�ݤ���*/
						{{0,-1},{0,0},{0,1},{1,1}}, {{0,0},{0,-1},{1,-1},{2,-1}}, {{-1,-1},{0,1},{0,0},{0,-1}}, {{-1,0},{0,0},{1,0},{1,-1}},
						/*����*/
						{{-1,1},{0,-1},{0,0},{0,1}}, {{-1,-1},{-1,0},{0,0},{1,0}}, {{0,-1},{0,0},{0,1},{1,-1}}, {{0,0},{0,-1},{-1,-1},{-2,-1}},
						/*��*/
						{{0,-1},{0,0},{0,1},{1,0}},{{-1,0},{0,0},{0,1},{1,0}}, {{-1,0},{0,0},{0,1},{0,-1}}, {{-1,0},{0,-1},{0,0},{1,0}}
};

//Ŀ�� ��ġ
void gotoXY(int x, int y) {
	COORD Pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//Ŀ�� ����
void Cursor(bool flag)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = flag;//true : ������, false : �� ������
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//������ ��� 10 * 20
int backup_Map[12][22] = { 0 };


/* 1. �׵θ� ���� */
void Create_Map() {
	for (int x = 1; x < 11; x++) {//���� : �� ��,�� �� 8��(����)  
		gotoXY(x * 2 + 2, 0);
		cout << "��";
		backup_Map[x][0] = 1;

		gotoXY(x * 2 + 2, 21);
		cout << "��";
		backup_Map[x][21] = 1;
	}
	for (int y = 0; y < 22; y++) {//���� : 22�� ���� 
		gotoXY(2, y);
		cout << "��";
		backup_Map[0][y] = 1;

		gotoXY(24, y);
		cout << "��";
		backup_Map[11][y] = 1;
	}
}

/* 2. �� ����, �̵�, ü���� ���� �Ǻ� */
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

/* 3. �� ĭ ä��� */
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

/* 4. �� ���� ���� ���� �Ǵ� �� ����, ���ӿ��� ó�� */
void create_Block(int block_select, block blockxy) {

	bool check = Check_Block(block_select, blockxy);//2��

	if (check == false) {//���� ����
		system("cls");
		cout << "\n\n    Game Over!!    \n\n\n\n";
		exit(0);
	}
	else {
		fill_Block(block_select, blockxy);//3��
	}
}

/* 5. �� ĭ ����(�� �̵��� �̿�) */
void Empty_Block(int block_select, block blockxy) {

	block temp;

	for (int i = 0; i < 4; i++) {
		temp.x = block_shape[block_select][i].x + blockxy.x;
		temp.y = block_shape[block_select][i].y + blockxy.y;
		backup_Map[temp.x][temp.y] = 0;
	}

}

/* 6. �� ���� */
void Clear_BlockLine() {

	int check;

	for (int i = 20; i > 0; i--) {
		for (int j = 1; j < 11; j++) {
			check = backup_Map[j][i];
			if (check == 0) {
				break;
			}
		}

		if (check == 1) {//�� ���� �� á�ٸ�!!
			for (int n = 1; n < 11; n++) {
				for (int m = i; m > 1; m--) {
					backup_Map[n][m] = backup_Map[n][m - 1];//�� ���̶� ü����
				}
			}
			i++;//�������� �ѹ��� ä������ �� �˻� �� ó��
		}
	}
}

/* 7. �� ���߰� ��� �����Ϳ� ���� */
void Stop_Block(int block_select, block blockxy) {

	block temp;

	for (int i = 0; i < 4; i++) {
		temp.x = block_shape[block_select][i].x + blockxy.x;
		temp.y = block_shape[block_select][i].y + blockxy.y;
		if (backup_Map[temp.x][temp.y] == 1) {//���߰� �� �� �ٿ� �پ��� ��
			exit(0);
		}
		backup_Map[temp.x][temp.y] = 1;
	}

	Clear_BlockLine();//6�� �� ����
}

/* 8. �� ������ */
void Redering_Map() {
	for (int i = 20; i > 0; i--) {
		for (int j = 1; j < 11; j++) {
			if (backup_Map[j][i] == 1) {//������ �� ��
				gotoXY(j * 2 + 2, i);
				cout << "��";
			}
			else if (backup_Map[j][i] == 2) {//�����̴� ��
				gotoXY(j * 2 + 2, i);
				cout << "��";
			}
			else {
				gotoXY(j * 2 + 2, i);
				cout << "  ";
			}
		}
	}
}

/* 9. �� ������ */
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

/* 10. �� ȸ�� */
void Rotate_Block(int* block_select, block blockxy) {

	bool check_rotate = false;//���� ���� ����

	//2�� ���ϴ� ���� ��
	if (*block_select >= 1 && *block_select <= 6) {
		if (*block_select % 2 == 0) {
			*block_select -= 1;
			check_rotate = Check_Block(*block_select, blockxy);//2��
			if (check_rotate == 0) {
				*block_select += 1;
			}
			else {
				*block_select += 1;
				Empty_Block(*block_select, blockxy);//5��
				*block_select -= 1;
				fill_Block(*block_select, blockxy);//3��
			}
		}
		else {
			*block_select += 1;
			check_rotate = Check_Block(*block_select, blockxy);//2��
			if (check_rotate == 0) {
				*block_select -= 1;
			}
			else {
				*block_select -= 1;
				Empty_Block(*block_select, blockxy);//5��
				*block_select += 1;
				fill_Block(*block_select, blockxy);//3��
			}
		}
	}
	//4�� ���ϴ� ���� ��
	else if (*block_select >= 7 && *block_select <= 18) {
		if ((*block_select - 7) % 4 == 3) {//�迭 ���� �ִ� ������ ó�� ������ ��ȯ 
			*block_select -= 3;
			check_rotate = Check_Block(*block_select, blockxy);//2��
			if (check_rotate == 0) {
				*block_select += 3;
			}
			else {
				*block_select += 3;
				Empty_Block(*block_select, blockxy);//5��
				*block_select -= 3;
				fill_Block(*block_select, blockxy);//3��
			}
		}
		else {
			*block_select += 1;
			check_rotate = Check_Block(*block_select, blockxy);//2��
			if (check_rotate == 0) {
				*block_select -= 1;
			}
			else {
				*block_select -= 1;
				Empty_Block(*block_select, blockxy);//5��
				*block_select += 1;
				fill_Block(*block_select, blockxy);//3��
			}
		}
	}
}

/* 11. ���� �� ���� �Լ� */
int Random_value() {

	srand((static_cast<unsigned int>(time(NULL))));
	int value = rand() % 19;//0���� 18����

	return value;
}

/* main * main * main * main * main * main * main * main * main */
int main() {
	Cursor(false);//Ŀ�� off

	block blockxy;//�� ��ǥ ����
	int block_select = 0;//���õ� ��
	int input = 0;//Ű �Է�
	bool check = 0;//�̵� ���� ����
	int level = 0;//���̵� ����

	int* _block_select = &block_select;//block_select�� �ּҰ��� ���� ������ ����(�� ȸ���� �̿�)

	//���̵� �Է�
	cout << "���̵��� �Է��� �ּ��� 1 ~ 2 >> ";
	cin >> level;
	if (level >= 2)
		level = 2;

	//�ܼ� â ����
	system("cls");

	//1��
	Create_Map();


	block_select = Random_value();//11��
	create_Block(block_select, blockxy);//4��

	unsigned __int64 Timer = GetTickCount64();//�ý����� ������ �������� �� �Լ��� ȣ���� �������� �귯�� �ð��� 1000���� 1�� ������ �ð��� �˷��ִ� �Լ�
	while (true) {

		/*Input - Input - Input - Input - Input - Input */
		if (_kbhit())
			input = _getch();
		else
			input = 0;


		/* Update - Update - Update - Update - Update */
		if (GetTickCount64() >= Timer + 1000 / level) //�ӵ� ���� 1000 <- �Է¹ޱ�
		{
			Empty_Block(block_select, blockxy);//5��
			blockxy.y++;
			check = Check_Block(block_select, blockxy);//2��

			if (check == false) {
				blockxy.y--;
				Stop_Block(block_select, blockxy);//7��

				//Ŀ���� ���� ��ǥ�� �̵�
				blockxy.x = 5;
				blockxy.y = 2;
				//�� �������� ����
				block_select = Random_value();//11��
				create_Block(block_select, blockxy);//4��
			}
			else {
				Move_Down(block_select, blockxy);//9��
			}
			Timer = GetTickCount64();
		}

		//�Է¹��� Ű Update
		switch (input) {
			//���� Ű
		case left:
			blockxy.x--;
			if (Check_Block(block_select, blockxy) == false)//2��
				blockxy.x++;
			else
				Move_Left(block_select, blockxy);//9��
			break;

			//������ Ű
		case right:
			blockxy.x++;
			if (Check_Block(block_select, blockxy) == false)//2��
				blockxy.x--;
			else
				Move_Right(block_select, blockxy);//9��
			break;

			//�Ʒ��� Ű
		case down:
			blockxy.y++;
			if (Check_Block(block_select, blockxy) == false)//2��
				blockxy.y--;
			else
				Move_Down(block_select, blockxy);//9��
			break;

			//���� Ű (���� ����)
		case up:
			Rotate_Block(_block_select, blockxy);//10��
			break;

			//�����̽��� (�ϵ� ���)
		case space:
			while (true) {
				blockxy.y++;
				check = Check_Block(block_select, blockxy);//2��
				if (check == 0) {
					blockxy.y--;
					break;
				}
				else {
					Move_Down(block_select, blockxy);//9��
				}
			}
			break;
		}

		/*rendering - rendering - rendering - rendering - rendering*/
		Redering_Map();//8��
	}
}
