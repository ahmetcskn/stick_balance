#include <cmath>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <cstdio>

int main(int argc, char** argv) {

    if (!glfwInit()) return 1;

    glutInit(&argc, argv);

    GLFWwindow* win = glfwCreateWindow(960, 540, "stick", nullptr, nullptr);
    if (!win) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(win);

    const double L = 2.5;
    const double g = 9.81;
    const double dt = 0.01;
    const double m = 0.6;
    const double m_cart = 2;
    double theta = 0.8;
    double omega = 0.0;
    double tork = 0.0;
    double F = 0.0;
    double V = 0.0;
    double X = 0.0;

    
    float pivot_x = (float)X;
    float pivot_y = 0.0f;

    while (!glfwWindowShouldClose(win)) {
        if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
            tork -= 4.0 * dt;
        } else if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
             tork += 4.0 * dt; // sol ve sag tuslari tork yonu ekler.:
        } else  if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
            tork = 0.0; // amgular
            omega = 0.0; // angular speed 
        }

        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
            F = -7.0;
        } else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
            F = +7.0;
        } else { 
            F = 0.0;
        }

        if (X > 6.0) {X = 6.0; V = ((-V) + (V/4));}
        if (X < -6.0) {X = -6.0; V = ((-V) + (V/4));}

// iki bilinmeyenli birbirine bagli iki denklemin 2x2 matris cozumu.j
        double A11 = L;
        double A12 = std::cos(theta);
        double A21 = m * L * std::cos(theta);
        double A22 = m_cart + m;
        double b1 = -g * std::sin(theta) + tork / (m*L);
        double b2 = F + m * L * std::sin(theta) * omega * omega;
        double det = A11 * A22 - A12 * A21;
        double theta_ddot = (b1 * A22 - b2 * A12) / det;
        double x_ddot = ( A11 * b2 - b1* A21) / det;

        omega += theta_ddot * dt;
        theta += omega * dt;
        V += x_ddot * dt;
        X += V * dt;
        float x = (float)(L * std::sin(theta));
        float y = (float)(-L * std::cos(theta));
        float pivot_x = (float)X;
        float pivot_y = 0.0f;

        int w, h;
        glfwGetFramebufferSize(win, &w, &h);
        glViewport(0, 0, w, h);

        float aspect = (h > 0) ? (float)w / (float)h : 1.f;

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLoadIdentity();
        glOrtho(-5.0 * aspect, 5.0 * aspect, -5.0, 5.0, -1.0, 1.0);

        //texts
        char tork_metni[32];
        char speed_metni[32];
        snprintf(tork_metni, sizeof(tork_metni), "Tork: %.2f Nm", tork);
        snprintf(speed_metni, sizeof(speed_metni), "Speed: %.2f m/s", V);

        glColor3f(1.f, 1.f, 1.f);

        glRasterPos2f(-4.5f * aspect, 4.2f);
        for (int i = 0; tork_metni[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, tork_metni[i]);
        }

        glRasterPos2f(-4.5f * aspect, 3.7f);
        for (int i = 0; speed_metni[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, speed_metni[i]);
        }
        

        //line
        glBegin(GL_LINES);
        glColor3f(1.f, 1.f, 1.f);
        glVertex2f(pivot_x, pivot_y);
        glVertex2f(pivot_x + x, pivot_y + y);
        glEnd();

        //circle
        glBegin(GL_LINE_LOOP);
        glColor3f(0.3f, 0.3f, 0.3f);
        const int N = 64;
        for (int i = 0; i < N; ++i) {
            float u = (float)(2.0 * 3.14159265 * i / N);
            glVertex2f(pivot_x + (float)(L * std::cos(u)), 
                    pivot_y + (float)(L * std::sin(u)));
        }
        glEnd();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
