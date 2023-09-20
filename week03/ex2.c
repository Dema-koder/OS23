#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

struct Point {
	double x, y;
};

struct Point* createPoint(double x, double y) {
	struct Point* cur = (struct Point*)(malloc(sizeof(struct Point)));
	cur->x = x;
	cur->y = y;
	return cur;
}

double distance(struct Point* a, struct Point* b) {
	return sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

double area(struct Point* a, struct Point* b, struct Point* c) {
	return 0.5 * (a->x * b->y - b->x * a->y + b->x * c->y - c->x * b->y + c->x * a->y - a->x * c->y);
}

int main(void) {
	struct Point* a = createPoint(2.5, 6);
	struct Point* b = createPoint(1, 2.2);
	struct Point* c = createPoint(10, 6);
	printf("Distance between A and B: %lf\n", distance(a, b));
	printf("Area of triangle ABC: %lf\n", area(a, b, c));
}
