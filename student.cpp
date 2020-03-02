#include "stdafx.h"
#include "student.h"

istream & operator>>(istream & in, student & obj)
{
	in >> obj.name;
	in >> obj.score;
	return in;
}

ostream & operator<<(ostream & out, const student & obj)
{
	out << obj.name << " ";
	out << obj.score << " ";
	return out;
}