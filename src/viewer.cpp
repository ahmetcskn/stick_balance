#include <cmath>
#include <cstdio>

#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include "cart_pole.hpp"

int main(int argc, char** argv) {
    if (!glfwInit()) {
        return 1;
    }

    glutInit(&argc, argv);

    GLFWwindow* win = glfwCreateWindow(960, 540, "Stick Balance", nullptr, nullptr);

    if (!win) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(win);

    CartPole simulation;

    double torque = 0.0;
    double force = 0.0;

    while (!glfwWindowShouldClose(win)) {
        const double dt = simulation.parameters().dt;

        // Input
        if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
            torque -= 4.0 * dt;
        }
        else if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            torque += 4.0 * dt;
        }
        else if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
            torque = 0.0;
        }

        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
            force = -10.0;
        }
        else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
            force = 10.0;
        }
        else {
            force = 0.0;
        }

        // Physics
        simulation.step(force, torque);

        const CartPoleState& state = simulation.state();
        const double L = simulation.parameters().length;

        // Pendulum position relative to the cart
        const float pendulum_x = static_cast<float>(L * std::sin(state.theta));
        const float pendulum_y = static_cast<float>(-L * std::cos(state.theta));

        const float pivot_x = static_cast<float>(state.x);
        const float pivot_y = 0.0f;

        //Viewport
        int w, h;
        glfwGetFramebufferSize(win, &w, &h);
        glViewport(0, 0, w, h);

        const float aspect = h > 0 ? static_cast<float>(w) / static_cast<float>(h) : 1.0f;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLoadIdentity();
        glOrtho(-5.0 * aspect, 5.0 * aspect, -5.0, 5.0, -1.0, 1.0);

        // Text
        char torque_text[32];
        char speed_text[32];

        snprintf(torque_text, sizeof(torque_text), "Torque: %.2f Nm", torque);
        snprintf(speed_text, sizeof(speed_text), "Speed: %.2f m/s", state.velocity);

        glColor3f(1.0f, 1.0f, 1.0f);

        glRasterPos2f(-4.5f * aspect, 4.2f);
        for (int i = 0; torque_text[i] != '\0'; ++i) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, torque_text[i]);
        }

        glRasterPos2f(-4.5f * aspect, 3.7f);
        for (int i = 0; speed_text[i] != '\0'; ++i) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, speed_text[i]);
        }

        //Pendulum
        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_LINES);
        glVertex2f(pivot_x, pivot_y);
        glVertex2f(pivot_x + pendulum_x, pivot_y + pendulum_y);
        glEnd();

        //x-axis
        glColor3f(0.2f, 0.2f, 0.2f);

        glBegin(GL_LINES);
        glVertex2f(-10.0f, 0.0f);
        glVertex2f(10.0f, 0.0f);
        glEnd();

        //Pendulum trajectory circle
        glColor3f(0.3f, 0.3f, 0.3f);

        glBegin(GL_LINE_LOOP);

        constexpr int N = 64;
        constexpr double PI = 3.14159265358979323846;

        for (int i = 0; i < N; ++i) {
            const double angle = 2.0 * PI * i / N;

            glVertex2f(
                pivot_x + static_cast<float>(L * std::cos(angle)),
                pivot_y + static_cast<float>(L * std::sin(angle))
            );
        }

        glEnd();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwDestroyWindow(win);
    glfwTerminate();

    return 0;
}
