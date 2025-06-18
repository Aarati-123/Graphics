#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const int WIDTH = 800, HEIGHT = 600;

bool waitingForSecondClick = false;
bool animating = false;

int startX, startY, endX, endY;

// Animation variables
int totalSteps = 0;
int currentStep = 0;

// Storage for points to draw
// Maximum line length won't exceed window diagonal
const int MAX_POINTS = WIDTH + HEIGHT;
int pointsX[MAX_POINTS];
int pointsY[MAX_POINTS];

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Bresenham's line algorithm to calculate all points between start and end
// Stores points in pointsX and pointsY arrays and sets totalSteps
void calculateBresenhamLine(int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    bool steep = dy > dx;

    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(dx, dy);
        std::swap(sx, sy);
    }

    int err = 2 * dy - dx;
    int x = x0;
    int y = y0;

    totalSteps = 0;

    for (int i = 0; i <= dx; i++) {
        if (steep) {
            pointsX[totalSteps] = y;
            pointsY[totalSteps] = x;
        } else {
            pointsX[totalSteps] = x;
            pointsY[totalSteps] = y;
        }
        totalSteps++;

        if (err > 0) {
            y += sy;
            err -= 2 * dx;
        }
        err += 2 * dy;
        x += sx;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !animating) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        ypos = HEIGHT - ypos; // Convert from window coordinates to OpenGL coords

        if (!waitingForSecondClick) {
            startX = (int)xpos;
            startY = (int)ypos;
            waitingForSecondClick = true;
            std::cout << "Start point: (" << startX << ", " << startY << ")\n";
        } else {
            endX = (int)xpos;
            endY = (int)ypos;
            waitingForSecondClick = false;

            calculateBresenhamLine(startX, startY, endX, endY);
            currentStep = 0;
            animating = true;

            std::cout << "End point: (" << endX << ", " << endY << ")\n";
            std::cout << "Animating Bresenham line with " << totalSteps << " points.\n";
        }
    }
}

GLFWwindow* initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bresenham Line Animation", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Setup orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPointSize(2.0f); // Make points visible

    return window;
}

void render(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        glColor3f(1.0f, 1.0f, 1.0f); // White color for points

        if (animating) {
            glBegin(GL_POINTS);
            for (int i = 0; i <= currentStep; i++) {
                glVertex2i(pointsX[i], pointsY[i]);
            }
            glEnd();

            if (currentStep < totalSteps - 1) {
                currentStep++;
            } else {
                animating = false; // Animation finished
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (animating) {
            glfwWaitEventsTimeout(0.01); // Slow down animation for visibility
        }
    }
}

#ifdef _WIN32
#include <windows.h>
int main(int argc, char** argv);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif

int main(int argc, char** argv) {
    GLFWwindow* window = initOpenGL();
    if (!window) return -1;

    render(window);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
