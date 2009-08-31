// MyWindowGc.h

#ifndef __MYWINDOWGC_H__
#define __MYWINDOWGC_H__

#include <w32std.h> // CWindowGc

class LoggingState;

class CMyWindowGc : public CWindowGc
    {
public:
    CMyWindowGc(CWsScreenDevice* aDevice);
    ~CMyWindowGc();
    static void SetupTls();
    static void TeardownTls();
    static LoggingState* GetState();
    static void LogText(const TDesC& call, const TPoint& position, const TDesC& text);
public:
    void SetSystemGc(CWindowGc* aGc);

public:
    void Activate(RDrawableWindow &aDevice);
    void Deactivate();

    void DrawText(const TDesC& aText,const TPoint& aPosition);
    void DrawText(const TDesC& aText,const TRect& aBox,TInt aBaselineOffset,TTextAlign aAlignment = ELeft,TInt aLeftMargin = 0);
    void DrawText(const TDesC& aText,const TPoint& aPosition,const TDrawTextParam& aParam);
    void DrawTextVertical(const TDesC& aText,const TPoint& aPos,TBool aUp);
    void DrawTextVertical(const TDesC& aText,const TRect& aBox,TInt aBaselineOffset,TBool aUp,TTextAlign aVert=ELeft,TInt aMargin=0);
    };

#endif // __MYWINDOWGC_H__

