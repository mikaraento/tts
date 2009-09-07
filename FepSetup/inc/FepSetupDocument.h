/*
* ============================================================================
*  Name     : CFepSetupDocument from FepSetupDocument.h
*  Part of  : FepSetup
*  Created  : 06/14/2006 by Forum Nokia 
*  Version  : 2.0
*  Copyright: Forum Nokia
* ============================================================================
*/

#ifndef FEPSETUPDOCUMENT_H
#define FEPSETUPDOCUMENT_H

// INCLUDES
#include <akndoc.h>
   
// CONSTANTS

// FORWARD DECLARATIONS
class  CEikAppUi;

// CLASS DECLARATION

/**
*  CFepSetupDocument application class.
*/
class CFepSetupDocument : public CAknDocument
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CFepSetupDocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        */
        virtual ~CFepSetupDocument();

    public: // New functions

    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * EPOC default constructor.
        */
        CFepSetupDocument(CEikApplication& aApp);
        void ConstructL();

    private:

        /**
        * From CEikDocument, create CFepSetupAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
    };

#endif

// End of File
