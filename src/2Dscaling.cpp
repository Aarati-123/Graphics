#include <GLFW/glfw3.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

float triangle[3][3];      // Original triangle vertices [x/y/homogeneous][vertex]
float transformed[3][3];   // Scaled triangle
float S[3][3];             // Scaling matrix

// Create scaling matrix for sx and sy
void scalingMatrix(float sx, float sy, float matrix[3][3]) {
    matrix[0][0] = sx;  matrix[0][1] = 0;   matrix[0][2] = 0;
    matrix[1][0] = 0;   matrix[1][1] = sy;  matrix[1][2] = 0;
    matrix[2][0] = 0;   matrix[2][1] = 0;   matrix[2][2] = 1;
}

// Apply transformation matrix to triangle
void applyTransformation(float triangle[3][3], float matrix[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        result[0][i] = matrix[0][0]*triangle[0][i] + matrix[0][1]*triangle[1][i] + matrix[0][2]*triangle[2][i];
        result[1][i] = matrix[1][0]*triangle[0][i] + matrix[1][1]*triangle[1][i] + matrix[1][2]*triangle[2][i];
        result[2][i] = 1;
    }
}

// Draw a triangle given matrix and color
void drawTriangle(float mat[3][3], float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; i++) {
        glVertex2f(mat[0][i], mat[1][i]);
    }
    glEnd();
}

// OpenGL Initialization
GLFWwindow* initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2D Scaling using Homogeneous Coordinates", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return window;
}

int main() {
    float sx, sy;

    // Input triangle vertices
    std::cout << "Enter coordinates of the triangle vertices:\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "Vertex " << i + 1 << " (x y): ";
        std::cin >> triangle[0][i] >> triangle[1][i];
        triangle[2][i] = 1; // homogeneous coordinate
    }

    // Input scaling factors
    std::cout << "Enter scaling factors (sx sy): ";
    std::cin >> sx >> sy;

    // Build scaling matrix and apply it
    scalingMatrix(sx, sy, S);
    applyTransformation(triangle, S, transformed);

    // Initialize window
    GLFWwindow* window = initOpenGL();
    if (!window) return -1;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Draw original triangle in red
        drawTriangle(triangle, 1.0f, 0.0f, 0.0f);

        // Draw scaled triangle in blue
        drawTriangle(transformed, 0.0f, 0.0f, 1.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
