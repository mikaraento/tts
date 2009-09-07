/*
* ============================================================================
*  Name     : CFepSetupApp from FepSetupApp.cpp
*  Part of  : FepSetup
*  Created  : 06/14/2006 by Forum Nokia
*  Version  : 2.0
*  Copyright: Forum Nokia
* ============================================================================
*/

// INCLUDE FILES
#ifdef __SERIES60_3X__
#include <eikstart.h>
#endif
#include    "FepSetupApp.h"
#include    "FepSetupDocument.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CFepSetupApp::AppDllUid()
// Returns application UID
// ---------------------------------------------------------
//
TUid CFepSetupApp::AppDllUid() const
    {
    return KUidFepSetup;
    }


// ---------------------------------------------------------
// CFepSetupApp::CreateDocumentL()
// Creates CFepSetupDocument object
// ---------------------------------------------------------
//
CApaDocument* CFepSetupApp::CreateDocumentL()
    {
    return CFepSetupDocument::NewL( *this );
    }

// ================= OTHER EXPORTED FUNCTIONS ==============
//
// ---------------------------------------------------------
// NewApplication()
// Constructs CFepSetupApp
// Returns: created application object
// ---------------------------------------------------------
//
EXPORT_C CApaApplication* NewApplication()
    {
    return new CFepSetupApp;
    }

#ifndef __SERIES60_3X__
// ---------------------------------------------------------
// E32Dll(TDllReason)
// Entry point function for EPOC Apps
// Returns: KErrNone: No error
// ---------------------------------------------------------
//
GLDEF_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }

#else
// ---------------------------------------------------------
// E32Main()
// Entry point function for new (>= 9.0) EPOC Apps (exe)
// Returns: Sistem Wide error codes or KErrNone if all goes well
// ---------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }
#endif


// End of File
