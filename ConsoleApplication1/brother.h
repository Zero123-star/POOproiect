#ifndef BROTHER_H
#define BROTHER_H
struct point
{
	int x;
	int y;
	
	int operator <(const point& z)
	{
		if (this->y < z.y)
			return true;
		else
			if (this->y == z.y && this->x < z.x)
				return true;
		return false;

	}
	point operator +=(const point& c)
	{
		this->x = this->x + c.x;
		this->y = this->y + c.y;
		return *this;
	}
	point operator -=(const point& c)
		{
			this->x = this->x - c.x;
			this->y = this->y - c.y;
			return *this;
		}
};
#endif

