#include <GL/gl.h>
#include <cstdio>
#include <cmath>
#include <GLFW/glfw3.h>

int main() {

    if (!glfwInit()) return 1;
    GLFWwindow* win = glfwCreateWindow(960, 540, "stick", nullptr, nullptr);
    if (!win) {
        glfwTerminate(); 
        return 1;
    }
    glfwMakeContextCurrent(win); // erkana cizim yapar.


    const double L = 0.5;
    const double g = 9.81;
    const double dt = 0.01;
    double theta = 0.4; // rad, kucuk aci (0 = asagi)
    double omega = 0.0; // saniyede kac radyan degisiyor

    while (!glfwWindowShouldClose(win)) {
        //fizik
        double theta_ddot = -(g/L) * std::sin(theta);
        omega += theta_ddot * dt;
        theta += omega * dt;
        

        //cizim
        glClearColor(0, 0, 0, 1); // r, g, b, a . a burada opakligi gosterir.
        glClear(GL_COLOR_BUFFER_BIT); // bu da rengi uygular.

        float x = (float)(L*std::sin(theta));
        float y = (float)(-L*std::cos(theta)); // 0 asagi
        
        int h, w;
        glfwGetFramebufferSize(win , &w, &h);
        glViewport(0,0,w,h);  // ekran oranti duzeltmesi.

        float aspect = (h>0) ? (float)w / (float)h : 1.f;
        glLoadIdentity();
        glOrtho(-5.0*aspect, 5.0*aspect, -5.0, 5.0, -1.0, 1.0); //2D dunya

        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex2f(0.f, 3.f); //pivot
        glVertex2f(x, y); //uc
        glEnd();
        
        //daire
        glBegin(GL_LINE_LOOP);
        glColor3f(0.3f,0.3f,0.3f);
        const int N = 64;
        for (int i=0; i<N; i++) {
            
            float a = (float)(2.0 * 3.14159265 * i / N);
            float cx = x + (float)(L * std::cos(a));
            float cy = y + (float)(L * std::sin(a));
            glVertex2f(cx, cy);
        }
        glEnd();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
