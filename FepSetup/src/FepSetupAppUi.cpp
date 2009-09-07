/*
* ============================================================================
*  Name     : CFepSetupAppUi from FepSetupAppUi.cpp
*  Part of  : FepSetup
*  Created  : 06/14/2006 by Forum Nokia
*  Version  : 2.0
*  Copyright: Forum Nokia
* ============================================================================
*/

// INCLUDE FILES
#include "FepSetupAppUi.h"
#include "FepSetupContainer.h"
#include <FepSetup.rsg>
#include "fepsetup.hrh"

#include <avkon.hrh>

#define KEnableSkinFlag 0x1000
// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------------
// CFepSetupAppUi::ConstructL()
// ?implementation_description
// ----------------------------------------------------------
//

void CFepSetupAppUi::ConstructL()
    {
#ifdef __SERIES60_3X__
    BaseConstructL(EAknEnableSkin);
#else
    BaseConstructL(KEnableSkinFlag);
#endif
    iAppContainer = new (ELeave) CFepSetupContainer;
    iAppContainer->SetMopParent(this);
    iAppContainer->ConstructL( ClientRect() );
    AddToStackL( iAppContainer );
    }

// ----------------------------------------------------
// CFepSetupAppUi::~CFepSetupAppUi()
// Destructor
// Frees reserved resources
// ----------------------------------------------------
//
CFepSetupAppUi::~CFepSetupAppUi()
    {
    if (iAppContainer)
        {
        RemoveFromStack( iAppContainer );
        delete iAppContainer;
        }
   }

// ------------------------------------------------------------------------------
// CFepSetupAppUi::::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
//  This function is called by the EIKON framework just before it displays
//  a menu pane. Its default implementation is empty, and by overriding it,
//  the application can set the state of menu items dynamically according
//  to the state of application data.
// ------------------------------------------------------------------------------
//
void CFepSetupAppUi::DynInitMenuPaneL(
    TInt /*aResourceId*/,CEikMenuPane* /*aMenuPane*/)
    {
    }

// ----------------------------------------------------
// CFepSetupAppUi::HandleKeyEventL(
//     const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
// ?implementation_description
// ----------------------------------------------------
//
TKeyResponse CFepSetupAppUi::HandleKeyEventL(
    const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/)
    {
    return EKeyWasNotConsumed;
    }

// ----------------------------------------------------
// CFepSetupAppUi::HandleCommandL(TInt aCommand)
// ?implementation_description
// ----------------------------------------------------
//
void CFepSetupAppUi::HandleCommandL(TInt aCommand)
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
        case EEikCmdExit:
            {
            Exit();
            break;
            }
        case EFepSetupCmdAppInstallFep:
            {
            iAppContainer->InstallFepL();
            break;
            }
        case EFepSetupCmdAppUninstallFep:
            {
            iAppContainer->UnInstallFepL();
            break;
            }
        default:
            break;
        }
    }

void CFepSetupAppUi::HandleResourceChangeL(TInt aType)
    {
    CAknAppUi::HandleResourceChangeL(aType); //call to upper class

    // ADDED FOR SCALABLE UI SUPPORT
    // *****************************
    //if ( aType == KEikDynamicLayoutVariantSwitch )
    //hard coded constant so it can be compiled with first edition

    if ( aType == 0x101F8121 )
        {
        iAppContainer->SetRect( ClientRect() );
        }
    }

// End of File
