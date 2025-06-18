#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const int WIDTH = 800, HEIGHT = 600;

bool waitingForRadius = false;
bool animating = false;

int centerX = 0, centerY = 0;
int radius = 0;

struct Point {
    int x, y;
};

const int MAX_POINTS = 2000; // max points for circle
Point circlePoints[MAX_POINTS];
int totalPoints = 0;
int currentPoint = 0;

// Function to plot all 8 symmetric points of the circle
void plotCirclePoints(int cx, int cy, int x, int y) {
    if (totalPoints + 8 > MAX_POINTS) return; // avoid overflow

    circlePoints[totalPoints++] = {cx + x, cy + y};
    circlePoints[totalPoints++] = {cx - x, cy + y};
    circlePoints[totalPoints++] = {cx + x, cy - y};
    circlePoints[totalPoints++] = {cx - x, cy - y};
    circlePoints[totalPoints++] = {cx + y, cy + x};
    circlePoints[totalPoints++] = {cx - y, cy + x};
    circlePoints[totalPoints++] = {cx + y, cy - x};
    circlePoints[totalPoints++] = {cx - y, cy - x};
}

// Midpoint Circle Drawing Algorithm to calculate points
void calculateMidpointCircle(int cx, int cy, int r) {
    totalPoints = 0;
    int x = 0;
    int y = r;
    int p = 1 - r;

    plotCirclePoints(cx, cy, x, y);

    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        plotCirclePoints(cx, cy, x, y);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !animating) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        ypos = HEIGHT - ypos; // Convert to OpenGL coords

        if (!waitingForRadius) {
            centerX = (int)xpos;
            centerY = (int)ypos;
            waitingForRadius = true;
            std::cout << "Center set at (" << centerX << ", " << centerY << ")\n";
        } else {
            int dx = (int)xpos - centerX;
            int dy = (int)ypos - centerY;
            radius = (int)std::round(std::sqrt(dx * dx + dy * dy));
            waitingForRadius = false;

            calculateMidpointCircle(centerX, centerY, radius);
            currentPoint = 0;
            animating = true;

            std::cout << "Radius set to " << radius << "\n";
            std::cout << "Animating circle with " << totalPoints << " points.\n";
        }
    }
}

GLFWwindow* initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Midpoint Circle Animation", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPointSize(2.0f);

    return window;
}

void render(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        glColor3f(1.0f, 1.0f, 1.0f);

        if (animating) {
            glBegin(GL_POINTS);
            for (int i = 0; i <= currentPoint && i < totalPoints; i++) {
                glVertex2i(circlePoints[i].x, circlePoints[i].y);
            }
            glEnd();

            if (currentPoint < totalPoints - 1) {
                currentPoint++;
            } else {
                animating = false;
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (animating) {
            glfwWaitEventsTimeout(0.01); // slow animation
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
