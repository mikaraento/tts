/*
* ============================================================================
*  Name     : CFepSetupContainer from FepSetupContainer.h
*  Part of  : FepSetup
*  Created  : 06/14/2006 by Forum Nokia 
*  Version  : 2.0
*  Copyright: Forum Nokia
* ============================================================================
*/

#ifndef FepSetupContainer_H
#define FepSetupContainer_H

// INCLUDES
#include <coecntrl.h>
#include <fepbase.h>
   
// FORWARD DECLARATIONS
class CEikLabel;        // for example labels

// CLASS DECLARATION

/**
*  CFepSetupContainer  container control class.
*  
*/
class CFepSetupContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect);

        /**
        * Destructor.
        */
        ~CFepSetupContainer();

    public: // New functions
		void InstallFepL();
		void UnInstallFepL();

    public: // Functions from base classes

    private: // Functions from base classes

       /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

       /**
        * From CoeControl,CountComponentControls.
        */
        TInt CountComponentControls() const;

       /**
        * From CCoeControl,ComponentControl.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

       /**
        * From CCoeControl,Draw.
        */
        void Draw(const TRect& aRect) const;

       /**
        * event handling section
        * e.g Listbox events
        */
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
        
    private: //data
        
        CEikLabel* iLabel;
		CFepGenericGlobalSettings* iFepSetting;		// Defined in FEPBASE.H
		CDesCArray* iNamesOfAvailableFeps;
#ifndef __SERIES60_3X__
		MDesCArray*	iFileNamesOfAvailableFeps;
#else		
		RArray<TUid> iFepUids;
#endif
    };

#endif

// End of File
