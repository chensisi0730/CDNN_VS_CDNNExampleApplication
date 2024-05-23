//////////////////////////////////////////////////////////////////////////
//			Copyright (c) 2018 CEVA DSP Inc
//////////////////////////////////////////////////////////////////////////
//	File Name	:	Visualize.h
//	Version		:	
//	Created	By	:	Ariela Blumer
//	Date		:	27/12/2018
//////////////////////////////////////////////////////////////////////////

#ifndef CDNN_VISUALIZE_H_
#define CDNN_VISUALIZE_H_

#ifdef WIN32

//=============================================================================
//									INCLUDE
//=============================================================================

//=============================================================================
//									Defines
//=============================================================================

//=============================================================================
//								 FUNCTIONS
//=============================================================================
/**
* \brief Calls segmentation/detection output functions based on network mode.
* \param[in]    pAppParameters          pointer to the application parameters
* \param[in]	ppf32Predictions		pointer to the predictions which are the inference results
* \param[in]	u32NumberOfPredictions	number of predictions that are in the result
* \param[in]	height					height of the predictions buffer
* \param[in]	width					width of the predictions buffer
* \param[in]	pcImageName				the input image name
* \return 0 if successful, -1 on error
*/
int visualizeOutput(void *pAppParameters, float **ppf32Predictions, unsigned int u32NumberOfPredictions, unsigned int height, unsigned int width, char *pcImageName);

#endif // WIN32

#endif //CDNN_VISUALIZE_H_










