//////////////////////////////////////////////////////////////////////////
//			Copyright (c) 2015 CEVA DSP Inc
//////////////////////////////////////////////////////////////////////////
//	File Name	:	inference.cpp
//	Version		:	
//	Created	By	:	Erez Natan
//	Date		:	07/04/2015
//////////////////////////////////////////////////////////////////////////


//=============================================================================
//									Include
//=============================================================================

#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#endif 


#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "CDNNUserInterface.h"
#include "ProcessAuxiliary.h"
#include "NodeLibraryInterface.h"
//#define Multiple_Networks 

//=============================================================================
//									Externs
//=============================================================================

//=============================================================================
//									Defines
//=============================================================================

//=============================================================================
//									Functions
//=============================================================================

int Initialization(void **pCDNNHandle, cdnnBringUpInfo_st *pCdnnBringUpInfo, cdnnNetworkParameters_st *pCDNNNetworkParameters, InputTestParameters_t *pApplicationParameters, cdnnProcessInputs_st *pInputs, ImageReader_t* pReader, void **pNetworkRawData, cdnnLibraryHandle *hUserDeviceLibrary, cdnnUserDevice * hUserDeviceHandle, cdnnDevice * hCDNNDeviceHandle, cdnnLibraryHandle *hUserOperatorLibrary, cdnnLibraryHandle *hUserOperationLibrary, cdnnLibraryHandle *hUserNodeLibrary)
{
	int s32verId;
	int s32Status = 0;
	int bInputDummies = TRUE;
	cdnnNetworkParams_st networkParam = { 0 };
	s32Status |= CDNNCreate(pCDNNHandle, pCdnnBringUpInfo);
	if (s32Status)
	{
		return s32Status;
	}//end if

	if (pApplicationParameters->DeviceType == E_CDNN_DEVICE_CUSTOM)
	{
		cdnnUserDeviceInfo_st cdnnUserDeviceInfo;
		s32Status = UserDeviceLoadLibrary(pApplicationParameters, &cdnnUserDeviceInfo, hUserDeviceLibrary, hUserDeviceHandle);
		if (s32Status)
		{
			printf("\nCDNN Error: User Device Load failed");
			return -1;
		}

		*hCDNNDeviceHandle = CDNNDeviceAdd(*pCDNNHandle, hUserDeviceHandle, &cdnnUserDeviceInfo);
		if (*hCDNNDeviceHandle == NULL)
		{
			printf("\nCDNN Error: User Device Add failed");
			return -1;
		}
	}
	
	/*printing the CDNN version*/
	s32Status |= CDNNGetAttribute(*pCDNNHandle, E_CDNN_ATTRIBUTE_VERSION_ID, &s32verId, sizeof(int));
	if (s32Status)
	{
		printf("\nNo CDNNversion number \t\n");		
		return s32Status;
	}
	else 
	{
		int s32Year, s32Major, s32Minor;

		s32Minor = s32verId % 10;
		s32verId /= 10;
		s32Major = s32verId % 10;
		s32verId /= 10;
		s32Year = s32verId;
		printf("\nCDNN Version %d.%d.%d\nCopyright (c) 2020 CEVA DSP Inc \n", s32Year, s32Major, s32Minor);
	}

	if (pApplicationParameters->doDebug)
	{
		cdnnDebugParams_st sDebugParams = { 0 };
		sDebugParams.u32Level = pApplicationParameters->u32DebugLevel;
		sDebugParams.s32Param = pApplicationParameters->s32DebugParameter;
		/*! \brief output fixed dumps path*/
		char s8dOutputFixedPath[INPUT_FILE_NAME_SIZE] = { 0 };
		/*! \brief output float dumps path*/
		char s8dOutputFloatPath[INPUT_FILE_NAME_SIZE] = { 0 };
		/*! \brief output fixed to float dumps path*/
		char s8dOutputFixedToFloatPath[INPUT_FILE_NAME_SIZE] = { 0 };
		/*! \brief input fixed dumps path*/
		char s8dInputFixedPath[INPUT_FILE_NAME_SIZE] ={ 0 };
		/*! \brief input float dumps path*/
		char s8dInputFloatPath[INPUT_FILE_NAME_SIZE] ={ 0 };
		/*! \brief input fixed to float dumps path*/
		char s8dInputFixedToFloatPath[INPUT_FILE_NAME_SIZE] ={ 0 };
		/*! \brief accelerator dumps fixed path*/
		char s8dParameterFixedPath[INPUT_FILE_NAME_SIZE] ={ 0 };
		/*! \brief accelerator dumps float path*/
		char s8dParameterFloatPath[INPUT_FILE_NAME_SIZE] ={ 0 };

		if (pApplicationParameters->pDumpPath)
		{
			sprintf(s8dOutputFixedPath, "%s", pApplicationParameters->pDumpPath);
			sprintf(s8dOutputFloatPath, "%s", pApplicationParameters->pDumpPath);
			sprintf(s8dOutputFixedToFloatPath, "%s", pApplicationParameters->pDumpPath);
			sprintf(s8dInputFixedPath, "%s", pApplicationParameters->pDumpPath);
			sprintf(s8dInputFloatPath, "%s", pApplicationParameters->pDumpPath);
			sprintf(s8dInputFixedToFloatPath, "%s", pApplicationParameters->pDumpPath);
			sprintf(s8dParameterFixedPath, "%s", pApplicationParameters->pDumpPath);
			sprintf(s8dParameterFloatPath, "%s", pApplicationParameters->pDumpPath);
		}

		if (pApplicationParameters->NoDMA)
		{
			strcat(s8dInputFixedPath, CDNN_DUMP_INPUT_FIXED_NODMA_PATH);
			strcat(s8dInputFixedToFloatPath, CDNN_DUMP_INPUT_FIXED_TO_FLOAT_NODMA_PATH);
			strcat(s8dInputFloatPath, CDNN_DUMP_INPUT_FLOAT_NODMA_PATH);
			strcat(s8dOutputFixedPath, CDNN_DUMP_OUTPUT_FIXED_NODMA_PATH);
			strcat(s8dOutputFloatPath, CDNN_DUMP_OUTPUT_FLOAT_NODMA_PATH);
			strcat(s8dOutputFixedToFloatPath, CDNN_DUMP_OUTPUT_FIXED_TO_FLOAT_NODMA_PATH);
		}
		else
		{
			strcat(s8dInputFixedPath, CDNN_DUMP_INPUT_FIXED_PATH);
			strcat(s8dInputFixedToFloatPath, CDNN_DUMP_INPUT_FIXED_TO_FLOAT_PATH);
			strcat(s8dInputFloatPath, CDNN_DUMP_INPUT_FLOAT_PATH);
			strcat(s8dOutputFixedPath, CDNN_DUMP_OUTPUT_FIXED_PATH);
			strcat(s8dOutputFloatPath, CDNN_DUMP_OUTPUT_FLOAT_PATH);
			strcat(s8dOutputFixedToFloatPath, CDNN_DUMP_OUTPUT_FIXED_TO_FLOAT_PATH);

		}
		strcat(s8dParameterFixedPath, CDNN_DUMP_PARAMETER_FIXED_PATH);
		strcat(s8dParameterFloatPath, CDNN_DUMP_PARAMETER_FLOAT_PATH);

		sDebugParams.s8dInputFixedPath = s8dInputFixedPath;
		sDebugParams.s8dInputFixedToFloatPath = s8dInputFixedToFloatPath;
		sDebugParams.s8dInputFloatPath = s8dInputFloatPath;
		sDebugParams.s8dOutputFixedPath = s8dOutputFixedPath;
		sDebugParams.s8dOutputFloatPath = s8dOutputFloatPath;
		sDebugParams.s8dOutputFixedToFloatPath = s8dOutputFixedToFloatPath;
		sDebugParams.s8dParameterFixedPath = s8dParameterFixedPath;
		sDebugParams.s8dParameterFloatPath = s8dParameterFloatPath;

		sDebugParams.s8DataBufferInName = CDNN_DUMP_DATABUFFER_IN_NAME;
		sDebugParams.s8DataBufferOutName = CDNN_DUMP_DATABUFFER_OUT_NAME;
		s32Status |= CDNNSetAttribute(*pCDNNHandle, E_CDNN_ATTRIBUTE_DEBUG_ENABLE, &sDebugParams, sizeof(sDebugParams));
	}//end if
	if (s32Status)
	{
		return s32Status;
	}

	/* disable dma while running */
	if (pApplicationParameters->NoDMA)
		s32Status |= CDNNSetAttribute(*pCDNNHandle, E_CDNN_ATTRIBUTE_DISABLE_DMA, &pApplicationParameters->NoDMA, sizeof(pApplicationParameters->NoDMA));

	if (pApplicationParameters->InputProfiler.bDoProfiling)
		s32Status |= CDNNSetAttribute(*pCDNNHandle, E_CDNN_ATTRIBUTE_PROFILER_ENABLE, &pApplicationParameters->InputProfiler.eProfilerParam, sizeof(pApplicationParameters->InputProfiler.eProfilerParam));

	if (pApplicationParameters->u8ProgressReport)
		s32Status |= CDNNSetAttribute(*pCDNNHandle, E_CDNN_ATTRIBUTE_PROGRESS_REPORT_ENABLE, &pApplicationParameters->u8ProgressReport, sizeof(pApplicationParameters->u8ProgressReport));

	//u32NumOfPreProcessInputs means how many inputs the network have, most of networks have 1 input. 
	//u32NumOfPreProcessInputBuffers means how many input buffers the network need (u32NumOfPreProcessInputs buffer + mean&std buffer)
	//There are some cases:
	//1 network with 1 input, but also this input needs to do mean subtraction, then the number of input will be 1, number of input buffer will be 2
	//1 network with 2 input, but also only 1 input needs to do mean subtraction, then the number of input will be 2, number of input buffer will be 3 (2 input buffers contain image values and 1 input contain mean&std values.)
	// If a pre-process CSV file is valid, number of input buffers is defined by the file. 
	// Otherwise it is defined by folder content and multi input switch.
	int s32NumInputBuffers = (pApplicationParameters->u32NumOfPreProcessInputBuffers) ? pApplicationParameters->u32NumOfPreProcessInputBuffers : pInputs->s32NumberOfInputs;

	/* allocate memory for the input buffers array according to the inputs number of the network */
	pCDNNNetworkParameters->pInBuffers->arrSize = s32NumInputBuffers;
	pCDNNNetworkParameters->pInBuffers->pArray = (cdnnDatab*)malloc(s32NumInputBuffers * sizeof(cdnnDatab));
	if (!pCDNNNetworkParameters->pInBuffers->arrSize)
	{
		printf("memory allocation failure\n");
		return s32Status;
	}

	/* load input buffers array. if the number of inferences is 1, load the true input/s. if the number of inferences is greater than 1,
	load dummy inputs (data buffers initialized with maximum size) and the right inputs will be loaded right before inference execution 把需要推理的图片的路径输入，读取到图片的数据 ，保存到input buffers*/
	bInputDummies = pInputs->s32NumberOfInferences > 1 ? TRUE : FALSE;//视频 35：30“
	s32Status |= loadInputBuffers(*pCDNNHandle, pCDNNNetworkParameters->pInBuffers, pReader, pApplicationParameters, pInputs->pppsProcessInputs[0], pInputs->s32NumberOfInputs, bInputDummies);//疑问0  断点
	if (s32Status)
	{
		return s32Status;
	}//end if

	/* initialize network and set network attributes*/

	networkParam.pInDataBuffersArray = pCDNNNetworkParameters->pInBuffers;//Array of handles to input buffers before initialization.
	networkParam.pNetworkFilePointer = (char*)*pNetworkRawData;//pNetworkRawData 是从deploy.cdnnQdata中读取到的原始数据的指针
	pCDNNNetworkParameters->network = CDNNCreateNetwork(*pCDNNHandle, &networkParam);//TODO:多网络STEP1：The CDNN Network related functions done per network.
	
	if (NULL == pCDNNNetworkParameters->network)
	{
		printf("Error in network creation\n");
		return -1;
	}

	//set the BW reduction value
	if (pApplicationParameters->InputBWReduction.bDoBWReduction)//断点 //TODO:多网络STEP1：The CDNN Network related functions done per network
		s32Status |= CDNNNetworkSetAttribute(pCDNNNetworkParameters->network, E_CDNN_ATTRIBUTE_NETWORK_BWRVALUE, &pApplicationParameters->InputBWReduction.f32ratio, sizeof(pApplicationParameters->InputBWReduction.f32ratio));

	if (s32Status)
	{
		printf("could not set network attribute\n");
		return s32Status;
	}

	if (pApplicationParameters->bCustomNode)
	{
		s32Status = UserOperatorLoadLibrary(pApplicationParameters, *pCDNNHandle, &hUserOperatorLibrary[0], "UserPCOperatorLibrary");
		if (s32Status)
		{
			printf("CDNN  Error: User Operator library load failed");
			return -1;
		}

		if (pApplicationParameters->BaseDeviceType == E_CDNN_DEVICE_SENSPRO)
		{
#if (!defined HARDWARE_PLATFORM) || (defined HARDWARE_PLATFORM  && defined SensPro)
			s32Status = UserOperatorLoadLibrary(pApplicationParameters, *pCDNNHandle, &hUserOperatorLibrary[1], "UserSensProOperatorLibrary");
			if (s32Status)
			{
				printf("CDNN  Error: User Operator library load failed");
				return -1;
			}
#endif
		}

		if (pApplicationParameters->BaseDeviceType == E_CDNN_DEVICE_XM6)
		{
#if (!defined HARDWARE_PLATFORM) || (defined HARDWARE_PLATFORM  && defined XM6)
			s32Status = UserOperatorLoadLibrary(pApplicationParameters, *pCDNNHandle, &hUserOperatorLibrary[1], "UserXM6OperatorLibrary");
			if (s32Status)
			{
				printf("CDNN  Error: User Operator library load failed");
				return -1;
			}
#endif
		}

		if (pApplicationParameters->BaseDeviceType == E_CDNN_DEVICE_XM4)
		{
#if (!defined HARDWARE_PLATFORM) || (defined HARDWARE_PLATFORM  && defined XM4)
			s32Status = UserOperatorLoadLibrary(pApplicationParameters, *pCDNNHandle, &hUserOperatorLibrary[1], "UserXM4OperatorLibrary");
			if (s32Status)
			{
				printf("CDNN  Error: User Operator library load failed");
				return -1;
			}
#endif
		}

		s32Status = UserOperationLoadLibrary(pApplicationParameters, *pCDNNHandle, hUserOperationLibrary, "UserPCOperationLibrary");
		if (s32Status)
		{
			printf("CDNN  Error: User Operation library load failed");
			return -1;
		}

		s32Status = UserNodeLoadLibrary(pApplicationParameters, *pCDNNHandle, hUserNodeLibrary, "UserNodeLibrary");
		if (s32Status)
		{
			printf("CDNN  Error: User Node library load failed");
			return -1;
		}
	}
	/* init CDNN */
	s32Status |= CDNNInitialize(*pCDNNHandle);//TODO:多网络STEP1 :After loading all of the networks and creating them on the CDNN side, the user must call the CDNNInitialize function,
	if (s32Status)
	{
		s32Status |= releaseInputBuffersArray(pCDNNHandle, pCDNNNetworkParameters->pInBuffers, pReader, s32NumInputBuffers);
		return s32Status;
	}

	/* set output buffers array */
	s32Status |= initOutputBuffers(*pCDNNHandle, pCDNNNetworkParameters->network, &(pCDNNNetworkParameters->tailCnt), pCDNNNetworkParameters->pOutBuffers);
	if (s32Status)
	{
		printf("could not init output buffers\n");
		return s32Status;
	}

	/* release the dummy input buffers in case of multi inference */
	if (pInputs->s32NumberOfInferences > 1) //一般一次推理一张图片  音频35：30”
	{
		s32Status |= releaseInputBuffersArray(*pCDNNHandle, pCDNNNetworkParameters->pInBuffers, pReader, s32NumInputBuffers);
		if (s32Status)
		{
			printf("could not release input buffers array\n");
			return s32Status;
		}
	}

	return s32Status;
}

int Execution(void *pCDNNHandle, cdnnNetworkParameters_st *pCDNNNetworkParameters, cdnnProcessInputs_st *pInputs, InputTestParameters_t *pApplicationParameters, ImageReader_t* pReader)
{
	int	s32Status = 0;
	int s32ImgIdx;
	for (s32ImgIdx = 0; s32ImgIdx < pInputs->s32NumberOfInferences; s32ImgIdx++)
	{
		/*clear output file/s before first inference*/
		if (s32ImgIdx == 0)
		{
			s32Status |= initOutputFiles(pApplicationParameters, pCDNNNetworkParameters->tailCnt);
			if (s32Status)
			{
				printf("could not open output files\n");
				return -1;
			}
		}

		if (pInputs->s32NumberOfInferences > 1)
			printf("inference on image number %d: %s\n", s32ImgIdx, pInputs->pppsProcessInputs[s32ImgIdx][0]);
		else
			printf("inference on image %s\n", pInputs->pppsProcessInputs[s32ImgIdx][0]);

		/* update the input buffers array before every inference when multi inference. When single inference, the input buffers array is already set */
		if (pInputs->s32NumberOfInferences > 1)//断点
		{
			s32Status |= loadInputBuffers(pCDNNHandle, pCDNNNetworkParameters->pInBuffers, pReader, pApplicationParameters, pInputs->pppsProcessInputs[s32ImgIdx], pInputs->s32NumberOfInputs, FALSE);
			s32Status |= CDNNNetworkUpdateParameter(pCDNNNetworkParameters->network, (cdnnReference)pCDNNNetworkParameters->pInBuffers, E_CDNN_NETWORK_STRUCT_INPUT_BUFFERS);//TODO:多网络STEP2
		}
		if (s32Status)
		{
			return s32Status;
		}

		/* Execute inference */
		s32Status |= CDNNNetworkInference(pCDNNHandle, pCDNNNetworkParameters->network);//断点 //TODO:多网络STEP2
		if (s32Status)
		{
			return s32Status;
		}

#ifdef PRINT_CLASSIFICATION_TIME
		CEVAFILE cycles_file;		
		unsigned long long int u64classTime = 0;

		s32Status |= CDNNNetworkGetAttribute(pCDNNNetworkParameters->network, E_CDNN_ATTRIBUTE_NETWORK_PERFORMANCE, &u64classTime, sizeof(u64classTime));
		CEVAPRINTF("\n\n Network Inference Time: \t %llu \n\n", u64classTime);

		cycles_file = CEVAFOPEN("mmc:/home/ubuntu/CDNN/out/cycles.txt", "a");
		if (cycles_file)
		{						
			CEVAFPRINTF(cycles_file, "%s : ", pApplicationParameters->psNetworkFilename);
			CEVAFPRINTF(cycles_file, "%d\n", u64classTime);
			CEVAFCLOSE(cycles_file);
		}
#endif
		
		/* output the inference results *///断点   音频41:10"
		s32Status |= outputResults(pCDNNNetworkParameters->pOutBuffers, pCDNNNetworkParameters->network, pApplicationParameters, pCDNNNetworkParameters->tailCnt, pInputs->pppsProcessInputs[s32ImgIdx]);
		if (s32Status)
		{
			printf("failed to analyze output %d\n", s32ImgIdx);
			return s32Status;
		}

		// If a pre-process CSV file is valid, number of input buffers is defined by the file. 
		// Otherwise it is defined by folder content and multi input switch.
		int s32NumInputBuffers = (pApplicationParameters->u32NumOfPreProcessInputBuffers) ? pApplicationParameters->u32NumOfPreProcessInputBuffers : pInputs->s32NumberOfInputs;
				
		/* release all the data buffers in the input buffers array */
		s32Status |= releaseInputBuffersArray(pCDNNHandle, pCDNNNetworkParameters->pInBuffers, pReader, s32NumInputBuffers);
	// 	outSt.s8FractionBits = fractionBits;
		if (s32Status)
		{
			return s32Status;
		}
	}

	return s32Status;
}

int Finalization(void *pCDNNHandle, cdnnBringUpInfo_st *pCdnnBringUpInfo, cdnnNetworkParameters_st *pCDNNNetworkParameters, InputTestParameters_t *pApplicationParameters, ImageReader_t* pReader, cdnnProcessInputs_st *pInputs, cdnnLibraryHandle *hUserDeviceLibrary, cdnnUserDevice * hUserDeviceHandle, cdnnDevice * hCDNNDeviceHandle, cdnnLibraryHandle *hUserOperatorLibrary, cdnnLibraryHandle *hUserOperationLibrary, cdnnLibraryHandle *hUserNodeLibrary)
{
	int s32Status = 0;
	int tailIdx;

	/* create error file if needed */
	if (pApplicationParameters->doDebug){
		char *pLoggerString = NULL;
		CDNNGetLoggerBuffer(pCDNNNetworkParameters->network, (char**)&pLoggerString);
		if (pLoggerString)
		{
			s32Status |= PrintToLogFile(pApplicationParameters->OutputResultsFilename, pLoggerString, 1);
		}
		if (CDNNErrorExists(pCDNNHandle))
		{
			char pOutputError[CMD_LINE_SIZE];
			memset(pOutputError, 0, CMD_LINE_SIZE);
			s32Status |= CDNNGetErrorMessage(pCDNNHandle, pOutputError);
			s32Status |= outputErrors(pApplicationParameters, pOutputError);
		}
	}


	/* print profiler */
	if (pApplicationParameters->InputProfiler.bDoProfiling)
	{
		char *pString = NULL;
		s32Status |= CDNNAccessNetworkProfilerResults(pCDNNNetworkParameters->network, (char**)&pString);
		s32Status |= PrintToProfilerFile(pApplicationParameters->OutputResultsFilename, pString, 1);
	}

	/* in case of multi inference of a multiple input network, release all string arrays containing the input names */
	if (pInputs->s32NumberOfInputs > 1 && pInputs->s32NumberOfInferences > 1)
	{
		int s32FolderIdx;
		for (s32FolderIdx = 0; s32FolderIdx < pInputs->s32NumberOfInferences; s32FolderIdx++)
		{
			int s32ImgIdx;
			for (s32ImgIdx = 0; s32ImgIdx < pInputs->s32NumberOfInputs; s32ImgIdx++)
			{
				if (NULL != pInputs->pppsProcessInputs[s32FolderIdx][s32ImgIdx])
					free(pInputs->pppsProcessInputs[s32FolderIdx][s32ImgIdx]);
			}
			if (NULL != pInputs->pppsProcessInputs[s32FolderIdx])
				free(pInputs->pppsProcessInputs[s32FolderIdx]);
		}
	}
	if (NULL != pInputs->pppsProcessInputs)
		free(pInputs->pppsProcessInputs);

	/*free all tail data buffers and malloc-ed memory */
	for (tailIdx = 0; tailIdx < pCDNNNetworkParameters->tailCnt; tailIdx++)
	{
		void* pData = NULL;
		s32Status |= CDNNAccessDataBuffer(pCDNNNetworkParameters->pOutBuffers->pArray[tailIdx], &pData);
		if (NULL != pData)
			free(pData);
		s32Status |= CDNNReleaseDataBuffer(pCDNNHandle, &(pCDNNNetworkParameters->pOutBuffers->pArray[tailIdx]));
	}


	/* release in/out arrays */
	if (NULL != pCDNNNetworkParameters->pInBuffers->pArray)
		free(pCDNNNetworkParameters->pInBuffers->pArray);
	if (NULL != pCDNNNetworkParameters->pOutBuffers->pArray)
		free(pCDNNNetworkParameters->pOutBuffers->pArray);

	/* release all objects */
	s32Status |= CDNNReleaseNetwork(pCDNNHandle, &(pCDNNNetworkParameters->network));
	
	if (pApplicationParameters->DeviceType == E_CDNN_DEVICE_CUSTOM)
	{
		//Release user device
		s32Status = CDNNDeviceRelease(pCDNNHandle, hCDNNDeviceHandle);
	}

	s32Status |= CDNNRelease(pCDNNHandle);
	iruDestroyImageReader(pReader); 
	
	if (pApplicationParameters->DeviceType == E_CDNN_DEVICE_CUSTOM)
	{
		//Release User Device handle
		s32Status |= UserDeviceUnLoadLibrary(hUserDeviceLibrary, hUserDeviceHandle);
	}
	//Release Operation Library
	s32Status |= UserOperationUnLoadLibrary(hUserOperationLibrary);
	//Release Operator Library
	s32Status |= UserOperatorUnLoadLibrary(&hUserOperatorLibrary[0]);
#if (!defined HARDWARE_PLATFORM) || (defined HARDWARE_PLATFORM  && defined XM6)
	s32Status |= UserOperatorUnLoadLibrary(&hUserOperatorLibrary[1]);
#endif
	//Release Node Library
	s32Status |= UserNodeUnLoadLibrary(hUserNodeLibrary);

	s32Status |= CevaHardwareDeinit(pCdnnBringUpInfo);

	s32Status |= destroyNetworkAllocationPC(pApplicationParameters);
	return s32Status;
}


int ApplicationProcess(InputTestParameters_t *pApplicationParameters)
{
	int		s32Status = 0;
	void	*pCDNNHandle = NULL;
	void *pNetworkRawData = NULL;
	cdnnLibraryHandle hUserDeviceLibrary = NULL;
	cdnnLibraryHandle hUserOperatorLibrary[3] ={ NULL , NULL , NULL };
	cdnnLibraryHandle hUserOperationLibrary = NULL;
	cdnnLibraryHandle hUserNodeLibrary = NULL;
	cdnnUserDevice hUserDeviceHandle = NULL;
	cdnnDevice hCDNNDeviceHandle = NULL;

#ifdef Multiple_Networks

	cdnnProcessInputs_st InputsArr[NUM_OF_NETWORKS] = { 0 };
	cdnnProcessInputs_st *pInputsArr[NUM_OF_NETWORKS] = { 0 };
	pInputsArr[0] = &InputsArr[0];
	pInputsArr[1] = &InputsArr[1];

	ImageReader_t* pReaderArr[NUM_OF_NETWORKS] = { 0 };
	pReaderArr[0] = iruCreateImageReader();
	pReaderArr[1] = iruCreateImageReader();

	/*Network Parameters*/
	cdnnDataBuffersArray inBuffersArr[NUM_OF_NETWORKS] = { 0 };
	cdnnDataBuffersArray outBuffersArr[NUM_OF_NETWORKS] = { 0 };


	cdnnNetworkParameters_st cdnnNetworkParametersArr[NUM_OF_NETWORKS] = { 0 };
	cdnnNetworkParameters_st *pCDNNNetworkParametersArr[NUM_OF_NETWORKS] = { 0 };//指针数组，是个数组

	inBuffersArr[0].pArray = NULL;
	inBuffersArr[0].arrSize = 0;
	inBuffersArr[1].pArray = NULL;
	inBuffersArr[1].arrSize = 0;
	outBuffersArr[0].pArray = NULL;
	outBuffersArr[0].arrSize = 0;
	outBuffersArr[1].pArray = NULL;
	outBuffersArr[1].arrSize = 0;

	memset(pCDNNNetworkParametersArr[0], 0, sizeof(cdnnNetworkParameters_st));
	memset(pCDNNNetworkParametersArr[1], 0, sizeof(cdnnNetworkParameters_st));


	int i = 0;
	while (i < NUM_OF_NETWORKS){
		pCDNNNetworkParametersArr[i]->pInBuffers = &inBuffersArr[i];
		pCDNNNetworkParametersArr[i]->pOutBuffers = &outBuffersArr[i];
	}
#else
	cdnnProcessInputs_st Inputs = { 0 }, *pInputs = &Inputs;
	ImageReader_t* pReader = iruCreateImageReader();//图像读取器实用程序API不是最终产品(即运行时 框架,如CDNN运行时框架概述中所述)的一部 分,仅用于模拟目的//file:///C:/CEVA-XM6_SW/CDNN_XM6_NPRS_v20.3.0/CDNNDocumentation/HTML/CDNN_Example_Application_Overview.html
	/*Network Parameters*/
	cdnnDataBuffersArray inBuffers;
	cdnnDataBuffersArray outBuffers;

	cdnnNetworkParameters_st cdnnNetworkParameters, *pCDNNNetworkParameters = &cdnnNetworkParameters;

	inBuffers.pArray = NULL;
	inBuffers.arrSize = 0;
	outBuffers.pArray = NULL;
	outBuffers.arrSize = 0;

	memset(pCDNNNetworkParameters, 0, sizeof(cdnnNetworkParameters_st));

	pCDNNNetworkParameters->pInBuffers = &inBuffers;
	pCDNNNetworkParameters->pOutBuffers = &outBuffers;

#endif
	
	/*Target Parameters*/
	cdnnBringUpInfo_st cdnnBringUpInfo = { 0 };
		
	/* get all process input names, number of inferences and the number of inputs the network requires for each inference */
	s32Status |= getProcessInputs(pApplicationParameters, pInputs, pReader);//zijijiaduandian自己加断点
	if (s32Status)
	{
		printf("could not parse the input names\n");

		Finalization(pCDNNHandle, NULL, pCDNNNetworkParameters, pApplicationParameters, pReader, pInputs, &hUserDeviceLibrary, &hUserDeviceHandle, &hCDNNDeviceHandle, hUserOperatorLibrary, &hUserOperationLibrary, &hUserNodeLibrary);
		return s32Status;
	}

#ifdef WIN32
	/* generate cdnnQdata by executing the generation process */
	if (pApplicationParameters->doGeneration)
	{
		ImageReader_t* pGeneratorReader = NULL;
		pGeneratorReader = iruCreateImageReader();
		s32Status = runCDNNNetworkGenerator(pApplicationParameters, pGeneratorReader);//注释：模型量化和转换，调试时可以屏蔽
		iruDestroyImageReader(pGeneratorReader);
		if (s32Status)
		{
			return s32Status;
		}
	}//end if
#endif 

	/* load cdnnQdata file to user memory space */
	s32Status = applicationLoadNetworkFromFile(pApplicationParameters, &pNetworkRawData);
	if (s32Status)
	{
		printf("Test failed applicationLoadNetworkFromFile (status: %d)\n", s32Status);

		Finalization(pCDNNHandle, NULL, pCDNNNetworkParameters, pApplicationParameters, pReader, pInputs, &hUserDeviceLibrary, &hUserDeviceHandle, &hCDNNDeviceHandle, hUserOperatorLibrary, &hUserOperationLibrary, &hUserNodeLibrary);
		return s32Status;
	}//end if

	/* Initialize all hardware devices, returns cdnnBringUpInfo which is used to create the CDNN */
	s32Status |= CevaHardwareInit(&cdnnBringUpInfo, pApplicationParameters); //音频34：30” //TODO:多网络STEP1
	if (s32Status)
	{
		printf("\n could not initialize CEVA DSP or CEVA accelerator \t\n");

		Finalization(pCDNNHandle, &cdnnBringUpInfo, pCDNNNetworkParameters, pApplicationParameters, pReader, pInputs, &hUserDeviceLibrary, &hUserDeviceHandle, &hCDNNDeviceHandle, hUserOperatorLibrary, &hUserOperationLibrary, &hUserNodeLibrary);
		return s32Status;
	}//end if

	/* Initialize Custom hardware devices */
	if (pApplicationParameters->DeviceType == E_CDNN_DEVICE_CUSTOMER)
	{
		s32Status = CustomHardwareInit(&cdnnBringUpInfo, pApplicationParameters);
		if (s32Status)
		{
			printf("CDNN Error: CustomHardwareInit failed\n");
			return -1;
		}
	}

	/* Boot time stages *///断点 //TODO:多网络STEP1  唯一包含了CDNNCreate 
	s32Status |= Initialization(&pCDNNHandle, &cdnnBringUpInfo, pCDNNNetworkParameters, pApplicationParameters, pInputs, pReader, &pNetworkRawData, &hUserDeviceLibrary, &hUserDeviceHandle, &hCDNNDeviceHandle, hUserOperatorLibrary, &hUserOperationLibrary, &hUserNodeLibrary);
	if (s32Status)
	{
		printf("Test failed Initialization (status: %d)\n", s32Status);

		// If a pre-process CSV file is valid, number of input buffers is defined by the file. 
		// Otherwise it is defined by folder content and multi input switch.
		int s32NumInputBuffers = (pApplicationParameters->u32NumOfPreProcessInputBuffers) ? pApplicationParameters->u32NumOfPreProcessInputBuffers : pInputs->s32NumberOfInputs;
				
		/* release all the data buffers in the input buffers array (normally done after execution) */
		releaseInputBuffersArray(pCDNNHandle, pCDNNNetworkParameters->pInBuffers, pReader, s32NumInputBuffers);

		Finalization(pCDNNHandle, &cdnnBringUpInfo, pCDNNNetworkParameters, pApplicationParameters, pReader, pInputs, &hUserDeviceLibrary, &hUserDeviceHandle, &hCDNNDeviceHandle, hUserOperatorLibrary, &hUserOperationLibrary, &hUserNodeLibrary);
		return s32Status;
	}//end if


	/* execute inference over all inputs */
	s32Status |= Execution(pCDNNHandle, pCDNNNetworkParameters, pInputs, pApplicationParameters, pReader);//断点
	if (s32Status)
	{
		Finalization(pCDNNHandle, &cdnnBringUpInfo, pCDNNNetworkParameters, pApplicationParameters, pReader, pInputs, &hUserDeviceLibrary, &hUserDeviceHandle, &hCDNNDeviceHandle, hUserOperatorLibrary, &hUserOperationLibrary, &hUserNodeLibrary);
		return s32Status;
	}

	/* release all objects and allocated memory and print profiler data if needed */
	s32Status |= Finalization(pCDNNHandle, &cdnnBringUpInfo, pCDNNNetworkParameters, pApplicationParameters, pReader, pInputs, &hUserDeviceLibrary, &hUserDeviceHandle, &hCDNNDeviceHandle, hUserOperatorLibrary, &hUserOperationLibrary, &hUserNodeLibrary);
	if (s32Status)
	{
		return s32Status;
	}


	return 0;
}


