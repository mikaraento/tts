// EIKLBX.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
   
#if !defined(_MODIFIED_EIKLBX_H__)
#define _MODIFIED_EIKLBX_H__

#if !defined(__GDI_H__)
#include <gdi.h>
#endif

#if !defined(__EIKBCTRL_H__)
#include <eikbctrl.h>
#endif

#if !defined(__EIKLBO_H__)
#include <eiklbo.h>
#endif

#if !defined(__EIKSBFRM_H__)
#include <eiksbfrm.h>
#endif

#if !defined(__EIKLBM_H__)
#include <eiklbm.h>
#endif

#if !defined(__EIKLBV_H__)
#include <eiklbv.h>
#endif

#if !defined(__GULBORDR_H__)
#include <gulbordr.h>
#endif

#if !defined(__EIKLBED_H__)
#include <eiklbed.h>
#endif

#if !defined(__GULUTIL_H__)
#include <gulutil.h>
#endif

#if !defined(__LAFPUBLC_H__)
#include <lafpublc.h>
#endif

enum TKeyCode;
class RIncrMatcherBase;
class CListItemDrawer;
class CEikScrollBarFrame;
class CEikButtonBase;
class CMatchBuffer;
class CListBoxExt;
class CEikListBox;

class CEikListBoxAccess : public CEikBorderedControl, public MEikScrollBarObserver
	{
public:
	friend class CListBoxExt;
public:
	enum TFlags
		{
		EMultipleSelection			= SLafListBox::EMultipleSelection,
		ENoExtendedSelection		= SLafListBox::ENoExtendedSelection,
		EIncrementalMatching		= SLafListBox::EIncrementalMatching,
		EPopout						= SLafListBox::EPopout,
		ELeftDownInViewRect			= SLafListBox::ELeftDownInViewRect,
		EItemDoubleClicked			= SLafListBox::EItemDoubleClicked,
		EKeepModel					= SLafListBox::EKeepModel,
		EScrollBarSizeExcluded		= SLafListBox::EScrollBarSizeExcluded,
		EStateChanged				= SLafListBox::EStateChanged,
		ECreateOwnWindow			= SLafListBox::ECreateOwnWindow,
        ENoFirstLetterMatching      = SLafListBox::ENoFirstLetterMatching,
 		EPaintedSelection			= SLafListBox::EPaintedSelection ,
		ELoopScrolling = 0x1000,
		EEnterMarks = 0x2000,		// Avkon multiselection list
		EShiftEnterMarks = 0x4000,	// Avkon markable list
		EViewerFlag = 0x8000,		// combined the two flags to fit to WORD.
		EPageAtOnceScrolling = 0x8000, // Avkon viewers
		EDisableHighlight = 0x8000,  // Avkon viewers		
 		ES60StyleMultiselection     = SLafListBox::ES60StyleMultiselection, 		
 		ES60StyleMarkable           = SLafListBox::ES60StyleMarkable
		};
	enum {KEikMaxMatchingBufferLength = 2};
	enum TScrollBarOwnerShip
	    {
	    ENotOwnedExternally=0x0000,
	    EOwnedExternally   =0x0001
		};
public:
	enum TReasonForFocusLost
		{ EFocusLostToExternalControl, EFocusLostToInternalEditor };
public:
	// construction/initialization
	IMPORT_C ~CEikListBoxAccess();
	IMPORT_C CEikListBoxAccess();
	IMPORT_C void ConstructL(MListBoxModel* aListBoxModel,CListItemDrawer* aListItemDrawer,const CCoeControl* aParent,TInt aFlags = 0);
	IMPORT_C void ConstructL(MListBoxModel* aListBoxModel,CListItemDrawer* aListItemDrawer,const CCoeControl* aParent, TGulBorder aBorder, TInt aFlags = 0);

	IMPORT_C virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	IMPORT_C virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	IMPORT_C virtual void SetContainerWindowL(const CCoeControl& aContainer);
	IMPORT_C virtual TSize MinimumSize();
	IMPORT_C virtual void SetDimmed(TBool aDimmed);

	IMPORT_C virtual void HandleScrollEventL(CEikScrollBar* aScrollBar, TEikScrollEvent aEventType);

	// model/view access functions 
	IMPORT_C MListBoxModel* Model() const;
	IMPORT_C CListBoxView* View() const;

	// functions for accessing top/current/bottom item index
	IMPORT_C TInt TopItemIndex() const;	 
	IMPORT_C virtual void SetTopItemIndex(TInt aItemIndex) const;
	IMPORT_C TInt BottomItemIndex() const;
	IMPORT_C TInt CurrentItemIndex() const;	  
	IMPORT_C void SetCurrentItemIndex(TInt aItemIndex) const;
	IMPORT_C void SetCurrentItemIndexAndDraw(TInt aItemIndex) const;

	// functions for dealing with the selection state
	IMPORT_C const CListBoxView::CSelectionIndexArray* SelectionIndexes() const;
	IMPORT_C void SetSelectionIndexesL(CListBoxView::CSelectionIndexArray* aArrayOfSelectionIndexes);
	IMPORT_C void ClearSelection();	

    // Functions for updating a listbox's internal state after its model has
    // been updated, all of them will emit item change event to item change
    // observers.
	IMPORT_C void HandleItemAdditionL();
	IMPORT_C void HandleItemRemovalL();
	IMPORT_C void HandleItemAdditionL(CArrayFix<TInt> &aArrayOfNewIndexesAfterAddition);
	IMPORT_C void HandleItemRemovalL(CArrayFix<TInt> &aArrayOfOldIndexesBeforeRemoval);
	IMPORT_C void Reset();

    /**
    * Adds an item change observer to the listbox. Duplicates are not checked
    * (i.e. adding the same observer multiple times is not prevented).
    *
    * @since 3.0
    * @param aObserver Must be non-NULL.
    */
    IMPORT_C void AddItemChangeObserverL( MListBoxItemChangeObserver* aObserver );
    /**
    * Removes an item change observer from the listbox.
    *
    * @since 3.0
    * @param aObserver The observer to be removed.
    * @return ETrue if removal ok, EFalse if observer was not removed (not
    *         found from the list of observers).
    */
    IMPORT_C TBool RemoveItemChangeObserver( MListBoxItemChangeObserver* aObserver );

	// functions for accessing the item height
	IMPORT_C virtual void SetItemHeightL(TInt aHeight);
	IMPORT_C TInt ItemHeight() const;

	// functions for scrollbars
	IMPORT_C CEikScrollBarFrame* CreateScrollBarFrameL(TBool aPreAlloc=EFalse);

	IMPORT_C void SetScrollBarFrame(CEikScrollBarFrame* aScrollBarFrame, TScrollBarOwnerShip aOwnerShip);
	IMPORT_C TBool IsScrollBarFrameOwnedExternally() const;

	IMPORT_C CEikScrollBarFrame* const ScrollBarFrame();
	IMPORT_C virtual void UpdateScrollBarsL();

	// construction support functions
	IMPORT_C void CalculatePopoutRect(TInt aTargetItemIndex, TInt aTargetYPos, TRect& aListBoxRect, TInt aMinHeightInNumOfItems = 1);
	IMPORT_C TSize CalcSizeInPixels(TInt aWidthAsNumOfChars, TInt aHeightAsNumOfItems) const;
	IMPORT_C TInt CalcWidthBasedOnNumOfChars(TInt aNumOfChars) const;
	IMPORT_C TInt CalcHeightBasedOnNumOfItems(TInt aNumOfItems) const;
	IMPORT_C TInt CalcWidthBasedOnRequiredItemWidth(TInt aTextWidthInPixels) const;

	// drawing/scrolling functions
	IMPORT_C void DrawItem(TInt aItemIndex) const;
	IMPORT_C void ScrollToMakeItemVisible(TInt aItemIndex) const;

	// observer support
	IMPORT_C void SetListBoxObserver(MEikListBoxObserver* aObserver);

	IMPORT_C TInt VerticalInterItemGap() const;

	// popouts only
	IMPORT_C void SetLaunchingButton(CEikButtonBase* aButton);

    // Editing support
    IMPORT_C void SetItemEditor(MEikListBoxEditor* aEditor);
    IMPORT_C void ResetItemEditor();
    IMPORT_C MEikListBoxEditor* ItemEditor();
    IMPORT_C virtual void EditItemL(TInt aMaxLength);
    IMPORT_C void StopEditingL(TBool aUpdateModel);

	// functions needed for Avkon shortcuts, 
	// passing information from one list to another
	IMPORT_C virtual TInt ShortcutValueForNextList();
	IMPORT_C virtual void SetShortcutValueFromPrevList(TInt aValue);

    // pop-up positioning support
    IMPORT_C TRect HighlightRect() const;

public:	// from CCoeControl
	IMPORT_C virtual void GetColorUseListL(CArrayFix<TCoeColorUse>& aColorUseList) const; // not available before Release 005u
	IMPORT_C virtual void HandleResourceChange(TInt aType);			// not available before Release 005u
	IMPORT_C virtual void ActivateL();
	IMPORT_C TCoeInputCapabilities InputCapabilities() const;

public:
    /**
    * From CAknControl
    */
    IMPORT_C void* ExtensionInterface( TUid aInterface );

public:
	// Shortcuts need access to Incremental matching
	// The shortcuts will be used inside OfferkeyEventL().
	friend class AknListBoxShortCutsImplementation;
	// Avkon layout uses SetVerticalMargin, which is protected.
	friend class AknListBoxLayouts;

	IMPORT_C virtual void FocusChanged(TDrawNow aDrawNow);
	IMPORT_C virtual void SizeChanged();
	IMPORT_C virtual void HandleViewRectSizeChangeL();
	IMPORT_C virtual TInt CountComponentControls() const;
	IMPORT_C virtual CCoeControl* ComponentControl(TInt aIndex) const;

	// functions that implement first letter and incremental matching
	IMPORT_C void CreateMatchBufferL();  // utility function called from RestoreL() of subclasses such as CEikStandardListBox
	IMPORT_C void ClearMatchBuffer() const;
	IMPORT_C void MatchTypedCharL(TUint aCode);
	IMPORT_C void UndoLastChar();
	IMPORT_C TBool LastCharMatched() const;

	// functions needed for supporting scrollbars
	IMPORT_C virtual void UpdateScrollBarThumbs() const;
	IMPORT_C virtual TInt HorizScrollGranularityInPixels() const;
	IMPORT_C virtual TInt HorizontalNudgeValue() const;
	IMPORT_C virtual void AdjustTopItemIndex() const;

	// navigation support functions
	IMPORT_C void SimulateArrowKeyEventL(TKeyCode aKeyCode);
	IMPORT_C virtual void HandleLeftArrowKeyL(CListBoxView::TSelectionMode aSelectionMode);
	IMPORT_C virtual void HandleRightArrowKeyL(CListBoxView::TSelectionMode aSelectionMode);

	// construction support functions
	IMPORT_C void RestoreCommonListBoxPropertiesL(TResourceReader& aReader); // utility function called from RestoreL() of subclasses such as CEikStandardListBox
	IMPORT_C virtual void ConstructL(const CCoeControl* aParent,TInt aFlags = 0);
	IMPORT_C virtual void CreateViewL();
	IMPORT_C virtual CListBoxView* MakeViewClassInstanceL();
	IMPORT_C void SetViewRectFromClientRect(const TRect& aClientRect);
	IMPORT_C virtual void RestoreClientRectFromViewRect(TRect& aClientRect) const;
	IMPORT_C virtual TInt AdjustRectHeightToWholeNumberOfItems(TRect& aRect) const;

	// accessor for Laf members
	IMPORT_C TMargins8 ListBoxMargins() const;

	// various accessors for private data members
	IMPORT_C TInt HorizontalMargin() const;
	IMPORT_C TInt VerticalMargin() const;
	IMPORT_C void SetHorizontalMargin(TInt aMargin);
	IMPORT_C void SetVerticalMargin(TInt aMargin);
	IMPORT_C RIncrMatcherBase* MatchBuffer() const;
	IMPORT_C TInt ViewRectHeightAdjustment() const;
	IMPORT_C TRgb BackColor() const;
	IMPORT_C void SetViewRectHeightAdjustment(TInt aAdjustment);

	// misc functions
	IMPORT_C virtual void ReportListBoxEventL(MEikListBoxObserver::TListBoxEvent aEvent);
	IMPORT_C virtual void Draw(const TRect& aRect) const;
	IMPORT_C void ClearMargins() const;
	IMPORT_C virtual void UpdateCurrentItem(TInt aItemIndex) const;
	IMPORT_C virtual void HandleDragEventL(TPoint aPointerPos);
	IMPORT_C TBool ItemExists(TInt aItemIndex) const;
	IMPORT_C void DrawMatcherCursor() const;

	IMPORT_C static TInt InterItemGap();

	IMPORT_C void UpdateViewColors();
	IMPORT_C void UpdateItemDrawerColors();

    /**
    * Notifies item change observers about item change. Subclasses must call
    * this method if they have implemented item handling functions (e.g.
    * HandleItemAdditionL or HandleItemRemovalL).
    * @since 3.0
    */
    IMPORT_C void FireItemChange();

public:	// functions which deal with extension

	IMPORT_C void SetReasonForFocusLostL(TReasonForFocusLost aReasonForFocusLost);
	IMPORT_C TReasonForFocusLost ReasonForFocusLostL();
	IMPORT_C TBool IsMatchBuffer() const;
	void CheckCreateExtensionL();
	TBool CheckCreateExtension();
	void CheckCreateBufferL();
	CMatchBuffer* Buffer() const;
public:
	IMPORT_C void CreateScrollBarFrameLayout(TEikScrollBarFrameLayout& aLayout) const;
public: // from CCoeControl
    /* 
    * Returns EventModifiers of pointerevent, when handling pointerevents
    */
	IMPORT_C TInt EventModifiers();

    /* 
    * Returns ETrue if list has ES60StyleMultiselection flag. 
    */
	IMPORT_C TBool IsMultiselection();
public:
	IMPORT_C virtual void CEikListBox_Reserved(); // listbox use only
	void HorizontalScroll(TInt aScrollAmountInPixels);
	void DrawItemInView(TInt aItemIndex) const;
	void ClearMargins(CWindowGc& aGc) const;
	TKeyResponse DoOfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	void UpdateScrollBarColors(CEikScrollBar* aScrollBar);
	void UpdateScrollBarsColors();

	void HandleItemRemovalWithoutSelectionsL();
 
public:
	TInt iListBoxFlags;
	CListBoxView* iView;
	CListItemDrawer* iItemDrawer;
	MListBoxModel* iModel;
	TInt iItemHeight; 	
	CEikScrollBarFrame* iSBFrame;
	TScrollBarOwnerShip iSBFrameOwned;
	TInt iRequiredHeightInNumOfItems;
	CEikButtonBase* iLaunchingButton; // only used by popouts
	MEikListBoxObserver* iListBoxObserver;
public:
	TRgb iBackColor;
//	TInt iHorizontalMargin;
//	TInt iVerticalMargin;
	TMargins8 iMargins ;
	CListBoxExt* iListBoxExt;
	TInt iViewRectHeightAdjustment;
    MEikListBoxEditor* iItemEditor;
	TBool* iLbxDestroyed;
	TBool iLastCharMatched;
	TInt iSpare;
	};

#endif
