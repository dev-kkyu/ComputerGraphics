#include "KeyboardUP.h"

GLvoid KeyboardUP(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
	case 'W':
		isW = false;
		break;
	case 'a':
	case 'A':
		isA = false;
		break;
	case 's':
	case 'S':
		isS = false;
		break;
	case 'd':
	case 'D':
		isD = false;
		break;
	}
}
