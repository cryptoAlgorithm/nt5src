/*++

Copyright (c) 1999-2001  Microsoft Corporation

Abstract:

    @doc
    @module softwrp.cxx | Implementation of CVssSoftwareProviderWrapper
    @end

Author:

    Adi Oltean  [aoltean]  03/11/2001

TBD:
	
	Add comments.

Revision History:

    Name        Date        Comments
    aoltean     03/11/2001  Created

--*/


/////////////////////////////////////////////////////////////////////////////
//  Includes


#include "stdafx.hxx"
#include "resource.h"

#include "vs_inc.hxx"

// Generated file from Coord.IDL
#include "vs_idl.hxx"

#include "softwrp.hxx"
#include "vssmsg.h"

////////////////////////////////////////////////////////////////////////
//  Standard foo for file name aliasing.  This code block must be after
//  all includes of VSS header files.
//
#ifdef VSS_FILE_ALIAS
#undef VSS_FILE_ALIAS
#endif
#define VSS_FILE_ALIAS "CORSOFTC"
//
////////////////////////////////////////////////////////////////////////


IVssSnapshotProvider* CVssSoftwareProviderWrapper::CreateInstance(
    IN VSS_ID ProviderId,
	IN CLSID ClassId,
    IN bool bCallOnLoad
    ) throw(HRESULT)
{
    CVssFunctionTracer ft(VSSDBG_COORD, L"CVssSoftwareProviderWrapper::CreateInstance");

    // Ref count becomes 1
    CComPtr<CVssSoftwareProviderWrapper> pWrapper = new CVssSoftwareProviderWrapper();
    if (pWrapper == NULL)
        ft.Throw( VSSDBG_COORD, E_OUTOFMEMORY, L"Memory allocation error");

	// Create the IVssSoftwareSnapshotProvider interface
	ft.hr = pWrapper->m_pSoftwareItf.CoCreateInstance(ClassId, NULL, CLSCTX_LOCAL_SERVER);
	if ( ft.HrFailed() ) {
	    ft.LogError(VSS_ERROR_CREATING_PROVIDER_CLASS, VSSDBG_COORD << ClassId << ft.hr );
		ft.Throw( VSSDBG_COORD, VSS_E_UNEXPECTED_PROVIDER_ERROR, L"CoCreateInstance failed with hr = 0x%08lx", ft.hr);
	}
	BS_ASSERT(pWrapper->m_pSoftwareItf);

	// Query the creation itf.
    ft.hr = pWrapper->m_pSoftwareItf->SafeQI( IVssProviderCreateSnapshotSet, &(pWrapper->m_pCreationItf) );
    if (ft.HrFailed()) {
        ft.TranslateProviderError(VSSDBG_COORD, ProviderId, L"QI for IVssProviderCreateSnapshotSet");
    }
	BS_ASSERT(pWrapper->m_pCreationItf);

	// Query the notification itf.
	// Execute the OnLoad, if needed
    ft.hr = pWrapper->m_pSoftwareItf->SafeQI( IVssProviderNotifications, &(pWrapper->m_pNotificationItf) );
    if (ft.HrSucceeded()) {
	    BS_ASSERT(pWrapper->m_pNotificationItf);
        if (bCallOnLoad) {
		    ft.hr = pWrapper->m_pNotificationItf->OnLoad(NULL);
		    if (ft.HrFailed()) 
                ft.TranslateProviderError(VSSDBG_COORD, ProviderId, L"IVssProviderNotifications::OnLoad");
        }
    } else if (ft.hr != E_NOINTERFACE) {
        BS_ASSERT(false);
        ft.TranslateProviderError(VSSDBG_COORD, ProviderId, L"QI for IVssProviderNotifications");
    }

    // return the created interface
    // Ref count is still 1
    return pWrapper.Detach();
}


/////////////////////////////////////////////////////////////////////////////
// IUnknown

STDMETHODIMP CVssSoftwareProviderWrapper::QueryInterface(REFIID iid, void** pp)
{
    CVssFunctionTracer ft(VSSDBG_COORD, L"CVssSoftwareProviderWrapper::QueryInterface");
    
    if (pp == NULL)
        return E_INVALIDARG;
    if (iid != IID_IUnknown)
        return E_NOINTERFACE;

    AddRef();
    IUnknown** pUnk = reinterpret_cast<IUnknown**>(pp);
    (*pUnk) = static_cast<IUnknown*>(this);
	return S_OK;
};

STDMETHODIMP_(ULONG) CVssSoftwareProviderWrapper::AddRef()
{
    CVssFunctionTracer ft(VSSDBG_COORD, L"CVssSoftwareProviderWrapper::AddRef");
    ft.Trace(VSSDBG_COORD, L"Provider Wrapper AddRef(%p) %lu --> %lu", this, m_lRef, m_lRef+1);
    
    return ::InterlockedIncrement(&m_lRef);
};

STDMETHODIMP_(ULONG) CVssSoftwareProviderWrapper::Release()
{
    CVssFunctionTracer ft(VSSDBG_COORD, L"CVssSoftwareProviderWrapper::Release");
    ft.Trace(VSSDBG_COORD, L"Provider Wrapper Release(%p) %lu --> %lu", this, m_lRef, m_lRef-1);
    
    LONG l = ::InterlockedDecrement(&m_lRef);
    if (l == 0)
        delete this; // We suppose that we always allocate this object on the heap!
    return l;
};


/////////////////////////////////////////////////////////////////////////////
//  IVssSoftwareSnapshotProvider


STDMETHODIMP CVssSoftwareProviderWrapper::SetContext(
	IN		LONG     lContext
	)
{
    BS_ASSERT(m_pSoftwareItf);
    return m_pSoftwareItf->SetContext(lContext);
}


STDMETHODIMP CVssSoftwareProviderWrapper::GetSnapshotProperties(
	IN      VSS_ID			SnapshotId,
	OUT 	VSS_SNAPSHOT_PROP	*pProp
	)
{
    BS_ASSERT(m_pSoftwareItf);
    return m_pSoftwareItf->GetSnapshotProperties(
                SnapshotId,
                pProp
                );
}


STDMETHODIMP CVssSoftwareProviderWrapper::Query(                                      
    IN      VSS_ID          QueriedObjectId,        
    IN      VSS_OBJECT_TYPE eQueriedObjectType,     
    IN      VSS_OBJECT_TYPE eReturnedObjectsType,   
    OUT     IVssEnumObject**ppEnum                 
	)
{
    BS_ASSERT(m_pSoftwareItf);
    return m_pSoftwareItf->Query(
                QueriedObjectId,        
                eQueriedObjectType,     
                eReturnedObjectsType,   
                ppEnum
                );
}


STDMETHODIMP CVssSoftwareProviderWrapper::DeleteSnapshots(                            
    IN      VSS_ID          SourceObjectId,         
	IN      VSS_OBJECT_TYPE eSourceObjectType,
	IN		BOOL			bForceDelete,			
	OUT		LONG*			plDeletedSnapshots,		
	OUT		VSS_ID*			pNondeletedSnapshotID
	)
{
    BS_ASSERT(m_pSoftwareItf);
    return m_pSoftwareItf->DeleteSnapshots(
                SourceObjectId,         
                eSourceObjectType,
                bForceDelete,			
                plDeletedSnapshots,		
                pNondeletedSnapshotID
                );
}


STDMETHODIMP CVssSoftwareProviderWrapper::BeginPrepareSnapshot(                                
    IN      VSS_ID          SnapshotSetId,              
    IN      VSS_ID          SnapshotId,
    IN      VSS_PWSZ		pwszVolumeName
	)
{
    BS_ASSERT(m_pSoftwareItf);
    return m_pSoftwareItf->BeginPrepareSnapshot(
                SnapshotSetId, 
                SnapshotId,
                pwszVolumeName 
                );
}


STDMETHODIMP CVssSoftwareProviderWrapper::IsVolumeSupported( 
    IN      VSS_PWSZ        pwszVolumeName, 
    OUT     BOOL *          pbSupportedByThisProvider
	)
{
    BS_ASSERT(m_pSoftwareItf);
    return m_pSoftwareItf->IsVolumeSupported(
                pwszVolumeName, 
                pbSupportedByThisProvider
                );
}


STDMETHODIMP CVssSoftwareProviderWrapper::IsVolumeSnapshotted( 
    IN      VSS_PWSZ        pwszVolumeName, 
    OUT     BOOL *          pbSnapshotsPresent,
	OUT 	LONG *		    plSnapshotCompatibility
	)
{
    BS_ASSERT(m_pSoftwareItf);
    return m_pSoftwareItf->IsVolumeSnapshotted(
                pwszVolumeName, 
                pbSnapshotsPresent,
                plSnapshotCompatibility
                );
}


STDMETHODIMP CVssSoftwareProviderWrapper::MakeSnapshotReadWrite(                            
    IN      VSS_ID          SourceObjectId
	)
{
    BS_ASSERT(m_pSoftwareItf);
    return m_pSoftwareItf->MakeSnapshotReadWrite(SourceObjectId);
}


STDMETHODIMP CVssSoftwareProviderWrapper::SetSnapshotProperty(
	IN   VSS_ID  			SnapshotId,
	IN   VSS_SNAPSHOT_PROPERTY_ID	eSnapshotPropertyId,
	IN   VARIANT 			vProperty
	)
/*++

Routine description:

    Implements IVssSoftwareSnapshotProvider::SetSnapshotProperty


--*/
{
    BS_ASSERT(m_pSoftwareItf);
    return m_pSoftwareItf->SetSnapshotProperty(SnapshotId, eSnapshotPropertyId, vProperty);
}


/////////////////////////////////////////////////////////////////////////////
// IVssProviderCreateSnapshotSet


STDMETHODIMP CVssSoftwareProviderWrapper::EndPrepareSnapshots(                            
    IN      VSS_ID          SnapshotSetId
	)
{
    BS_ASSERT(m_pCreationItf);
    return m_pCreationItf->EndPrepareSnapshots(SnapshotSetId);
}


STDMETHODIMP CVssSoftwareProviderWrapper::PreCommitSnapshots(                            
    IN      VSS_ID          SnapshotSetId
    )
{
    BS_ASSERT(m_pCreationItf);
    return m_pCreationItf->PreCommitSnapshots(SnapshotSetId);
}


STDMETHODIMP CVssSoftwareProviderWrapper::CommitSnapshots(                            
    IN      VSS_ID          SnapshotSetId
    )
{
    BS_ASSERT(m_pCreationItf);
    return m_pCreationItf->CommitSnapshots(SnapshotSetId);
}


STDMETHODIMP CVssSoftwareProviderWrapper::PostCommitSnapshots(                            
    IN      VSS_ID          SnapshotSetId,
	IN      LONG            lSnapshotsCount
    )
{
    BS_ASSERT(m_pCreationItf);
    return m_pCreationItf->PostCommitSnapshots(SnapshotSetId, lSnapshotsCount);
}


STDMETHODIMP CVssSoftwareProviderWrapper::AbortSnapshots(                             
    IN      VSS_ID          SnapshotSetId
    )
{
    BS_ASSERT(m_pCreationItf);
    return m_pCreationItf->AbortSnapshots(SnapshotSetId);
}


////////////////////////////////////////////////////////////////////////
// IVssProviderNotifications

STDMETHODIMP CVssSoftwareProviderWrapper::OnUnload(								
	IN  	BOOL	bForceUnload				
    )
{
    return m_pNotificationItf? m_pNotificationItf->OnUnload(bForceUnload): S_OK;
}

