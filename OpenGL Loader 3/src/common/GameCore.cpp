#include "CommonHeader.h"
#include "ResourceUtils.h"

GameCore* g_pGameCore = 0;

GameCore::GameCore()
{
    g_pGameCore = this;

    m_TimeInGame = 0;

    m_pShaderTexture2D = 0;
    m_pShaderTexture3D = 0;
    m_pShaderColor = 0;

    m_TextureGround = 0;
    m_TextureBGFront = 0;
    m_TextureBGRight = 0;
    m_TextureBGTop = 0;
    m_TextureBGPerspective = 0;
    m_LoadedUvBuffer = 0;
    m_CubeTexture = 0;

    m_PyramidVBO = 0;
    m_PyramidIBO = 0;
    
    m_CubeVBO = 0;
    m_CubeIBO = 0;

    m_GroundVBO = 0;
    m_GroundIBO = 0;

    m_Quad2DVBO = 0;
    m_Quad2DIBO = 0;
    
    m_LoadedVBO = 0;

    m_ObjectPos.Set( 0, 0, 0 );
    m_CameraPos.Set( 0, 0, 0 );
    
    m_ViewNumber = 1;
    
    m_ModelScale = 0;
    m_Vertices.clear();
    
    m_InitialModelAngleOld = m_InitialModelAngleNew = 45;
    getCameraRotCoords(m_CameraPos,m_InitialModelAngleOld, 3);
   
    m_xAxisAngle = degrees2Radians(25.0f);
    m_yAxisAngle = degrees2Radians(180.0f);
    
    m_ZValueforZoom = 4;
}

GameCore::~GameCore()
{
    delete m_pShaderTexture2D;
    delete m_pShaderTexture3D;
    delete m_pShaderColor;

    glDeleteTextures( 1, &m_TextureGround );
    glDeleteTextures( 1, &m_TextureBGFront );
    glDeleteTextures( 1, &m_TextureBGRight );
    glDeleteTextures( 1, &m_TextureBGTop );
    glDeleteTextures( 1, &m_TextureBGPerspective );
    
    glDeleteTextures( 1, &m_CubeTexture );

    glDeleteBuffers( 1, &m_PyramidVBO );
    glDeleteBuffers( 1, &m_PyramidIBO );

    glDeleteBuffers( 1, &m_GroundVBO );
    glDeleteBuffers( 1, &m_GroundIBO );

    glDeleteBuffers( 1, &m_Quad2DVBO );
    glDeleteBuffers( 1, &m_Quad2DIBO );
    
    glDeleteBuffers( 1, &m_LoadedVBO );
    glDeleteBuffers( 1, &m_LoadedUvBuffer );
    
}

// load a texture from disk
GLuint GameCore::LoadTexture(const char* filename)
{
    unsigned char* pngbuffer;
    unsigned int width, height;
    int error = lodepng_decode32_file( &pngbuffer, &width, &height, filename );
    assert( error == 0 );

    GLuint texhandle = 0;
    glGenTextures( 1, &texhandle );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texhandle );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pngbuffer );
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    delete( pngbuffer );

    return texhandle;
}

void GameCore::OneTimeInit()
{
    // Enable depthmap writing/reading
    glDepthMask( GL_TRUE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );

    // setup culling of clockwise faces.
    glFrontFace( GL_CW );
    
// code below enable culling!!! This saves gpu's resoucres because it does not have to draw outside and inside surface of object at the same time
    
//this will disable drawing of insides of object-------------------------
    
    //glCullFace( GL_BACK );
    //glEnable( GL_CULL_FACE );
    
//-----------------------------------------------------------------------
    
//this will disable drawing of outside of object-------------------------

    //glCullFace(GL_FRONT);
    //glEnable( GL_CULL_FACE );

//-----------------------------------------------------------------------
    
    
    // load our shaders
    {
        m_pShaderTexture2D = new ShaderBase();
        m_pShaderTexture2D->Init( "texture2d", "texture2d" );

        m_pShaderTexture3D = new ShaderBase();
        m_pShaderTexture3D->Init( "texture3d", "texture3d" );

        m_pShaderColor = new ShaderBase();
        m_pShaderColor->Init( "color", "color" );
    }

    const char* pngPath = ResourceUtils::getPathForSpecifiedResource("podium", "png");
    const char* pngPath2 = ResourceUtils::getPathForSpecifiedResource("QuadFront", "png");
    const char* pngPath3 = ResourceUtils::getPathForSpecifiedResource("QuadRight", "png");
    const char* pngPath4 = ResourceUtils::getPathForSpecifiedResource("QuadTop", "png");
    const char* pngPath5 = ResourceUtils::getPathForSpecifiedResource("QuadPerspective", "png");
    
   // const char* pngPath6 = ResourceUtils::getPathForSpecifiedResource("uvtemplate", "png");
    const char* pngPath6 = ResourceUtils::getPathForSpecifiedResource("invasteranko_d", "png");
     //const char* pngPath6 = ResourceUtils::getPathForSpecifiedResource("sonic2", "png");
   // const char* pngPath7 = ResourceUtils::getPathForSpecifiedResource("sonicShoes", "png");
    
    // load our textures
    m_TextureGround = LoadTexture( pngPath );
    m_TextureBGFront = LoadTexture( pngPath2 );
    m_TextureBGRight = LoadTexture( pngPath3 );
    m_TextureBGTop = LoadTexture( pngPath4 );
    m_TextureBGPerspective = LoadTexture( pngPath5 );
    m_CubeTexture = LoadTexture( pngPath6 );
    

   // GeneratePyramidVerts();
   // GenerateCubeVerts();
    GenerateGroundVerts();
    GenerateQuad2DVerts();
    
    //const char* pngPath0 = ResourceUtils::getPathForSpecifiedResource("cube", "obj");
    const char* pngPath0 = ResourceUtils::getPathForSpecifiedResource("ster2", "obj");
   // const char* pngPath0 = ResourceUtils::getPathForSpecifiedResource("kostasBox", "obj");
     //const char* pngPath0 = ResourceUtils::getPathForSpecifiedResource("sonic2", "obj");
    
    std::vector< Vector2 > uvs;
    std::vector< Vector3 > normals; // Won't be used at the moment.
    bool res = loadOBJ(pngPath0, m_Vertices, uvs, normals);
   
    Vector3 maXModelDimension = Vector3(0,0,0);
    findModelsDimension(m_Vertices,maXModelDimension);
    m_ModelScale = 1.3f/maXModelDimension.y;
   
    glGenBuffers( 1, &m_LoadedVBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_LoadedVBO );
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vector3), &m_Vertices[0], GL_STATIC_DRAW);
    
          
    glGenBuffers(1, &m_LoadedUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_LoadedUvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2), &uvs[0], GL_STATIC_DRAW);
    
    
}

void GameCore::GeneratePyramidVerts()
{
    static float radius = 0.25f;
    static Vector3 verttop( 0, radius * 1.5f, 0 );
    static Vector3 vert000( radius * cos(  0.0f*PI/180), 0, radius * sin(  0.0f*PI/180) );
    static Vector3 vert120( radius * cos(120.0f*PI/180), 0, radius * sin(120.0f*PI/180) );
    static Vector3 vert240( radius * cos(240.0f*PI/180), 0, radius * sin(240.0f*PI/180) );

    VertexFormat VertAttribs[12];
    
    VertAttribs[ 0] = VertexFormat( verttop,    Vector4(0.5f, 0.0f, 0.0f, 1),  Vector2(0.5f, 1.0f) ); // for uv x and y positions of indecies
    VertAttribs[ 1] = VertexFormat( vert000,    Vector4(0.5f, 0.0f, 0.0f, 1),  Vector2(0.0f, 0.0f) );
    VertAttribs[ 2] = VertexFormat( vert120,    Vector4(0.5f, 0.0f, 0.0f, 1),  Vector2(1.0f, 0.0f) );

    VertAttribs[ 3] = VertexFormat( verttop,    Vector4(0.0f, 0.5f, 0.0f, 1),  Vector2(0.5f, 1.0f) );
    VertAttribs[ 4] = VertexFormat( vert120,    Vector4(0.0f, 0.5f, 0.0f, 1),  Vector2(0.0f, 0.0f) );
    VertAttribs[ 5] = VertexFormat( vert240,    Vector4(0.0f, 0.5f, 0.0f, 1),  Vector2(1.0f, 0.0f) );

    VertAttribs[ 6] = VertexFormat( verttop,    Vector4(0.0f, 0.0f, 1.0f, 1),  Vector2(0.5f, 1.0f) );
    VertAttribs[ 7] = VertexFormat( vert240,    Vector4(0.0f, 0.0f, 1.0f, 1),  Vector2(0.0f, 0.0f) );
    VertAttribs[ 8] = VertexFormat( vert000,    Vector4(0.0f, 0.0f, 1.0f, 1),  Vector2(1.0f, 0.0f) );

    VertAttribs[ 9] = VertexFormat( vert000,    Vector4(1.0f, 0.5f, 1.0f, 1),  Vector2(0.5f, 1.0f) );
    VertAttribs[10] = VertexFormat( vert240,    Vector4(1.0f, 0.5f, 1.0f, 1),  Vector2(0.0f, 0.0f) );
    VertAttribs[11] = VertexFormat( vert120,    Vector4(1.0f, 0.5f, 1.0f, 1),  Vector2(1.0f, 0.0f) );

    unsigned short Indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    glGenBuffers( 1, &m_PyramidVBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_PyramidVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat)*12, VertAttribs, GL_STATIC_DRAW );

    glGenBuffers( 1, &m_PyramidIBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_PyramidIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*12, Indices, GL_STATIC_DRAW );
}

void GameCore::GenerateCubeVerts()
{
    static Vector3 frontBottomLeft( -1.0f, -1.0f, -1.0f);
    static Vector3 frontBottomRight( 1.0f, -1.0f, -1.0f);
    
    static Vector3 frontTopLeft( -1.0f, 1.0f, -1.0f);
    static Vector3 frontTopRight( 1.0f, 1.0f, -1.0f);
    
    static Vector3 backBottomRight( 1.0f, -1.0f, 1.0f);
    static Vector3 backTopRight( 1.0f, 1.0f, 1.0f);
    
    static Vector3 backBottomLeft( -1.0f, -1.0f, 1.0f);
    static Vector3 backTopLeft( -1.0f, 1.0f, 1.0f);
   
    
    VertexFormat VertAttribs[24];
    //front
    VertAttribs[0] = VertexFormat( frontTopLeft,    Vector4(0.5f, 0.0f, 0.0f, 1),  Vector2(0.0f, 1.0f) );
    VertAttribs[1] = VertexFormat( frontBottomLeft, Vector4(0.5f, 0.0f, 0.0f, 1),  Vector2(0.0f, 0.0f) );
    VertAttribs[2] = VertexFormat( frontBottomRight,Vector4(0.5f, 0.0f, 0.0f, 1),  Vector2(1.0f, 0.0f) );
    VertAttribs[3] = VertexFormat( frontTopRight,   Vector4(0.5f, 0.0f, 0.0f, 1),  Vector2(1.0f, 1.0f) );
    
    //right
    VertAttribs[4] = VertexFormat( frontBottomRight,Vector4(0.0f, 0.5f, 0.0f, 1),  Vector2(1.0f, 0.0f) );
    VertAttribs[5] = VertexFormat( frontTopRight,   Vector4(0.0f, 0.5f, 0.0f, 1),  Vector2(1.0f, 1.0f) );
    VertAttribs[6] = VertexFormat( backBottomRight,Vector4(0.0f, 0.5f, 0.0f, 1),  Vector2(1.0f, 0.0f) );
    VertAttribs[7] = VertexFormat( backTopRight,   Vector4(0.0f, 0.5f, 0.0f, 1),  Vector2(1.0f, 1.0f) );
    
    //left
    VertAttribs[8] = VertexFormat( frontTopLeft,    Vector4(0.0f, 0.0f, 0.5f, 1),  Vector2(0.0f, 1.0f) );
    VertAttribs[9] = VertexFormat( frontBottomLeft, Vector4(0.0f, 0.0f, 0.5f, 1),  Vector2(0.0f, 0.0f) );
    VertAttribs[10] = VertexFormat( backBottomLeft,    Vector4(0.0f, 0.0f, 0.5f, 1),  Vector2(0.0f, 1.0f) );
    VertAttribs[11] = VertexFormat( backTopLeft, Vector4(0.0f, 0.0f, 0.5f, 1),  Vector2(0.0f, 0.0f) );
    
    //back
    VertAttribs[12] = VertexFormat( backBottomRight,Vector4(0.0f, 0.5f, 0.5f, 1),  Vector2(1.0f, 0.0f) );
    VertAttribs[13] = VertexFormat( backTopRight,   Vector4(0.0f, 0.5f, 0.5f, 1),  Vector2(1.0f, 1.0f) );
    VertAttribs[14] = VertexFormat( backBottomLeft,Vector4(0.0f, 0.5f, 0.5f, 1),  Vector2(0.0f, 1.0f) );
    VertAttribs[15] = VertexFormat( backTopLeft,   Vector4(0.0f, 0.5f, 0.5f, 1),  Vector2(0.0f, 0.0f) );
    
    //top
    VertAttribs[16] = VertexFormat( backTopLeft,   Vector4(0.5f, 0.5f, 0.0f, 1),  Vector2(1.0f, 1.0f) );
    VertAttribs[17] = VertexFormat( frontTopLeft,    Vector4(0.5f, 0.5f, 0.0f, 1),  Vector2(0.0f, 1.0f) );
    VertAttribs[18] = VertexFormat( frontTopRight,   Vector4(0.5f, 0.5f, 0.0f, 1),  Vector2(1.0f, 1.0f) );
    VertAttribs[19] = VertexFormat( backTopRight,   Vector4(0.5f, 0.5f, 0.0f, 1),  Vector2(1.0f, 1.0f) );
    
    //bottom
     VertAttribs[20] = VertexFormat( backBottomLeft,    Vector4(0.5f, 0.5f, 0.5f, 1),  Vector2(0.0f, 1.0f) );
     VertAttribs[21] = VertexFormat( frontBottomLeft, Vector4(0.5f, 0.5f, 0.5f, 1),  Vector2(0.0f, 0.0f) );
     VertAttribs[22] = VertexFormat( frontBottomRight,Vector4(0.5f, 0.5f, 0.5f, 1),  Vector2(1.0f, 0.0f) );
     VertAttribs[23] = VertexFormat( backBottomRight,Vector4(0.5f, 0.5f, 0.5f, 1),  Vector2(1.0f, 0.0f) );
    



    
    //all triangles are drawn counter clock wise
    
     unsigned short Indices[] = { 0,1,2, 0,2,3,   5,4,6, 5,6,7,   11,10,9, 11,9,8,   13,12,14, 13,14,15,   16,17,18, 16,18,19,    21,20,23, 21,23,22 };
    
    glGenBuffers( 1, &m_CubeVBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_CubeVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat) * 24, VertAttribs, GL_STATIC_DRAW );
    
    glGenBuffers( 1, &m_CubeIBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_CubeIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 36, Indices, GL_STATIC_DRAW );
    
    
}

void GameCore::GenerateGroundVerts()
{
    VertexFormat VertAttribs[4];
   // VertAttribs[0] = VertexFormat( Vector3(-2, 0, 2),    Vector4(1, 1, 1, 1),  Vector2(0, 0) ); // top left
   // VertAttribs[1] = VertexFormat( Vector3( 2, 0, 2),    Vector4(1, 1, 1, 1),  Vector2(1, 0) ); // top right
   // VertAttribs[2] = VertexFormat( Vector3(-2, 0,-2),    Vector4(1, 1, 1, 1),  Vector2(0, 1) ); // bottom left
    //VertAttribs[3] = VertexFormat( Vector3( 2, 0,-2),    Vector4(1, 1, 1, 1),  Vector2(1, 1) ); // bottom right
    
    VertAttribs[0] = VertexFormat( Vector3(-2, 0, 2),    Vector4(1, 1, 1, 1),  Vector2(0, 1) ); // top left
    VertAttribs[1] = VertexFormat( Vector3( 2, 0, 2),    Vector4(1, 1, 1, 1),  Vector2(1, 1) ); // top right
    VertAttribs[2] = VertexFormat( Vector3(-2, 0,-2),    Vector4(1, 1, 1, 1),  Vector2(0, 0) ); // bottom left
    VertAttribs[3] = VertexFormat( Vector3( 2, 0,-2),    Vector4(1, 1, 1, 1),  Vector2(1, 0) ); // bottom right
    
    //unsigned short Indices[] = { 0,2,3, 0,3,1 };//counter clock wise winding
     unsigned short Indices[] = { 3,2,0, 1,3,0 };//clock wise winding

    glGenBuffers( 1, &m_GroundVBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_GroundVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat)*4, VertAttribs, GL_STATIC_DRAW );

    glGenBuffers( 1, &m_GroundIBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_GroundIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*6, Indices, GL_STATIC_DRAW );
}

void GameCore::GenerateQuad2DVerts()
{
    VertexFormat VertAttribs[4];
    VertAttribs[0] = VertexFormat( Vector3(-1,  1, 0),    Vector4(1, 1, 1, 1),  Vector2(0, 0) ); // top left
    VertAttribs[1] = VertexFormat( Vector3( 1,  1, 0),    Vector4(1, 1, 1, 1),  Vector2(1, 0) ); // top right
    VertAttribs[2] = VertexFormat( Vector3(-1, -1, 0),    Vector4(1, 1, 1, 1),  Vector2(0, 1) ); // bottom left
    VertAttribs[3] = VertexFormat( Vector3( 1, -1, 0),    Vector4(1, 1, 1, 1),  Vector2(1, 1) ); // bottom right

    //unsigned short Indices[] = { 0,2,3, 0,3,1 };//counter clock wise winding
    unsigned short Indices[] = { 3,2,0, 1,3,0 };//clock wise winding

    glGenBuffers( 1, &m_Quad2DVBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_Quad2DVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat)*4, VertAttribs, GL_STATIC_DRAW );

    glGenBuffers( 1, &m_Quad2DIBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Quad2DIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*6, Indices, GL_STATIC_DRAW );
}

//extern bool g_KeyStates[256];

void GameCore::Tick(double TimePassed)
{
    //if( g_KeyStates['P'] == true )
        m_TimeInGame += (float)TimePassed;

  //  float speed = 1;

    //if( g_KeyStates['R'] == true )
    //    m_CameraPos.y += (float)TimePassed * speed;

   // if( g_KeyStates['F'] == true )
      //  m_CameraPos.y -= (float)TimePassed * speed;

    //if( g_KeyStates['Q'] == true )
       // m_CameraPos.x -= (float)TimePassed * speed;

    //if( g_KeyStates['E'] == true )
       // m_CameraPos.x += (float)TimePassed * speed;

   // if( g_KeyStates['Z'] == true )
        //m_CameraPos.z -= (float)TimePassed * speed;

    //if( g_KeyStates['X'] == true )
        //m_CameraPos.z += (float)TimePassed * speed;

    //if( g_KeyStates['W'] == true )
       // m_ObjectPos.z += (float)TimePassed * speed;
    //if( g_KeyStates['S'] == true )
      //  m_ObjectPos.z -= (float)TimePassed * speed;
   // if( g_KeyStates['A'] == true )
      //  m_ObjectPos.x -= (float)TimePassed * speed;
   // if( g_KeyStates['D'] == true )
     //   m_ObjectPos.x += (float)TimePassed * speed;
}

void GameCore::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    glViewport( 0, 0, width, height );

    m_WindowWidth = width;
    m_WindowHeight = height;
    m_MatrixPerspective.SetPerspective( 60, (float)width/height, 1, 100 );
   // m_MatrixPerspective.showDebugMatrixInfo();
}

void GameCore::OnDrawFrame()
{
    // clear the screen.
    glClearColor( 0.0f, 0.0f, 0.8f, 0.0f );
    glClearDepthf( 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport( 0, 0, m_WindowWidth, m_WindowHeight );
   
    
    switch(m_ViewNumber)
    {
        case 0:
        {
            //glViewport( 0, m_WindowHeight/2, m_WindowWidth/2, m_WindowHeight/2 );
            
            DrawQuad2D( m_TextureBGTop );
            
            Matrix cam;
            cam.SetLookAt( Vector3(0,5,0), Vector3(0,0,1), Vector3(0,0,0) );
            
            DrawScene( &cam );
        }
        break;
            
        case 1:
        {
           // glViewport( m_WindowWidth/2, m_WindowHeight/2, m_WindowWidth/2, m_WindowHeight/2 );
            
            DrawQuad2D( m_TextureBGFront );
            
            Matrix cam;     //cam pos              camera up vec     lokking at
            //cam.SetLookAt( Vector3(0,1.5,1.7), Vector3(0,1,0), Vector3(0,0.5,0) );
           // printf("Y Rot ANGLE %f \n", m_yAxisAngle);
            
                    //point of rotation
            cam.arcBall(Vector3(0, 0, 0), m_xAxisAngle, m_yAxisAngle, 0, m_ZValueforZoom);
            DrawScene( &cam );
        }
        break;
            
        case 2:
        {
            // glViewport( 0, 0, m_WindowWidth/2, m_WindowHeight/2 );
            
            DrawQuad2D( m_TextureBGRight );
            
            Matrix cam;
            cam.SetLookAt( Vector3(3,1,0), Vector3(0,1,0), Vector3(0,0,0) );
            
            DrawScene( &cam );
        }
            break;
            
        case 3:
        {
            //glViewport( m_WindowWidth/2, 0, m_WindowWidth/2, m_WindowHeight/2 );
            
            DrawQuad2D( m_TextureBGPerspective );
           // float lerpAngle = lerp(m_InitialModelAngleOld, m_InitialModelAngleNew, delta);
           
            getCameraRotCoords(m_CameraPos, m_InitialModelAngleOld, 3);
            Matrix cam;
             m_InitialModelAngleOld = lerp(m_InitialModelAngleOld,m_InitialModelAngleNew,0.25f);
            
            
            // lerp(previousPos.x, m_CameraPos.x, 0.05f);
            // lerp(previousPos.y, m_CameraPos.y, 0.05f);
            // lerp(previousPos.z, m_CameraPos.z, 0.05f);
           
            cam.SetLookAt(m_CameraPos, Vector3(0,1,0), Vector3(0,0,0) );
            
            //cam.showDebugMatrixInfo();
            //printf("prev pos : %f %f %f\n",previousPos.x, previousPos.y, previousPos.z);
            //printf("current pos %f %f %f\n",m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
            DrawScene( &cam );
            break;
        }
    }

    
}

void GameCore::DrawScene(Matrix* pCameraMatrix)
{
    Matrix mvp;

    // draw the ground
    {
        mvp.SetIdentity();
        mvp.Multiply( pCameraMatrix );
        mvp.Multiply( &m_MatrixPerspective );

        DrawGround( &mvp );
    }

    // draw user controlled pyramid.
    {
        mvp.SetIdentity();
        mvp.TranslatePostRotation( m_ObjectPos.x, m_ObjectPos.y, m_ObjectPos.z );
        mvp.Multiply( pCameraMatrix );
        mvp.Multiply( &m_MatrixPerspective );

       // DrawPyramid( &mvp );
    }

    // draw small pyramid rotating around zero on z plane
    {
        mvp.SetIdentity();
        mvp.Scale( 0.5f, 0.5f, 0.5f );
        mvp.TranslatePostRotation( 0, 0, -1 );
        mvp.Rotate( -(float)m_TimeInGame*50, 0, 1, 0 );
        mvp.Multiply( pCameraMatrix );
        mvp.Multiply( &m_MatrixPerspective );

      //  DrawPyramid( &mvp );
    }

    // draw even smaller pyramid rotating around previous pyramid on z plane
    {
        
        mvp.SetIdentity();
        //mvp.Rotate( -(float)m_TimeInGame*500, 0, 1, 0 );
       // mvp.Scale( 0.007, 0.007, 0.007 );//ivan
         mvp.Scale( m_ModelScale, m_ModelScale, m_ModelScale);//ivan
        mvp.SetPosition(0,0,0);
       // mvp.TranslatePostRotation( 0, 0, -0.2f );
       // mvp.Rotate( (float)m_TimeInGame*20, 0, 1, 0 );
        
        mvp.Multiply( pCameraMatrix );
        mvp.Multiply( &m_MatrixPerspective );

       DrawCube( &mvp );
    }

    // draw upside down pyramid bobbing up and down
    {
        mvp.SetIdentity();
        mvp.Rotate( (float)180, 1, 0, 0 );
        mvp.TranslatePostRotation( 0.5f, sin(m_TimeInGame) + 1, 0 );
        mvp.Multiply( pCameraMatrix );
        mvp.Multiply( &m_MatrixPerspective );

       // DrawPyramid( &mvp );
    }
}

void GameCore::DrawQuad2D(GLuint texturehandle)
{
    // start using the program
    glUseProgram( m_pShaderTexture2D->m_Program );

    // Get our shader attribute and uniform handles
    GLint aPos = glGetAttribLocation( m_pShaderTexture2D->m_Program, "a_Position" );
    GLint aUV = glGetAttribLocation( m_pShaderTexture2D->m_Program, "a_UV" );
    GLint uTexture = glGetUniformLocation( m_pShaderTexture2D->m_Program, "u_MainTex" );

    // program the attributes
    glBindBuffer( GL_ARRAY_BUFFER, m_Quad2DVBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Quad2DIBO );

    if( aPos != -1 )
    {
        glVertexAttribPointer( aPos, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,pos) );
        glEnableVertexAttribArray( aPos );
    }

    if( aUV != -1 )
    {
        glVertexAttribPointer( aUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,uv) );
        glEnableVertexAttribArray( aUV );
    }

    // program the uniforms
    if( uTexture != -1 )
    {
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texturehandle );
        glUniform1i( uTexture, 0 );
    }

    // draw the verts
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0 );

    glDisableVertexAttribArray( aPos );
    glDisableVertexAttribArray( aUV );
}

void GameCore::DrawGround(Matrix* pMVP)
{
    // start using the program
    glUseProgram( m_pShaderTexture3D->m_Program );

    // Get our shader attribute and uniform handles
    GLint aPos = glGetAttribLocation( m_pShaderTexture3D->m_Program, "a_Position" );
    GLint aUV = glGetAttribLocation( m_pShaderTexture3D->m_Program, "a_UV" );
    GLint uTexture = glGetUniformLocation( m_pShaderTexture3D->m_Program, "u_MainTex" );
    GLint uMatMVP = glGetUniformLocation( m_pShaderTexture3D->m_Program, "u_MatMVP" );    

    // program the attributes
    glBindBuffer( GL_ARRAY_BUFFER, m_GroundVBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_GroundIBO );

    if( aPos != -1 )
    {
        glVertexAttribPointer( aPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,pos) );
        glEnableVertexAttribArray( aPos );
    }

    if( aUV != -1 )
    {
        glVertexAttribPointer( aUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,uv) );
        glEnableVertexAttribArray( aUV );
    }

    // program the uniforms
    if( uTexture != -1 )
    {
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, m_TextureGround );
        glUniform1i( uTexture, 0 );
    }

    glUniformMatrix4fv( uMatMVP, 1, false, (GLfloat*)&pMVP->m11 );

    // draw the verts
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0 );

    glDisableVertexAttribArray( aPos );
    glDisableVertexAttribArray( aUV );
}

void GameCore::DrawPyramid(Matrix* pMVP)
{
    // start using the program
    glUseProgram( m_pShaderColor->m_Program );

    // Get our shader attribute and uniform handles
    GLint aPos = glGetAttribLocation( m_pShaderColor->m_Program, "a_Position" );
    GLint aColor = glGetAttribLocation( m_pShaderColor->m_Program, "a_Color" );
    GLint uMatMVP = glGetUniformLocation( m_pShaderColor->m_Program, "u_MatMVP" );    

    // program the attributes
    glBindBuffer( GL_ARRAY_BUFFER, m_PyramidVBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_PyramidIBO );

    if( aPos != -1 )
    {
        glVertexAttribPointer( aPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,pos) );
        glEnableVertexAttribArray( aPos );
    }

    if( aColor != -1 )
    {
        glVertexAttribPointer( aColor, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,color) );
        glEnableVertexAttribArray( aColor );
    }

    glUniformMatrix4fv( uMatMVP, 1, false, (GLfloat*)&pMVP->m11 );

    // draw the verts
    glDrawElements( GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0 );

    glDisableVertexAttribArray( aPos );
    glDisableVertexAttribArray( aColor );
}

 void GameCore::DrawCube(Matrix* pMVP)
{
    // start using the program
    //glUseProgram( m_pShaderColor->m_Program );
     glUseProgram( m_pShaderTexture3D->m_Program );
    
    // Get our shader attribute and uniform handles
    GLint aPos = glGetAttribLocation( m_pShaderColor->m_Program, "a_Position" );
   // GLint aColor = glGetAttribLocation( m_pShaderColor->m_Program, "a_Color" );
    GLint uMatMVP = glGetUniformLocation( m_pShaderColor->m_Program, "u_MatMVP" );
    
   
    GLint aUV = glGetAttribLocation( m_pShaderTexture3D->m_Program, "a_UV" );
    GLint uTexture = glGetUniformLocation( m_pShaderTexture3D->m_Program, "u_MainTex" );
    
  
    // program the attributes
    //glBindBuffer( GL_ARRAY_BUFFER, m_CubeVBO );
   // glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_CubeIBO );
    
    
    glBindBuffer(GL_ARRAY_BUFFER, m_LoadedUvBuffer);
    
    if( aUV != -1 )
    {
        glVertexAttribPointer( aUV, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
        glEnableVertexAttribArray( aUV );
    }
    
    if( uTexture != -1 )
    {
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, m_CubeTexture);
        glUniform1i( uTexture, 0 );
    }

    glBindBuffer( GL_ARRAY_BUFFER, m_LoadedVBO );
  //  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_LoadedVBO);
  //
   
    if( aPos != -1 )
    {
        glVertexAttribPointer( aPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
        glEnableVertexAttribArray( aPos );
    }
    
    
    // program the uniforms
    
   
    
    //if( aColor != -1 )
    {
        //glVertexAttribPointer( aColor, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
        //glEnableVertexAttribArray( aColor );
    }
    
    glUniformMatrix4fv( uMatMVP, 1, false, (GLfloat*)&pMVP->m11 );
    
    // draw the verts
   // glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0 );
    glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
    
    
   // glCullFace(GL_FRONT);
   // glEnable( GL_CULL_FACE );
   
   // glCullFace(GL_BACK);
   // glEnable( GL_CULL_FACE );
    
    glDisableVertexAttribArray( aPos );
    glDisableVertexAttribArray( aUV );
}

void GameCore::SwitchView()
{
    const int maxViewNumber = 4;
    
    if(m_ViewNumber < maxViewNumber - 1)
    {
        m_ViewNumber++;
    }
    else
    {
        m_ViewNumber = 0;
    }
}

void GameCore::RotateCameraAroundAxis(float angle)
{
    if(m_ViewNumber == 3) //ONLY INCREASE ANGLE WHEN WE ARE ON THE PERSPECTIVE SCREEN
    m_InitialModelAngleNew += angle;
}

 void GameCore::ZoomZoom(float value)
{
    if(m_ZValueforZoom < 1.3f) //minimum zoom guard
        m_ZValueforZoom = 1.3f;
    if(m_ViewNumber == 1) //ONLY INCREASE VALUE WHEN WE ARE ON THE PERSPECTIVE SCREEN
    m_ZValueforZoom += value;
   // printf("Z value is %f \n",m_ZValueforZoom);
}
