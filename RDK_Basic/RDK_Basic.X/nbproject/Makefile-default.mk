#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RDK_Basic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RDK_Basic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../main.c ../MtrCtrl.c ../spi.c ../util.c ../PID.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/MtrCtrl.o ${OBJECTDIR}/_ext/1472/spi.o ${OBJECTDIR}/_ext/1472/util.o ${OBJECTDIR}/_ext/1472/PID.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/MtrCtrl.o.d ${OBJECTDIR}/_ext/1472/spi.o.d ${OBJECTDIR}/_ext/1472/util.o.d ${OBJECTDIR}/_ext/1472/PID.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/MtrCtrl.o ${OBJECTDIR}/_ext/1472/spi.o ${OBJECTDIR}/_ext/1472/util.o ${OBJECTDIR}/_ext/1472/PID.o

# Source Files
SOURCEFILES=../main.c ../MtrCtrl.c ../spi.c ../util.c ../PID.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/RDK_Basic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX460F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/e755b538d861a6cde3e399a2eab03df54bbb0bb5 .generated_files/flags/default/f2d2849a63ebd9d9bb029b271492fb5470a4c3d1
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/MtrCtrl.o: ../MtrCtrl.c  .generated_files/flags/default/e4303271afaccd5964482e1e4f4d17d1c33a08d5 .generated_files/flags/default/f2d2849a63ebd9d9bb029b271492fb5470a4c3d1
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MtrCtrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MtrCtrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/_ext/1472/MtrCtrl.o.d" -o ${OBJECTDIR}/_ext/1472/MtrCtrl.o ../MtrCtrl.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/spi.o: ../spi.c  .generated_files/flags/default/adba73dd677d69eece04edce7ebaef15e38cda42 .generated_files/flags/default/f2d2849a63ebd9d9bb029b271492fb5470a4c3d1
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/_ext/1472/spi.o.d" -o ${OBJECTDIR}/_ext/1472/spi.o ../spi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/util.o: ../util.c  .generated_files/flags/default/54082cb384577b7c460a5ab391f872ce5b90374e .generated_files/flags/default/f2d2849a63ebd9d9bb029b271492fb5470a4c3d1
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/util.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/util.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/_ext/1472/util.o.d" -o ${OBJECTDIR}/_ext/1472/util.o ../util.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/PID.o: ../PID.c  .generated_files/flags/default/6ab9193bc42524d077242616167ba41cede86541 .generated_files/flags/default/f2d2849a63ebd9d9bb029b271492fb5470a4c3d1
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/PID.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/PID.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/_ext/1472/PID.o.d" -o ${OBJECTDIR}/_ext/1472/PID.o ../PID.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
else
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/d595bf943d773b32f182dc677965041b01b5d291 .generated_files/flags/default/f2d2849a63ebd9d9bb029b271492fb5470a4c3d1
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/MtrCtrl.o: ../MtrCtrl.c  .generated_files/flags/default/d10ada72df85f6671c271d60a19eaba5e6c3465d .generated_files/flags/default/f2d2849a63ebd9d9bb029b271492fb5470a4c3d1
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MtrCtrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MtrCtrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/_ext/1472/MtrCtrl.o.d" -o ${OBJECTDIR}/_ext/1472/MtrCtrl.o ../MtrCtrl.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/spi.o: ../spi.c  .generated_files/flags/default/1c9da881018bec6b833aef69e47eeac361c93423 .generated_files/flags/default/f2d2849a63ebd9d9bb029b271492fb5470a4c3d1
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/spi.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/_ext/1472/spi.o.d" -o ${OBJECTDIR}/_ext/1472/spi.o ../spi.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/util.o: ../util.c  .generated_files/flags/default/6509d50652f1616d74a483939572e629feb6b959 .generated_files/flags/default/f2d2849a63ebd9d9bb029b271492fb5470a4c3d1
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/util.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/util.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/_ext/1472/util.o.d" -o ${OBJECTDIR}/_ext/1472/util.o ../util.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/PID.o: ../PID.c  .generated_files/flags/default/15a6649393b4af804bc418b8736dbcea4e0c2627 .generated_files/flags/default/f2d2849a63ebd9d9bb029b271492fb5470a4c3d1
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/PID.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/PID.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/_ext/1472/PID.o.d" -o ${OBJECTDIR}/_ext/1472/PID.o ../PID.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/RDK_Basic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PIC32MXSK=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RDK_Basic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PIC32MXSK=1,-L"../../../../../../../../../Program Files (x86)/Microchip/MPLAB C32 Suite/lib",-L"../../../../../../../../../Program Files (x86)/Microchip/MPLAB C32 Suite/pic32mx/lib",-L".",-Map="${DISTDIR}/RDK_Basic.X.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/RDK_Basic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RDK_Basic.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-L"../../../../../../../../../Program Files (x86)/Microchip/MPLAB C32 Suite/lib",-L"../../../../../../../../../Program Files (x86)/Microchip/MPLAB C32 Suite/pic32mx/lib",-L".",-Map="${DISTDIR}/RDK_Basic.X.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml 
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/RDK_Basic.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
