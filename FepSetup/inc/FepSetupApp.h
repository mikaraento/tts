/*
* ============================================================================
*  Name     : CFepSetupApp from FepSetupApp.h
*  Part of  : FepSetup
*  Created  : 06/14/2006 by Forum Nokia 
*  Version  : 2.0
*  Copyright: Forum Nokia
* ============================================================================
*/

#ifndef FEPSETUPAPP_H
#define FEPSETUPAPP_H

// INCLUDES
#include <aknapp.h>

// CONSTANTS
// UID of the application
#ifndef __SERIES60_3X__
const TUid KUidFepSetup = { 0x00003473 };
#else
const TUid KUidFepSetup = { 0xE0003473 };
#endif
// CLASS DECLARATION

/**
* CFepSetupApp application class.
* Provides factory to create concrete document object.
* 
*/
class CFepSetupApp : public CAknApplication
    {
    
    public: // Functions from base classes
    private:

        /**
        * From CApaApplication, creates CFepSetupDocument document object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
        
        /**
        * From CApaApplication, returns application's UID (KUidFepSetup).
        * @return The value of KUidFepSetup.
        */
        TUid AppDllUid() const;
    };

#endif

// End of File

