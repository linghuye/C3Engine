#ifndef _C3_BASE_FUNC_H_
#define _C3_BASE_FUNC_H_

#include "C3BaseType.h"

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <io.h>
#include <assert.h>

#include <vector>
#include <map>
#include <set>
#include <deque>
#include <string>
#include <list>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){ delete p; p=NULL; }}
#endif

#ifndef SAFE_DELETE_EX
#define SAFE_DELETE_EX(p) { if(p){ delete [] p; p=NULL; }}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) delete [] p; p=NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(ptr)	{ if(ptr){ (ptr)->Release(); (ptr) = NULL; } }
#endif//SAFE_RELEASE

#ifndef SAFE_FREE
#define SAFE_FREE(p)		{ if(p) { free(p); (p)=NULL; } }
#endif//SAFE_FREE

#ifndef SAFE_FCLOSE
#define SAFE_FCLOSE(p)		{ if(p) { fclose(p); (p)=NULL; } }
#endif//SAFE_FCLOSE

#endif//_C3_BASE_FUNC_H_