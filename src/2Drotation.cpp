#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


const int WIDTH = 800;
const int HEIGHT = 600;

float triangle[3][3];         // Original triangle: [x/y/homogeneous][vertex_index]
float transformed[3][3];      // Rotated triangle
float R[3][3];                // Rotation matrix

// Convert degrees to radians
float deg2rad(float deg) {
    return deg * M_PI / 180.0f;
}

// Create the 3x3 rotation matrix for angle θ (in degrees)
void rotationMatrix(float angleDeg, float matrix[3][3]) {
    float angleRad = deg2rad(angleDeg);
    float cosA = cos(angleRad);
    float sinA = sin(angleRad);

    matrix[0][0] = cosA;  matrix[0][1] = -sinA; matrix[0][2] = 0;
    matrix[1][0] = sinA;  matrix[1][1] = cosA;  matrix[1][2] = 0;
    matrix[2][0] = 0;     matrix[2][1] = 0;     matrix[2][2] = 1;
}

// Rotate triangle about its centroid
void rotateAboutCentroid(float matrix[3][3]) {
    // Calculate centroid of triangle
    float centroidX = (triangle[0][0] + triangle[0][1] + triangle[0][2]) / 3.0f;
    float centroidY = (triangle[1][0] + triangle[1][1] + triangle[1][2]) / 3.0f;

    for (int i = 0; i < 3; i++) {
        // Translate vertex to origin (centroid at origin)
        float x = triangle[0][i] - centroidX;
        float y = triangle[1][i] - centroidY;

        // Apply rotation
        transformed[0][i] = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * 1;
        transformed[1][i] = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * 1;
        transformed[2][i] = 1;

        // Translate back to original centroid position
        transformed[0][i] += centroidX;
        transformed[1][i] += centroidY;
    }
}

// Function to draw a triangle with given color
void drawTriangle(float mat[3][3], float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; i++) {
        glVertex2f(mat[0][i], mat[1][i]);
    }
    glEnd();
}

// Initialize OpenGL window and projection
GLFWwindow* initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2D Rotation about Centroid", NULL, NULL);
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
    float angle;

    // Input triangle vertices
    std::cout << "Enter coordinates of the triangle vertices:\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "Vertex " << i + 1 << " (x y): ";
        std::cin >> triangle[0][i] >> triangle[1][i];
        triangle[2][i] = 1; // homogeneous coordinate
    }

    // Input rotation angle in degrees
    std::cout << "Enter rotation angle (in degrees): ";
    std::cin >> angle;

    // Build rotation matrix
    rotationMatrix(angle, R);

    // Rotate triangle about centroid
    rotateAboutCentroid(R);

    GLFWwindow* window = initOpenGL();
    if (!window) return -1;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Draw original triangle in red
        drawTriangle(triangle, 1.0f, 0.0f, 0.0f);

        // Draw rotated triangle in green
        drawTriangle(transformed, 0.0f, 1.0f, 0.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
