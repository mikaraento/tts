/*
* ============================================================================
*  Name     : AknView.h
*  Part of  : Avkon
*
*  Description:
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
// AknView.h
//
// Copyright (c) 1997-2001 Symbian Ltd.  All rights reserved.
//

#ifndef AKNVIEW_H
#define AKNVIEW_H

//  INCLUDES
#include <babitflags.h>
#include <coecntrl.h>
#include <eikmobs.h>
#include <coeaui.h>
#include <coeview.h>
#include <eikdef.h>
#include <coemop.h>

// FORWARD DECLARATIONS
class CAknViewAppUi;
class CEikStatusPane;
class CEikButtonGroupContainer;
class CAknControlStack;
class CAknViewExtension;
class CAknToolbar;

// CLASS DECLARATION

/**
 * This is an abstract base class for a Series 60 application view.
 *
 * @since Series 60 0.9
 */
class CAknView : public CBase, public MCoeView, public MEikMenuObserver, 
                public MObjectProvider
	{
public:

    /** 
     * Standard constructor.
     */
	IMPORT_C CAknView();

    /** 
     * Standard destructor.
     */	
	IMPORT_C ~CAknView();

    /** 
     * This is a second stage constructor initializing this view with standard 
     * values. Should be called from derived classes second stage construction
     * functions.  
     * @param aResId The numeric ID of the resource (@c iViewInfo) to be read. 
     *        If aResId=0, resource file is not accessed.
     */
	IMPORT_C void BaseConstructL(TInt aResId=0);
	
    /** 
     * Constructs invisible button group container and menu if they don't exist
     * yet. They are placed on the view stack. To be used by views which need 
     * to access the menu or cba before the view is activated for the first 
     * time.
     */	
	IMPORT_C void ConstructMenuAndCbaEarlyL();		

    /** 
     * Activate any view in the product. Calls @c CCoeAppUi::ActivateViewL(
     * aViewId)
     * @param aViewId Identifies the view to activate.
     */
	IMPORT_C void ActivateViewL(const TVwsViewId& aViewId);

    /** 
     * Activate any view, then passes the message text @c aCustomMessage of 
     * type @c aCustomMessageId. Calls @c CCoeAppUi::ActivateViewL(
     * aViewId,aCustomMessageId,aCustomMessage) 
     * @param aViewId Identifies the view to activate.
     * @param aCustomMessageId Specifies the message type.
     * @param aCustomMessage The message passed to the activated view.
     */
	IMPORT_C void ActivateViewL(const TVwsViewId& aViewId,
	                            TUid aCustomMessageId,
	                            const TDesC8& aCustomMessage);

    /**
    * Returns views id, intended for overriding by sub classes.
    * @return id for this view.
    */
	IMPORT_C virtual TUid Id() const =0;

    /** 
     * Event handler for status pane size changes.
     * @c CAknView provides an empty implementation for sub classes that do 
     * not want to handle this event.
     */
	IMPORT_C virtual void HandleStatusPaneSizeChange();

    /** 
     * From @c MCoeView. The full view Id for this view. Calls @c Id()
     * @return View identification number.
     */
	IMPORT_C TVwsViewId ViewId() const;

	/** 
     * From @c MEikMenuObserver. This function processes user commands by 
     * handling @c aCommand values @c EAknSoftkeyOptions, @c EAknCmdExit, 
     * @c EEikCmdCanceled and passing the others to @c HandleCommandL() 
     * for sub class to decide the actions.  
     * @param aCommand ID of the command to respond to. 
     */	 
	IMPORT_C virtual void ProcessCommandL(TInt aCommand);

    /** 
     * Command handling function intended for overriding by sub classes. 
     * Default implementation is empty.  
     * @param aCommand ID of the command to respond to. 
     */
	IMPORT_C virtual void HandleCommandL(TInt aCommand);

    /** 
     * Menu bar accessor method. This returns the views menu if available, 
     * otherwise it returns the applications menu bar.
     * @return  @c CEikMenuBar pointer to views menu bar.
     */
	IMPORT_C virtual CEikMenuBar* MenuBar() const;

	/** 
     * Calculates the client rectangle of the view, taking the applications 
     * client rectangle and the views CBA into account.
     * Calls @c CEikAppUi::ClientRect() 
     * @return @c TRect the area of the screen available to the application 
     *          for drawing.
     */
	IMPORT_C TRect ClientRect() const;

    /** 
     * This function causes the menu bar to disappear from the screen until it
     * is invoked again by the user.
     * Calls @c CEikAppUi::StopDisplayingMenuBar().
     */
	IMPORT_C void StopDisplayingMenuBar();

    /** 
     * Determines whether the application is foreground. 
     * @return @c ETrue if view is foreground.
     */
	IMPORT_C TBool IsForeground() const;

    /**
    * Called by the framework when view's rectangle changes. The default
    * implementation is empty. This function is not used yet.
    * @since Series 60 3.0
    */
    IMPORT_C virtual void HandleViewRectChange();
	
    /**
    * Asks the view to redraw its area. Needs to be implemented only by
    * views used in split view architecture. The default implementation is
    * empty. This function is not used yet.
    * @since Series 60 3.0
    */
    IMPORT_C virtual void Redraw() const;


    /** 
     * Avkon view architecture system. Internal use only.
     * Calls @c DoActivateL.
     */	 
	IMPORT_C virtual void AknViewActivatedL(const TVwsViewId& aPrevViewId,
	                                        TUid aCustomMessageId,
	                                        const TDesC8& aCustomMessage);

    /** 
     * Avkon view architecture system. Internal use only. Function is called 
     * by @c CAknViewAppUi::~CAknViewAppUi() for immediate view deactivation. 
     * Calls @c DoDeactivate.     
     */  
	void AknViewDeactivated();

    /** 
     * Avkon view architecture system. Internal use only.
     */ 
	void ProcessForegroundEventL(TBool aForeground);

public: // formerly from @c MTopSetMember<CEikMenuBar>
	IMPORT_C virtual void Reserved_MtsmPosition();
	IMPORT_C virtual void Reserved_MtsmObject();

public:

    /** 
     * Accessor for the applications user interface as a @c CAknViewAppUi.
     * @return Pointer to user interface instance.
     */
	IMPORT_C CAknViewAppUi* AppUi() const;

    /** 
     * Status pane accessor method.
     * @return Pointer to status pane. 
     */
	IMPORT_C CEikStatusPane* StatusPane() const;

    /** 
     * Button group container (=CBA) accessor method. 
     * This returns the views CBA if there is one, and the applications user 
     * interfaces CBA otherwise.
     * @return Pointer to @c CEikButtonGroupContainer.
     */
	IMPORT_C CEikButtonGroupContainer* Cba() const;

    /** 
     * Handles a view activation and passes the message of type 
     * @c aCustomMessageId. This function is intended for overriding by 
     * sub classes. This function is called by @c AknViewActivatedL().
     * @param aPrevViewId Specifies the view previously active.
     * @param aCustomMessageId Specifies the message type.
     * @param aCustomMessage The activation message.
     */
	IMPORT_C virtual void DoActivateL(const TVwsViewId& aPrevViewId,
	                                  TUid aCustomMessageId,
	                                  const TDesC8& aCustomMessage) = 0;

    /** 
     * View deactivation function intended for overriding by sub classes. 
     * This function is called by @c AknViewDeactivated().
     */
	virtual void DoDeactivate() = 0;

public: // From MCoeView.

    /** 
     * From @c MCoeView. Takes any action required when this view is activated.
     * Handles this view activation event from view server. Calls 
     * @c CAknViewAppUi::ViewActivatedL()
     * @param aPrevViewId Specifies the view previously active.
     * @param aCustomMessageId Specifies the message type.
     * @param aCustomMessage Activation message.
     */
	IMPORT_C void ViewActivatedL(const TVwsViewId& aPrevViewId,
	                             TUid aCustomMessageId,
	                             const TDesC8& aCustomMessage);

    /** 
     * From @c MCoeView. Takes any action that is required following 
     * deactivation of this view. Handles this view deactivation event from
     * viewserver. Calls @c CAknViewAppUi::ViewDeactivated()
     */
	IMPORT_C void ViewDeactivated();

    /** 
     * From @c MCoeView. Screen device changed function from viewserver.
     * @return TVwsViewIdAndMessage created with default values.
     */
	IMPORT_C TVwsViewIdAndMessage ViewScreenDeviceChangedL();

public: // from MObjectProvider

    /** 
     * From @c MObjectProvider. Gets an (@c MAknsControlContext) object whose 
     * type is encapsulated by the specified TTypeUid object. Calls 
     * @c SupplyMopObject( @c TTypeUid aId, 
     *      @c CEikButtonGroupContainer* iCba, @c CEikMenuBar* iMenu ). 
     * @param aId Encapsulates the @c Uid that identifies the type of object 
     *         required. 
     * @return Pointer to the @c MAknsControlContext object provided. Note that
     *          the pointer may be @c NULL. 
     */
	IMPORT_C TTypeUid::Ptr MopSupplyObject(TTypeUid aId);

    /** 
     * From @c MObjectProvider. Gets the parent object provider.   
     * @return Pointer to the parent object provider (@c MObjectProvider) that
     *          is @c CAknViewAppUi* iAppUi, or @c NULL.
     */
	IMPORT_C MObjectProvider* MopNext();

public: // From MEikMenuObserver.

    /** 
     * From @c MEikMenuObserver. Menu emphasising or de-emphasising function. 
     * @c CEikMenuBar objects call this on their observer. Updates the value of
     * the flags for the (@c aMenuControl) on the control stack. 
     * @param aMenuControl The control to be emphasised or de-emphasisied.
     * @param aEmphasis @c ETrue to emphasize the menu, @c EFalse to refuse the
     *          focus.
     */
	IMPORT_C virtual void SetEmphasis(CCoeControl* aMenuControl,
	                                  TBool aEmphasis);

    /** 
     * From @c MEikMenuObserver. Foreground event handling function intended 
     * for overriding by sub classes.  
     * @param aForeground Indicates the required focus state of the control.
     */	
	IMPORT_C virtual void HandleForegroundEventL(TBool aForeground);

public: // internal functions
	void ConstructMenuAndCbaL(TBool aVisible);

	/**
	* Asserts that extension class object exists.
	* @return Extension object.
	*/
	CAknViewExtension* Extension() const;
	
public: // new functions

    /**
	* @since Series 60 3.1
	* @return toolbar object.
	*/
	IMPORT_C CAknToolbar* Toolbar() const;
	
	/** 
     * This function causes toolbar to be hidden
     * @since Series 60 3.1
     */
	IMPORT_C void StopDisplayingToolbar();
		
public:  // Data
	// The follow object is not owned by this class. iCoeEnv is protected in 
	// keeping with @c CCoeAppUi & @c CCoeControl
	CCoeEnv* iCoeEnv;

public:
	CAknViewAppUi* iAppUi;

	// @c SAknViewInfo is the C++ image of resource structure AVKON_VIEW
	struct SAknViewInfo
		{
	public:
        TUint8 iVersion;    // resource struct version
		TInt iHotKeys;		// resource id
		TInt iMenu;			// resource id
		TInt iCba;			// resource id
        TInt iToolbar;      // resource id
        TInt iExtension;    // resource id
		};

	// The following member data are owned by this class
	SAknViewInfo iViewInfo;
	CEikMenuBar* iMenu;
	CEikButtonGroupContainer* iCba;
	TBitFlags iFlags;
    CAknViewExtension* iExtension;
	};

#endif      // AKNVIEW_H
