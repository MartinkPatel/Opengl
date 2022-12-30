#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
using namespace std;


struct  ShaderProgramSource
{
    std::string VertexSource;
   std:: string FragmentSource;
};

static ShaderProgramSource ParseShader(const string& Filepath) {

    ifstream stream(Filepath);
    string line;
    stringstream ss[2];

    enum class ShaderType {

        NONE = -1, VERTEX=0, FRAGMENT=1
    };
    ShaderType type;
    type = ShaderType::NONE;
    while (getline(stream, line)) {
    
        if (line.find("#shader") != string::npos)
        {
            if (line.find("vertex") != string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != string::npos)
                type = ShaderType::FRAGMENT;
        }
            
        else {
        
            ss[(int)type] << line << "\n";
        }



    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "FAILED COMPILED " <<(type==GL_VERTEX_SHADER ? "vertex ":"fragment ") << "SHADER:  ";
        cout << message << endl;
        glDeleteShader(id);
        return 0;
    }


    return id;
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader) {


    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK) {
        cout << "ERROR\n";
    }

    cout << glGetString(GL_VERSION) << endl;
    
        float positions[] = {
           -0.5f,  -0.5f,
            0.5f,   -0.5f,
            0.5f,   0.5f,
            -0.5f, 0.5f,

        };

        unsigned int indices[]{

            0,1,2,
            2,3,0

        };

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        VertexArray va;

        VertexBuffer vb(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push(GL_FLOAT,2);
        va.AddBuffer(vb,layout);
       /* glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);*/


        IndexBuffer ib(indices, 6);




        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

        /* cout << "VERTEX\n";
         cout << source.VertexSource << endl;
         cout << "FRAGMENT\n";
         cout << source.FragmentSource << endl;
          */
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

        glUseProgram(shader);

        glBindVertexArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        int location = glGetUniformLocation(shader, "u_color");
        ASSERT(location != -1);

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shader);
            glUniform4f(location, r, 0.3f, 0.8f, 1.0f);


            //glBindVertexArray(vao);

            va.Bind();
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            if (r < 0.0f)
                increment = 0.05f;

            r += increment;


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glDeleteProgram(shader);
    
    glfwTerminate();
    return 0;
}