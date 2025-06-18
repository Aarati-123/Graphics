#include <GLFW/glfw3.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

float triangle[3][3];         // Original triangle
float transformed[3][3];      // Translated triangle
float T[3][3];                // Translation matrix

// Create the 3x3 translation matrix
void translationMatrix(float tx, float ty, float matrix[3][3]) {
    matrix[0][0] = 1;  matrix[0][1] = 0;  matrix[0][2] = tx;
    matrix[1][0] = 0;  matrix[1][1] = 1;  matrix[1][2] = ty;
    matrix[2][0] = 0;  matrix[2][1] = 0;  matrix[2][2] = 1;
}

// Multiply translation matrix with original triangle
void multiplyMatrix(float matrix[3][3]) {
    for (int i = 0; i < 3; i++) {
        transformed[0][i] = matrix[0][0] * triangle[0][i] + matrix[0][1] * triangle[1][i] + matrix[0][2] * triangle[2][i];
        transformed[1][i] = matrix[1][0] * triangle[0][i] + matrix[1][1] * triangle[1][i] + matrix[1][2] * triangle[2][i];
        transformed[2][i] = matrix[2][0] * triangle[0][i] + matrix[2][1] * triangle[1][i] + matrix[2][2] * triangle[2][i];
    }
}

// Function to draw a triangle
void drawTriangle(float mat[3][3], float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; i++) {
        glVertex2f(mat[0][i], mat[1][i]);
    }
    glEnd();
}

// Initialize OpenGL window
GLFWwindow* initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2D Translation - Homogeneous Coordinates", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    // Set up 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return window;
}

int main() {
    float tx, ty;

    // Take input from user
    std::cout << "Enter coordinates of the triangle vertices:\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "Vertex " << i + 1 << " (x y): ";
        std::cin >> triangle[0][i] >> triangle[1][i];
        triangle[2][i] = 1; // homogeneous coordinate
    }

    std::cout << "Enter translation values (tx ty): ";
    std::cin >> tx >> ty;

    translationMatrix(tx, ty, T);
    multiplyMatrix(T);

    GLFWwindow* window = initOpenGL();
    if (!window) return -1;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Draw original triangle in red
        drawTriangle(triangle, 1.0f, 0.0f, 0.0f);

        // Draw translated triangle in blue
        drawTriangle(transformed, 0.0f, 0.0f, 1.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
