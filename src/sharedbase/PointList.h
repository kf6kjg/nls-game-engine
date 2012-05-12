/**
 * \file
 * \author Adam Martin
 * \brief *TODO:
*/
#pragma once

#include <map>
#include <string>
#include <vector>

struct Point {
	int x,y;
	bool operator<(const Point& c) const {
		return ((x != c.x) ? (x < c.x) : (y < c.y));
	}
	Point() : x(-1), y(-1) { }
};

class PointList {
public:
	PointList();
	~PointList();

	void AddPoint(Point); // Adds the point to the list.
	void RemovePoint(Point); // Removes the point from the list
	bool PointExists(Point); // Returns if the point is in the list
	void Clear(); // Clears all points from the list

	// Movement through the list via internal index
	Point FirstPoint(); // Moves to and returns the first point in the list.
	Point NextPoint(); // Moves forward in the list and then returns the current point.
	Point PeekNextPoint(); // Returns the next point, without moving forward.
	Point CurrentPoint(); // Returns the current point in the list.
	Point IndexPoint(unsigned int i); // Moves to and returns the point at index i in the list. (0 based)
	Point GotoPoint(Point p); // Moves to and returns the point at point p in the list.
	Point PrevPoint(); // Moves backward in the list and then returns the current point.
	Point PeekPrevPoint(); // Returns the previous point, without moving backward.
	Point LastPoint(); // Moves to and returns the last point in the list.


	// Movement through the list through iterator
	std::vector<Point>::iterator BeginPoint(); // Returns an iterator to the first point in the list.
	std::vector<Point>::iterator NthPoint(unsigned int n); // Returns an iterator to the Nth point in the list. (0 based)
	std::vector<Point>::iterator EndPoint(); // Returns an iterator to the last point in the list.
	unsigned int Size(); // Returns the number of points in the list.

	void AddRef() { }
	void Release() { }

	void Loop(bool loop);

private:
	std::vector<Point> points;
	unsigned int index;
	bool loop; // Determine if it should loop back to the start (true), stop at the end (false)
};
