/*
* ============================================================================
*  Name     : AknSfld.h
*  Part of  : Avkon
*
*  Description:
*  Search Field support. 
*
*  Version:
*
*  Copyright © 2002 Nokia Corporation.
*  This material, including documentation and any related 
*  computer programs, is protected by copyright controlled by 
*  Nokia Corporation. All rights are reserved. Copying, 
*  including reproducing, storing,  adapting or translating, any 
*  or all of this material requires the prior written consent of 
*  Nokia Corporation. This material also contains confidential 
*  information which may not be disclosed to others without the 
*  prior written consent of Nokia Corporation.
* ============================================================================
*/

#ifndef _MODIFIED_AKNSFLD_H__
#define _MODIFIED_AKNSFLD_H__

// INCLUDES
#include <AknControl.h>
#include <gulicon.h>
#include <eikgted.h>

// FORWARD DECLARATIONS
class CEikEdwin;
class CGlobalText;
class CAknInputFrame;
class CAknSearchFieldIndicator;
class CAknsListBoxBackgroundControlContext;
class CAknsFrameBackgroundControlContext;
class CEikListBox;

 
// CLASS DECLARATION

/**
* The CAknSearchField class implements a Search field control. There are four
* different styles for the search field defined by a TSearchFieldStyle.
*
* @since Series 60 0.9
*/
NONSHARABLE_CLASS(CAknSearchFieldAccess) : public CAknControl,
                        public MCoeControlObserver,
                        public MEikCommandObserver
{
public:

    /**
    * Search field style.
    */
    enum TSearchFieldStyle 
        {
        /** Use only ESearch in Series 60 */
        ESearch,
        /** (Not used) */        
        EInput,
        /** (Not used) */
        EUrl,  
        /** (Not used) */
        EPhone,
        /** (Not used) */
        EPlain,
        /** Use as popup search field. */
        EPopup,         
        /** Use as pinb search field. */
        EPinb,          
        /** For Clock application/list_single_heading_pane. */
        EClockApp,      
        /** Same as ESearch, but using CreateWindowL(). */
        EFixed,         
        /** Layout where no line is used. */
        ESearchWithoutLine, 
        /** Popup window layout. */
        EPopupWindow
        };
public:

    /**
    * Destructor.
    */
    IMPORT_C ~CAknSearchFieldAccess();

    /**
    * Two-phased constructor.
    * @param aParent Reference to parent control.
    * @param aFieldStyle Defines the UI style.
    * @param aIcon A pointer to the icon to be shown in the search box frame.
    *              Takes ownerships. Can be @c NULL.
    * @param aTextLimit Defines the maximum text length.
    * @return A pointer to contructed @c CAknSearchField object.
    */
    IMPORT_C static CAknSearchFieldAccess* NewL(const CCoeControl& aParent, TSearchFieldStyle aFieldStyle, CGulIcon* aIcon, TInt aTextLimit);

public:                 // for search string manipulation
    /**
    * Returns the length of search text.
    * @return The length of search text.
    */
    IMPORT_C TInt TextLength() const;

    /**
    * Returns the text of search field by writing it into the 
    * buffer aSearchTxt.
    * @param aSearchTxt Reference to the text buffer. Buffer size must be same
    *                   or greater than maximum text length, which is given in 
    *                   constructor.
    */
    IMPORT_C void GetSearchText(TDes& aSearchTxt) const;

    /**
    * Sets text into search field.
    * @param aSearchTxt Reference to the text buffer.
    */
    IMPORT_C void SetSearchTextL(const TDesC& aSearchTxt);

    /**
    * Selects the whole search text.
    */
    IMPORT_C void SelectSearchTextL();

    /**
    * Clears text in the search field.
    */
    IMPORT_C void ResetL();

    /**
    * Sets additional info text to the search field.
    * @param aText Reference to the text buffer.
    */
    IMPORT_C void SetInfoTextL(const TDesC& aText);

    /**
    * Calls editor's @c ClipboardL() function.
    * @param aClipboardFunc Clipboard function.
    */
    IMPORT_C void ClipboardL(CEikEdwin::TClipboardFunc aClipboardFunc);

public: // new methods for Avkon Skins support

    /**
    * Sets whether skins are enabled on this control. The default value is 
    * determined from global setting from @c CAknAppUi.
    * @since Series 60 2.0 
    * @param aEnabled @c ETrue if skins are enabled, @c EFalse if not.
    */
    IMPORT_C void SetSkinEnabledL( const TBool aEnabled );

public: // from MCoeControlObserver

    /**
    * From @c MCoeControlObserver, Acts upon changes in the hosted control's state.
    * This class's implementation is trivial and should be able to be safely 
    * re-implemented in directly client-derived classes. For non-base setting
    * page classes, a call to the base class should be made.
    * @param aControl Control that caused the event. (not used in default implementation)
    * @param aEventType Type of the event. 
    */
    void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

public: // From CCoeControl

    /**
    * From @c CCoeControl, Handles key event.
    * @param aKeyEvent The key event. 
    * @param aType The type of the event.
    * @return Indicates whether the key event was used by this control or not.
    */
    IMPORT_C TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

    /**
    * From @c CCoeControl, Returns minimum size. Minimum size depends on 
    * zoom factor. 
    * @return Minimum size.
    */
    IMPORT_C TSize MinimumSize();

public:

    /**
    * Sets position of the line on main pane.
    * @since Series 60 2.0
    * @param aLinePos The position as defined in @c TAknSeparatorLinePos
    */
    IMPORT_C void SetLinePos(TInt aLinePos);

    /**
    * Access to the edwin owned by the search field.
    * @since Series 60 2.6
    * @return Reference to the edwin owned by the search field.
    */
    IMPORT_C CEikEdwin& Editor() const;

public: // from MObjectProvider
    TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
public:
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
    void SizeChanged();
    void FocusChanged(TDrawNow aDrawNow);
public:
    void ConstructL(const CCoeControl& aParent, TSearchFieldStyle aFieldStyle, CGulIcon* aIcon, TInt aTextLimit);
    CAknSearchFieldAccess(); 
public:
    void ZoomEditorL();
public: // from MEikCommandObserver
    IMPORT_C void ProcessCommandL(TInt aCommandId);

public:
    void UpdatePopupCBAL();
    void RestorePopupCBA();
    void SetupSkinContextL();
public:

    /**
    * Sets a list box for this control.
    * @since Series 60 2.6
    * @param aListBox A pointer to a list box object.
    */
    void SetListbox(CEikListBox* aListBox);

    /**
    * Sets the parent control for this control.
    * @since Series 60 2.6
    * @param aParent A pointer to the parent control.
    */
    void SetParentCtrl(CCoeControl* aParent);

    /**
    * Indicates that is the control popup or not.
    * @since Series 60 2.6
    * @return @c ETrue if it is a popup, @c EFalse if not.
    */
    TBool IsPopup();

public:
    CEikEdwin* iEditor;
    CGlobalText* iGlobalText;
    CAknInputFrame* iInputFrame;
    CAknSearchFieldIndicator* iIndicator; 
    CAknsListBoxBackgroundControlContext* iSkinContext;
    TInt iFlags;  // was TBool iUseSkinContext;
    TInt iLinePos;
    CAknsFrameBackgroundControlContext* iInputContext;
    TBool iIsPopup;
    TBool iCBAObserverUpdated;
    CEikListBox *iListBox; // not owned
    CCoeControl *iParent; // not owned
    CEikButtonGroupContainer *iCba; // this is only for checking whether cba changed while we're storing modifications in it.
    };

#endif
