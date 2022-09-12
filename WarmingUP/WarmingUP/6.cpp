#define	wleft 0
#define wright 1
#define wup 2
#define wdown 3

#include <iostream>
#include <random>
#include <format>

using namespace std;

class game {
private:
	int board[30][30];
	int x, y;
	int num;	//현재 몇번째
	int cont;	//연속으로 몇번째인지
	int prevway;	//어디로 연속
	bool end;		//끝났는지

public:
	game();
	void go();
	void prt();
	bool isend();
	void reset();
	void move_r();
	void move_l();
};

int main()
{
	game play;

	while (!play.isend()) {
		play.go();
	}


	while (true) {
		cout << "명령어 입력 : ";
		char order{};

		cin >> order;

		switch (order) {
		case 'R':
			play.reset();
			while (!play.isend()) {
				play.go();
			}
			break;
		case 'r':
			play.move_r();
			play.prt();
			break;
		case 'l':
			play.move_l();
			play.prt();
			break;
		case 'q':
			return 0;
			break;

		}
	}


	return 0;

}

game::game()
{
	for (int i = 0; i < 30; ++i)
		for (int j = 0; j < 30; ++j)
			board[i][j] = 0;

	board[0][0] = 1;
	x = y = 0;
	num = 1;
	cont = 0;
	prevway = wleft;
	end = false;
}

void game::go()
{
	if (x == 29 && y == 29) {
		prt();
		end = true;
		return;
	}

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 3);

	int way = dis(gen);


	bool iscont = false;
	while (true) {
		if (cont == 8) {
			iscont = true;
			cont = 0;
		}

		if (x == 0 && way == wleft) {
			way = dis(gen);
			continue;
		}
		if (x == 29 && way == wright) {
			way = dis(gen);
			continue;
		}
		if (y == 0 && way == wup) {
			way = dis(gen);
			continue;
		}
		if (y == 29 && way == wdown) {
			way = dis(gen);
			continue;
		}
		if (iscont && way == prevway) {
			way = dis(gen);
			continue;
		}
		break;
	}

	if (prevway == way)
		++cont;
	else
		cont = 0;

	prevway = way;

	switch (way) {
	case wleft:
		--x;
		break;
	case wright:
		++x;
		break;
	case wup:
		--y;
		break;
	case wdown:
		++y;
		break;
	default:
		break;
	}

	board[y][x] = ++num;
	 

}

void game::prt()
{
	for (int i = 0; i < 30; ++i) {
		for (int j = 0; j < 30; ++j) {
			cout << format("{:6} ", board[i][j]);
		}
		cout << endl;
	}
}

bool game::isend()
{
	return end;
}

void game::reset()
{
	for (int i = 0; i < 30; ++i)
		for (int j = 0; j < 30; ++j)
			board[i][j] = 0;

	board[0][0] = 1;
	x = y = 0;
	num = 1;
	cont = 0;
	prevway = wleft;
	end = false;
}

void game::move_r()
{
	int temp[30]{};

	for (int i = 0; i < 30; ++i) {
		temp[i] = board[i][29];
	}

	for (int i = 0; i < 29; ++i) {
		for (int j = 0; j < 30; ++j) {
			board[j][29 - i] = board[j][28 - i];
		}
	}

	for (int i = 0; i < 30; ++i) {
		board[i][0] = temp[i];
	}
}

void game::move_l()
{
	int temp[30]{};

	for (int i = 0; i < 30; ++i) {
		temp[i] = board[i][0];
	}

	for (int i = 0; i < 29; ++i) {
		for (int j = 0; j < 30; ++j) {
			board[j][i] = board[j][i + 1];
		}
	}

	for (int i = 0; i < 30; ++i) {
		board[i][29] = temp[i];
	}
}
