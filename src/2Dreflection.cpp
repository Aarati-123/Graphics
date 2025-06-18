#include <GLFW/glfw3.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

float triangle[3][3];      // Original triangle vertices
float reflected[3][3];     // Reflected triangle
float R[3][3];             // Reflection matrix

void reflectionMatrix(char axis, float matrix[3][3]) {
    // Identity Matrix
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] = (i == j) ? 1 : 0;

    if (axis == 'x') {
        matrix[1][1] = -1;
    } else if (axis == 'y') {
        matrix[0][0] = -1;
    } else if (axis == 'o') {
        matrix[0][0] = -1;
        matrix[1][1] = -1;
    } else if (axis == 'd') {
        matrix[0][0] = 0;
        matrix[0][1] = 1;
        matrix[1][0] = 1;
        matrix[1][1] = 0;
    }
}

void applyTransformation(float src[3][3], float matrix[3][3], float result[3][3]) {
    for (int i = 0; i < 3; ++i) {
        result[0][i] = matrix[0][0]*src[0][i] + matrix[0][1]*src[1][i] + matrix[0][2]*src[2][i];
        result[1][i] = matrix[1][0]*src[0][i] + matrix[1][1]*src[1][i] + matrix[1][2]*src[2][i];
        result[2][i] = 1;
    }
}

void drawTriangle(float mat[3][3], float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; ++i) {
        glVertex2f(mat[0][i], mat[1][i]);
    }
    glEnd();
}

GLFWwindow* initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2D Reflection", NULL, NULL);
    if (!window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2, -1, 1);  // Centered coordinate system
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return window;
}

int main() {
    char axis;

    std::cout << "Enter coordinates of triangle vertices:\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "Vertex " << i + 1 << " (x y): ";
        std::cin >> triangle[0][i] >> triangle[1][i];
        triangle[2][i] = 1;
    }

    std::cout << "Reflect about: x (x-axis), y (y-axis), o (origin), d (y = x): ";
    std::cin >> axis;

    reflectionMatrix(axis, R);
    applyTransformation(triangle, R, reflected);

    GLFWwindow* window = initOpenGL();
    if (!window) return -1;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        drawTriangle(triangle, 1.0f, 0.0f, 0.0f);     // Red: Original
        drawTriangle(reflected, 0.0f, 1.0f, 0.0f);    // Green: Reflected

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
