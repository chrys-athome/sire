#ifndef __GLOBAL_VAR_FIX_H
#define __GLOBAL_VAR_FIX_H

class QVar;
class QString;
class UploadThread;
class DownloadThread;

#ifdef WIN32
#ifdef ConspireGUI_EXPORTS
#define CONSPIREGUI_EXPORT __declspec(dllexport)
#define CONSPIREGUI_EXTERN
#else
#define CONSPIREGUI_EXPORT
#define CONSPIREGUI_EXTERN extern
#endif
#else
#define CONSPIREGUI_EXPORT
#define CONSPIREGUI_EXTERN
#endif

CONSPIREGUI_EXTERN QMap<QString, UploadThread *> *GetUploadArray();
CONSPIREGUI_EXTERN QMap<QString, DownloadThread *> *GetDownloadArray();
CONSPIREGUI_EXTERN void CONSPIREGUI_EXPORT DestroyUploadArray();
CONSPIREGUI_EXTERN void CONSPIREGUI_EXPORT DestroyDownloadArray();

#endif
