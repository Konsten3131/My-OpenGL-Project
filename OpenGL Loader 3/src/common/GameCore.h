#ifndef __GameCore_H__
#define __GameCore_H__
#include "CommonHeader.h"

class GameCore;

extern GameCore* g_pGameCore;

struct VertexFormat
{
    Vector3 pos;
    Vector4 color;
    Vector2 uv;

    VertexFormat() {}
    VertexFormat(Vector3 newpos, Vector4 newcolor, Vector2 newuv)
    {
        pos = newpos;
        color = newcolor;
        uv = newuv;
    }
};

class GameCore
{
public:
    ShaderBase* m_pShaderTexture2D;
    ShaderBase* m_pShaderTexture3D;
    ShaderBase* m_pShaderColor;

    GLuint m_TextureGround;
    GLuint m_TextureBGFront;
    GLuint m_TextureBGRight;
    GLuint m_TextureBGTop;
    GLuint m_TextureBGPerspective;
    GLuint m_CubeTexture;

    float m_TimeInGame;

    Matrix m_MatrixPerspective;

    GLuint m_PyramidVBO;
    GLuint m_PyramidIBO;
    
    GLuint m_CubeVBO;
    GLuint m_CubeIBO;

    GLuint m_GroundVBO;
    GLuint m_GroundIBO;

    GLuint m_Quad2DVBO;
    GLuint m_Quad2DIBO;
    
    GLuint m_LoadedVBO;
    GLuint m_LoadedUvBuffer;

    Vector3 m_ObjectPos;
    Vector3 m_CameraPos;

    unsigned int m_WindowWidth;
    unsigned int m_WindowHeight;
    
    float m_ModelScale;
    float m_InitialModelAngleOld;
    float m_InitialModelAngleNew;
   
    
    int m_ViewNumber;
    std::vector< Vector3 > m_Vertices;
    float m_xAxisAngle;
    float m_yAxisAngle;
    float m_ZValueforZoom;
public:
    GameCore();
    virtual ~GameCore();

    GLuint LoadTexture(const char* filename);
    virtual void OneTimeInit();
    virtual void Tick(double TimePassed);

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
    virtual void OnDrawFrame();

    void DrawScene(Matrix* pCameraMatrix);

    void GeneratePyramidVerts();
    void GenerateCubeVerts();
    void GenerateGroundVerts();
    void GenerateQuad2DVerts();

    void DrawQuad2D(GLuint texturehandle);
    void DrawGround(Matrix* pMVP);
    void DrawPyramid(Matrix* pMVP);
    void Draw3DModel(Matrix* pMVP);

    void SwitchView();
    void RotateCameraAroundAxis(float angle);
    void ZoomZoom(float value);
};

#endif //__GameCore_H__
