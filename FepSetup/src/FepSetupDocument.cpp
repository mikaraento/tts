/*
* ============================================================================
*  Name     : CFepSetupDocument from FepSetupDocument.cpp
*  Part of  : FepSetup
*  Created  : 06/14/2006 by Forum Nokia 
*  Version  : 2.0
*  Copyright: Forum Nokia
* ============================================================================
*/

// INCLUDE FILES
#include "FepSetupDocument.h"
#include "FepSetupAppUi.h"

// ================= MEMBER FUNCTIONS =======================

// constructor
CFepSetupDocument::CFepSetupDocument(CEikApplication& aApp)
: CAknDocument(aApp)    
    {
    }

// destructor
CFepSetupDocument::~CFepSetupDocument()
    {
    }

// EPOC default constructor can leave.
void CFepSetupDocument::ConstructL()
    {
    }

// Two-phased constructor.
CFepSetupDocument* CFepSetupDocument::NewL(
        CEikApplication& aApp)     // CFepSetupApp reference
    {
    CFepSetupDocument* self = new (ELeave) CFepSetupDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }
    
// ----------------------------------------------------
// CFepSetupDocument::CreateAppUiL()
// constructs CFepSetupAppUi
// ----------------------------------------------------
//
CEikAppUi* CFepSetupDocument::CreateAppUiL()
    {
    return new (ELeave) CFepSetupAppUi;
    }

// End of File  
