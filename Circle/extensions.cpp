/*
    Library Name:

        ARB Extensions


    Description:

        get OpenGL API functions
*/

#include "stdafx.h"
#include "extensions.h"

PFNGLGETINFOLOGARBPROC                glGetInfoLogARB                = NULL;
PFNGLCREATESHADEROBJECTARBPROC        glCreateShaderObjectARB        = NULL;
PFNGLDELETEOBJECTARBPROC              glDeleteObjectARB              = NULL;
PFNGLSHADERSOURCEARBPROC              glShaderSourceARB              = NULL;
PFNGLCOMPILESHADERARBPROC             glCompileShaderARB             = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC      glGetObjectParameterivARB      = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC       glCreateProgramObjectARB       = NULL;
PFNGLATTACHOBJECTARBPROC              glAttachObjectARB              = NULL;
PFNGLDETACHOBJECTARBPROC              glDetachObjectARB              = NULL;
PFNGLLINKPROGRAMARBPROC               glLinkProgramARB               = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC          glUseProgramObjectARB          = NULL;
PFNGLBINDATTRIBLOCATIONARBPROC        glBindAttribLocationARB        = NULL;
PFNGLGENBUFFERSARBPROC                glGenBuffersARB                = NULL;
PFNGLBINDBUFFERARBPROC                glBindBufferARB                = NULL;
PFNGLBUFFERDATAARBPROC                glBufferDataARB                = NULL;
PFNGLDELETEBUFFERSARBPROC             glDeleteBuffersARB             = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC   glEnableVertexAttribArrayARB   = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC  glDisableVertexAttribArrayARB  = NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC       glVertexAttribPointerARB       = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC        glGetUniformLocationARB        = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC          glUniformMatrix4fvARB          = NULL;

//
bool LoadFunctions()
{
    bool support, exist;

     // check for the necessary extensions
    char *extensions = (char*)glGetString(GL_EXTENSIONS);

    support = strstr(extensions, "GL_ARB_shading_language_100") != NULL &&
              strstr(extensions, "GL_ARB_shader_objects")       != NULL &&
              strstr(extensions, "GL_ARB_fragment_shader")      != NULL &&
              strstr(extensions, "GL_ARB_vertex_shader")        != NULL &&
              strstr(extensions, "GL_ARB_vertex_buffer_object") != NULL;

    // get functions
    glGetInfoLogARB                =  (PFNGLGETINFOLOGARBPROC)               wglGetProcAddress("glGetInfoLogARB");
    glCreateShaderObjectARB        =  (PFNGLCREATESHADEROBJECTARBPROC)       wglGetProcAddress("glCreateShaderObjectARB");
    glDeleteObjectARB              =  (PFNGLDELETEOBJECTARBPROC)             wglGetProcAddress("glDeleteObjectARB");
    glShaderSourceARB              =  (PFNGLSHADERSOURCEARBPROC)             wglGetProcAddress("glShaderSourceARB");
    glCompileShaderARB             =  (PFNGLCOMPILESHADERARBPROC)            wglGetProcAddress("glCompileShaderARB");
    glGetObjectParameterivARB      =  (PFNGLGETOBJECTPARAMETERIVARBPROC)     wglGetProcAddress("glGetObjectParameterivARB");
    glCreateProgramObjectARB       =  (PFNGLCREATEPROGRAMOBJECTARBPROC)      wglGetProcAddress("glCreateProgramObjectARB");
    glAttachObjectARB              =  (PFNGLATTACHOBJECTARBPROC)             wglGetProcAddress("glAttachObjectARB");
    glDetachObjectARB              =  (PFNGLDETACHOBJECTARBPROC)             wglGetProcAddress("glDetachObjectARB");
    glLinkProgramARB               =  (PFNGLLINKPROGRAMARBPROC)              wglGetProcAddress("glLinkProgramARB");
    glUseProgramObjectARB          =  (PFNGLUSEPROGRAMOBJECTARBPROC)         wglGetProcAddress("glUseProgramObjectARB");
    glBindAttribLocationARB        =  (PFNGLBINDATTRIBLOCATIONARBPROC)       wglGetProcAddress("glBindAttribLocationARB");
    glGenBuffersARB                =  (PFNGLGENBUFFERSARBPROC)               wglGetProcAddress("glGenBuffersARB");
    glBindBufferARB                =  (PFNGLBINDBUFFERARBPROC)               wglGetProcAddress("glBindBufferARB");
    glBufferDataARB                =  (PFNGLBUFFERDATAARBPROC)               wglGetProcAddress("glBufferDataARB");
    glDeleteBuffersARB             =  (PFNGLDELETEBUFFERSARBPROC)            wglGetProcAddress("glDeleteBuffersARB");
    glEnableVertexAttribArrayARB   =  (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)  wglGetProcAddress("glEnableVertexAttribArrayARB");
    glDisableVertexAttribArrayARB  =  (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) wglGetProcAddress("glDisableVertexAttribArrayARB");
    glVertexAttribPointerARB       =  (PFNGLVERTEXATTRIBPOINTERARBPROC)      wglGetProcAddress("glVertexAttribPointerARB");
    glGetUniformLocationARB        =  (PFNGLGETUNIFORMLOCATIONARBPROC)       wglGetProcAddress("glGetUniformLocationARB");
    glUniformMatrix4fvARB          =  (PFNGLUNIFORMMATRIX4FVARBPROC)         wglGetProcAddress("glUniformMatrix4fvARB");

    exist = glGetInfoLogARB                != NULL &&
            glCreateShaderObjectARB        != NULL &&
            glDeleteObjectARB              != NULL &&
            glShaderSourceARB              != NULL &&
            glCompileShaderARB             != NULL &&
            glGetObjectParameterivARB      != NULL &&
            glCreateProgramObjectARB       != NULL &&
            glAttachObjectARB              != NULL &&
			glDetachObjectARB              != NULL &&
            glLinkProgramARB               != NULL &&
            glUseProgramObjectARB          != NULL &&
            glBindAttribLocationARB        != NULL &&
            glGenBuffersARB                != NULL &&
            glBindBufferARB                != NULL &&
            glBufferDataARB                != NULL &&
            glDeleteBuffersARB             != NULL &&
            glEnableVertexAttribArrayARB   != NULL &&
            glDisableVertexAttribArrayARB  != NULL &&
            glVertexAttribPointerARB       != NULL &&
            glGetUniformLocationARB        != NULL &&
            glUniformMatrix4fvARB          != NULL;

    return support && exist;
}

//
