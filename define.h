#define VER_Major            1
#define VER_Minor            0
#define VER_Release          0
#define VER_Build            13
#define VER_Version          MAKELONG(MAKEWORD(VER_Major, VER_Minor), VER_Release)

#define _Stringizing(v)      #v
#define _VerJoin(a,b,c,d)    _Stringizing(a.b.c.d)
#define VERSION              VER_Major,VER_Minor,VER_Release,VER_Build

#define STR_BuildDate        TEXT(__DATE__)
#define STR_BuildTime        TEXT(__TIME__)
#define STR_BuilDateTime     TEXT(__DATE__) TEXT(" ") TEXT(__TIME__)
#define STR_Version          TEXT(_VerJoin(VER_Major, VER_Minor, VER_Release, VER_Build))

#define STR_AppName        TEXT("MemoryStack.dll")
#define STR_Author         TEXT("wangfei")
#define STR_Corporation    TEXT("江苏汉德天坤科技发展有限公司")
#define STR_Web            TEXT("www.handkoo.com")
#define STR_Email          TEXT("1509379222@qq.com")
#define STR_WebUrl         TEXT("http://www.handkoo.com") STR_Web
#define STR_EmailUrl       TEXT("mailto:") STR_Email TEXT("?Subject=") STR_AppName

#define STR_Description    TEXT("汉德天坤大内存处理模块")
#define STR_Copyright      TEXT("版权所有 (C) 2012-2022") STR_Corporation TEXT("，保留所有权利。")

// #define STR_Description    TEXT("Picture cutting tool for batch-scanned photo.")
// #define STR_Copyright      TEXT("Copyright (C) 2002-2004 ") STR_Corporation TEXT(". All rights reserved.")
