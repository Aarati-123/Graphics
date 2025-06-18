#include <GLFW/glfw3.h>
#include <iostream>

void drawRectangle(float x, float y, float w, float h) {
    glBegin(GL_TRIANGLES);
    
   
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);

  
    glVertex2f(x, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);

    glEnd();
}


void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}


void drawLetterA(float offsetX) {
    float width = 80;  
    float height = 150; 
    float thickness = 20;

    
    drawTriangle(offsetX, 300, offsetX + thickness, 300, offsetX + 40, 150);
    
    
    drawTriangle(offsetX + width - thickness, 300, offsetX + width, 300, offsetX + 40, 150);

    drawRectangle(offsetX + 20, 230, 40, thickness);
}


void drawLetterR(float offsetX) {
    float width = 80;  
    float height = 150;  
    float thickness = 20;  


    drawRectangle(offsetX, 150, thickness, height);


    drawRectangle(offsetX + thickness, 150, 50, thickness);


    drawRectangle(offsetX + thickness, 230, 40, thickness);


    drawRectangle(offsetX + 50, 150, thickness, 100);

    
    drawTriangle(offsetX + thickness, 230, offsetX + width, 300, offsetX + width - thickness, 300);
}


void drawLetterT(float offsetX) {
    float width = 80;  
    float height = 150;  
    float thickness = 20;  

    
    drawRectangle(offsetX, 150, width, thickness);

    
    drawRectangle(offsetX + width / 2 - thickness / 2, 150, thickness, height);
}


void drawLetterI(float offsetX) {
    float width = 20;  
    float height = 150;  

    
    drawRectangle(offsetX, 150, width, height);
}


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(900, 400, "Name: AARATI ", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 900, 400, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.6f, 0.0f, 0.6f); // Purple color

        float startX = 50;  
        float spacing = 120;

    
        drawLetterA(startX + spacing * 0);
        drawLetterA(startX + spacing * 1);
        drawLetterR(startX + spacing * 2);
        drawLetterA(startX + spacing * 3);
        drawLetterT(startX + spacing * 4);
        drawLetterI(startX + spacing * 5);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}