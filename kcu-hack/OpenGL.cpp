#include "OpenGL.h"
#include <Windows.h>
#include <stdio.h>
#include <geometry.h>

OpenGL::OpenGL() {
    std::cout << "OpenGL initialized." << std::endl;
}

OpenGL::~OpenGL() {
    std::cout << "OpenGL cleaned up." << std::endl;
}

void OpenGL::SetupOrtho() {
    GLint viewport[4];
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, viewport[2], viewport[3]);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
}

void OpenGL::RestoreGL() {
    glPopMatrix();
    glPopAttrib();
}

void OpenGL::DrawLine(Line line, const GLubyte color[3]) {
    glColor3ub(color[0], color[1], color[2]);
    glBegin(GL_LINES);
    glVertex2f(line.a.x, line.a.y);
    glVertex2f(line.b.x, line.b.y);
    glEnd();
}

void OpenGL::DrawRect(Rect rect, const GLubyte color[3]) {
    glColor3ub(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
    glVertex2f(rect.x, rect.y);
    glVertex2f(rect.x + rect.width, rect.y);
    glVertex2f(rect.x + rect.width, rect.y + rect.height);
    glVertex2f(rect.x, rect.y + rect.height);
    glEnd();
}

void OpenGL::Outline(Rect rect, float lineWidth, const GLubyte color[3]) {
    glLineWidth(lineWidth);
    glBegin(GL_LINE_STRIP);
    glColor3ub(color[0], color[1], color[2]);
    glVertex2f(rect.x - 0.5f, rect.y - 0.5f);
    glVertex2f(rect.x + rect.width + 0.5f, rect.y - 0.5f);
    glVertex2f(rect.x + rect.width + 0.5f, rect.y + rect.height + 0.5f);
    glVertex2f(rect.x - 0.5f, rect.y + rect.height + 0.5f);
    glVertex2f(rect.x - 0.5f, rect.y - 0.5f);
    glEnd();
}