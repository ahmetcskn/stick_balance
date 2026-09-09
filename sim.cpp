#include <cmath>
#include <GLFW/glfw3.h>

int main() {
    if (!glfwInit()) return 1;
    GLFWwindow* win = glfwCreateWindow(960, 540, "stick", nullptr, nullptr);
    if (!win) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(win);

    const double L = 2.5;
    const double g = 9.81;
    const double dt = 0.01;
    const double m = 1.0;
    double theta = 0.8;
    double omega = 0.0;
    double tork = 0.0;

    const float pivot_x = 0.f;
    const float pivot_y = 0.f;

    while (!glfwWindowShouldClose(win)) {
        if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) tork = -4.0;
        if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) tork = +4.0; // sol ve sag tuslari tork yonu ekler.:
        if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) tork = 0.0;
        double theta_ddot = -(g / L) * std::sin(theta) + tork / (m *L *L);
        omega += theta_ddot * dt;
        theta += omega * dt;

        float x = (float)(L * std::sin(theta));
        float y = (float)(-L * std::cos(theta));

        int w, h;
        glfwGetFramebufferSize(win, &w, &h);
        glViewport(0, 0, w, h);

        float aspect = (h > 0) ? (float)w / (float)h : 1.f;

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLoadIdentity();
        glOrtho(-5.0 * aspect, 5.0 * aspect, -5.0, 5.0, -1.0, 1.0);

        glBegin(GL_LINES);
        glColor3f(1.f, 1.f, 1.f);
        glVertex2f(pivot_x, pivot_y);
        glVertex2f(pivot_x + x, pivot_y + y);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glColor3f(0.3f, 0.3f, 0.3f);
        const int N = 64;
        for (int i = 0; i < N; ++i) {
            float a = (float)(2.0 * 3.14159265 * i / N);
            glVertex2f(pivot_x + (float)(L * std::cos(a)), pivot_y + (float)(L * std::sin(a)));
        }
        glEnd();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
