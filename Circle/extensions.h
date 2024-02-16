/*
    Library Name:

        ARB Extensions


    Description:

        get OpenGL API functions
*/

#ifndef ARB_EXTENSIONS_H
#define ARB_EXTENSIONS_H

extern  PFNGLGETINFOLOGARBPROC                glGetInfoLogARB;
extern  PFNGLCREATESHADEROBJECTARBPROC        glCreateShaderObjectARB;
extern  PFNGLDELETEOBJECTARBPROC              glDeleteObjectARB;
extern  PFNGLSHADERSOURCEARBPROC              glShaderSourceARB;
extern  PFNGLCOMPILESHADERARBPROC             glCompileShaderARB;
extern  PFNGLGETOBJECTPARAMETERIVARBPROC      glGetObjectParameterivARB;
extern  PFNGLCREATEPROGRAMOBJECTARBPROC       glCreateProgramObjectARB;
extern  PFNGLATTACHOBJECTARBPROC              glAttachObjectARB;
extern  PFNGLDETACHOBJECTARBPROC              glDetachObjectARB;
extern  PFNGLLINKPROGRAMARBPROC               glLinkProgramARB;
extern  PFNGLUSEPROGRAMOBJECTARBPROC          glUseProgramObjectARB;
extern  PFNGLBINDATTRIBLOCATIONARBPROC        glBindAttribLocationARB;
extern  PFNGLGENBUFFERSARBPROC                glGenBuffersARB;
extern  PFNGLBINDBUFFERARBPROC                glBindBufferARB;
extern  PFNGLBUFFERDATAARBPROC                glBufferDataARB;
extern  PFNGLDELETEBUFFERSARBPROC             glDeleteBuffersARB;
extern  PFNGLENABLEVERTEXATTRIBARRAYARBPROC   glEnableVertexAttribArrayARB;
extern  PFNGLDISABLEVERTEXATTRIBARRAYARBPROC  glDisableVertexAttribArrayARB;
extern  PFNGLVERTEXATTRIBPOINTERARBPROC       glVertexAttribPointerARB;
extern  PFNGLGETUNIFORMLOCATIONARBPROC        glGetUniformLocationARB;
extern  PFNGLUNIFORMMATRIX4FVARBPROC          glUniformMatrix4fvARB;

bool LoadFunctions();

#endif

//
