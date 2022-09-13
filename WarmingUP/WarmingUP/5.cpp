#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

struct Data {
	int x, y, z;
	bool isEmpty = true;
};

void copyData(Data& cng, Data copy);
bool compare(const Data& a, const Data& b);

class setData {
private:
	Data list[10]{};
	bool isEmpty;
	int front, rear;

	Data templist[10]{};
	bool isSort;
	int save_front, save_rear;


public:
	setData();

	void enque_f();
	void enque_r();
	void deque_f();
	void deque_r();

	void prtPoint();
	void clearList();
	void farPoint();
	void nearPoint();

	void sortPoint();

	void prtList();
};


int main()
{
	setData play;

	play.prtList();

	while (true) {
		char order;
		cout << "\nInput : ";
		cin >> order;

		switch (order) {
		case '+':
			play.enque_f();
			break;
		case '-':
			play.deque_f();
			break;
		case 'e':
			play.enque_r();
			break;
		case 'd':
			play.deque_r();
			break;
		case 'l':
			play.prtPoint();
			break;
		case 'c':
			play.clearList();
			break;
		case 'm':
			play.farPoint();
			break;
		case 'n':
			play.nearPoint();
			break;
		case 's':
			play.sortPoint();
			break;
		case 'q':
			return 0;
			break;
		default:
			cout << "ERROR" << endl;
			continue;
		}
		play.prtList();
	}
}

setData::setData()
{
	isEmpty = true;
	front = rear = 0;
	isSort = false;
	save_front = save_rear = 0;
}

void setData::enque_f()		//À§¿¡´Ù ½×´Â°Å
{
	isSort = false;
	Data temp{};
	temp.isEmpty = false;
	cin >> temp.x >> temp.y >> temp.z;

	if (!isEmpty)
		++front;
	else
		isEmpty = false;

	if (rear == front - 10) {
		cout << "¸ðµç Ä­ÀÌ ´Ù Ã¡½À´Ï´Ù." << endl;
		--front;
	}
	else
		copyData(list[front % 10], temp);
}

void setData::enque_r()		//¹Ø¿¡´Ù ½×´Â°Å
{
	isSort = false;
	if (isEmpty) {
		isEmpty = false;

		Data temp{};
		temp.isEmpty = false;
		cin >> temp.x >> temp.y >> temp.z;

		copyData(list[front], temp);
	}
	else {
		Data temp{};
		temp.isEmpty = false;
		cin >> temp.x >> temp.y >> temp.z;

		if (front - 9 == rear)
			cout << "¸ðµç Ä­ÀÌ ´Ù Ã¡½À´Ï´Ù." << endl;
		else {
			for (int i = 0; i <= front - rear; ++i) {
				copyData(list[(front + 1 - i) % 10], list[(front - i) % 10]);
			}
			++front;

			copyData(list[rear], temp);
		}
	}
}

void setData::deque_f()
{
	isSort = false;
	if (!isEmpty) {
		list[front % 10].isEmpty = true;
		if (front == rear)
			isEmpty = true;
		else
			--front;
	}
}

void setData::deque_r()
{
	isSort = false;
	if (!isEmpty) {
		list[rear].isEmpty = true;
		if (front == rear)
			isEmpty = true;
		else
			++rear;
	}
}

void setData::prtPoint()
{
	cout << front - rear + 1 << "°³\n" << endl;
}

void setData::clearList()
{
	if (!isEmpty)
		for (int i = rear; i <= front; ++i) {
			list[i].isEmpty = true;
		}
	isEmpty = true;
	front = rear = 0;
}

void setData::farPoint()
{
	if (!isEmpty) {
		int max = rear;
		for (int i = rear; i <= front; ++i) {
			if (pow(list[i].x, 2) + pow(list[i].y, 2) + pow(list[i].z, 2) > pow(list[max].x, 2) + pow(list[max].y, 2) + pow(list[max].z, 2))
				max = i;
		}
		cout << list[max].x << " " << list[max].y << " " << list[max].z << endl;
	}
	cout << endl;
}

void setData::nearPoint()
{
	if (!isEmpty) {
		int min = rear;
		for (int i = rear; i <= front; ++i) {
			if (pow(list[i].x, 2) + pow(list[i].y, 2) + pow(list[i].z, 2) < pow(list[min].x, 2) + pow(list[min].y, 2) + pow(list[min].z, 2))
				min = i;
		}
		cout << list[min].x << " " << list[min].y << " " << list[min].z << endl;
	}
	cout << endl;
}

void setData::sortPoint()
{
	if (!isEmpty) {
		if (!isSort) {
			isSort = true;

			for (int i = 0; i < 10; ++i) {
				copyData(templist[i], list[i]);
			}
			save_front = front;
			save_rear = rear;

			sort(list, list + 10, compare);
			front = front - rear;
			rear = 0;
		}
		else {
			isSort = false;
			for (int i = 0; i < 10; ++i) {
				copyData(list[i], templist[i]);
			}
			front = save_front;
			rear = save_rear;
		}
	}
}

void setData::prtList()
{
	for (int i = 9; i >= 0; --i) {
		cout << i << " | ";
		if (!list[i].isEmpty)
			cout << list[i].x << " " << list[i].y << " " << list[i].z;
		cout << endl;
	}
}

void copyData(Data& cng, Data copy)
{
	cng.isEmpty = copy.isEmpty;
	cng.x = copy.x;
	cng.y = copy.y;
	cng.z = copy.z;
}

bool compare(const Data& a, const Data& b)
{
	int avalue = 0, bvalue = 0;
	if (!a.isEmpty)
		avalue = pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2);
	if (!b.isEmpty)
		bvalue = pow(b.x, 2) + pow(b.y, 2) + pow(b.z, 2);
	return avalue > bvalue;
}