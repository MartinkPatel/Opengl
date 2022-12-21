#include "Renderer.h"
#include <iostream>
using namespace std;





 void GLClearError() {

    while (glGetError() != GL_NO_ERROR);

}

 bool GLLogCall(const char* function, const char* file, int line) {


    while (GLenum error = glGetError()) {

        cout << "[ OpenGL ERROR] ( " << error << ")" << function << " " << file << ": " << line << endl;
        return false;
    }
    return true;
}