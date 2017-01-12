#include "CommonHeader.h"

#include "ResourceUtils.h"



void LogError(char* string, ...)
{
    char tempbuffer[1024];

    va_list args;
    va_start( args, string );
  //  vsprintf_s( tempbuffer, 1024, string, args );
    va_end( args );

   // OutputDebugString( tempbuffer );
}

bool CheckForGLErrors(const char* file, int line)
{
    GLenum errorID = GL_NO_ERROR;
    char* errorStr;
    int errorCount = 0;

    while( (errorID = glGetError()) != GL_NO_ERROR )
    {
        errorCount++;

        switch( errorID )
        {
        case GL_INVALID_ENUM:
            errorStr = "GL_INVALID_ENUM";
            break;

        case GL_INVALID_VALUE:
            errorStr = "GL_INVALID_VALUE";
            break;

        case GL_INVALID_OPERATION:
            errorStr = "GL_INVALID_OPERATION";
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;

        case GL_OUT_OF_MEMORY:
            errorStr = "GL_OUT_OF_MEMORY";
            break;

        default:
            errorStr = "Unknown GL message type.";
            break;
        }

        LogError( "GLError -> %d - %s - %d - %s(%d)\n", errorID, errorStr, errorCount, file, line );

        assert( false );
    }

    return( errorCount > 0 ? true : false );
}

char* LoadCompleteFile(const char* fileName, const char* extension)
{
    FILE* file;
   
    const char* pngPath = ResourceUtils::getPathForSpecifiedResource(fileName, extension);
    file = fopen(pngPath, "rb" );
    char* buffer;
    
    if( file )
    {
       int test = fseek( file, 0, SEEK_END );
       long size = ftell( file );
        rewind( file );

        buffer = new char[size+1];
        fread( buffer, size, 1, file );
        buffer[size] = 0;

        fclose( file );

        return buffer;
    }

    return 0;
}

double MyGetSystemTime()
{
   //unsigned __int64 freq;
    //QueryPerformanceFrequency( (LARGE_INTEGER*)&freq );

   // unsigned __int64 time;
   // QueryPerformanceCounter( (LARGE_INTEGER*)&time );

   // double timeseconds = (double)time / freq;

    return 1; //timeseconds;
}

void GetMouseCoordinates(int* mx, int* my)
{
   // extern HWND hWnd;

   // POINT p;
   // if( GetCursorPos( &p ) )
    {
       // if( ScreenToClient( hWnd, &p ) )
        {
          //  *mx = p.x;
           // *my = p.y;
        }
    }
}

bool loadOBJ(
             const char * path,
             std::vector<Vector3> & out_vertices,
             std::vector<Vector2> & out_uvs,
             std::vector<Vector3> & out_normals
             ){
    printf("Loading OBJ file %s...\n", path);
    
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<Vector3> temp_vertices;
    std::vector<Vector2> temp_uvs;
    std::vector<Vector3> temp_normals;
    
    
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        getchar();
        return false;
    }
    
    while( 1 ){
        
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.
        
        // else : parse lineHeader
        
        if ( strcmp( lineHeader, "v" ) == 0 )
        {
            Vector3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }
        else if ( strcmp( lineHeader, "vt" ) == 0 )
        {
            Vector2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
           // uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            temp_uvs.push_back(uv);
        }
        else if ( strcmp( lineHeader, "vn" ) == 0 )
        {
            Vector3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9)
            {
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        else
        {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
        
    }
    
    // For each vertex of each triangle
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
        
        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];
        
        // Get the attributes thanks to the index
        Vector3 vertex = temp_vertices[ vertexIndex-1 ];
        Vector2 uv = temp_uvs[ uvIndex-1 ];
        Vector3 normal = temp_normals[ normalIndex-1 ];
        
        // Put the attributes in buffers
        out_vertices.push_back(vertex);
        out_uvs     .push_back(uv);
        out_normals .push_back(normal);
        
    }
    
    return true;
}

void findModelsDimension(const std::vector<Vector3> & vertices, Vector3 & out_Dimensions)
{
    for(int i =0; i<vertices.size();i++)
    {
        if(out_Dimensions.x < vertices[i].x)
            out_Dimensions.x = vertices[i].x;
        
        if(out_Dimensions.y < vertices[i].y)
            out_Dimensions.y = vertices[i].y;
        
        if(out_Dimensions.z < vertices[i].z)
            out_Dimensions.z = vertices[i].z;
        
    }
}

void getCameraRotCoords(Vector3 & outCords,float angle, float radius)
{
   
    outCords.x = radius * cos(angle * PI/180);// horiz movement x z vert movement y z
    outCords.y = 2;//radius * cos(angle * PI/180);
    outCords.z = radius * sin(angle * PI/180);
    
  //  outCords.x = radius * -sin(angleX*(PI/180)) * cos((angleY)*(PI/180));
  //  outCords.y = radius * -sin((angleY)*(PI/180));
   // outCords.z = -radius * cos((angleX)*(PI/180)) * cos((angleY)*(PI/180));

   // printf("Y value is: %f\n",outCords.y);
}

float degrees2Radians(float angle)
{
    return (angle * PI) / 180.0f;
}

float lerp(float value1, float value2, float amount)
{
    return value1 + (value2 - value1) * amount;
    
}
