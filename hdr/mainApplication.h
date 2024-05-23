//////////////////////////////////////////////////////////////////////////
//			Copyright (c) 2015 CEVA DSP Inc
//////////////////////////////////////////////////////////////////////////
//	File Name	:	mainApplication.h
//	Version		:	
//	Created	By	:	Erez Natan
//	Date		:	01/22/2015
//////////////////////////////////////////////////////////////////////////

#ifndef __DNN_MAINAPPLICATION_H
#define __DNN_MAINAPPLICATION_H

//=============================================================================
//									INCLUDE
//=============================================================================

#include "AppUtility.h"

//=============================================================================
//									Defines
//=============================================================================



/*! \brief the functions operated after the inference operation */
typedef void(*PostInferenceFunctionPtr)(void* cookie, float* predictions, int predictionsLength);

typedef struct STrainingCookieStruct
{
	float label;
	void* trainer;
} STrainingCookie;

typedef struct STestingCookieStruct
{
	float expectedLabel;
	void* predictor;
	float threshold;
	int truePositiveCount;
	int falsePositiveCount;
	int trueNegativeCount;
	int falseNegativeCount;
	int total;
} STestingCookie;

typedef struct SPredictionCookieStruct
{
	const char* outputDirectory;
	void* predictor;
} SPredictionCookie;


/*!
* \brief The CDNN process input parameters
*/
typedef struct CDNNProcessInputs {
	/*! \brief An array of string arrays, each string array contains the input file names for one inference of the network */
	char ***pppsProcessInputs;
	/*! \brief  The number of inferences to be made */
	int s32NumberOfInferences;
	/*! \brief  The number of inputs the network requires for each inference */
	int s32NumberOfInputs;
}cdnnProcessInputs_st;

/*!
* \brief network parameters
*/
typedef struct CDNNNetworkParameters {
	/*! \brief  An opaque reference to a network object */
	cdnnNetwork network;
	/*! \brief  The network's input buffers */
	cdnnDataBuffersArray *pInBuffers;
	/*! \brief  The network's output buffers */
	cdnnDataBuffersArray *pOutBuffers;//  ??? tail 里面的BUF
	/*! \brief  The number of network outputs */
	int tailCnt;//计算图中的尾巴？
}cdnnNetworkParameters_st;

//=============================================================================
//									Globals
//=============================================================================

//=============================================================================
//									FUNTIONS
//=============================================================================


/*! \brief The application process, including all stages for executing inference
* \param [in] application parameters
* \ingroup group_cdnn_application
*/
int ApplicationProcess(InputTestParameters_t *pApplicationParameters);
#endif