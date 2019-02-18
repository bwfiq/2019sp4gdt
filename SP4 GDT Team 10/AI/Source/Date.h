#pragma once
#include <iostream>
using namespace std;

class Date
{
public:
	Date(int m, int d)
	{
		mo = m; 
		da = d;
	}
	friend ostream& operator<<(ostream& os, const Date& dt);
	int mo, da;
};

ostream& operator<<(ostream& os, const Date& dt)
{
	switch (dt.mo)
	{
	case 1:
		os << "JAN";
		break;
	case 2:
		os << "FEB";
		break;
	case 3:
		os << "MAR";
		break;
	case 4:
		os << "APR";
		break;
	case 5:
		os << "MAY";
		break;
	case 6:
		os << "JUN";
		break;
	case 7:
		os << "JUL";
		break;
	case 8:
		os << "AUG";
		break;
	case 9:
		os << "SEP";
		break;
	case 10:
		os << "OCT";
		break;
	case 11:
		os << "NOV";
		break;
	case 12:
		os << "DEC";
		break;
	}
	os << " ";
	os << dt.da;
	return os;
}
