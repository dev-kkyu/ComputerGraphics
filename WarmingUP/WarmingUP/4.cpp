#include <iostream>
#include <random>
#include <format>
#include <algorithm>

class Rect {
public:
	int x1, y1;
	int x2, y2;

	Rect();
};

bool up(Rect* R);
bool left(Rect* R);
bool down(Rect* R);
bool right(Rect* R);

bool isCollide(Rect r1, Rect r2);

int main()
{
	using namespace std;

	Rect r1;
	Rect r2;
	bool seq = true;

	cout << "Input Shape Coordinates value:" << endl;
	while (true) {

		cout << "	Rect 1: (" << format("{:3}", r1.x1) << ", " << format("{:3}", r1.y1) << ") (" << format("{:3}", r1.x2) << ", " << format("{:3}", r1.y2) << ")" << endl;
		cout << "	Rect 2: (" << format("{:3}", r2.x1) << ", " << format("{:3}", r2.y1) << ") (" << format("{:3}", r2.x2) << ", " << format("{:3}", r2.y2) << ")" << endl;

		if (isCollide(r1, r2)) {
			cout << "Rectangle 1 & Rectangle 2 collide!!" << endl;
			return 0;
		}
		
		char order;
		if (seq)cout << "Rect 1 차례" << endl;
		else cout << "Rect 2 차례" << endl;
		cout << "Command: ";
		cin >> order;

		switch (order) {
		case 'w':
			if (up(seq ? &r1 : &r2))seq = !seq;
			seq = !seq;
			break;
		case 'a':
			if (left(seq ? &r1 : &r2))seq = !seq;
			seq = !seq;
			break;
		case 's':
			if (down(seq ? &r1 : &r2))seq = !seq;
			seq = !seq;
			break;
		case 'd':
			if (right(seq ? &r1 : &r2))seq = !seq;
			seq = !seq;
			break;
		default:
			cout << "잘못된 입력입니다." << endl;
			continue;
			break;
		}
	}



}

Rect::Rect()
{
	using namespace std;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> rndWidth(0, 800);
	uniform_int_distribution<int> rndHeight(0, 600);

	x1 = rndWidth(gen);
	y1 = rndHeight(gen);
	x2 = rndWidth(gen);
	y2 = rndHeight(gen);

	if (x1 > x2)		//x1, y1이 항상 x2, y2보다 작게 설정
		swap(x1, x2);
	if (y1 > y2)
		swap(y1, y2);
}

bool up(Rect* R)
{
	using namespace std;
	if (R->y1 - 50 < 0 || R->y2 - 50 < 0) {
		cout << "Error! Out of Range" << endl;
		return true;
	}
	R->y1 -= 50;
	R->y2 -= 50;
	return false;
}

bool left(Rect* R)
{
	using namespace std;
	if (R->x1 - 50 < 0 || R->x2 - 50 < 0) {
		cout << "Error! Out of Range" << endl;
		return true;
	}
	R->x1 -= 50;
	R->x2 -= 50;
	return false;
}

bool down(Rect* R)
{
	using namespace std;
	if (R->y1 + 50 > 600 || R->y2 + 50 > 600) {
		cout << "Error! Out of Range" << endl;
		return true;
	}
	R->y1 += 50;
	R->y2 += 50;
	return false;
}

bool right(Rect* R)
{
	using namespace std;
	if (R->x1 + 50 > 800 || R->x2 + 50 > 800) {
		cout << "Error! Out of Range" << endl;
		return true;
	}
	R->x1 += 50;
	R->x2 += 50;
	return false;
}

bool isCollide(Rect r1, Rect r2)
{
	if (r1.x2 < r2.x1 || r1.x1 > r2.x2) return false;
	if (r1.y2 < r2.y1 || r1.y1 > r2.y2) return false;
	return true;
}
