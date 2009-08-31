// MyWindowGc.cpp

#include "logging_window_gc.h"

#include <e32base.h>
#include <flogger.h>

class LoggingState {
 public:
  LoggingState() {
    RProcess me;
    User::LeaveIfError(me.Open(me.Id()));
    process_name_ = me.Name().Left(50);
    me.Close();
    User::LeaveIfError(logger_.Connect());
    logger_.CreateLog(_L("textdump"), _L("log.txt"), EFileLoggingModeAppend);
  }
  ~LoggingState() {
    logger_.Close();
  }
  void Log(const TDesC& message) {
    buffer_ = process_name_;
    buffer_.Append(_L(": "));
    buffer_.Append(message.Left(70));
    logger_.Write(buffer_);
    buffer_.Append(_L("\n"));
    RDebug::RawPrint(buffer_);
  }
private:
 TBuf<50> process_name_;
 TBuf<128> buffer_;
 RFileLogger logger_;
};

CMyWindowGc::CMyWindowGc(CWsScreenDevice* aDevice) : CWindowGc(aDevice)
    {
    }

CMyWindowGc::~CMyWindowGc()
    {
    }

void CMyWindowGc::SetupTls() {
  LoggingState* log = new (ELeave) LoggingState;  
  Dll::SetTls(log);
}

void CMyWindowGc::TeardownTls() {
  LoggingState* log = (LoggingState*)Dll::Tls();
  delete log;
  Dll::SetTls(NULL);
}

LoggingState* CMyWindowGc::GetState() {
  return (LoggingState*)Dll::Tls();
}

void CMyWindowGc::LogText(const TDesC& call, const TPoint& position, const TDesC& text) {
  LoggingState* log = GetState();
  if (!log) return;
  HBufC* buf = HBufC::NewLC(call.Length() + 30 + text.Length());
  buf->Des().Append(call);
  buf->Des().Append(_L(" "));
  buf->Des().AppendNum(position.iX);
  buf->Des().Append(_L(" "));
  buf->Des().AppendNum(position.iY);
  buf->Des().Append(_L(" "));
  buf->Des().Append(text);
  log->Log(*buf);
  CleanupStack::PopAndDestroy();
}

void CMyWindowGc::Activate(RDrawableWindow &aDevice)
   {
   TPoint p = aDevice.Position();
   TSize s = aDevice.Size();
   RDebug::Print(_L("Activate: window"));
   CWindowGc::Activate(aDevice);
   }

void CMyWindowGc::Deactivate()
    {
    RDebug::Print(_L("Deactivate: window"));
    CWindowGc::Deactivate();
    }

void CMyWindowGc::DrawText(const TDesC& aText,const TPoint& aPosition)
    {
    LogText(_L("DrawText1"), aPosition, aText);
    CWindowGc::DrawText(aText, aPosition);
    }

void CMyWindowGc::DrawText(const TDesC& aText,const TRect& aBox,TInt aBaselineOffset,TTextAlign aAlignment, TInt aLeftMargin)
    {
    LogText(_L("DrawText2"), aBox.iTl, aText);
    CWindowGc::DrawText(aText, aBox, aBaselineOffset, aAlignment, aLeftMargin);
    }

void CMyWindowGc::DrawText(const TDesC& aText,const TPoint& aPosition,const TDrawTextParam& aParam)
    {
    LogText(_L("DrawText3"), aPosition, aText);
    CWindowGc::CBitmapContext::DrawText(aText, aPosition, aParam);
    }

void CMyWindowGc::DrawTextVertical(const TDesC& aText,const TPoint& aPos,TBool aUp)
    {
    LogText(_L("DrawTextVertical1"), aPos, aText);
    CWindowGc::DrawTextVertical(aText, aPos, aUp);
    }

void CMyWindowGc::DrawTextVertical(const TDesC& aText,const TRect& aBox,TInt aBaselineOffset,TBool aUp,TTextAlign aVert,TInt aMargin)
    {
    LogText(_L("DrawTextVertical2"), aBox.iTl, aText);
    CWindowGc::DrawTextVertical(aText, aBox, aBaselineOffset, aUp, aVert, aMargin);
    }
