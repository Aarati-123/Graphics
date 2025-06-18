#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const int WIDTH = 800, HEIGHT = 600;

bool waitingForRadii = false;
bool animating = false;

int centerX = 0, centerY = 0;
int rx = 0, ry = 0;

struct Point {
    int x, y;
};

const int MAX_POINTS = 4000;
Point ellipsePoints[MAX_POINTS];
int totalPoints = 0;
int currentPoint = 0;

// Plot all 4 symmetric points for the ellipse
void plotEllipsePoints(int cx, int cy, int x, int y) {
    if (totalPoints + 4 > MAX_POINTS) return;
    ellipsePoints[totalPoints++] = {cx + x, cy + y};
    ellipsePoints[totalPoints++] = {cx - x, cy + y};
    ellipsePoints[totalPoints++] = {cx + x, cy - y};
    ellipsePoints[totalPoints++] = {cx - x, cy - y};
}

// Midpoint Ellipse Drawing Algorithm to calculate points
void calculateMidpointEllipse(int cx, int cy, int rx, int ry) {
    totalPoints = 0;
    int x = 0;
    int y = ry;

    int rx2 = rx * rx;
    int ry2 = ry * ry;
    int twoRx2 = 2 * rx2;
    int twoRy2 = 2 * ry2;

    int px = 0;
    int py = twoRx2 * y;

    // Region 1
    int p1 = (int)(ry2 - (rx2 * ry) + (0.25 * rx2));
    plotEllipsePoints(cx, cy, x, y);

    while (px < py) {
        x++;
        px += twoRy2;
        if (p1 < 0) {
            p1 += ry2 + px;
        } else {
            y--;
            py -= twoRx2;
            p1 += ry2 + px - py;
        }
        plotEllipsePoints(cx, cy, x, y);
    }

    // Region 2
    int p2 = (int)(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
    while (y > 0) {
        y--;
        py -= twoRx2;
        if (p2 > 0) {
            p2 += rx2 - py;
        } else {
            x++;
            px += twoRy2;
            p2 += rx2 - py + px;
        }
        plotEllipsePoints(cx, cy, x, y);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !animating) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        ypos = HEIGHT - ypos;

        if (!waitingForRadii) {
            centerX = (int)xpos;
            centerY = (int)ypos;
            waitingForRadii = true;
            std::cout << "Center set at (" << centerX << ", " << centerY << ")\n";
        } else {
            int dx = (int)xpos - centerX;
            int dy = (int)ypos - centerY;
            rx = std::abs(dx);
            ry = std::abs(dy);
            waitingForRadii = false;

            calculateMidpointEllipse(centerX, centerY, rx, ry);
            currentPoint = 0;
            animating = true;

            std::cout << "Radii set to rx = " << rx << ", ry = " << ry << "\n";
            std::cout << "Animating ellipse with " << totalPoints << " points.\n";
        }
    }
}

GLFWwindow* initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Midpoint Ellipse Animation", nullptr, nullptr);
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
                glVertex2i(ellipsePoints[i].x, ellipsePoints[i].y);
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
            glfwWaitEventsTimeout(0.01);
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
