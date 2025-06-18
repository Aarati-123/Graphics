#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <cmath>

const int WIDTH = 800, HEIGHT = 600;
const float gravity = -0.0005f, restitution = 0.8f, deltaTime = 0.016f;

struct Ball {
    float x, y, vx, vy, radius;
    bool launched;
    Ball(float px, float py) : x(px), y(py), vx(0.0f), vy(0.0f), radius(0.05f), launched(false) {}
};

std::vector<Ball> balls;
unsigned int VAO, VBO;
bool firstClick = true;
glm::vec2 initialClick, launchClick;

const char* vertexShaderSource = "#version 330 core\n"
    "layout(location = 0) in vec2 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "}";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0, 0.5, 0.0, 1.0);\n"
    "}";

unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
    }
    return shader;
}

unsigned int createShaderProgram() {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void setupOpenGL(GLFWwindow*& window) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        exit(-1);
    }
    window = glfwCreateWindow(WIDTH, HEIGHT, "Bouncing Ball Simulation", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        exit(-1);
    }
    glViewport(0, 0, WIDTH, HEIGHT);
}

void updateBalls() {
    for (auto& ball : balls) {
        if (ball.launched) {
            ball.vy += gravity * deltaTime;
            ball.x += ball.vx * deltaTime;
            ball.y += ball.vy * deltaTime;

            // Bottom (floor)
            if (ball.y - ball.radius <= -1.0f) {
                ball.y = -1.0f + ball.radius;
                ball.vy *= -restitution;
            }

            // Top wall
            if (ball.y + ball.radius >= 1.0f) {
                ball.y = 1.0f - ball.radius;
                ball.vy *= -restitution;
            }

            // Left wall
            if (ball.x - ball.radius <= -1.0f) {
                ball.x = -1.0f + ball.radius;
                ball.vx *= -restitution;
            }

            // Right wall
            if (ball.x + ball.radius >= 1.0f) {
                ball.x = 1.0f - ball.radius;
                ball.vx *= -restitution;
            }
        }
    }
}

void updateBallVertices() {
    std::vector<float> circleVertices;
    const int segments = 36;
    for (const auto& ball : balls) {
        circleVertices.push_back(ball.x);
        circleVertices.push_back(ball.y);
        for (int i = 0; i <= segments; i++) {
            float angle = glm::radians((float)i * 360.0f / segments);
            circleVertices.push_back(ball.x + ball.radius * cos(angle));
            circleVertices.push_back(ball.y + ball.radius * sin(angle));
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_DYNAMIC_DRAW);
}

void renderBalls(unsigned int shaderProgram) {
    if (balls.empty()) return;

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    const int verticesPerBall = 1 + 36 + 1;
    int offset = 0;
    for (size_t i = 0; i < balls.size(); ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, offset, verticesPerBall);
        offset += verticesPerBall;
    }
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        float px = (xpos / WIDTH) * 2.0f - 1.0f;
        float py = 1.0f - (ypos / HEIGHT) * 2.0f;

        if (firstClick) {
            balls.emplace_back(px, py);
            initialClick = glm::vec2(px, py);
            std::cout << "Ball created at: (" << px << ", " << py << ")\n";
            firstClick = false;
        } else {
            launchClick = glm::vec2(px, py);
            Ball& ball = balls.back();
            ball.vx = (launchClick.x - initialClick.x) * 1.5f;
            ball.vy = (launchClick.y - initialClick.y) * 1.5f;
            ball.launched = true;
            std::cout << "Ball launched with velocity: (" << ball.vx << ", " << ball.vy << ")\n";
            firstClick = true;
        }
    }
}

int main() {
    GLFWwindow* window;
    setupOpenGL(window);
    glfwSetMouseButtonCallback(window, mouseCallback);

    unsigned int shaderProgram = createShaderProgram();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        updateBalls();
        updateBallVertices();
        renderBalls(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
