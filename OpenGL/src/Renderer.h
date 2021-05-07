#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG
    #define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//assertLogCall(__FUNCTION__, __FILE__, __LINE__)    

/*
inline static void assertLogCall(const char* function, const char* file, int line)
{
    assert(GLLogCall(function, file, line));
}
*/
#else
    #define GLCall(x)x
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


