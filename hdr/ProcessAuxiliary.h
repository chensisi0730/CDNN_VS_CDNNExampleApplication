//////////////////////////////////////////////////////////////////////////
//			Copyright (c) 2018 CEVA DSP Inc
//////////////////////////////////////////////////////////////////////////
//	File Name	:	ProcessAuxiliary.c
//	Version		:	
//	Created	By	:	Ido Gershtein
//	Date		:	20/02/2018
//////////////////////////////////////////////////////////////////////////

#ifndef CDNN_PROCESSAUXILIARY_H__
#define CDNN_PROCESSAUXILIARY_H__

//=============================================================================
//									INCLUDE
//=============================================================================

#include "mainApplication.h"
//=============================================================================
//									Defines
//=============================================================================

//=============================================================================
//									Functions
//=============================================================================

/*!
* \brief Initializes the output buffers of the network
* \param[in]	pCDNNHandle					CDNN handle
* \param[in]	network						The network which it's input buffers are to be initialized
* \param[out]	pOutBuffers					Array of cdnnDatab, used to store the output buffers
*/
int initOutputBuffers(void *pCDNNHandle, cdnnNetwork network, int *tailCnt, cdnnDataBuffersArray *pOutBuffers);

/*!
* \brief Initializes the output files, clears them if already exist
* \param[in]	pApplicationParameters		Application parameters
* \param[in]	tailCnt						The number of network outputs
*/
int initOutputFiles(InputTestParameters_t *pApplicationParameters, int tailCnt);

/*!
* \brief Outputs the results of an inference 
* \param[in]	pOutBuffers					Array of cdnnDatab, used to store the output buffers
* \param[in]	network						The network which it's output is to be handled
* \param[in]	pApplicationParameters		Application parameters
* \param[in]	tailCnt						The number of network outputs
* \param[in]	inferenceInputs				The inference input files names
*/
int outputResults(cdnnDataBuffersArray *pOutBuffers, cdnnNetwork network, InputTestParameters_t *pApplicationParameters, int tailCnt, char **inferenceInputs);

/*!
* \brief Outputs the errors of an inference
* \param[in]	pApplicationParameters		Application parameters
* \param[in]	ps8ErrorString				A pointer to receive error message
*/
int outputErrors(InputTestParameters_t *pApplicationParameters, char* ps8ErrorString);

/*!
* \brief Gets the process inputs file names, number of inferences and number of inputs the network requires for each inference
* \param[in]	pApplicationParameters		Application parameters
* \param[in]	pReader						Image reader which is used to get the input names
* \param[out]	pInputs						A struct that contains the process inputs information
*/
int getProcessInputs(InputTestParameters_t *pApplicationParameters, cdnnProcessInputs_st *pInputs, ImageReader_t* pReader);

/*!
* \brief Loads the input buffers, reads the files indicated, generates data buffers and updates pInBuffers.
*		 if bInputDummies is True, creates the maximum input image buffer size
* \param[in]	pCDNNHandle					CDNN handle
* \param[in]	pInBuffers					Array of cdnnDatab, used to store the input buffers
* \param[out]	pReader						image reader which is used to read the images
* \param[out]	pApplicationParameters		Application parameters
* \param[out]	inferenceInputs				the file names which are to be loaded to the input buffers 
* \param[out]	numberOfFiles				number of files to be loaded to the input buffers 
* \param[out]	bInputDummies				boolean which indicates if to load input dummies, in case of multi inference 
*/
int loadInputBuffers(void *pCDNNHandle, cdnnDataBuffersArray *pInBuffers, ImageReader_t* pReader, InputTestParameters_t *pApplicationParameters, char **inferenceInputs, int numberOfFiles, int bInputDummies);

/*!
* \brief releases the data buffers in the buffers array
* \param[in]	pCDNNHandle					CDNN handle
* \param[in]	pInBuffers					Array of cdnnDatab, which it's buffers are to be released
* \param[out]	pReader						image reader which is used to read the images
* \param[out]	numberOfBuffers				The number of buffers in the array
*/
int releaseInputBuffersArray(void *pCDNNHandle, cdnnDataBuffersArray *pInBuffers, ImageReader_t* pReader, int numberOfBuffers);

#endif //CDNN_PROCESSAUXILIARY_H__











