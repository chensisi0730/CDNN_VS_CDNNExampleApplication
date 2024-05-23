//////////////////////////////////////////////////////////////////////////
//			Copyright (c) 2018 CEVA DSP Inc
//////////////////////////////////////////////////////////////////////////
//	File Name	:	ProcessAuxiliary.c
//	Version		:	
//	Created	By	:	Ido Gershtein
//	Date		:	20/02/2018
//////////////////////////////////////////////////////////////////////////


//=============================================================================
//									Include
//=============================================================================
#include "ProcessAuxiliary.h"
#include "CDNNUserInterface.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

//=============================================================================
//									Functions
//=============================================================================
int getProcessInputs(InputTestParameters_t *pApplicationParameters, cdnnProcessInputs_st *pInputs, ImageReader_t* pReader)
{
	int s32Status = 0;
	int s32folderIdx;
	char **fileNamesTmp = NULL;
	char **fileNames = NULL;
	int s32NumberOfInputsTmp = 0;
	int s32ImgIdx;
	int s32NameSize = 0;
	
	char* pcInput = pApplicationParameters->psInputImageOrDirectoryName;
	int bSingleInference;
	/* in case of single inference, input is an image file for a single input network or a text file for a multiple input network.
	   in case of multi inference, input is a folder both for a single and multiple input network  这个多multi inference的意思是指定输入是文件夹，文件夹下多个文件，按照顺序推理，不是并行推理*/
	if (!iruIsFolder(pcInput) && !pApplicationParameters->doIncrementalFileSearch)
	{
		bSingleInference = TRUE;
	}
	else if (iruIsFolder(pcInput) || (!iruIsFolder(pcInput) && pApplicationParameters->doIncrementalFileSearch))
	{
		bSingleInference = FALSE;
	}
	else
	{
		printf("unexpected input\n");
		return -1;
	}


	if (bSingleInference)//单次推理
	{
		/* single inference of a multiple input network 多输入网络的单次推理 */
		if (pApplicationParameters->bMultiInput)
		{
			pInputs->s32NumberOfInferences = 1;
			pInputs->pppsProcessInputs = (char***)malloc(pInputs->s32NumberOfInferences * sizeof(char**));
			if (iruGetFilesInFile(pReader, pApplicationParameters->psInputImageOrDirectoryName, pInputs->pppsProcessInputs, &(pInputs->s32NumberOfInputs)) != E_IR_SUCCESS)
			{
				printf("can't read file %s\n", pApplicationParameters->psInputImageOrDirectoryName);
				return -1;
			}
		}
		/* single inference of a single input network 单输入网络的单次推理*/
		else
		{
			pInputs->s32NumberOfInferences = 1;
			pInputs->s32NumberOfInputs = 1;
			pInputs->pppsProcessInputs = (char***)malloc(pInputs->s32NumberOfInferences * sizeof(char**));
			pInputs->pppsProcessInputs[0] = &pApplicationParameters->psInputImageOrDirectoryName;
		}
	}
	else
	{
		/* multi inference of a multiple input network. each inference input files are located in a folder. get the number of folders and for 
		   each folder get the file names inside the folder. check that all folders contain the same number of files, exit if not. */
		if (pApplicationParameters->bMultiInput)
		{
			char **folderNames = NULL;
			if (iruGetSubFoldersInFolder(pReader, pApplicationParameters->psInputImageOrDirectoryName, &folderNames, &(pInputs->s32NumberOfInferences)) != E_IR_SUCCESS)
			{
				printf("cant read folder %s\n", pApplicationParameters->psInputImageOrDirectoryName);
				return -1;
			}
			pInputs->pppsProcessInputs = (char***)malloc(pInputs->s32NumberOfInferences * sizeof(char**));
			if (pInputs->pppsProcessInputs == NULL)
			{
				printf("Memory allocation failure\n");
				return -1;
			}
			for (s32folderIdx = 0; s32folderIdx < pInputs->s32NumberOfInferences; s32folderIdx++)
			{
				if (iruGetFilesInFolder(pReader, folderNames[s32folderIdx], &fileNamesTmp, &s32NumberOfInputsTmp) != E_IR_SUCCESS)
				{
					printf("can't read folder %s\n", folderNames[s32folderIdx]);
					return -1;
				}
				if (s32folderIdx == 0)
					pInputs->s32NumberOfInputs = s32NumberOfInputsTmp;
				else
				{
					if (s32NumberOfInputsTmp != pInputs->s32NumberOfInputs)
					{
						printf("number of input files is not the same in all folders\n");
						return -1;
					}
				}

				pInputs->pppsProcessInputs[s32folderIdx] = (char**)malloc(pInputs->s32NumberOfInputs * sizeof(char*));
				if (pInputs->pppsProcessInputs[s32folderIdx] == NULL)
				{
					printf("Memory allocation failure\n");
					return -1;
				}
				for (s32ImgIdx = 0; s32ImgIdx < pInputs->s32NumberOfInputs; s32ImgIdx++)
				{
					s32NameSize = strlen(fileNamesTmp[s32ImgIdx]) + 1;
					pInputs->pppsProcessInputs[s32folderIdx][s32ImgIdx] = (char*)malloc(s32NameSize * sizeof(char));
					if (pInputs->pppsProcessInputs[s32folderIdx][s32ImgIdx] == NULL)
					{
						printf("Memory allocation failure\n");
						return -1;
					}
					strcpy(pInputs->pppsProcessInputs[s32folderIdx][s32ImgIdx], fileNamesTmp[s32ImgIdx]);
				}
			}
		}
		/* multi inference of a single input network */
		else
		{
			pInputs->s32NumberOfInputs = 1;
			if (iruGetFilesInFolder(pReader, pApplicationParameters->psInputImageOrDirectoryName, &fileNames, &(pInputs->s32NumberOfInferences)) != E_IR_SUCCESS)
			{
				if (!pApplicationParameters->doIncrementalFileSearch)
					printf("can't read files in folder %s\n", pApplicationParameters->psInputImageOrDirectoryName);
				else
					printf("can't read files that are incremented after %s\n", pApplicationParameters->psInputImageOrDirectoryName);
				return -1;
			}
			pInputs->pppsProcessInputs = (char***)malloc(pInputs->s32NumberOfInferences * sizeof(char**));
			for (s32folderIdx = 0; s32folderIdx < pInputs->s32NumberOfInferences; s32folderIdx++)
			{
				pInputs->pppsProcessInputs[s32folderIdx] = &fileNames[s32folderIdx];
			}
		}
	}

	return s32Status;
}

int fillBufferParameters(cdnnDatabufferParameters_t* pstParams, InputTestParameters_t *pApplicationParameters)
{
	unsigned char u8IsSpecificInputRawScaleParameterFound = FALSE;
	unsigned int i = 0;

	pstParams->resizeHeight = pstParams->resizeWidth = - 1;
	pstParams->f32ZeroPoint = 0;

	if (pApplicationParameters->ChannelSwap)
		pstParams->eChannelSwap = E_CDNN_BUFFER_IMAGE_CHANNEL_SWAP_TRUE;
	else
		pstParams->eChannelSwap = E_CDNN_BUFFER_IMAGE_CHANNEL_SWAP_FALSE;

	if (pApplicationParameters->eTranspose)  
		pstParams->eTranspose = E_CDNN_BUFFER_IMAGE_TRANSPOSE_TRUE;
	else
		pstParams->eTranspose = E_CDNN_BUFFER_IMAGE_TRANSPOSE_FALSE;
	
	if (pApplicationParameters->doResizeInput)
	{
		//resize according to the model parameters (from parser)
		pstParams->eResize = E_CDNN_BUFFER_IMAGE_RESIZE_TRUE;
	}
	else
	{
		pstParams->eResize = E_CDNN_BUFFER_IMAGE_RESIZE_FALSE;
	}

	if (pApplicationParameters->RawScale == E_CDNN_INPUT_IMAGE_RAW_SCALE_1)
		pstParams->scaleFactor = (float)1 / 256; //疑问7
	
	for (i = 0; i < pApplicationParameters->u32NumOfPreProcessInputs; i++)
	{
		InputPrameters_t * pInputParameters = &pApplicationParameters->pInputParameters[i];
		if (pApplicationParameters->pInputParameters &&
			(pApplicationParameters->u32NumOfPreProcessInputs == 1 || !strcmp(pInputParameters->sInputName, pstParams->pBufferName)) &&
			pInputParameters->s8Valid)
		{
			u8IsSpecificInputRawScaleParameterFound = TRUE;

			if ((pInputParameters->s32height != -1) && (pInputParameters->s32width != -1) && (pInputParameters->s8Resize == 1))//对应C:\CEVA-XM6_SW\CDNN_XM6_NPRS_v20.3.0\NetworkExamples\TensorFlow\Opensource\mobilenet\v1_1.0_224\deploy.csv的 Resize
			{
				pstParams->resizeWidth = pInputParameters->s32width;
				pstParams->resizeHeight = pInputParameters->s32height;
			}
			if (pInputParameters->s32rawScale != -1)//s32rawScale  为什么不对应CSV的值？疑问7  ：CSV中Rawscale=256就是不处理，Rawscale=1就是归一化，并且要利用均值和方差的值来归一化，其他值报错
			{
				if (E_CDNN_INPUT_IMAGE_RAW_SCALE_1 == pInputParameters->s32rawScale)//Rawscale=1就是归一化
					pstParams->scaleFactor = (float)1 / 256;
			}
			if (pInputParameters->s8Swap == 1)
				pstParams->eChannelSwap = E_CDNN_BUFFER_IMAGE_CHANNEL_SWAP_TRUE;
			else if (pInputParameters->s8Swap == 0)
				pstParams->eChannelSwap = E_CDNN_BUFFER_IMAGE_CHANNEL_SWAP_FALSE;

			if (pInputParameters->eTranspose)  
				pstParams->eTranspose = E_CDNN_BUFFER_IMAGE_TRANSPOSE_TRUE;
			else
				pstParams->eTranspose = E_CDNN_BUFFER_IMAGE_TRANSPOSE_FALSE;

			if (pInputParameters->s8Resize == 1)
				pstParams->eResize = E_CDNN_BUFFER_IMAGE_RESIZE_TRUE;
			else if (pInputParameters->s8Resize == 0)
				pstParams->eResize = E_CDNN_BUFFER_IMAGE_RESIZE_FALSE;

			pstParams->f32ZeroPoint = pInputParameters->f32ZeroPoint;

		}
	}
	return 0;
}

int loadInputBuffers(void *pCDNNHandle, cdnnDataBuffersArray *pInBuffers, ImageReader_t* pReader, InputTestParameters_t *pApplicationParameters, char **inferenceInputs, int numberOfFiles, int bInputDummies)
{
	cdnnDatab inputImage = NULL;

	int fileIdx;
	for (fileIdx = 0; fileIdx < numberOfFiles; fileIdx++)
	{
		DataBuffer_t* pDataBufferTmp = NULL;

		/* read a single file, if bMultiInput parse it's name. Place the result in pDataBufferTmp. */
		E_IMAGE_READER_EXTRACT_LAYER_FROM_FILE_NAME eExtractLayerName = pApplicationParameters->bMultiInput ?  E_EXTRACT_BUFFER_FROM_FILENAME : E_DONT_EXTRACT_LAYER_FROM_FILENAME;
		if (iruReadFile(pReader, inferenceInputs[fileIdx], eExtractLayerName, &pDataBufferTmp) != E_IR_SUCCESS)
		{
			printf("could not read file: %s\n", inferenceInputs[fileIdx]);
			return -1;
		}

		fillBufferParameters(pDataBufferTmp->pstParams, pApplicationParameters);

		/* create the maximum input image buffer if needed and create data a buffer*/
		if (bInputDummies)
		{
			pDataBufferTmp->pstParams->width = 1280;
			pDataBufferTmp->pstParams->height = 720;
			inputImage = CDNNCreateDataBuffer(pCDNNHandle, pDataBufferTmp->pstParams);
			if (iruDeleteFile(pReader, pDataBufferTmp) != E_IR_SUCCESS) //delete the dummy image 
			{
				printf("could not delete data buffer of image %s\n", inferenceInputs[fileIdx]);
				return -1;
			}
		}
		else
		{
			inputImage = CDNNCreateDataBufferFromHandle(pCDNNHandle, pDataBufferTmp->pstParams, pDataBufferTmp->pRawData);
		}
		

		pInBuffers->pArray[fileIdx] = inputImage;

	}
	
	// The below handles mean buffer if user has supplied pre-processing mean values in CSV file//以下句柄表示缓冲区
	int s32NumMeanBuffers = pApplicationParameters->u32NumOfPreProcessInputBuffers - pApplicationParameters->u32NumOfPreProcessInputs;

	for (fileIdx = 0; fileIdx < s32NumMeanBuffers; fileIdx++)
	{
		//create mean buffer in case user provide their own mean and std values
		float* f32MeanAndStdValues = pApplicationParameters->pInputParameters[fileIdx].f32MeanAndStdValues;
		cdnnDatabufferParameters_t pMeanAndStdBufferParams;
		memset(&pMeanAndStdBufferParams, 0, sizeof(cdnnDatabufferParameters_t));
		pMeanAndStdBufferParams.width = 1;// ???疑问2 输入不是224吗
		pMeanAndStdBufferParams.height = 1;
		pMeanAndStdBufferParams.dataType = E_CDNN_DATATYPE_F32;
		pMeanAndStdBufferParams.depth = E_CDNN_PRECISION_32BIT;
		pMeanAndStdBufferParams.nChannels = E_CDNN_CHANNELS_PER_INPUT;//4个通道，包括一个透明
		pMeanAndStdBufferParams.dataOrder = E_CDNN_MEMORY_DATAORDER_NCHW;//NCHW
		pMeanAndStdBufferParams.nInputs = 2; // 1 is for mean values and another is for std values
		pMeanAndStdBufferParams.pBufferName = pApplicationParameters->bMultiInput ? pApplicationParameters->pInputParameters[fileIdx].sInputName : NULL;
		pMeanAndStdBufferParams.bufferId = 1;
		pMeanAndStdBufferParams.bufferType = E_CDNN_BUFFER_TYPE_DATA;
		pMeanAndStdBufferParams.f32ZeroPoint = 0;
		inputImage = CDNNCreateDataBufferFromHandle(pCDNNHandle, &pMeanAndStdBufferParams, (void*)f32MeanAndStdValues);
		pInBuffers->pArray[numberOfFiles + fileIdx] = inputImage;
	}

	return 0;
}

int initOutputBuffers(void *pCDNNHandle, cdnnNetwork network, int *tailCnt, cdnnDataBuffersArray *pOutBuffers)
{
	int s32status = 0;
	int tailIdx;
	uchar* pRawData = NULL;

	/* query for the network mode and change the output data order accordingly */
	int s32NetworkMode = 0;
	s32status |= CDNNNetworkGetAttribute(network, E_CDNN_ATTRIBUTE_NETWORK_NETWORK_MODE, &s32NetworkMode, sizeof(s32NetworkMode));//MOBILENET yolov3 = 1

	/* retrieve number of network tails (network outputs) */
	s32status |= CDNNNetworkGetAttribute(network, E_CDNN_ATTRIBUTE_NETWORK_TAILS_COUNT, tailCnt, sizeof(tailCnt));//tailCnt ==network outputs  从模型结构中计算出来的

	/* create a buffer-array for all tails */
	pOutBuffers->arrSize = *tailCnt;
	pOutBuffers->pArray = (cdnnDatab*)malloc(sizeof(cdnnDatab)*pOutBuffers->arrSize);
	assert(pOutBuffers->pArray != NULL);

	if (s32NetworkMode == E_CDNN_NETWORK_MODE_FULLYCONVOLUTIONAL)
		assert(*tailCnt == 1);

	/* retrieve network tails */
	for (tailIdx = 0; tailIdx < *tailCnt; tailIdx++)
	{
		char psLayerName[E_CDNN_NETWORK_MAX_LAYER_NAME_SIZE];
		cdnnDatabufferParameters_t newParams, *pNewParams = &newParams;
		pNewParams->pBufferName = psLayerName;

		s32status |= CDNNNetworkGetTailBuffParamsByIndex(network, tailIdx, pNewParams);//+ 音频	 获得TAIL里面的参数,放入pNewParams，包括tail节点的名字psLayerName="MobilenetV1/Predictions/Softmax"
		if (s32status)
			return s32status;

		//make all outputs float(even in fixed-point run)
		pNewParams->dataOrder = (s32NetworkMode == E_CDNN_NETWORK_MODE_FULLYCONVOLUTIONAL) ? E_CDNN_MEMORY_DATAORDER_CNHW : E_CDNN_MEMORY_DATAORDER_NCHW;
		pNewParams->dataType = E_CDNN_DATATYPE_F32;//疑问3？？指定输出的数据类型
		//for float output scale factor must be 0
		pNewParams->scaleFactor = 0;//疑问4？？为何要修改CDNNNetworkGetTailBuffParamsByIndex自动获取的参数值
		pNewParams->eChannelSwap = (E_CDNN_BUFFER_IMAGE_CHANNEL_SWAP)-1;//疑问5？？输出是概率向量，为何还要置换通道 ？ 输出结果不止有概率值，还有框框等
		//for float depth must be set to 4 bytes (32bits)
		pNewParams->depth = E_CDNN_PRECISION_32BIT;
		pNewParams->f32ZeroPoint = 0;

		pRawData = (uchar*)malloc(pNewParams->nInputs * pNewParams->nChannels * pNewParams->height * pNewParams->width * (pNewParams->depth >> 3));//height  ？？==1   ；nChannels = 1001 疑问6  ？都是从模型中获取的，输出节点的矩阵大小 ？是
		if (!pRawData)
			return -1;
		pOutBuffers->pArray[tailIdx] = CDNNCreateDataBufferFromHandle(pCDNNHandle, pNewParams, pRawData);
	}
	
	s32status |= CDNNNetworkSetParameter(network, (cdnnReference)pOutBuffers, E_CDNN_NETWORK_STRUCT_OUTPUT_BUFFERS);

	return s32status;
}

int initOutputFiles(InputTestParameters_t *pApplicationParameters, int tailCnt)
{
	int s32status = 0;

	char pOutputFilename[INPUT_FILE_NAME_SIZE + 3]; //leave room for tail index 
	int tailIdx;
	for (tailIdx = 0; tailIdx < tailCnt; tailIdx++)
	{
		strcpy(pOutputFilename, pApplicationParameters->OutputResultsFilename);
		if (tailCnt > 1)
			appendTailIndex(pApplicationParameters->OutputResultsFilename, pOutputFilename, tailIdx);

		s32status |= resetFile(pOutputFilename);
	}

	return s32status;
}

int outputResults(cdnnDataBuffersArray *pOutBuffers, cdnnNetwork network, InputTestParameters_t *pApplicationParameters, int tailCnt, char **inferenceInputs)
{
	int s32status = 0;
	unsigned int u32LabelsNamelength = 0, channels = 0, width = 0, height = 0, inputs = 0;
	int elementSize = 0, elementCount = 0;
	float* pf32Data = NULL;
	char* psPredictionLabels = NULL;
	char pOutputFilename[INPUT_FILE_NAME_SIZE + 3]; //leave room for tail index
	char top5Name[CMD_LINE_SIZE];

	int tailIdx;
	for (tailIdx = 0; tailIdx < tailCnt; ++tailIdx)
	{//音频43:00"   tail 里面的输出结果（已经保存在pOutBuffers）获取到局部变量elementCount等 
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_ELEMENT_SIZE_IN_BYTES, &elementSize, sizeof(elementSize));//4
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_ELEMENT_COUNT, &elementCount, sizeof(elementCount));//mobilenet  : elementCount = 1000 类别 + 1背景   YOLOV3: elementCount = 255*13*13
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_CHANNELS, &channels, sizeof(channels));
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_WIDTH, &width, sizeof(width));
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_HEIGHT, &height, sizeof(height));
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_INPUTS, &inputs, sizeof(inputs));
		s32status |= CDNNAccessDataBuffer(pOutBuffers->pArray[tailIdx], (void**)&pf32Data);//15：00”疑问9；推理结果在此！！！查看内存地址能看出来，后面是有内容的，要看其排列方式，我们的目的就是要将一维的排列方式解析成原始的方式。例如人脸识别的模型，只能改ProTailFile的文件, CAFFE定义了一个
		s32status |= CDNNNetworkGetAttribute(network, E_CDNN_ATTRIBUTE_NETWORK_CLASSES_NAME_SIZE, &u32LabelsNamelength, sizeof(u32LabelsNamelength));

		strcpy(pOutputFilename, pApplicationParameters->OutputResultsFilename);
		if (tailCnt > 1)
			appendTailIndex(pApplicationParameters->OutputResultsFilename, pOutputFilename, tailIdx);

		/* print results */
		if (!s32status)
		{
			s32status |= appendToFile(pOutputFilename, (char*)pf32Data, elementCount, elementSize);//疑问8 ？？pOutputFilename = CDNNOutputmobilenet_v1_1.0_224_os_tf.xls 打开是乱码??
			if (u32LabelsNamelength)
			{
				if (width == 1 && height == 1 && inputs == 1)//疑问8  ： 限定了是分类模型，其他模型不支持打印结果,例如YOLOV3代码里面还是有的，能转换，可以推理，但是没有后处理。需要自己把一维的pf32Data数据转换成原始的13*13*255的输出格式， 
				{
					s32status |= CDNNAccessNetworkClassesLabels(network, &psPredictionLabels);
					strcpy(top5Name, pOutputFilename);
					strcat(top5Name, "Top5.txt");
					s32status |= resetFile(top5Name);
					s32status |= printResults(inferenceInputs[0], top5Name, 0, psPredictionLabels, u32LabelsNamelength, pf32Data, elementCount);
				}
				else if (tailCnt == 1)
				{
					printf("Error: Prediction labels are not displayed.\nThe network output buffer dimensions are expected to be [1,label_count,1,1] but its: [%d,%d,%d,%d]\n", inputs, channels, height, width);
				}
			}
		}

	}

	return s32status;
}

int releaseInputBuffersArray(void *pCDNNHandle, cdnnDataBuffersArray *pInBuffers, ImageReader_t* pReader, int numberOfBuffers)
{
	int s32status = 0;

	int s32BufferIdx;
	for (s32BufferIdx = 0; s32BufferIdx < numberOfBuffers; s32BufferIdx++)
	{
		s32status |= CDNNReleaseDataBuffer(pCDNNHandle, &(pInBuffers->pArray[s32BufferIdx]));
	}
	iruDeleteAllFiles(pReader);

	return s32status;
}

int outputErrors(InputTestParameters_t *pApplicationParameters, char* ps8ErrorString)
{
	int s32status = 0;

	char Errors[CMD_LINE_SIZE];
	strcpy(Errors, pApplicationParameters->OutputResultsFilename);
	strcat(Errors, "Errors.txt");
	s32status |= resetFile(Errors);
	s32status |= printErrors(Errors, ps8ErrorString);

	return s32status;
}

#if defined (HARDWARE_PLATFORM)
#if defined(XM4) // temporal
unsigned int ll = 0;
void DynamicDeadCodeEliminationError() {
	__asm("mov retreg.ui, r0.ui ");
	__asm("st r0.i,(#_ll).di");
	printf("\nDynamic Dead Code Elimination removed used function that was called from address %x\n", ll);
}
void(*DDCEE)() = DynamicDeadCodeEliminationError;
#endif
#if defined(SensPro) // temporal
unsigned int ll = 0;
void DynamicDeadCodeEliminationError() {
	__asm("movp retreg.ui, r0.ui");
	__asm("st r0.i,(#ll).di");
	printf("\nDynamic Dead Code Elimination removed used function that was called from address %x\n", ll);
}
void(*DDCEE)() = DynamicDeadCodeEliminationError;
#endif
#endif


