#pragma once

#include <GLEW/glew.h>

/*
I know I'm quite late to the game, but here's my take on the macros. I use 2 - one for functions returning void, one for everything else.
This is on macOS using C++11, but is easily adapted to other compilers.
#ifdef DEBUG
   #define GLCallV( x ) \
     GLClearErrors(); \
     x; \
     if ( GLCheckErrors( #x, __FILE__, __LINE__) ) __builtin_debugtrap();
   #define GLCall( x ) [&]() { \
     GLClearErrors(); \
     auto retVal = x; \
     if ( GLCheckErrors( #x, __FILE__, __LINE__) ) __builtin_debugtrap(); \
     return retVal; \
   }()
#else
   #define GLCallV( x ) x
   #define GLCall( x ) x
#endif
Basically what I'm doing in GLCall() is using a lambda, capturing the return using "auto", and then returning it after the check. This way you don't wrap the whole assignment which feels really strange to me.
So instead of this:
   GLCall( GLuint program = glCreateProgram() );
You do this which feels more natural:
   GLuint program = GLCall( glCreateProgram() );
*/

/*
If anybody is interested in glDebugMessageCallback:
Take a look into docs.gl and the glfw documentation. Or if you don't have time:

* to load the function pointer for glDebugMessageCallback and 'enable' debugging in your current glfw context
put the following before glfwCreateWindow: *
glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

* as the name suggests, glDebugMessageCallback takes a callback function, here is one posibility
in CPP, the signature might look way different, please consult google. Also, on windows, there are some
issues where you have to mark sth. in the signature with __std or something  *
void GLAPIENTRY errorOccurredGL(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
        "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
    printf("%s\n", message);
    exit(-1); // shut down the program gracefully (it does cleanup stuff too)
    // without exit(), OpenGL will constantly print the error message... make sure to kill your program.
}

// Now, just befor entering the main loop, put:
glEnable(GL_DEBUG_OUTPUT);
glDebugMessageCallback(errorOccurredGL, NULL);
*/



#ifdef _DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif // _DEBUG


void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);