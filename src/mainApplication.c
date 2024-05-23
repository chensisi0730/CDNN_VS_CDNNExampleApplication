//////////////////////////////////////////////////////////////////////////
//			Copyright (c) 2015 CEVA DSP Inc
//////////////////////////////////////////////////////////////////////////
//	File Name	:	mainApplication.cpp
//	Version		:	
//	Created	By	:	Erez Natan
//	Date		:	01/22/2015
//////////////////////////////////////////////////////////////////////////

//=============================================================================
//									INCLUDE
//=============================================================================
#include <assert.h>
#include <stdlib.h>
#include "mainApplication.h"

//=============================================================================
//									DEFINES
//=============================================================================


#ifdef WIN32
/*! \brief defines visual studio operation
* \ingroup group_cdnn_defines
*/
#define CDNN_PC_SIMULATION
#else
/*! \brief simulation platform eclipse or visual studio
* \ingroup group_cdnn_defines
*/
//#define CDNN_DSP_SIMULATION

/*! \brief emulation platform - board
* \ingroup group_cdnn_defines
*/
//#define CDNN_DSP_EMULATION
#endif

//=============================================================================
//									Globals
//=============================================================================


//=============================================================================
//									FUNCTIONS
//=============================================================================

int main(int argc, char *argv[])
{
	InputTestParameters_t*  pApplicationParameters = NULL;
	int	s32TestsCnt = 0;
	int	s32TestsIdx = 0;
	char* pcInput = NULL;
	char bIncrementalSearch = 0;
	char bMultiInput = 0;
	
	// add tests
	#if defined(CDNN_PRINT_INFO)
	CEVAPRINTF("debug@ - start parsing cfg file \n");
	#endif
	s32TestsCnt = AddPrepareTests(argc, argv);//¶Ïµã
	#if defined(CDNN_PRINT_INFO)
	CEVAPRINTF("debug@ - end parsing cfg file \n");
	#endif

	for (s32TestsIdx = 0; s32TestsIdx < s32TestsCnt; s32TestsIdx++)
	{
		int	s32status = 0;

		pApplicationParameters = g_ppApplicationParameters[s32TestsIdx];
		pApplicationParameters->bDemoMode = FALSE;

		printf("Start\n");

		pcInput = pApplicationParameters->psInputImageOrDirectoryName;
		bIncrementalSearch = pApplicationParameters->doIncrementalFileSearch;
		bMultiInput = pApplicationParameters->bMultiInput;

		if (!(iruIsFolder(pcInput) || !iruIsFolder(pcInput)))
		{
			printf("Unexpected application parameters...\n");
			s32status = -1;
		}
		else
		{
			if (pApplicationParameters->bDoPrecision){
				printf("Example application does not support precision...\n");
				s32status = -1;
			}
			else
				s32status = ApplicationProcess(pApplicationParameters);//¶Ïµã
		}

		if (s32status)
		{
			printf("Test failed (status: %d)\n", s32status);
		}
		else
		{
			printf("Finished\n");
		}

	}
	ReleaseTests();
	
	if (s32TestsCnt == -1)
		return -1;
	return 0;
}







