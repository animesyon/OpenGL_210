/*
    Class Name:

        CShader

    Description:

        load, compile, link and use shaders
*/

#include "stdafx.h"
#include "shader.h"

// constructor
CShader::CShader()
{
    hVertex   = 0;
    hFragment = 0;
    hProgram  = 0;

	infolog = NULL;
}

// destructor
CShader::~CShader()
{
}

// error checking
bool CShader::GetInfoLog(GLhandleARB handle, GLenum status)
{
    char *src = NULL;
    GLint result = 0;
    GLsizei length = 0;

    glGetObjectParameterivARB(handle, status, &result);
    if(!result)
	{
        glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
        if(length > 0)
		{
			if(infolog != NULL) delete[] infolog;
            infolog = new char[length];
            glGetInfoLogARB(handle, length, NULL, infolog);
		}
	}

	return (length > 0);
}

// get GLSL source
bool CShader::LoadSource(char** pSource, size_t* n, wchar_t* pFile)
{
    FILE *fp;
    errno_t err;
	char str[100];
	size_t len;

    // open file for reading
    if( (err = _wfopen_s(&fp,pFile,L"rb")) != 0 )
	{
		if(infolog != NULL) delete[] infolog;

		strcpy_s(str, 100, "cannot open file                    ");
		len = strlen(str) + 1;
        infolog = new char[len];
		strcpy_s(infolog, len, str);

        return false;
	}

    // get file size
    fseek(fp, 0, SEEK_END);
    *n = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // allocate memory
    if(*pSource != NULL) delete[] *pSource;
    *pSource = new char[*n+1];

    // error checking
    if(*pSource == NULL)
	{
        fclose(fp);

		if(infolog != NULL) delete[] infolog;

		strcpy_s(str, 100, "out of memory");
		len = strlen(str) + 1;
        infolog = new char[len];
		strcpy_s(infolog, len, str);

        return false;
	}

    // read the whole file into the buffer
    if(fread(*pSource, sizeof(char), *n, fp) != *n)
	{
		delete[] *pSource;
        fclose(fp);

		if(infolog != NULL) delete[] infolog;

		strcpy_s(str, 100, "read error");
		len = strlen(str) + 1;
        infolog = new char[len];
		strcpy_s(infolog, len, str);

        return false;
	}

    // null terminated
    *(*pSource + *n) = '\0';

    // close file
    fclose(fp);

     return true;
}

// create shader
bool CShader::Create(wchar_t* vertFile, wchar_t* fragFile)
{
    char *source = NULL;
    size_t n;

    // create vertex shader
	strcpy_s(caption, 100, "Vertex Shader Error");

    if(!LoadSource(&source, &n, vertFile)) return false;

    hVertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);     // create shader
    glShaderSourceARB(hVertex, 1, (const char**)&source, NULL);  // add source code
    glCompileShaderARB(hVertex);                                 // compile

    if(source != NULL)
	{
        delete[] source;
        source = NULL;
	}

    if(GetInfoLog(hVertex, GL_OBJECT_COMPILE_STATUS_ARB)) return false;

    // create fragment shader
	strcpy_s(caption, 100, "Fragment Shader Error");

    if(!LoadSource(&source, &n, fragFile)) return false;

    hFragment = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);       // create shader
    glShaderSourceARB(hFragment, 1, (const char**)&source, NULL);  // add source code
    glCompileShaderARB(hFragment);                                 // compile

    if(source != NULL)
	{
        delete[] source;
        source = NULL;
	}

    if(GetInfoLog(hFragment, GL_OBJECT_COMPILE_STATUS_ARB)) return false;

    // creating a program
    hProgram = glCreateProgramObjectARB();

    // attach the shaders to the program
    glAttachObjectARB(hProgram, hVertex);
    glAttachObjectARB(hProgram, hFragment);

    // associate attribute index LOC_VERTEX with attribute variable v_vertex
    glBindAttribLocationARB(hProgram, LOC_VERTEX, "v_vertex");

    // link the program
	strcpy_s(caption, 100, "Link Error");

    glLinkProgramARB(hProgram);

    if(GetInfoLog(hProgram, GL_OBJECT_LINK_STATUS_ARB)) return false;

    // use the program
    glUseProgramObjectARB(hProgram);

	return true;
}

// destroy shader
void CShader::Destroy()
{
	if(infolog != NULL) delete[] infolog;

     glDetachObjectARB(hProgram, hVertex);
     glDetachObjectARB(hProgram, hFragment);

     glDeleteObjectARB(hVertex);
     glDeleteObjectARB(hFragment);
     glDeleteObjectARB(hProgram);
}

// return program handle
GLhandleARB CShader::GetHandle()
{
	   return hProgram;
}

//
