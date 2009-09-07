/*
* ==============================================================================
*  Name        : akntabgrp.h
*  Part of     : AVKON
*  Interface   : ?Interface_category, ?Interface_name
*  Description : Implements tab control to the status 
				 pane's navigation pane.
*  Version     : ?Version
*
*  Copyright © 2002-2004 Nokia. All rights reserved.
*  This material, including documentation and any related 
*  computer programs, is protected by copyright controlled by 
*  Nokia. All rights are reserved. Copying, including 
*  reproducing, storing, adapting or translating, any 
*  or all of this material requires the prior written consent of 
*  Nokia. This material also contains confidential 
*  information which may not be disclosed to others without the 
*  prior written consent of Nokia.
* ==============================================================================
*/


#ifndef MODIFIED_AKNTABGRP_H
#define MODIFIED_AKNTABGRP_H

//  INCLUDES
#include <AknControl.h>
#include <coeccntx.h>
#include <coecobs.h>
#include <aknnavidecoratorobserver.h>
#include "aknconsts.h"
#include "aknutils.h"



// FORWARD DECLARATIONS

class CAknTab;
class CEikLabel;
class CEikImage;
class CAknTabGroup;
class CAknTabGroupExtension;
class MAknTabObserver;
class MAknNaviDecoratorObserver;
class TAknWindowLineLayout;
class CAknTabExtension;
/**
*  Implements tab control
*
*  @since Series 60 0.9
*/
NONSHARABLE_CLASS(CAknTabAccess) : public CAknControl
	{
public:
	/**
	* Indicates a tab title's type
	*/
	enum TAknTabDataType
		{
		/** Tab title is empty.*/
		EAknTabNone,
		/** Tab title has only text. */ 
		EAknTabText,
		/** Tab title has only icon */
		EAknTabIcon,
		/** Tab title has both text and icon. */
		EAknTabTextAndIcon
		};

public:
    /**
    * Destructor
    */
	~CAknTabAccess();
	
    /**
    * Two-phased constructor.
    * @param aParent Reference to parent control.
    * @param aReader Reference to resource reader.
    * @return A pointer to constructed @c CAknTab object.
    */
	static CAknTab* NewL(const CCoeControl& aParent,TResourceReader& aReader);
	
    /**
    * Two-phased constructor.
    * @param aParent Reference to parent control.
    * @param aReader Reference to resource reader.
    * @return A pointer to constructed @c CAknTab object.
    */
	static CAknTab* NewLC(const CCoeControl& aParent,TResourceReader& aReader);
	
    /**
    * Two-phased constructor.
    * @param aId Tab ID.
    * @param aParent Reference to parent control.
    * @param aTabText Reference to title text.
    * @return A pointer to constructed @c CAknTab object.
    */
	static CAknTab* NewL(TInt aId,const CCoeControl& aParent,const TDesC& aTabText);
	
    /**
    * Two-phased constructor.
    * @param aId Tab ID.
    * @param aParent Reference to parent control.
    * @param aTabText Reference to title text.
    * @return A pointer to constructed @c CAknTab object.
    */
	static CAknTab* NewLC(TInt aId,const CCoeControl& aParent,const TDesC& aTabText);
	
    /**
    * Two-phased constructor.
    * @param aId Tab ID.
    * @param aParent Reference to parent control.
    * @param aTabText Reference to title text.
    * @param aTabBitmap Bitmap for the tab. Ownership of a bitmap is taken.
    * @param aMask Bitmap's mask. Default is @c NULL.
    * @return A pointer to constructed @c CAknTab object.
    */
	static CAknTab* NewL(TInt aId,const CCoeControl& aParent,const TDesC& aTabText,
							const CFbsBitmap* aTabBitmap,const CFbsBitmap* aMask = NULL);
							
    /**
    * Two-phased constructor.
    * @param aId Tab ID.
    * @param aParent Reference to parent control.
    * @param aTabText Reference to title text.
    * @param aTabBitmap Bitmap for the tab. Ownership of a bitmap is taken.
    * @param aMask Bitmap's mask. Default is @c NULL.
    * @return A pointer to constructed @c CAknTab object.
    */
	static CAknTab* NewLC(TInt aId,const CCoeControl& aParent,const TDesC& aTabText,
							const CFbsBitmap* aTabBitmap,const CFbsBitmap* aMask = NULL);
							
	/**
    * Two-phased constructor.
    * @param aId Tab ID.
    * @param aParent Reference to parent control.
    * @param aTabBitmap Bitmap for the tab. Ownership of a bitmap is taken.
    * @param aMask Bitmap's mask. Default is @c NULL.
    * @return A pointer to constructed @c CAknTab object.
    */
	static CAknTab* NewL(TInt aId,const CCoeControl& aParent,const CFbsBitmap* aTabBitmap,
															const CFbsBitmap* aMask = NULL);
															
	/**
    * Two-phased constructor.
    * @param aId Tab ID.
    * @param aParent Reference to parent control.
    * @param aTabBitmap Bitmap for the tab. Ownership of a bitmap is taken.
    * @param aMask Bitmap's mask. Default is @c NULL. 
    * @return A pointer to constructed @c CAknTab object.
    */
	static CAknTab* NewLC(TInt aId,const CCoeControl& aParent,const CFbsBitmap* aTabBitmap,
															const CFbsBitmap* aMask = NULL);

    /**
    * Replace a tab title.
    * @param aTabText A new title text.
    */
    void ReplaceTextL(const TDesC& aTabText);

    /**
    * Activate or deactive this tab.
    * @param aActive @c ETrue for activation, @c EFalse for deactivation.
    * @param aDrawNow Indicates whether control need to be redrawn.
    */
	void SetActive(TBool aActive,TDrawNow aDrawNow);
	
    /**
    * Determines whether this tab is active.
    * @return @c ETrue if a tab is active, @c EFalse if not.
    */
	TBool Active() const;
	
    /**
    * Gets tab's ID.
    * @return Tabs' ID.
    */
	TInt Id() const;
	
    /**
    * Gets this tab's type.
    * @return Tab's type.
    */
	enum TAknTabDataType TabType() const;
	
	
    /**
    * Sets the total amount of tabs in the tabgroup where this tab belongs.
    */
	void SetTotalAmountOfTabs(TInt aAmount);
	
    /**
    * Sets the narrow tab layout.
	* Used by the UI framework. Applications can set the narrow
	* layout using CAknNaviDecorator class.
    */
    void SetNarrowTabLayout(TBool aNarrow);	

public: // from CCoeControl

    /**
    * From @c CCoeControl. Gets a minimum size of the tab.
    * @return A Minimum size of the tab.
    */
	virtual TSize MinimumSize();
	
    /**
    * From @c CCoeControl. Sets a tab dimmed.
    * @param aDimmed @c ETrue to dim the control, 
    * @c EFalse to set the control as not dimmed.
    */
	virtual void SetDimmed(TBool aDimmed);
	
    /**
    * From @c CCoeControl. Gets the list of logical colors employed 
    * in the drawing of the control, paired with an explanation of how
    * they are used. Appends the list into @c aColorUseList.
    * @param aColorUseList The colour list.
    */
	virtual void GetColorUseListL(CArrayFix<TCoeColorUse>& aColorUseList) const;

public: // from CCoeControl

    /**
    * From @c CCoeControl. Responds to size changes to sets the size and 
    * position of the contents of this control.
    */
	virtual void SizeChanged();
	
    /**
    * From @c CCoeControl. Gets the number of controls contained in a compound control. 
    * @return The number of component controls contained by this control.
    */
	virtual TInt CountComponentControls() const;
    
    /**
    * From @c CCoeControl. Gets the specified component of a compound control. 
    * @param aIndex The index of the control to get.
    * @return The component control with an index of aIndex.

    */
	virtual CCoeControl* ComponentControl(TInt aIndex) const;
	
    /**
    * From @c CCoeControl. Handles pointer events.
    * This function is called by framework whenever a pointer event occurs in the control, 
    * @param aPointerEvent The pointer event that occurs.
    */
	virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);

public:
    /**
    * From CAknControl
    */
    IMPORT_C void* ExtensionInterface( TUid aInterface );

public:
    virtual void Draw(const TRect& aRect) const;

public:
	CAknTabAccess(TInt aId);

	void ConstructFromResourceL(TResourceReader& aReader);
	void ConstructL(const TDesC& aTabText);
	void ConstructL(const CFbsBitmap* aTabBitmap,const CFbsBitmap* aMask = NULL);
    
    void LoadColorBitmapL();

    /**
	 * Handles a change to the control's resources of type aType
	 * which are shared across the environment.
	 * @param aType Event type.
	 */
	void HandleResourceChange( TInt aType );

    void SizeChangedInNormalLayout();
    void SizeChangedInFlatLayout();    
    void SizeChangedInNarrowLayout();

public:
	CEikLabel* iLabel;
	TInt iId;
	TBool iActive;

    const CFbsBitmap* iBitmap;
    const CFbsBitmap* iBitmapMask;
    TAknLayoutRect    iBitmapRect;

	CAknTabExtension* iExtension;
	typedef CArrayPtrFlat<CFbsBitmap> CAknTabColorBitmapArray;    
	CAknTabColorBitmapArray* iColorBitmaps;
    };


/**
 * Tab group functionality for navigation pane.
 */
class CAknTabGroupAccess : public CAknControl, public MCoeControlObserver, public MCoeControlContext, public MAknNaviDecoratorObserver
	{
public:
    /**
    * Destructor.
    */
	IMPORT_C ~CAknTabGroupAccess();

    /** 
    * Creates new tab group.
    *
    * @param aParent Parent control.
    */
	IMPORT_C static CAknTabGroup* NewL(const CCoeControl& aParent);

    /** 
    * Creates new tab group and pushes it to the clean up stack.
    *
    * @param aParent Parent control.
    */
    IMPORT_C static CAknTabGroup* NewLC(const CCoeControl& aParent);

public:
    /**
    * Add a tab to the tab group from resources.
    *
    * @param aReader resource reader.
    * @return Id of the added tab.
    */
	IMPORT_C TInt AddTabL(TResourceReader& aReader);
    
    /**
    * Add a tab to the tab group.
    *
    * @param aTabId id of the added tab.
    * @param aTabText text for the tab.
    */
	IMPORT_C void AddTabL(TInt aTabId, const TDesC& aText);
    
    /**
    * Add a tab to the tab group.
    *
    * @param aTabId id of the tab.
    * @param aTabText text for the tab.
    * @param aTabBitmap bitmap for the tab. Ownership of a bitmap is taken.
    * @param aMask bitmap's mask.
    */
	IMPORT_C void AddTabL(TInt aTabId, const TDesC& aText, const CFbsBitmap* aTabBitmap,
										const CFbsBitmap* aMask = NULL);

    /**
    * Add a tab to the tab group.
    *
    * @param aTabId id of the tab.
    * @param aTabBitmap bitmap for the tab. Ownership of a bitmap is taken.
    * @param aMask bitmap's mask.
    */
	IMPORT_C void AddTabL(TInt aTabId, const CFbsBitmap* aTabBitmap, 
										const CFbsBitmap* aMask = NULL);
    /**
    * Replaces a tab with a new one from resources. Tab with same id is replaced.
    *
    * @param aReader resource reader.
    */
	IMPORT_C void ReplaceTabL(TResourceReader& aReader);
    
    /**
    * Replaces a tab with a new one.
    *
    * @param aTabId id of the tab to be replaced.
    * @param aTabText text for the new tab.
    */
	IMPORT_C void ReplaceTabL(TInt aTabId, const TDesC& aText);
    
    /**
    * Replaces a tab with a new one.
    *
    * @param aTabId id of the tab to be replaced.
    * @param aTabText text for the new tab.
    * @param aTabBitmap bitmap for the tab. Ownership of a bitmap is taken.
    * @param aMask bitmap's mask.
    */
	IMPORT_C void ReplaceTabL(TInt aTabId, const TDesC& aText, const CFbsBitmap* aTabBitmap,
										const CFbsBitmap* aMask = NULL);

    /**
    * Replaces a tab with a new one.
    *
    * @param aTabId id of the tab to be replaced.
    * @param aTabBitmap bitmap for the tab. Ownership of a bitmap is taken.
    * @param aMask bitmap's mask.
    */
	IMPORT_C void ReplaceTabL(TInt aTabId, const CFbsBitmap* aTabBitmap, 
										const CFbsBitmap* aMask = NULL);
    
    /**
    * Replaces a tab text with a new one.
    *
    * @param aTabId Id of the tab of replaced tab text.
    * @param aTabText New tab text.
    */
    IMPORT_C void ReplaceTabTextL(TInt aTabId, const TDesC& aTabText);

    /**
    * Delete a tab.
    *
    * @param aTabId id of the tab to be deleted.
    */
	IMPORT_C void DeleteTabL(TInt aTabId);

    /**
    * Hightlight (activate) a tab.
    *
    * @param aTabId id of the tab to be set highlighted.
    */
	IMPORT_C void SetActiveTabById(TInt aTabId);
    
    /**
    * Hightlight (activate) a tab.
    *
    * @param aTabIndex index of the tab to be set highlighted.
    */
	IMPORT_C void SetActiveTabByIndex(TInt aTabIndex);
    
    /**
    * Returns the id of the currently active (=highlighted) tab.
    *
    * @return id of the highlighted tab.
    */
	IMPORT_C TInt ActiveTabId() const;
    
    /**
    * Returns the index of the currently active (=highlighted) tab.
    *
    * @return index of the highlighted tab.
    */
	IMPORT_C TInt ActiveTabIndex() const;
	
    /**
    * Set width of each tab. This width determines what tab layout is used.
    * To get specific tab layout, use following constants/enumerations:
    * (KTabWidthWithOneTab, KTabWidthWithTwoTabs, KTabWidthWithThreeTabs, 
    * KTabWidthWithFourTabs, KTabWidthWithTwoLongTabs, KTabWidthWithThreeLongTabs).
    *
    * @param aWidth width of each tab.
    */
	IMPORT_C void SetTabFixedWidthL(TInt aWidth);

	IMPORT_C void DimTab(TInt aTabId,TBool aDimmed);

	IMPORT_C virtual TBool IsTabDimmed(TInt aTabId) const;

    /**
    * Returns index of a tab.
    *
    * @param aTabId id of a tab.
    * @return index of the tab.
    */
	IMPORT_C virtual TInt TabIndexFromId(TInt aTabId) const;
    
    /**
    * Returns id of a tab.
    *
    * @param aTabIndex index of a tab.
    * @return id of the tab.
    */
	IMPORT_C virtual TInt TabIdFromIndex(TInt aTabIndex) const;
    
    /**
    * Number of tabs in the tab group.
    *
    * @return number of tabs in the tab group.
    */
	IMPORT_C virtual TInt TabCount() const;

	/**
	* Sets tab observer for this tab group.
	*
	* @param aObserver tab observer to set.
	*/ 
	IMPORT_C void SetObserver(MAknTabObserver* aObserver);

public: // from CCoeControl

    /**
    * From CCoeControl. Return minimum size of the control.
    *
    * @return Minimum size of the control.
    */
    IMPORT_C virtual TSize MinimumSize();

    /**
    * From CCoeControl. Handles key events.
    * @param aKeyEvent The key event.
    * @param aType The type of the event.
    * @return Indicates whether or not the key event was used by this control.
    */
    IMPORT_C virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

    /**
    * Gets the list of logical colors employed in the drawing of the control,
    * paired with an explanation of how they are used. Appends the list into aColorUseList.
    */
	IMPORT_C virtual void GetColorUseListL(CArrayFix<TCoeColorUse>& aColorUseList) const;

    /**
    * Handles the resource change.
    **/
    void HandleResourceChange(TInt aType);  

    /**
    * From CCoeControl
    * handles pointer events of tabs
    * @param aPointerEvent pointerevent to be handled
    */
    IMPORT_C void HandlePointerEventL(const TPointerEvent& aPointerEvent);

public: // from MCoeControlObserver

    /**
    * From MCoeControlObserver.
    * @param aControl Control that caused the event.
    * @param aEventType Type of the event.
    */
    IMPORT_C virtual void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

public:	// from MCoeControlContext

    /** 
    * From MCoeControlContext. Allows to modify graphics context before Draw.
    *
    * @param aGc Graphics context to be modified.
    */
    IMPORT_C virtual void PrepareContext(CWindowGc& aGc) const;

public: 
    /*
    * From MAknNaviDecoratorObserver
	* Handles events from NaviDecorator
	* @param aEventID ID of event to be handled
	*/
	IMPORT_C virtual void HandleNaviDecoratorEventL( TInt aEventID );
	
public:	// from CCoeControl

    /**
    * From CCoeControl. Handle the size change events.
    */
	IMPORT_C virtual void SizeChanged();

    /**
    * From CCoeControl. Returns number of controls inside the context pane control.
    * @return Number of component controls.
    */
    IMPORT_C virtual TInt CountComponentControls() const;

    /**
    * From CCoeControl. Returns a control determined by control id.
    * @param anIndex Index of a control to be returned.
    * @return Pointer to control
    */
    IMPORT_C virtual CCoeControl* ComponentControl(TInt aIndex) const;

    /**
    * From CCoeControl. This is called when a focus is changed.
    */
    IMPORT_C virtual void FocusChanged(TDrawNow aDrawNow);

	/**
	* Reverse the order of tabs in tab array.
	*/
	void ReverseTabOrderL();
public: 
    /*
	* Reserved
	*/
	IMPORT_C virtual void Reserved1();
	
	/*
	* Reserved
	*/
    IMPORT_C virtual void Reserved2();
    
    /*
	* Reserved
	*/
    IMPORT_C virtual void Reserved3();
 
 
public:
	IMPORT_C CAknTabGroupAccess();

	/**
	* Tab groups can be mirrored depending on current locale. To client
	* application everything always seems like tabs are not mirrored but
	* the internal order of tabs in tab group may or may not be mirrored. 
	* If mirroring is off then this function just returns given
	* parameter as it is. If mirroring is on then this function returns 
	* unmirrored index if given index was mirrord and vice versa. Two 
	* nested calls to this function cancels its effect
	* (ConvertTabIndex(ConvertTabIndex(aIndex)) == aIndex).
	*
	* @param aIndex   index to be converted.
	* @return  converted index.
	*/
	TInt ConvertTabIndex(TInt aIndex) const;

public:
	IMPORT_C virtual void Draw(const TRect& aRect) const;
    
public:
    /**
    * From CAknControl
    */
    IMPORT_C void* ExtensionInterface( TUid aInterface );

public:
	enum TDirection
		{
		ELeft,
		ERight
		};

public:
	void ConstructL();
	void CommonAddTabL(CAknTab* aTab);
	void CommonReplaceTabL(CAknTab* aTab);
	void HandleTabStateChangedL(CCoeControl* aControl);
	TInt Index(TInt aTabId) const;
	TBool CycleFocus(TDirection aDirection,TDrawNow aDrawNow);
	TInt LeftMostTabIndex() const;
	TInt RightMostTabIndex() const;
	void DrawLowerBorder(TDrawNow aDrawNow) const;
	void DoDrawLowerBorder() const;
    void ReportNavigationEvent();
	void LoadTabBitmapsL(TInt aNumberOfTabsShown, TBool aLongTabs);
    TBool TabsVisibleInCurrentLayout() const; 
    void CurrentLayout( TAknWindowLineLayout& aLayout ) const;
	void LoadStaconTabBitmapsL();
    void InitTabGroupGraphicsL();
    
    void InitTabAnimationEventTimerL();
    void CancelTabAnimationEventTimer();    
    static TInt TabAnimationEvent(TAny * aPtr);
    void DoTabAnimationEvent();
    
    TBool NarrowTabLayoutNext();
    TBool NarrowTabLayout();    
    void SetDecoratorLayout(TBool aNarrow);

    void SizeChangedInNormalLayout();
    void SizeChangedInFlatLayout();
    void SizeChangedInNarrowLayout();
    void InitTabAnimationDoubleBufferL(TBool aCreateDoubleBuffer);

public: 
   
    /*
	* Used by the UI framework. Applications can set the narrow
	* layout using CAknNaviDecorator class.
	*
	*/
    void SetNarrowTabLayout(TBool aNarrow);
        

public:
	CArrayPtr<CAknTabAccess>* iTabArray;
	TInt iActiveTab;
	TRect iSpareRect;

	TInt iTabFixedWidth;
	TBool iLongTabs;
	TInt iNumberOfTabsShown;
	TInt iFirstShownTab;

	CFbsBitmap* iTabBitmaps[KTabNumberOfTabBitmaps];
	CFbsBitmap* iTabMaskBitmaps[KTabNumberOfTabBitmaps];

	TInt iBitmapNames[KTabNumberOfTabBitmaps];
	TInt iBitmapMaskNames[KTabNumberOfTabBitmaps];
    CAknTabGroupExtension*  iExtension; 

	MAknTabObserver* iTabObserver;
	TBool            iMirrored;
	};


#endif // AKNTABGRP_H
