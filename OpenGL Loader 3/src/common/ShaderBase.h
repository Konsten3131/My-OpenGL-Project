#ifndef __ShaderBase_H__
#define __ShaderBase_H__
#include "glwrapper.h"

class ShaderBase
{
public:
    char* m_VertShaderString;
    char* m_FragShaderString;

    GLuint m_VertShader;
    GLuint m_FragShader;
    GLuint m_Program;

public:
    ShaderBase();
    virtual ~ShaderBase();

    void Cleanup();

    GLuint CompileShader(GLenum shaderType, const char* shaderstring);
    bool Init(const char* vertfilename, const char* fragfilename);
};

#endif //__ShaderBase_H__
