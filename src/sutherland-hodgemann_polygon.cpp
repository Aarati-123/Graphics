#include <GL/glut.h>
#include <vector>

struct Point {
    float x, y;
};

std::vector<Point> polygon = {
    {20, 20}, {80, 20}, {80, 90}, {50, 120}, {20, 90}
};

float xmin = 40, xmax = 100, ymin = 30, ymax = 100;

bool inside(Point p, int edge) {
    switch (edge) {
        case 0: return p.x >= xmin;
        case 1: return p.x <= xmax;
        case 2: return p.y >= ymin;
        case 3: return p.y <= ymax;
    }
    return false;
}

Point intersect(Point p1, Point p2, int edge) {
    Point p;
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    if (dx == 0) dx = 0.0001f; // to prevent division by zero
    float m = dy / dx;

    switch (edge) {
        case 0: p.x = xmin; p.y = p1.y + m * (xmin - p1.x); break;
        case 1: p.x = xmax; p.y = p1.y + m * (xmax - p1.x); break;
        case 2: p.y = ymin; p.x = p1.x + (1 / m) * (ymin - p1.y); break;
        case 3: p.y = ymax; p.x = p1.x + (1 / m) * (ymax - p1.y); break;
    }
    return p;
}

std::vector<Point> clipPolygon(std::vector<Point> poly) {
    for (int edge = 0; edge < 4; edge++) {
        std::vector<Point> newPoly;
        for (int i = 0; i < poly.size(); i++) {
            Point curr = poly[i];
            Point prev = poly[(i + poly.size() - 1) % poly.size()];
            bool currIn = inside(curr, edge);
            bool prevIn = inside(prev, edge);

            if (currIn && prevIn)
                newPoly.push_back(curr);
            else if (!currIn && prevIn)
                newPoly.push_back(intersect(prev, curr, edge));
            else if (currIn && !prevIn) {
                newPoly.push_back(intersect(prev, curr, edge));
                newPoly.push_back(curr);
            }
        }
        poly = newPoly;
    }
    return poly;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Clipping window
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin); glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax); glVertex2f(xmin, ymax);
    glEnd();

    // Original polygon (Blue)
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_LOOP);
    for (auto p : polygon)
        glVertex2f(p.x, p.y);
    glEnd();

    // Clipped polygon (Green)
    auto clipped = clipPolygon(polygon);
    glColor3f(0, 1, 0);
    glBegin(GL_POLYGON);
    for (auto p : clipped)
        glVertex2f(p.x, p.y);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 150, 0, 150);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sutherland–Hodgman Polygon Clipping");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}