#ifndef OPENGL_H
#define OPENGL_H

#include <Windows.h>
#include <stdio.h>
#include <gl\GL.h>
#include <geometry.h>

class OpenGL {
public:
    OpenGL();   // Constructor
    ~OpenGL();  // Destructor

    // change to 2D projection
    void SetupOrtho();
    void RestoreGL();

    // draw on screen
    void DrawLine(Line line, const GLubyte color[3]);
    void DrawRect(Rect rect, const GLubyte color[3]);
    void Outline(Rect rect, float lineWidth, const GLubyte color[3]);


    // RGB value of colors
    const GLubyte BLACK[3] = { 0, 0, 0 };
    const GLubyte WHITE[3] = { 255, 255, 255 };
    const GLubyte GRAY[3] = { 128, 128, 128 };
    const GLubyte GRAYLIGHT[3] = { 211, 211, 211 };

    const GLubyte RED[3] = { 237, 46, 81 };
    const GLubyte REDLIGHT[3] = { 255, 0, 0 };
    const GLubyte GREEN[3] = { 0, 221, 0 };
    const GLubyte GREENLIGHT[3] = { 0, 255, 0 };
    const GLubyte BLUE[3] = { 0, 0, 255 };
    const GLubyte ORANGE[3] = { 255, 165, 0 };

};

#endif // OPENGL_H