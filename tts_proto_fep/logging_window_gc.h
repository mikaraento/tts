#ifndef LOGGING_WINDOW_GC_H_
#define LOGGING_WINDOW_GC_H_

#include <w32std.h> // CWindowGc

class LoggingState;

// LoggingWindowGc is a class that can be used to replace the default system
// gc with one that logs all the calls made before forwarding them to the
// real implementation. It is used in an odd way: it's instantiated just to
// get hold of the vtable that is then used to override the system gc's vtable.
class LoggingWindowGc : public CWindowGc {
 public:
  LoggingWindowGc(CWsScreenDevice* aDevice);
  ~LoggingWindowGc();
  static void LogText(const TDesC& call,
                      const TPoint& position,
                      const TDesC& text);
 public:
  void SetSystemGc(CWindowGc* aGc);

 public:
  void Activate(RDrawableWindow &aDevice);
  void Deactivate();

  void DrawText(const TDesC& aText, const TPoint& aPosition);
  void DrawText(const TDesC& aText, const TRect& aBox, TInt aBaselineOffset,
                TTextAlign aAlignment = ELeft, TInt aLeftMargin = 0);
  void DrawText(const TDesC& aText, const TPoint& aPosition,
                const TDrawTextParam& aParam);
  void DrawTextVertical(const TDesC& aText, const TPoint& aPos, TBool aUp);
  void DrawTextVertical(const TDesC& aText, const TRect& aBox,
                        TInt aBaselineOffset, TBool aUp, TTextAlign aVert =
                                ELeft, TInt aMargin = 0);
};

#endif  // LOGGING_WINDOW_GC_H_
