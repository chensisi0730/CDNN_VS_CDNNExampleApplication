#CDNNExampleApplication

#rs-todo: fix this dependencies and make it general for all configurations

# the dependencies from CDNNPCDevice and above are for setting build order 

if(${BUILD_PLATFORM} STREQUAL "llvm")
	set(InternalDependencies "${XM_TOOLS_LIBS_PATH}/crt0.o" "${XM_TOOLS_LIBS_PATH}/crtn.o" "--start-group" "CDNNTarget" "CDNNManager" "CDNNAppUtility" "CDNNKernelModuleCommon" "CDNNDevice" "CDNNPCKernelModule" "CDNNUtility" "CDNNInfrastructure")
else()
	set(InternalDependencies "libjpeg" "CDNNTarget" "CDNNManager" "CDNNAppUtility" "CDNNKernelModuleCommon" "CDNNDevice" "CDNNPCKernelModule" "CDNNUtility" "CDNNInfrastructure")
endif()
# "CDNNXM6KernelModule" "CDNNXM4KernelModule" "CDNNXM6Device" "CDNNXM4Device")
#
# "CDNNXM6Device" 
#set(InternalDependencies "CDNNXM4Device") 

# "CDNNXM4Device" 
if(XM_TOOLS)
	if(${BUILD_PLATFORM} STREQUAL "open64")
		set(LINKER_FLAGS "-p,-d,HARDWARE_PLATFORM")
	endif()
	
	add_compile_definitions(${HARDWARE_FLAGS})
	add_compile_definitions(${PRINT_LEVEL_FLAGS})
	
	set(directories "${XM_TOOLS_LIBS_PATH}"
	"${CMAKE_BINARY_DIR}/CDNNBin/Lib/${DSP_TYPE}/${SDT_BUILD_CONFIGURATION}/")
		
	#csl_xm#d never taken ?!
	# set(ExternalDependencies "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/csl_${xm_}")
	if(${BUILD_PLATFORM} STREQUAL "open64")
		set(ExternalDependenciesPaths "${XM_TOOLS_LIBS_PATH}/libcc.lib"
		"${XM_TOOLS_LIBS_PATH}/libc.lib"
		"${XM_TOOLS_LIBS_PATH}/libcio.lib"
		"${XM_TOOLS_LIBS_PATH}/complexlib.lib"
		"${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/csl_${xm_}.lib"
		)
	# todo: this CompilerRTL is temp fix for the full path of this ExternalDependenciesPaths , move this to appMethods and do it like in open64
	elseif(${BUILD_PLATFORM} STREQUAL "llvm") 
		if(SensPro)
			set(ExternalDependenciesPaths "${XM_TOOLS_LIBS_PATH}/libc.a"
			"${XM_TOOLS_LIBS_PATH}/libc++.a"
			"${XM_TOOLS_LIBS_PATH}/libc++abi.a"
			"${XM_TOOLS_LIBS_PATH}/libcompiler-rt.a"
			"${CMAKE_SOURCE_DIR}/../../Installs/senspro/lib/ceva_tools/libcsl.a"
			)
		else()
			set(ExternalDependenciesPaths "${XM_TOOLS_LIBS_PATH}/libc.a"
			"${XM_TOOLS_LIBS_PATH}/libc++.a"
			"${XM_TOOLS_LIBS_PATH}/libc++abi.a"
			"${XM_TOOLS_LIBS_PATH}/libcompiler-rt.a"
			"${CMAKE_SOURCE_DIR}/../../Installs/install_xm6/lib/libcsl_xm6_elf.a"
			)
		endif()
	endif()

	
	### todo: llvm compiler need convert lib type alone ? - check it  
	# if(open64)
	# 	list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/csl_${xm_}.lib")
	# elseif(llvm)
	# 	list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/libcsl_${xm_}.a")
	# endif()
	
	
	if(PLATFORM-CONFIGURATION__HOST_IO_ENABLED)
		include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../CDNNTools/CDNNAppUtility/3rdparty/CevaHostIO/Include")
		add_compile_definitions(CEVA_HOST_IO_ENABLED)
		if(${BUILD_PLATFORM} STREQUAL "open64")
			list(APPEND ExternalDependenciesPaths "${CMAKE_CURRENT_SOURCE_DIR}/../CDNNTools/CDNNAppUtility/3rdparty/CevaHostIO/${xm_}/hostio_${xm_}.lib"
			"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNTools/CDNNAppUtility/3rdparty/CevaHostIO/${xm_}/ceva_link_share.lib"
			)
			set(LINKER_FLAGS "${LINKER_FLAGS} -p,-d,CEVA_HOST_IO_ENABLED")
		elseif(${BUILD_PLATFORM} STREQUAL "llvm")
			list(APPEND ExternalDependenciesPaths "${CMAKE_CURRENT_SOURCE_DIR}/../CDNNTools/CDNNAppUtility/3rdparty/CevaHostIO/${xm_}/libhostio_${xm_}_lib.a"
			"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNTools/CDNNAppUtility/3rdparty/CevaHostIO/${xm_}/libceva_link_share.a"
			)
		endif()
	endif()
	
	if(XM4)
		list(APPEND ExternalDependenciesPaths "${XM_TOOLS_LIBS_PATH}/xm4lib.lib")
		list(APPEND InternalDependencies "CDNNXM4Device" "CDNNXM4KernelModule" "UserXM4OperatorLibrary")
	elseif(XM6)
		# todo: change the lib location according to rtl version
		if(BUILD_PLATFORM)
			if(${BUILD_PLATFORM} STREQUAL "open64")
				list(APPEND ExternalDependenciesPaths "${XM_TOOLS_LIBS_PATH}/rtl1_2_0/cevaxm6lib.lib")
			elseif(${BUILD_PLATFORM} STREQUAL "llvm")
				list(APPEND ExternalDependenciesPaths "--end-group")
				#list(APPEND ExternalDependenciesPaths "C:/CEVA-ToolBox/V18/XM6/cevatools/lib/clang/4.0.1/cevaxm6-unknown-unknown-elf/lib/rtlv1.2.0-fp4/libxm6lib.a" "--end-group")
			endif()
		endif()
		list(APPEND InternalDependencies "CDNNXM6Device" "CDNNXM6KernelModule" "UserXM6OperatorLibrary")
	else() #SENSPRO
		list(APPEND ExternalDependenciesPaths "--end-group")
		list(APPEND InternalDependencies "CDNNSensProDevice" "CDNNSensProKernelModule" "UserSensProOperatorLibrary")
	endif()

	if (DEVICES__CDNNHWADEVICE)
		if (${DEVICES__CDNNHWADEVICE} STREQUAL ON)
			list(APPEND InternalDependencies "CDNNCNNHADevice")
			list(APPEND ExternalDependencies "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/platform_mgr")
			if(BUILD_PLATFORM)
				if(${BUILD_PLATFORM} STREQUAL "open64")
					set(LINKER_FLAGS "${LINKER_FLAGS} -p,-d,CNNHWA")
				elseif(${BUILD_PLATFORM} STREQUAL "llvm")
					#set(LINKER_FLAGS "${LINKER_FLAGS} -defsym CNNHWA")
				endif()
			endif()
		endif()
	endif()
	if (DEVICES__NPRDEVICE)
		if (${DEVICES__NPRDEVICE} STREQUAL ON)
			list(APPEND InternalDependencies "CDNNNeuProDevice") 
			if (SensPro)
				list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/senspro/lib/ceva_tools/libplatform_mgr.a")
				list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/senspro/lib/ceva_tools/libNeuProRuntime.a")
			else()
				list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/libplatform_mgr.a")
				list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/libNeuProRuntime.a")
			endif()
			if(BUILD_PLATFORM)
				if(${BUILD_PLATFORM} STREQUAL "open64")
					set(LINKER_FLAGS "${LINKER_FLAGS} -p,-d,CNNHWA")#in the linker this CNNHWA flag means it's a sandwitch board
				elseif(${BUILD_PLATFORM} STREQUAL "llvm")
					#set(LINKER_FLAGS "${LINKER_FLAGS} -defsym CNNHWA")
				endif()
			endif()
		endif()
	endif()
	if (DEVICES__NPRSDEVICE)
		if (${DEVICES__NPRSDEVICE} STREQUAL ON)
			list(APPEND InternalDependencies "CDNNNeuProSDevice") 
			if (SensPro)
				list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/senspro/lib/ceva_tools/libplatform_mgr.a")
				list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/senspro/lib/ceva_tools/libNeuProRuntime.a")
			else()
				list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/libplatform_mgr.a")
				list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/libNeuProRuntime.a")
				list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/libneupro_service.a")
			endif()
			list(APPEND ExternalDependenciesPaths "${XM_TOOLS_LIBS_PATH}/libc.a")
			if(BUILD_PLATFORM)
				if(${BUILD_PLATFORM} STREQUAL "open64")
					set(LINKER_FLAGS "${LINKER_FLAGS} -p,-d,CNNHWA")#in the linker this CNNHWA flag means it's a sandwitch board
				elseif(${BUILD_PLATFORM} STREQUAL "llvm")
					#set(LINKER_FLAGS "${LINKER_FLAGS} -defsym CNNHWA")
				endif()
			endif()
		endif()
	endif()
	if (DEVICES__CUSTOMERDEVICE)
		if (${DEVICES__CUSTOMERDEVICE} STREQUAL ON)
			list(APPEND InternalDependencies "${CDNNCUSTOMERDevice}") 
			list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/libplatform_mgr.a")
			list(APPEND ExternalDependenciesPaths "${CDNNCUSTOMERDevice_runtime_sdt_driver}")
			if(BUILD_PLATFORM)
				if(${BUILD_PLATFORM} STREQUAL "open64")
					set(LINKER_FLAGS "${LINKER_FLAGS} -p,-d,CNNHWA")#in the linker this CNNHWA flag means it's a sandwitch board
				elseif(${BUILD_PLATFORM} STREQUAL "llvm")
					#set(LINKER_FLAGS "${LINKER_FLAGS} -defsym CNNHWA")
				endif()
			endif()
		endif()
	endif()
	if (DEVICES__CUSTOMDEVICE)
		if (${DEVICES__CUSTOMDEVICE} STREQUAL ON)
			list(APPEND InternalDependencies "CDNNCustomDevice")
			list(APPEND InternalDependencies "ExampleUserDeviceLibrary") #Example
			if (BINARY_PACKAGE)
				if (SensPro)
					list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/senspro/lib/ceva_tools/libplatform_mgr.a")
					list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/senspro/lib/ceva_tools/libNeuProRuntime.a")
				endif()
				if (XM6)
					list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/libplatform_mgr.a")
					list(APPEND ExternalDependenciesPaths "${CMAKE_SOURCE_DIR}/../../Installs/install_${xm_}/lib/libNeuProRuntime.a")
				endif()	
			endif()	
		endif()
	endif()
elseif(LINUX)
	
	
else() # MSVC
	
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CONSOLE _LIB CDNN_PC_SIMULATION)
	ADD_DEFINITIONS(-DUNICODE)
	ADD_DEFINITIONS(-D_UNICODE)

	#todo : remove unrelevant directories and move to general configuration if needed 
	include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNBin/Lib/${CMAKE_BUILD_TYPE}/${CMAKE_VS_PLATFORM_NAME}/"
	"${CMAKE_SOURCE_DIR}/../../3rdparty/sdtguard/sgutilLib/${CMAKE_BUILD_TYPE}"
	"${CMAKE_SOURCE_DIR}/../../Installs/install_generic/Accelerators/CNNHWA/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNCommonDefines/Include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNTools/CDNNUtility/Include/"
	"${CMAKE_SOURCE_DIR}/CDNNManager/Interface/Include"
	)
	
	if (DEVICES__XM6DEVICE)
		if (${DEVICES__XM6DEVICE} STREQUAL "ON")
			list(APPEND InternalDependencies "CDNNXM6Device" "UserXM6OperatorLibrary")
		endif()
	endif()
	if (DEVICES__XM4DEVICE)
		if (${DEVICES__XM4DEVICE} STREQUAL "ON")
			list(APPEND InternalDependencies "CDNNXM4Device" "UserXM4OperatorLibrary")
		endif()
	endif()
	if (DEVICES__SensProDEVICE)
		if (${DEVICES__SensProDEVICE} STREQUAL "ON")
			list(APPEND InternalDependencies "CDNNSensProDevice")
		endif()
	endif()
	if (DEVICES__CDNNHWADEVICE)
		if (${DEVICES__CDNNHWADEVICE} STREQUAL ON)
			LIST(APPEND InternalDependencies "CDNNCNNHADevice")
		endif()
	endif()
	if (DEVICES__NPRDEVICE)
		if (${DEVICES__NPRDEVICE} STREQUAL ON)
			LIST(APPEND InternalDependencies "CDNNNeuProDevice")
		endif()
	endif()
	if (DEVICES__NPRSDEVICE)
		if (${DEVICES__NPRSDEVICE} STREQUAL ON)
			LIST(APPEND InternalDependencies "CDNNNeuProSDevice")
		endif()
	endif()
	if (DEVICES__NPRMDEVICE)
		if (${DEVICES__NPRMDEVICE} STREQUAL ON)
			LIST(APPEND InternalDependencies "CDNNNeuProMDevice")
		endif()
	endif()
	if (DEVICES__CUSTOMERDEVICE)
		if (${DEVICES__CUSTOMERDEVICE} STREQUAL ON)
			LIST(APPEND InternalDependencies "CDNNCUSTOMERDevice")
		endif()
	endif()
	if (DEVICES__CUSTOMDEVICE)
		if (${DEVICES__CUSTOMDEVICE} STREQUAL ON)
			LIST(APPEND InternalDependencies "CDNNCUSTOMDevice")
			LIST(APPEND InternalDependencies "ExampleUserDeviceLibrary") #Example
		endif()
	endif()
	
	if (NOT BINARY_PACKAGE AND NOT SOURCE_PACKAGE)
		LIST(APPEND InternalDependencies "Ws2_32")
	endif()
	
	set(directories 
	"${CMAKE_SOURCE_DIR}/../../Installs/install_generic/lib/"
	"${CMAKE_SOURCE_DIR}/../../Installs/install_generic/lib/Win32/"
	"${CMAKE_CURRENT_BINARY_DIR}/../CDNNBin/Lib/${CMAKE_VS_PLATFORM_NAME}/${CMAKE_BUILD_TYPE}/"
	"${CMAKE_CURRENT_BINARY_DIR}/../CDNNBin/Dll/${CMAKE_VS_PLATFORM_NAME}/${CMAKE_BUILD_TYPE}/")
	 
endif()


	
list(APPEND InternalDependencies "UserPCOperationLibrary" "UserPCOperatorLibrary"  "UserNodeLibrary")

include_directories(
"${CMAKE_CURRENT_SOURCE_DIR}/hdr"
"${CMAKE_SOURCE_DIR}/../../Installs/install_generic/include/"
"${CMAKE_SOURCE_DIR}/CDNNTools/CDNNAppUtility/hdr"
"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNInterface/External"
"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNTools/CDNNUtility/Include/"
)





set (folder CDNNApplications)
createExe(${folder} ${InternalDependencies} ${directories} ${ExternalDependenciesPaths} ${ExternalDependencies} ${InternalDependencies} ${LINKER_FLAGS})

# create ouput directory -- todo: remove it , it's diplicated
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/out)



