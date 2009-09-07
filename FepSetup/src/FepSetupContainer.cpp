/*
* ============================================================================
*  Name     : CFepSetupContainer from FepSetupContainer.h
*  Part of  : FepSetup
*  Created  : 06/14/2006 by Forum Nokia 
*  Version  : 2.0
*  Copyright: Forum Nokia
* ============================================================================
*/

// INCLUDE FILES
#include "FepSetupContainer.h"

#include <eiklabel.h>			// for example label control
#include <f32file.h> // for TParseBase
#include <avkon.hrh>
#include <fepsetup.rsg>
#include "fepsetup.hrh"
#include <AknListQueryDialog.h> // for query dialog
#include <aknnotewrappers.h>
#include <coeutils.h>               // for ConeUtils::FileExists

#ifndef __SERIES60_3X__
_LIT( KT9FepFileName, "z:\\system\\fep\\t9fep.fep" );
_LIT( KAknFepFileName, "z:\\system\\fep\\aknfep.fep" );
#else
const TUid KAknFepUid = { 0x101FD65A }; // aknfep Uid
#endif

_LIT( KFepInstalled, "%S Installed!" );
_LIT( KFepUninstalled, "%S Uninstalled!" );
_LIT( KFepAlreadyInstalled, "%S Already Installed!" );
_LIT( KSelectOptions, "Select Options to\nInstall or Uninstall\nFepExample" );
_LIT( KNoFep, "No Fep Selected" );

const TUint KFormatBufSize = 256;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CFepSetupContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CFepSetupContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();

    iLabel = new (ELeave) CEikLabel;
    iLabel->SetContainerWindowL( *this );
    iLabel->SetTextL( KSelectOptions );

	iFepSetting = CFepGenericGlobalSettings::NewL(*iEikonEnv,
					TFepOnOrOffKeyData(EKeyEnter, EModifierFunc|EModifierShift, EModifierFunc),
					TFepOnOrOffKeyData(EKeyEnter, EModifierFunc|EModifierShift, EModifierShift),
					EFalse);

#ifndef __SERIES60_3X__ // Series60 1.x/2.x API
	iFileNamesOfAvailableFeps = iEikonEnv->FileNamesOfAvailableFepsL();

	// Get the number of all Feps available in the system
	const TInt numberOfAvailableFeps = iFileNamesOfAvailableFeps->MdcaCount();

	// Get the name, including the path, of all the Feps in the system
	iNamesOfAvailableFeps = new(ELeave) CDesCArrayFlat(numberOfAvailableFeps+1);
	for(TInt i=0; i<numberOfAvailableFeps; i++)
		{
		// Get name of fep excluding path
		iNamesOfAvailableFeps->AppendL(TParsePtrC(iFileNamesOfAvailableFeps->MdcaPoint(i)).Name());
		}

#else // Series60 3.x API

	// Get available Fep Uids
	iEikonEnv->AvailableFepsL( iFepUids, NULL );
	iNamesOfAvailableFeps = new(ELeave) CDesCArrayFlat( iFepUids.Count() + 1 );
	// Get Fep names also
	iEikonEnv->AvailableFepsL( iFepUids, iNamesOfAvailableFeps );
	for (int i = 0; i < iFepUids.Count(); ++i) {
	  TBuf<50> buf;
	  buf.AppendNum(iFepUids[i].iUid, EHex);
	  buf.Append(' ');
	  buf.Append(iNamesOfAvailableFeps->MdcaPoint(i));
	  RDebug::Print(buf);
	}
#endif

    SetRect(aRect);
    ActivateL();
    }

// Destructor
CFepSetupContainer::~CFepSetupContainer()
    {
    delete iLabel;
	delete iFepSetting;
#ifndef __SERIES60_3X__	
	delete iFileNamesOfAvailableFeps;
#else
	iFepUids.Close();
#endif	
	delete iNamesOfAvailableFeps;

    }

// ---------------------------------------------------------
// CFepSetupContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CFepSetupContainer::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    iLabel->SetExtent( TPoint(30,50), iLabel->MinimumSize() );
    }

// ---------------------------------------------------------
// CFepSetupContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CFepSetupContainer::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CFepSetupContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CFepSetupContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            return iLabel;
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CFepSetupContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CFepSetupContainer::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
    // TODO: Add your drawing code here
    // example code...
    gc.SetPenStyle(CGraphicsContext::ENullPen);
    gc.SetBrushColor(KRgbGray);
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.DrawRect(aRect);
    }

// ---------------------------------------------------------
// CFepSetupContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CFepSetupContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }

// ---------------------------------------------------------
// CFepSetupContainer::InstallFepL
// ---------------------------------------------------------
//
void CFepSetupContainer::InstallFepL()
	{
	TBuf<KFormatBufSize> formatingBuf;

    TInt index( 0 );
    CAknListQueryDialog* dlg = new(ELeave) CAknListQueryDialog( &index );
    dlg->PrepareLC( R_FEPSETUP_LIST_QUERY );
    dlg->SetItemTextArray( iNamesOfAvailableFeps );
    dlg->SetOwnershipType( ELbmDoesNotOwnItemArray );

    // Execute the dialog
	TInt answer = dlg->RunLD();

    if( answer == EAknSoftkeyOk )
		{
#ifndef __SERIES60_3X__ // Series60 1.x/2.x API
		iEikonEnv->InfoMsg( TParsePtrC( iFileNamesOfAvailableFeps->MdcaPoint( index )).Name());

		// The code below is to check whether the selected Fep is already installed.
		HBufC* fepToInstall = iFileNamesOfAvailableFeps->MdcaPoint( index ).AllocL();
		CleanupStack::PushL( fepToInstall );
		TPtr fepToInstallPtr( fepToInstall->Des() );
        fepToInstallPtr.LowerCase(); // remove case-sensitivity
		TPtrC fepToInstallPtrC( TParsePtrC( fepToInstallPtr ).Name()); // file name parsing

		HBufC* currentFep = iCoeEnv->NameOfInstalledFepL();
		
		CleanupStack::PushL( currentFep );
		TPtr currentFepPtr( currentFep->Des() );
        currentFepPtr.LowerCase(); // remove case-sensitivity
		TPtrC currentFepPtrC( TParsePtrC( currentFepPtr ).Name()); // file name parsing

		if( fepToInstallPtrC.Compare( currentFepPtrC ) == 0) // Already installed?
			{
			formatingBuf.Format( KFepAlreadyInstalled, &fepToInstallPtrC );
			}
		else
			{
			// Loads the specified FEP DLL into all running applications.
			// The current loaded FEP, if any, will be unloaded.
			iCoeEnv->InstallFepL( iFileNamesOfAvailableFeps->MdcaPoint( index ));

			formatingBuf.Format( KFepInstalled, &fepToInstallPtrC );
			}

		CAknConfirmationNote* confirmationDialog = new(ELeave)CAknConfirmationNote();
		confirmationDialog->ExecuteLD( formatingBuf );

		CleanupStack::PopAndDestroy( 2 ); // currentFep, fepToInstall;

#else // Series60 3.x API

		TPtrC fepToInstallPtr( TParsePtrC( iNamesOfAvailableFeps->MdcaPoint( index )).Name()); // file name parsing
		iEikonEnv->InfoMsg( fepToInstallPtr );

		// The code below is to check whether the selected Fep is already installed.
		TUid fepToInstallUid = iFepUids[index];
		TUid currentFepUid = iEikonEnv->FepUid();
		
		if( currentFepUid == fepToInstallUid )
			{
			formatingBuf.Format( KFepAlreadyInstalled, &fepToInstallPtr );
			}
		else
			{
			// Loads the specified FEP DLL into all running applications.
			// The current loaded FEP, if any, will be unloaded.
			iCoeEnv->InstallFepL( fepToInstallUid );

			formatingBuf.Format( KFepInstalled, &fepToInstallPtr );
			}
		CAknConfirmationNote* confirmationDialog = new(ELeave)CAknConfirmationNote();
		confirmationDialog->ExecuteLD( formatingBuf );			
#endif		
		}
	else
		{
		iEikonEnv->InfoMsg( KNoFep );
		}
	}

// ---------------------------------------------------------
// CFepSetupContainer::UnInstallFepL
// ---------------------------------------------------------
//
void CFepSetupContainer::UnInstallFepL()
	{
	TBuf<KFormatBufSize> formatingBuf;

#ifndef __SERIES60_3X__ // Series60 1.x/2.x API
	HBufC* currentFep = iCoeEnv->NameOfInstalledFepL();
	CleanupStack::PushL( currentFep );
	
	TPtr currentFepPtr( currentFep->Des() );
    currentFepPtr.LowerCase(); // remove case-sensitivity
	TPtrC currentFepPtrC( TParsePtrC( currentFepPtr ).Name()); // file name parsing

	if( ConeUtils::FileExists( KAknFepFileName )) // 2ndfp2 platform and newer use aknfep.fep
		{
		if( currentFepPtrC.Compare( TParsePtrC( KAknFepFileName ).Name() ) != 0 ) // Already installed?
			{
			iCoeEnv->InstallFepL( KAknFepFileName ); // Install the default Fep
			formatingBuf.Format( KFepUninstalled, &currentFepPtrC );
			}
		else
			{
			formatingBuf.Format( KFepAlreadyInstalled, &currentFepPtrC );
			}
		}
	else // older platforms use T9fep.fep
		{
		if( currentFepPtrC.Compare( TParsePtrC( KT9FepFileName ).Name() ) != 0 ) // Already installed?
			{
			iCoeEnv->InstallFepL( KT9FepFileName ); // Install the default Fep
			formatingBuf.Format( KFepUninstalled, &currentFepPtrC );
			}
		else
			{
			formatingBuf.Format( KFepAlreadyInstalled, &currentFepPtrC );
			}
		}

	CAknConfirmationNote* dialog = new(ELeave)CAknConfirmationNote();
	dialog->ExecuteLD( formatingBuf );

	CleanupStack::PopAndDestroy(); // currentFep
	
#else // Series60 3.x API

	TUid currentFepUid = iEikonEnv->FepUid();
	TInt defaultIndex = iFepUids.Find( KAknFepUid );
	TPtrC defaultFepPtr( TParsePtrC( iNamesOfAvailableFeps->MdcaPoint( defaultIndex )).Name()); // file name parsing
	if( currentFepUid == KAknFepUid )  // Already installed?
		{
		formatingBuf.Format( KFepAlreadyInstalled, &defaultFepPtr );
		}
	else
		{
		iCoeEnv->InstallFepL( KAknFepUid );  // Install the default Fep

		formatingBuf.Format( KFepInstalled, &defaultFepPtr );
		}
	CAknConfirmationNote* confirmationDialog = new(ELeave)CAknConfirmationNote();
	confirmationDialog->ExecuteLD( formatingBuf );			
#endif	
	}

// End of File
