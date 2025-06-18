#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const int WIDTH = 800, HEIGHT = 600;

bool waitingForSecondClick = false;
bool animating = false;

float startX, startY, endX, endY;


int totalSteps = 0;
int currentStep = 0;
float xInc = 0.0f, yInc = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !animating) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        ypos = HEIGHT - ypos; 

        if (!waitingForSecondClick) {
            startX = (float)xpos;
            startY = (float)ypos;
            waitingForSecondClick = true;
            std::cout << "Start point: (" << startX << ", " << startY << ")\n";
        } else {
            endX = (float)xpos;
            endY = (float)ypos;
            waitingForSecondClick = false;

           
            float dx = endX - startX;
            float dy = endY - startY;
            totalSteps = (int)std::max(std::abs(dx), std::abs(dy));
            if (totalSteps == 0) totalSteps = 1; 

            xInc = dx / totalSteps;
            yInc = dy / totalSteps;

            currentStep = 0;
            animating = true;

            std::cout << "End point: (" << endX << ", " << endY << ")\n";
            std::cout << "Animating line with " << totalSteps << " steps.\n";
        }
    }
}

GLFWwindow* initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "DDA Line Animation", nullptr, nullptr);
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
            for (int i = 0; i <= currentStep; i++) {
                glVertex2f(startX + xInc * i, startY + yInc * i);
            }
            glEnd();

            if (currentStep < totalSteps) {
                currentStep++;
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
