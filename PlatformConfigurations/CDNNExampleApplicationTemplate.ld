OUTPUT(a.elf)

/* By default, program starts from reset address (the default location of the interrupt table) */
ENTRY(__cxd_inttbl_start)

/** Memory configuration parameters.
 *  The parameters become application symbols and can be referred from application
 */
__internal_code_start = DEFINED(__internal_code_start) ? __internal_code_start : 0x00000000;
__internal_code_size  = DEFINED(__internal_code_size) ? __internal_code_size  : 32k;
__internal_data_start = DEFINED(__internal_data_start) ? __internal_data_start : 0x00000000;
__internal_size  = DEFINED(__internal_data_size) ? __internal_data_size  : 256k;
__external_start_data = DEFINED(__external_start) ? __external_start      : 0x40000000;
__external_start_code = DEFINED(__external_start) ? __external_start      : 0x5f000000;
/*L2 memory address is the same for simulator and FPGA, should never be changed*/
__l2_start_data  	  = DEFINED(__l2_start_data) ? __l2_start_data				: 0x5fe00000;
__external_size_data  = DEFINED(__external_size) ? __external_size				: 496M;
__external_size_code  = DEFINED(__external_size) ? __external_size				: 14M;
/*L2 memory size is 2MB by default*/
__l2_size_data  	  = DEFINED(__l2_size_data) ? __l2_size_data  				: 2048k;
__rom_start           = DEFINED(__rom_start) ? __rom_start           : 0xC0000000;
__rom_size            = DEFINED(__rom_size) ? __rom_size            : 1024M;

__malloc_size         = DEFINED(__malloc_size) ? __malloc_size		: 80M;
__stack_size          = DEFINED(__stack_size) ? __stack_size          : 16k;
__arg_sect_size       = DEFINED(__arg_sect_size) ? __arg_sect_size       : 512;

__external_end_data = __external_start_data + __external_size_data;
__internal_end_data = __internal_data_start + __internal_size;
__l2_end_data = __l2_start_data + __l2_size_data;

MEMORY {
    /*INTERNAL_CODE (rx) : ORIGIN = __internal_code_start	, LENGTH = __internal_code_size*/
    INTERNAL_DATA  (rw) : ORIGIN = __internal_data_start	, LENGTH = __internal_size
    EXTERNAL_DATA  (rw) : ORIGIN = __external_start_data   	, LENGTH = __external_size_data
    EXTERNAL_CODE  (rx) : ORIGIN = 0     					, LENGTH = __external_size_code
	L2_DATA  	   (rw) : ORIGIN = __l2_start_data     	, LENGTH = __l2_size_data
    EXTERNAL_HOSTIO1(rw) : ORIGIN = 0xE0000000     			, LENGTH = 64k
    EXTERNAL_HOSTIO2(rw) : ORIGIN = 0xE0010000     			, LENGTH = 16M
}

SECTIONS {
    .inttbl : ALIGN(0x20) {
        /** The interrupt vector resides at address zero and contains the NMI
         *  and maskable interrupt handlers
         */
        . = 0x0;
        KEEP(*(.inttbl))
    } >EXTERNAL_CODE

	.code.internal.csl : ALIGN(0x20) {
	        KEEP(*(CSL_INTERNAL_CODE_ALLOCATION))
	} >EXTERNAL_CODE
	
	.code.external.csl : ALIGN(0x20) {
		KEEP(*(CSL_EXTERNAL_CODE_ALLOCATION)) 
	} >EXTERNAL_CODE
	
    .text : ALIGN(0x20) {
        PROVIDE(__text_start = ABSOLUTE(.));
        /* The __call_saved* functions need to be placed at low addresses for
         * calling with absolute call instructions
         */
        *(.text.__call_saved*)
        *(.text .text.*)
        PROVIDE(__text_end = ABSOLUTE(.));
    } >EXTERNAL_CODE

	.code.external.cdnn : ALIGN(0x20) {
		KEEP(*(CDNN_MEMORY_CODE_SECTION)) 
	} >EXTERNAL_CODE

	.data.const.cdnn : ALIGN(0x4) {
		*Math*(.rodata .rodata.*)
		*Matrix*(.rodata .rodata.*)
		*VecC*(.rodata .rodata.*)
		*FullyConnected*(.rodata .rodata.*)
		ConvolutionWeightsReorder*(.rodata .rodata.*)
		DetectionOutputNms*(.rodata .rodata.*)
		FastRCNNNNms*(.rodata .rodata.*)
		VectorOperations*(.rodata .rodata.*)
	} > INTERNAL_DATA
	
	.data.external : ALIGN(0x4) {
        PROVIDE(__data_internal_start = ABSOLUTE(.));
        /* Don't map any data at address zero to avoid issues with C NULL
         * pointer checks
         */
        . += 0x4;
        PROVIDE(__data_start = ABSOLUTE(.));
        *(.data .data.*)
        PROVIDE(__data_end = ABSOLUTE(.));
        PROVIDE(__data_size = +__data_end - __data_start);

        PROVIDE(__sdata_start = ABSOLUTE(.));
        *(.sdata .sdata.*)
        PROVIDE(__sdata_end = ABSOLUTE(.));
        PROVIDE(__sdata_size = +__sdata_end - __sdata_start);

        PROVIDE(__rodata_start = ABSOLUTE(.));
        *(.rodata .rodata.*)
        PROVIDE(__rodata_end = ABSOLUTE(.));
        PROVIDE(__rodata_size = +__rodata_end - __rodata_start);
        PROVIDE(__data_internal_end = ABSOLUTE(.));
        PROVIDE(__data_internal_size = __data_internal_end - __data_internal_start);
    } >EXTERNAL_DATA
	
	data_internal_loadable_addr = __data_internal_start;
   
	.bss (NOLOAD) : ALIGN(0x20) {
        PROVIDE(__bss_start = ABSOLUTE(.));
        *(.bss .bss.*)
        PROVIDE(__common_start = ABSOLUTE(.));
        *(COMMON)
        PROVIDE(__common_end = ABSOLUTE(.));
        PROVIDE(__common_size = +__common_end - __common_start);
        PROVIDE(__bss_end = ABSOLUTE(.));
        PROVIDE(__bss_size = +__bss_end - __bss_start);
    } >EXTERNAL_DATA
	
     __STACK_SECT (NOLOAD) : ALIGN(0x10) {
    	__stack_start = ABSOLUTE(.);
        . = . + __stack_size;
        __stack_end = ABSOLUTE(.);
    }  >INTERNAL_DATA 
	
	    /* Program arguments are loaded by `_start` routine from `__arg_sect_start`.
     * When the user has set a zero size for the section, argc, and argv
     * will be zero and NULL, respectively.
     * Although likely small, they are on the slow path so by default they
     * go at the end of external memory
     */
    __ARG_SECT (NOLOAD) : ALIGN(0x4) {
        __arg_sect_start = ABSOLUTE(.);
        . = . + (__arg_sect_size ? __arg_sect_size + 4 : 0);
        __arg_sect_end = ABSOLUTE(.);
    } >EXTERNAL_DATA 
	
	
	.data.internal.csl : {
		KEEP(*(CSL_INTERNAL_DATA_ALLOCATION))
	} >INTERNAL_DATA 
	
	.data.internal.cdnn.const : {
		KEEP(*(CDNN_const_Data))
	} >INTERNAL_DATA 
	
	.data.internal.cdnn.system (NOLOAD) : {
		KEEP(*(CDNN_INTERNAL_SYSTEM_MEMORY_ALLOCATION))
	} >INTERNAL_DATA 

	.data.internal.hostio : {
		KEEP(*(int_fileio_data))
	} >INTERNAL_DATA 

	/*********************************************************************
		The .data.internal.cdnn.memory section must be the last section
		mapped to in the internal memory!!!
	**********************************************************************/
	.data.internal.cdnn.memory (NOLOAD) : {
		PROVIDE(gu8MemoryStartInternalAllocationBuffer = ABSOLUTE(.));
		. = . + (__internal_end_data - gu8MemoryStartInternalAllocationBuffer);
		PROVIDE(gu8MemoryEndInternalAllocationBuffer = ABSOLUTE(.));
	} >INTERNAL_DATA
	
	.data.internal.neupro : ALIGN(0x10) {
		KEEP(*(neupro_int_data))
	} > EXTERNAL_DATA

 	.data.external.csl : ALIGN(0x10) {
		KEEP(*(CSL_EXTERNAL_DATA_ALLOCATION))
	} > EXTERNAL_DATA

 	.data.external.cdnn.static : ALIGN(0x10) {
		KEEP(*(CDNN_EXTERNAL_MEMORY_ALLOCATION_STATIC))
	} > EXTERNAL_DATA

    .data.external : ALIGN(0x4) {
        /** .data1, .rodata1, .sdata1 are all for large symbols which cannot
         * fit in limited internal memory. We put them in external memory by
         * default. */
        PROVIDE(__data_external_start = ABSOLUTE(.));

        PROVIDE(__data1_start = ABSOLUTE(.));
        *(.data1 .data1.*)
        PROVIDE(__data1_end = ABSOLUTE(.));

        PROVIDE(__sdata1_start = ABSOLUTE(.));
        *(.sdata1 .sdata1.*)
        PROVIDE(__sdata1_end = ABSOLUTE(.));
        PROVIDE(__sdata1_size = +__sdata1_end - __sdata1_start);

        PROVIDE(__rodata1_start = ABSOLUTE(.));
        *(.rodata1 .rodata1.*)
        PROVIDE(__rodata1_end = ABSOLUTE(.));
        PROVIDE(__rodata1_size = +__rodata1_end - __rodata1_start);

        /* Constructors and destructors are called once per program invocation,
         * so are never in the hot path; they shouldn't waste space in limited
         * internal memory so we place them in slower, external memory
         */

        . = ALIGN(4); /* constructors must be aligned on a word boundary */
        PROVIDE(__init_array_start = ABSOLUTE(.));
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)));
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array*) SORT_BY_INIT_PRIORITY(.ctors*)));
        PROVIDE(__init_array_end = ABSOLUTE(.));

        PROVIDE(__fini_array_start = ABSOLUTE(.));
        /* destructors are run in reverse order of their priority */
        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)));
        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array*) SORT_BY_INIT_PRIORITY(.dtors*)));
        PROVIDE(__fini_array_end = ABSOLUTE(.));
        PROVIDE(__data_external_end = ABSOLUTE(.));
        PROVIDE(__data_external_size = __data_external_end - __data_external_start);
    } >EXTERNAL_DATA
	
	data_external_loadable_addr = __data_external_start;
	
	.bss1 (NOLOAD) : ALIGN(0x20) {
        /**
         * `.bss1` is for large zero-initialized symbols that do not fit in
         * internal data
         */
        PROVIDE(__bss1_start = ABSOLUTE(.));
        *(.bss1 .bss1.*)
        PROVIDE(__large_common_start = ABSOLUTE(.));
        *(LARGE_COMMON)
        PROVIDE(__large_common_end = ABSOLUTE(.));
        PROVIDE(__large_common_size = +__large_common_end - __large_common_start);
        PROVIDE(__bss1_end = ABSOLUTE(.));
        PROVIDE(__bss1_size = +__bss1_end - __bss1_start);
    } >EXTERNAL_DATA



	__MALLOC_SECT (NOLOAD) : ALIGN(0x4) {
        PROVIDE(__malloc_start = ABSOLUTE(.));
        . = . + __malloc_size;
        PROVIDE(__malloc_end = ABSOLUTE(.));
    } >EXTERNAL_DATA

	/*********************************************************************
		The .data.external.cdnn.memory section must be the last section
		mapped to in the external memory!!!
	**********************************************************************/
	.data.external.cdnn.memory (NOLOAD) : ALIGN(0x4) {
		PROVIDE(gu8MemoryStartExternalAllocationBuffer = ABSOLUTE(.));
		. = . + (__external_end_data - gu8MemoryStartExternalAllocationBuffer);
		PROVIDE(gu8MemoryEndExternalAllocationBuffer = ABSOLUTE(.));
	} >EXTERNAL_DATA

	.data.L2.cdnn.memory (NOLOAD) : ALIGN(0x4) {
	PROVIDE(gu8MemoryStartL2AllocationBuffer = ABSOLUTE(.));
	. = . + (__l2_end_data - gu8MemoryStartL2AllocationBuffer);
	PROVIDE(gu8MemoryEndL2AllocationBuffer = ABSOLUTE(.));
	} >L2_DATA

	.data.hostio1 : {
		KEEP(*(AMF_shared_info_mem_sect))
	} >EXTERNAL_HOSTIO1 

	
	.data.hostio2 : {
		KEEP(*(ext_fileio_data))
	} >EXTERNAL_HOSTIO2
	
    /DISCARD/ : {
        /* Discarding .note.CEVA-arch saves a fair amount of space but
         * confounds the restriction checker. YMMV */
         /* *(.note.CEVA-arch)*/
         /* *(.debug_*) */
        *(.comment)
        *(.note.GNU-stack)
        /* The X-DSP ABI uses a custom relocation format stored in its own
         * section. These are left in the binary by default but are unneeded. */
        *(.ceva_reloc)
    }

}
