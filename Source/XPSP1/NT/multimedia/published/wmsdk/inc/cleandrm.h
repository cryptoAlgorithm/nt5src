//////////////////////////////////////////////////////////////////////
//
// CleanDRM.h
//
//////////////////////////////////////////////////////////////////////

#ifndef _CLEANDRM_H_
#define _CLEANDRM_H_

#ifdef  __cplusplus
extern "C" {
#endif  


/////////////////////////////////////////////////////////////////////
//
// CleanDRM
//
// szLogFile: Full path to file to log information to.
//            Set to NULL to not log any information.
//
HRESULT __stdcall CleanDRM( LPCSTR szLogFile );


#ifdef  __cplusplus
}
#endif  

#endif // _CLEANDRM_H_
