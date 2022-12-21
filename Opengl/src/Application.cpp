#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

static void GLClearError() {

    while (glGetError() != GL_NO_ERROR);

}

static void GLCheckError() {


    while (GLenum error = glGetError()) {
    
        cout << "[ OpenGL ERROR] ( " << error << " )\n";
    }

}

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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

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
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    
    
    
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    

   

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

   /* cout << "VERTEX\n";
    cout << source.VertexSource << endl;
    cout << "FRAGMENT\n";
    cout << source.FragmentSource << endl;
     */
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
   
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        GLClearError();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        GLCheckError();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

  glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}