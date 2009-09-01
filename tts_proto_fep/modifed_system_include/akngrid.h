/*
* =============================================================================
*  Name        : AknGrid.h
*  Part of     : Avkon
*
*  Description : 
*     This is a concrete class for the handling of a grid. The class handles a
*     rectangular grid arrangement of items held in any linear ordering i.e 
*     cells ordered top to bottom and left, left to right and down etc.
*
*  Version     : 
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
* =============================================================================
*/

#if !defined(_MODIFIED_AKNGRID_H__)
#define _MODIFIED_AKNGRID_H__

// INCLUDES
#include <eiktxlbx.h>
#include <eiklbv.h>
#include <eikfrlbd.h>
#include <eikfrlb.h>

#include <AknGridM.h>
#include <AknGridView.h>
#include <Aknappui.h>

// CLASS PREDEFINITIONS
class CAknGridExtension;

// CLASS DECLARATION

/**
*  Application grid handling class from Avkon.
*  Provides support for ordering application grids items.
*
*  @since Series 60 0.9
*/
class CAknGridAccess : public CEikListBox
	{
public:// enums
	enum TIndicatorEvent
		{
		EMove,
		EChangeNumOfItems,
		EResize
		};
public:

	/**
	* Base class default constructor.
	* @return A pointer to a new @c CAknGrid object.
	*/
	IMPORT_C CAknGridAccess();
	
	/**
	* Destructor.
	*/
	IMPORT_C virtual ~CAknGridAccess();
	
	/**
	* This function gives @c CAknGridM class pointer to @c CAknGrid class. 
	* Usually, the @c CAknGridM class object is instantiated automatically 
	* during the construction phase of the @c CAknGrid object. If an 
	* application programmer provides their own grid model class, the 
	* application programmer must instantiate their own grid model class object
	* and give it to the @c CAknGrid object using the @c SetModel function 
	* before calling the @c ConstructL/ConstructFromResourceL function.
	* @param aModel Pointer to @c CAknGridM class.
	*/	
	IMPORT_C void SetModel(CAknGridM* aModel);

	/**
	* This is Symbian default constructor. The @c ConstructL function needs to
	* be called in case resource data is not used. If a leave occurs the 
	* framework generates a Symbian Leave code.
	* @param aPatent A CCoeControl pointer.
	* @param aFlags Parameter for @c CEikListBox constructor. If the 
	* parameter is missing default value is 0.
	*/	
	IMPORT_C void ConstructL(const CCoeControl* aParent, TInt aFlags = 0);

	/**
	* The @c ConstructFromResourceL function needs to be called in case 
	* resource data is used. Usually, the @c CAknGridM class object is 
	* instantiated automatically during the construction phase of the 
	* @c CAknGrid object. If an application programmer provides their own grid
	* model class, the application programmer must instantiate their own grid 
	* model class object and give it to the @c CAknGrid object using the 
	* @c SetModel function before calling the 
	* @c ConstructL/ConstructFromResourceL function. If a leave occurs the 
	* framework generates a Symbian Leave code.
	* @param aReader Construct controls from resource file.
	*/
 	IMPORT_C void ConstructFromResourceL(TResourceReader& aReader);

	/**
	* Sets the orientation of the grid, either vertical or horizontal, the
	* ordering of the data and the size of the primary dimension of the
	* grid. The value for the parameter @c aNumOfItemsInPrimaryOrient must be
	* greater than zero since this determines the number of items (be it rows
	* or columns) in the primary orientation of the grid. If a leave occurs
	* the framework generates a Symbian Leave code.
	* @param aVerticalOrientation Items vertical orientation.
	* @param aLeftToRight @c ETrue left to right.
	* @param aTopToBottom @c ETrue top to bottom.
	* @param aNumOfItemsInPrimaryOrient Number of items in primary orient.  
	* @param aNumOfItemsInSecondaryOrient Number of items in Secondary orient.
	* @param aSizeOfItems Item size.
	* @param aWidthOfSpaceBetweenItems =0 Width of space between items.
	* @param aHeightOfSpaceBetweenItems =0 Height of space between items.
	*/
	IMPORT_C void SetLayoutL(TBool aVerticalOrientation,
							 TBool aLeftToRight,
							 TBool aTopToBottom,
							 TInt aNumOfItemsInPrimaryOrient,
							 TInt aNumOfItemsInSecondaryOrient,
							 TSize aSizeOfItems,
							 TInt aWidthOfSpaceBetweenItems=0,
							 TInt aHeightOfSpaceBetweenItems=0);

	/**
	* Sets the layout from a resource. Layout includes orientation (either
	* vertical or horizontal), horizontal and vertical direction of numbering,
	* the number of items in the primary and secondary orientation, and the 
	* primary and secondary scrolling types. If a leave occurs the framework 
	* generates a Symbian Leave code.
	* @param aReader Constructs controls from a resource file. 
	*/
	IMPORT_C void SetLayoutFromResourceL(TResourceReader& aReader);

	/**
	* Sets the movement of the cursor with respect to scrolling when the
	* end item in the current row or column, whichever is the primary
	* orientation of the data items, is encountered. The movement maybe
	* either stop, loop back to same row or column or move onto the
	* next logical data item in the sequence.
	* @param aScrollingType Items scrolling type enum definition.
	*/
	IMPORT_C void SetPrimaryScrollingType(CAknGridView::TScrollingType aScrollingType);
	
	/**	
	* Sets the movement of the cursor with respect to scrolling when the
	* end item in the secondary dimension of the grid is encountered.
	* The movement maybe either stop, loop back back to same row or column
	* or move onto the next logical data item in the sequence.
	* @param aSecondaryScrolling Secondary scrolling type enum definition.
	*/
	IMPORT_C void SetSecondaryScrollingType(CAknGridView::TScrollingType aSecondaryScrolling);

	/**
	* Sets the starting position of the data within the grid. A blank page 
	* cannot be accessed (since cannot move into empty cells) so  a totally 
	* blank page is the same as if the page never existed since the user 
	* cannot scroll into it. For this reason it is suggested that the start
	* position be no more than one page into the grid. If a leave occurs 
	* the framework generates a Symbian Leave code.
	* @param aGridStartPosition Parameter manipulate the grid's starting 
	* position.
	*/
	IMPORT_C void SetStartPositionL(TPoint aGridStartPosition);

	/**
    * @c CurrentDataIndex retrieves the index of the selection. In grids, you 
    * should use this method instead of @c CEikListBox::CurrentItemIndex(), 
    * when you are manipulating data stored in the grid's @c ItemTextArray. 
    * While @c CurrentItemIndex() returns the same value as 
    * @c CurrentItemIndex() on most phones, there will be difference in some 
    * language variants where text reading/writing direction is different from 
    * left-to-right.
    * @return A current position of highlight.
    */
	IMPORT_C TInt CurrentDataIndex() const;
	
    /**
    * Moves the cursor to the required grid data index.
    * @param aDataIndex Data index value.
    */
	IMPORT_C void SetCurrentDataIndex(TInt aDataIndex);

	/**
	* The grid position function. Gives the data index by using grid's 
	* position. The position must be given from the top left corner. If 
	* the function returns -1 the item's position is invalid.
	* @param aGridPosition The data item's co-ordinate values. 
	* @return Activated item's index value.
	*/
	IMPORT_C TInt IndexOfPosition(TPoint aGridPosition) const;
	
	/**
	* The grid position function. Gives the data item's co-ordinate values 
	* by using item's index value.
	* @param aItemIndex The data item's index value.   
	* @return @c TPoint co-ordinate values of active item.
	*/
	IMPORT_C TPoint PositionAtIndex(TInt aItemIndex) const;

	/**
	* Item drawer. Gets the pointer to the grid class.
	* @return A pointer to @c CFormattedCellListBoxItemDrawer object.
	*/
    IMPORT_C CFormattedCellListBoxItemDrawer* ItemDrawer() const;

	/**
	* Function sets a grid text to the data item. If a leave occurs the 
	* framework generates a Symbian Leave code.
	* @since Series S60 3.0
	* @param aText Descriptor parameter.
	*/
    IMPORT_C void SetEmptyGridTextL(const TDesC& aText);
    
   	/**
	* This function sets the empty grid text.
	* @return A pointer to the grid text descriptor.
	*/
	inline const TDesC* EmptyGridText() const;

public:

   	/**
	* This function creates a new object and returns pointer to it. If the 
	* leave occurs the framework generates a Symbian Leave code.
	* @return A pointer to @c CListBoxView class.
	*/
    IMPORT_C virtual CListBoxView* MakeViewClassInstanceL();

   	/**
	* This function sets the column width of the grid. Column width cannot be 
	* set in a horizontal grid since the number of columns in the grid is 
	* defined by the initialising call to @c SetLayoutL. The column width 
	* cannot be larger than the width of the viewing rectangle.
	* @param aColumnWidth A parameter defines a width of the column.
	*/
	IMPORT_C void SetColumnWidth(TInt aColumnWidth);
	
   	/**
	* This function gives a width of the column.
	* @return A width of the column. 
	*/
   	IMPORT_C TInt ColumnWidth() const;

public: //from CEikListBox

   	/**
	* This function updates the scrollbars, including item position. This 
	* function is called when the size of the scrollbars or data changes. If 
	* the leave occurs the framework generates a Symbian Leave code.
	*/
	IMPORT_C virtual void UpdateScrollBarsL();
	
   	/**
	* This function should be called after one or more items have been added to
	* the grid. If a leave occurs the framework generates a Symbian Leave 
	* code.
	*/	
	IMPORT_C void HandleItemAdditionL();
	
	/**
	* This function should be called after one or more items have been removed 
	* from the grid. If a leave occurs the framework generates a Symbian 
	* Leave code.
	*/	
	IMPORT_C void HandleItemRemovalL();
	
	/**
	* This function is from @c CEikAppUi, handles key events. If a leave 
	* occurs the framework generates a Symbian Leave code.
	* @param aKeyEvent Event to handled.
	* @param aType of the key event.
	* @return Response code ( @c EKeyWasConsumed, @c EKeyWasNotConsumed )
	*/	
	IMPORT_C virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	
   	/**
	* This function sets the row height of the grid. Row height cannot be set
	* in a vertical grid since the number of rows in the grid is defined by 
	* the initialising call to @c SetLayoutL. The row height cannot be larger 
	* than the height of the viewing rectangle. If a leave occurs the 
	* framework generates a Symbian Leave code. 
	* @param aHeight The height of the item's rows.
	*/	
	IMPORT_C void SetItemHeightL(TInt aHeight);
	
	/**
	* This function handles size changes. This routine assumes that 
	* @c SetLayoutL has been called to set up the grid.
	*/	
	IMPORT_C void SizeChanged();
	
   	/**
	* This function creates a new object and returns pointer to 
	* @c CTextListBoxModel class.
	* @return A pointer to @c CTextListBoxModel object.
	*/	
	IMPORT_C CTextListBoxModel* Model() const;
		
	/**
	* This function handles viewable rectangle.
	* @param aRect Rectangle parameter. 
	*/	
	IMPORT_C void SetRect(const TRect& aRect);
	
	/**
	* This function is called when the grid's items, item's data and scroll 
	* bars has been changed. This implementation ensures that the current 
	* item is visible after resize. If a leave occurs the framework generates
	* a Symbian Leave code.
	*/	
	IMPORT_C virtual void HandleViewRectSizeChangeL();
	
	/**
	* This function sets top item index.
	* @param aItemIndex Item index value.
	*/	
	IMPORT_C virtual void SetTopItemIndex(TInt aItemIndex) const;
	
	/**
	* This function handles resource changes.
	* @since Series S60 2.6.
	* @param aType Message type.
	*/	
	IMPORT_C virtual void HandleResourceChange(TInt aType);
	
	/**
	* Indicates whether the control should be redrawn now. If @c ENoDrawNow, 
	* the function has no immediately effect. EDrawNow redraw control 
	* immediately.
	* @since Series S60 3.0.
	*/
    IMPORT_C void FocusChanged(TDrawNow aDrawNow);

public: // From CCoeControl
    IMPORT_C void HandlePointerEventL(const TPointerEvent& aPointerEvent);

public: // from CEikListBox

	/**
	* This function scroll horizontal by one column when the left/right scroll
	* arrows (i.e. the nudge buttons) are tapped.
	* @return A nudge value when the buttons are pressed.
	*/
	IMPORT_C virtual TInt HorizontalNudgeValue() const;
	
	/**
	* This function gets the granularity for horizontal scrolls. The 
	* granularity is the minimum size of a horizontal move of the client area.
	* @return A grain size for horizontal scrolling in pixels. 
	*/	
	IMPORT_C virtual TInt HorizScrollGranularityInPixels() const;
	
	/**
	* This function called by various functions of this class to ensure that
	* the top item index is always a sane value. The implementation in 
	* @c CEikListBox tries to ensure the minimum amount of white space at the 
	* bottom of the list box. Note that this function does not affect the 
	* current item index.
	*/	
	IMPORT_C virtual void AdjustTopItemIndex() const;
	
	/** 
	* This function handles drag events. If a leave occurs the framework 
	* generates a Symbian Leave code.
	* @param aPointerPos The position of the @c TPointerEvent for which this 
	* handler is invoked.
	*/
	IMPORT_C virtual void HandleDragEventL(TPoint aPointerPos);

	/** 
	* This function calculates the client area. This method is called by 
	* various functions of this class to recalculate the extent of the client
	* area from @c iViewRect. This implementation takes into account any 
	* rounding of the viewing rectangle made to fit a whole number of items.
	* @param aClientRect On return contains a size for the client area in 
	* pixels. 
	*/
	IMPORT_C virtual void RestoreClientRectFromViewRect(TRect& aClientRect) const;

	/** 
	* This function rounds down the height of the rectangle (if necessary) so 
	* that only a whole number of items can be displayed inside the list box.
	* @param aRect Rectangle parameter.
	*/
	IMPORT_C virtual TInt AdjustRectHeightToWholeNumberOfItems(TRect& aRect) const;

	/**
	* Updates the position of grids scroll bars’ thumbs to reflect the vertical 
	* position of selector.
	*/
	IMPORT_C virtual void UpdateScrollBarThumbs() const;
	
	/**
	* Gets a count of the component controls of this grid.
	* @return The number of component controls.
	*/
	IMPORT_C virtual TInt CountComponentControls() const;

public:

	/**
	* Moves to the next or previous item on the grid. If a leave occurs the 
	* framework generates a Symbian Leave code.
	* @param aPoint Co-ordinate object parameter.
	*/
	IMPORT_C void MoveToNextOrPreviousItemL(TPoint aPoint);
	
	/**	
	* This protected function used by functions to check/alter the dimensions
	* of the grid as data items are added or removed or the size of the items
	* are altered. It also assumes that @c SetLayoutL has been called. 
	* This will not leave if scrollbars have both been turned off. If a leave 
	* occurs the framework generates a Symbian Leave code.
	*/
	IMPORT_C virtual void CalcGridSizeL();
	
	/**
	* Creates a formatted list item drawer. If a leave occurs the framework 
	* generates a Symbian Leave code.
	*/
    IMPORT_C virtual void CreateItemDrawerL();

public:
	__DECLARE_TEST;

	// grid model helper
	inline CAknGridM* GridModel() const;

	// grid view helper
	inline CAknGridView* GridView() const;

    /**
    * This function sets the size and initial layout of items. If a leave 
    * occurs the framework generates a Symbian Leave code.
	* @param aOrientation
	* @param aHorizontal
	* @param aVertical
	* @param aNumOfItemsInPrimaryOrient
	* @param aNumOfItemsInSecondaryOrient
	* @param aSizeOfItems
	* @param aWidthOfSpaceBetweenItems
	* @param aHeightOfSpaceBetweenItems
    */
	void DoSetLayoutL(TInt aOrientation,
					  TInt aHorizontal,
					  TInt aVertical,
					  TInt aNumOfItemsInPrimaryOrient,
					  TInt aNumOfItemsInSecondaryOrient,
					  TSize aSizeOfItems,
					  TInt aWidthOfSpaceBetweenItems=0,
					  TInt aHeightOfSpaceBetweenItems=0);

public:
    /**
    * From CAknControl
    */
    IMPORT_C void* ExtensionInterface( TUid aInterface );
public: // from MListBoxModel
	IMPORT_C virtual TAny* MListBoxModel_Reserved();
	
public: 
    /**
	* From @c MEikScrollBarObserver
	*
    * This function handles scroll events caused by scroll bar. 
    * It updates grid's view by event and new thumb position. 
    *
	* @param aScrollBar pointer to scroll bar sent this event
	* @param aEventType type of event
    */
	IMPORT_C virtual void HandleScrollEventL(CEikScrollBar* aScrollBar, TEikScrollEvent aEventType);
	
public:
	TInt  iMinColWidth;
	TBool iCurrentIsValid;
	TInt  iNumOfColsInView;
	TInt  iNumOfRowsInView;
	TSize iSpaceBetweenItems;
	TInt iNumOfItemsInPrimaryOrient;
	TBitFlags iOrientationFlags;
	TBitFlags iHorizontalFlags;
	TBitFlags iVerticalFlags;
	CAknGridExtension *iExtension; // spare taken to use as extension class
	};

#endif //__AKNGRID_H__
