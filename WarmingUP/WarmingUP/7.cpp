#include <iostream>
#include <string>
#include <random>
#include <windows.h>

void resetBoard(char(*board)[4]);
void prtBoard(char(*board)[4]);

int main()
{
	using namespace std;

	char board[4][4];		// ���� ������ ����
	char prtboard[4][4];	// ������ ��µ� ����
	memset(prtboard, '*', sizeof(prtboard));

	resetBoard(board);
	int endCount{};

	prtBoard(board);
	cout << endl;

	int score{ 100 };

	while (true) {
		char col1, col2;
		int row1, row2;

		cout << "���� ���� : " << score << "��" << endl;
		prtBoard(prtboard);

		cout << endl << "input card 1:";
		cin >> col1;
		if (col1 == 'r') {
			cout << "���带 �ʱ�ȭ�մϴ�." << endl;
			while (getchar() != '\n');
			resetBoard(board);
			endCount = 0;
			prtBoard(board);
			continue;
		}
		cin >> row1;
		prtboard[row1 - 1][col1 - 'a'] = 'O';

		prtBoard(prtboard);

		cout << endl << "input card 2:";
		cin >> col2;
		if (col2 == 'r') {
			cout << "���带 �ʱ�ȭ�մϴ�." << endl;
			while (getchar() != '\n');
			resetBoard(board);
			endCount = 0;
			prtBoard(board);
			continue;
		}
		cin >> row2;
		prtboard[row2 - 1][col2 - 'a'] = 'O';

		prtBoard(prtboard);

		cout << "\n����Ű�� ������ ī�带 �����մϴ�." << endl;
		system("pause");
		
		
		prtboard[row1 - 1][col1 - 'a'] = board[row1 - 1][col1 - 'a'];
		prtboard[row2 - 1][col2 - 'a'] = board[row2 - 1][col2 - 'a'];

		prtBoard(prtboard);

		if (prtboard[row1 - 1][col1 - 'a'] != prtboard[row2 - 1][col2 - 'a']) {
			cout << "ī�尡 ���� �ʽ��ϴ�. ī�带 ����ϴ�." << endl;
			score -= 25;
			system("pause");
			prtboard[row1 - 1][col1 - 'a'] = '*';
			prtboard[row2 - 1][col2 - 'a'] = '*';
		}
		else {
			++endCount;
			score += 25;
			cout << endl << "������ϴ�." << endl;
			system("pause");
		}

		if (score < 0) {
			cout << "�ʹ� ���� Ʋ�Ⱦ��.\n������ �����մϴ�." << endl;
			break;
		}

		if (endCount == 8) {
			cout << "\n������ �������ϴ�.\n������ �����մϴ�." << endl;
			cout << "���� ���� : " << score << "��" << endl;
			break;
		}
		
		system("cls");
	}
	return 0;
}

void resetBoard(char(*board)[4])
{
	using namespace std;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 15);

	char list[16] = { 'A', 'A', 'B', 'B', 'C', 'C', 'D', 'D', 'E', 'E', 'F', 'F', 'G', 'G', 'H', 'H' };

	bool isempty[4][4];
	memset(isempty, true, sizeof(isempty));

	for (int i = 0; i < 16; ++i) {
		while (true) {
			int R = dis(gen);

			if (!isempty[R / 4][R % 4])
				continue;

			board[R / 4][R % 4] = list[i];
			isempty[R / 4][R % 4] = false;
			break;
		}
	}
}

void prtBoard(char(*board)[4])
{
	using namespace std;

	cout << "  a b c d" << endl;
	for (int i = 0; i < 4; ++i) {
		cout << i + 1 << ' ';
		for (int j = 0; j < 4; ++j) {
			int color;
			switch (board[i][j]) {
			case 'A':
				color = 4;
				break;
			case 'B':
				color = 6;
				break;
			case 'C':
				color = 8;
				break;
			case 'D':
				color = 9;
				break;
			case 'E':
				color = 10;
				break;
			case 'F':
				color = 11;
				break;
			case 'G':
				color = 13;
				break;
			case 'H':
				color = 14;
				break;
			default:
				color = 15;
				break;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
			cout << board[i][j] << ' ';
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
		cout << endl;
	}
}
