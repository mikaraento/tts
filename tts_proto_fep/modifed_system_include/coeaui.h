// COEAUI.H
//
// Copyright (c) Symbian Software Ltd 1997 - 2004. All rights reserved.
//

#if !defined(_MODIFIED_COEAUI_H__)
#define _MODIFIED_COEAUI_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__W32STD_H__)
#include <w32std.h>
#endif

#if !defined(__VWSDEF_H__)
#include <vwsdef.h>
#endif

#if !defined(__COEDEF_H__)
#include <coedef.h>
#endif

#if !defined(__COEHELP_H__)
#include <coehelp.h>
#endif

#if !defined(__COEVIEW_H__)
#include <coeview.h>
#endif

#if !defined(__COEINPUT_H__)
#include <coeinput.h>
#endif

class CCoeEnv;
class CCoeControl;

class CCoeControlStack;
class CCoeViewManager;
class MCoeViewDeactivationObserver;
class MCoeViewActivationObserver;
class MCoeViewObserver;

/** Application user interface (app UI) base class. 

The app UI's responsibilities include owning the application's control stack and views, 
handling user commands, (see the derived class CEikAppUi), and handling events sent by 
the OS to the application, for instance being brought to the foreground.

The UI framework class CEikAppUi is derived from this class. UIs may derive 
further to add their own UI-specific features to the app UI; each application must 
derive its own concrete app UI class from this.

@publishedAll 
@released */
class CCoeAppUiAccess : public CBase
	{
public:
	IMPORT_C CCoeAppUiAccess();
	IMPORT_C ~CCoeAppUiAccess();
	IMPORT_C void ConstructL(CCoeAppUi* aPrevious=NULL);
public: // new functions
	IMPORT_C void AddToStackL(CCoeControl* aControl,TInt aPriority=ECoeStackPriorityDefault,TInt aStackingFlags=ECoeStackFlagStandard);
	IMPORT_C void RemoveFromStack(CCoeControl* aControl);
	IMPORT_C void UpdateStackedControlFlags(CCoeControl* aControl,TInt aFlags,TInt aMask);
	IMPORT_C void HandleStackChanged();
	IMPORT_C TBool IsDisplayingMenuOrDialog() const; 
	IMPORT_C TBool IsDisplayingDialog() const; 
	IMPORT_C TBool IsDisplayingControlBetweenPriorities(TInt aLowerPriority, TInt aHigherPriority) const;
	IMPORT_C CArrayFix<TCoeHelpContext>* AppHelpContextL() const;
	IMPORT_C virtual TCoeInputCapabilities InputCapabilities() const;
	IMPORT_C void HandleStackedControlsResourceChange(TInt aType);
	IMPORT_C void ActivateViewL(const TVwsViewId& aViewId);
	IMPORT_C void ActivateViewL(const TVwsViewId& aViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage);
	IMPORT_C TBool CheckSourceOfViewSwitchL(const TSecurityPolicy& aSecurityPolicy,const char* aDiagnostic=NULL) const;
	IMPORT_C void RegisterViewL(MCoeView& aView);
	IMPORT_C void DeregisterView(const MCoeView& aView);
	IMPORT_C void SetDefaultViewL(const MCoeView& aView);
	IMPORT_C TInt GetDefaultViewId(TVwsViewId& aViewId) const;
	IMPORT_C TInt GetActiveViewId(TVwsViewId& aViewId) const;
	IMPORT_C void AddToStackL(const MCoeView& aView,CCoeControl* aControl,TInt aPriority=ECoeStackPriorityDefault,TInt aStackingFlags=ECoeStackFlagStandard);
	IMPORT_C CCoeControl* TopFocusedControl() const;

public: // but not exported
	TBool IsControlOnStack(CCoeControl* aControl) const;
	void SetCurrentControlStackGroupId(TInt aGroupId);
	void NotifyFontChange(const CCoeFontProvider& aFontProvider); 
	void RefetchPixelMapping();
	

public: // 
	IMPORT_C void CreateActivateViewEventL(const TVwsViewId& aViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage);
	IMPORT_C void ActivateTopViewL();
	IMPORT_C void DeactivateActiveViewL();
	IMPORT_C void RegisterApplicationViewL(TUid aAppUid);
	IMPORT_C void DeregisterApplicationView();
	IMPORT_C void CheckInitializeViewsL(TUid aAppUid);
	IMPORT_C void SetApplicationViewAsDefaultL(); //internalTechnology
	IMPORT_C void AddViewDeactivationObserverL(MCoeViewDeactivationObserver* aViewDeactivationObserver);
	IMPORT_C void RemoveViewDeactivationObserver(MCoeViewDeactivationObserver* aViewDeactivationObserver);
	IMPORT_C void NotifyNextDeactivation(const TVwsViewId& aViewId, MCoeViewDeactivationObserver& aViewDeactivationObserver);
	IMPORT_C void NotifyNextDeactivation(MCoeViewDeactivationObserver& aViewDeactivationObserver);
	IMPORT_C void AddViewActivationObserverL(MCoeViewActivationObserver* aViewActivationObserver);
	IMPORT_C void RemoveViewActivationObserver(MCoeViewActivationObserver* aViewActivationObserver);
	IMPORT_C void NotifyNextActivation(const TVwsViewId& aViewId, MCoeViewActivationObserver& aViewActivationObserver);
	IMPORT_C void NotifyNextActivation(MCoeViewActivationObserver& aViewActivationObserver);
	IMPORT_C void AddViewObserverL(MCoeViewObserver* aViewObserver);
	IMPORT_C void RemoveViewObserver(MCoeViewObserver* aViewObserver);
	IMPORT_C void WriteInternalStateOfStackedControlsL(RWriteStream& aWriteStream) const; //internalTechnology
 	IMPORT_C void SetSystemDefaultViewL(const TVwsViewId& aViewId,TInt aMode); //internalTechnology
 	IMPORT_C void SetSystemDefaultViewL(const TVwsViewId& aViewId); //internalTechnology
 	IMPORT_C void GetSystemDefaultViewL(TVwsViewId& aViewId); //internalTechnology
public: 
	IMPORT_C void RegisterViewAndAddStackL(MCoeView& aView);
	IMPORT_C void DeregisterViewAndRemoveStack(const MCoeView& aView);
	IMPORT_C void RemoveFromViewStack(const MCoeView& aView,CCoeControl* aControl);
	IMPORT_C void UpdateViewStackedControlFlags(const MCoeView& aView,CCoeControl* aControl,TInt aFlags,TInt aMask);
	IMPORT_C void AddToViewStackL(const MCoeView& aView,CCoeControl* aControl,TInt aPriority=ECoeStackPriorityDefault,TInt aStackingFlags=ECoeStackFlagStandard);
public: // from CCoeAppUiBase, or new when SYMBIAN_REMOVE_UI_FRAMEWORKS_V1 is turned on
	IMPORT_C virtual void HandleWsEventL(const TWsEvent& aEvent,CCoeControl* aDestination);
	IMPORT_C virtual void PrepareToExit();
public:
	IMPORT_C virtual void HandleScreenDeviceChangedL();

public: // new functions
	IMPORT_C virtual TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	IMPORT_C virtual void HandleForegroundEventL(TBool aForeground);
	IMPORT_C virtual void HandleSwitchOnEventL(CCoeControl* aDestination);
	IMPORT_C virtual void HandleSystemEventL(const TWsEvent& aEvent);
	IMPORT_C virtual void HandleApplicationSpecificEventL(TInt aType,const TWsEvent& aEvent);
	IMPORT_C virtual void SetAndDrawFocus(TBool aFocus);
	IMPORT_C virtual CArrayFix<TCoeHelpContext>* HelpContextL() const;
public: //new functions
	IMPORT_C TBool IsViewConstructed(const TVwsViewId& aViewId) const;
	IMPORT_C virtual void CCoeAppUi_Reserved1();
	IMPORT_C virtual void CCoeAppUi_Reserved2();
public:
	enum TRemoveCondition
		{
		ERemoveUnconditionally,
		ERemoveOnlyIfSharable
		};
public:
	CCoeControl* TopFocusableControl() const;
	TInt FindPos(CCoeControlStack* aStack,CCoeControl* aControl) const;
	void SetFocusToControl(CCoeControl* aControl,TBool aFocus);
	void DoAddToStackL(CCoeControlStack* aStack,CCoeControl* aControl,TInt aPriority,TInt aStackingFlags);
	void DoAddToStackL(CCoeControlStack* aStack,CCoeControl* aControl,TInt aPriority,TInt aStackingFlags, TInt aGroupId);
	void DoRemoveFromStack(CCoeControlStack* aStack,CCoeControl* aControl,TRemoveCondition aRemoveCondition=ERemoveUnconditionally);
	void DoUpdateStackedControlFlags(CCoeControlStack* aStack,CCoeControl* aControl,TInt aFlags,TInt aMask);
public:
/**Monitor function for passing all windows events to registered monitor observers for optional inspection*/
	void MonitorWsEvent(const TWsEvent& aEvent);
public:
	class CExtra;
	friend class CExtra;
	friend class CTestDriver;
public:
	CCoeEnv* iCoeEnv;
public:
	CCoeViewManager* iViewManager;
	CCoeControlStack* iStack;
	CExtra* iExtra;
	TInt iCCoeAppUi_Reserved1;
	};

#endif
