#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int WIDTH = 800;
const int HEIGHT = 600;

struct Point {
    float x, y;
};

Point applyTransformation(const float mat[3][3], const Point& p) {
    Point result;
    result.x = mat[0][0] * p.x + mat[0][1] * p.y + mat[0][2];
    result.y = mat[1][0] * p.x + mat[1][1] * p.y + mat[1][2];
    return result;
}

void drawTriangle(const Point& p1, const Point& p2, const Point& p3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glVertex2f(p3.x, p3.y);
    glEnd();
}

int main() {
    // Ask user for input
    Point p1, p2, p3;
    std::cout << "Enter coordinates for triangle vertices:\n";
    std::cout << "Vertex 1 (x y): "; std::cin >> p1.x >> p1.y;
    std::cout << "Vertex 2 (x y): "; std::cin >> p2.x >> p2.y;
    std::cout << "Vertex 3 (x y): "; std::cin >> p3.x >> p3.y;

    // Transformation parameters
    float shy = 0.5f;
    float sx = 1.2f, sy = 1.2f;
    float angle = 30 * (M_PI / 180); // 30 degrees in radians
    float tx = 100, ty = 80;

    // Define transformation matrices
    float shearMat[3][3] = {
        {1, 0, 0},
        {shy, 1, 0},
        {0, 0, 1}
    };

    float scaleMat[3][3] = {
        {sx, 0, 0},
        {0, sy, 0},
        {0, 0, 1}
    };

    float rotateMat[3][3] = {
        {(float)cos(angle), (float)-sin(angle), 0},
        {(float)sin(angle), (float) cos(angle), 0},
        {0, 0, 1}
    };

    float translateMat[3][3] = {
        {1, 0, tx},
        {0, 1, ty},
        {0, 0, 1}
    };

    // Apply transformations to each point
    Point tp1 = applyTransformation(shearMat, p1);
    tp1 = applyTransformation(scaleMat, tp1);
    tp1 = applyTransformation(rotateMat, tp1);
    tp1 = applyTransformation(translateMat, tp1);

    Point tp2 = applyTransformation(shearMat, p2);
    tp2 = applyTransformation(scaleMat, tp2);
    tp2 = applyTransformation(rotateMat, tp2);
    tp2 = applyTransformation(translateMat, tp2);

    Point tp3 = applyTransformation(shearMat, p3);
    tp3 = applyTransformation(scaleMat, tp3);
    tp3 = applyTransformation(rotateMat, tp3);
    tp3 = applyTransformation(translateMat, tp3);

    // OpenGL setup
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW.\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2D Composite Transformation", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Original triangle (white)
        glColor3f(1.0f, 1.0f, 1.0f);
        drawTriangle(p1, p2, p3);

        // Transformed triangle (red)
        glColor3f(1.0f, 0.0f, 0.0f);
        drawTriangle(tp1, tp2, tp3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
