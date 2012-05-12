/**
 * \file
 * \author Ricky Curtice
 * \date 20120112
 * \brief A dynamic helical tube mesh
 *
 * <p>If any point accessor or mover doesn't return CurrentPoint, points must be check for a 0 size.
 * </p>
 *
 */
#include "PointList.h"

// System Library Includes

// Application Library Includes

// Local Includes

// Forward Declarations

// Typedefs and data structures

// Constants

// Class methods

PointList::PointList() : index(0), loop(true) {

}


PointList::~PointList() {

}

void PointList::AddPoint( Point p ) {
	if (!PointExists(p)) {
		this->points.push_back(p);
	}
}

void PointList::RemovePoint( Point p ) {
	for (auto itr = this->points.begin(); itr != this->points.end(); ++itr) {
		if ((itr->x == p.x) && (itr->y == p.y)) {
			this->points.erase(itr);
			return;
		}
	}
}

bool PointList::PointExists( Point p ) {
	for (auto itr = this->points.begin(); itr != this->points.end(); ++itr) {
		if ((itr->x == p.x) && (itr->y == p.y)) {
			return true;
		}
	}

	return false;
}

Point PointList::FirstPoint() {
	this->index = 0;

	return CurrentPoint();
}

Point PointList::NextPoint() {
	if ((this->index + 1) < this->points.size()) {
		++this->index;
	}
	else {
		if (loop == true) {
			this->index = 0;
		}
	}

	return CurrentPoint();
}

Point PointList::PeekNextPoint() {
	if (this->points.size() > 0) {
		if ((this->index + 1) < this->points.size()) {
			return this->points[this->index + 1];
		}
		else {
			if (loop == true) {
				return this->points[0];
			}
			else {
				return this->points[this->points.size() - 1];
			}
		}
	}
	return Point();
}

Point PointList::CurrentPoint() {
	if ((this->points.size() > 0) && (this->index < this->points.size())) {
		return this->points[this->index];
	}
	return Point();
}

Point PointList::IndexPoint( unsigned int i ) {
	if (i < (this->points.size() - 1)) {
		this->index = i;
	}

	return CurrentPoint();
}
Point PointList::GotoPoint( Point p ) {
	for (unsigned int i = 0; i < this->points.size(); ++i) {
		if ((this->points[i].x == p.x) && (this->points[i].y == p.y))   {
			this->index = i;
		}
	}
	return CurrentPoint();
}


Point PointList::PrevPoint() {
	if ((this->index - 1) > 0 ) {
		--this->index;
	}
	else {
		if (loop == true) {
			this->index = this->points.size() - 1;
		}
	}

	return CurrentPoint();
}

Point PointList::PeekPrevPoint() {
	if (this->points.size() > 0) {
		if ((this->index - 1) > 0) {
			return this->points[this->index - 1];
		}
		else {
			if (loop == true) {
				return this->points[this->points.size() - 1];
			}
			else {
				return this->points[0];
			}
		}
	}
	return Point();
}


Point PointList::LastPoint() {
	this->index = this->points.size() - 1;

	return CurrentPoint();
}

std::vector<Point>::iterator PointList::BeginPoint() {
	return this->points.begin();
}

std::vector<Point>::iterator PointList::NthPoint( unsigned int n ) {
	if (n < (this->points.size() - 1)) {
		return (this->points.begin() + n);
	}

	return this->points.end();
}

std::vector<Point>::iterator PointList::EndPoint() {
	return this->points.end();
}
unsigned int PointList::Size() {
	return this->points.size();
}


void PointList::Loop( bool loop ) {
	this->loop = loop;
}

void PointList::Clear() {
	this->points.clear();
}
