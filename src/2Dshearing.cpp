#include <GLFW/glfw3.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

float triangle[3][3];   // Original triangle vertices
float sheared[3][3];    // Sheared triangle
float S[3][3];          // Shearing matrix

void shearingMatrix(char axis, float shx, float shy, float matrix[3][3]) {
    // Identity matrix
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] = (i == j) ? 1 : 0;

    if (axis == 'x') {
        matrix[0][1] = shx;
    } else if (axis == 'y') {
        matrix[1][0] = shy;
    }
}

void applyTransformation(float src[3][3], float matrix[3][3], float result[3][3]) {
    for (int i = 0; i < 3; ++i) {
        result[0][i] = matrix[0][0] * src[0][i] + matrix[0][1] * src[1][i] + matrix[0][2] * src[2][i];
        result[1][i] = matrix[1][0] * src[0][i] + matrix[1][1] * src[1][i] + matrix[1][2] * src[2][i];
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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2D Shearing", NULL, NULL);
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
    float shx = 0, shy = 0;

    std::cout << "Enter coordinates of triangle vertices:\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "Vertex " << i + 1 << " (x y): ";
        std::cin >> triangle[0][i] >> triangle[1][i];
        triangle[2][i] = 1;
    }

    std::cout << "Shear along axis (x or y): ";
    std::cin >> axis;

    if (axis == 'x') {
        std::cout << "Enter shear factor shx: ";
        std::cin >> shx;
    } else if (axis == 'y') {
        std::cout << "Enter shear factor shy: ";
        std::cin >> shy;
    } else {
        std::cerr << "Invalid axis\n";
        return -1;
    }

    shearingMatrix(axis, shx, shy, S);
    applyTransformation(triangle, S, sheared);

    GLFWwindow* window = initOpenGL();
    if (!window) return -1;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        drawTriangle(triangle, 1.0f, 0.0f, 0.0f);     // Red: Original
        drawTriangle(sheared, 0.0f, 1.0f, 0.0f);      // Green: Sheared

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
