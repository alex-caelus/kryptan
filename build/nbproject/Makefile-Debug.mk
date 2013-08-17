#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1228218613/ModifiedEncryptor.o \
	${OBJECTDIR}/_ext/1228218613/Pwd.o \
	${OBJECTDIR}/_ext/1228218613/PwdFile.o \
	${OBJECTDIR}/_ext/1228218613/PwdFileWorker.o \
	${OBJECTDIR}/_ext/1228218613/PwdList.o \
	${OBJECTDIR}/_ext/1228218613/SecureString.o \
	${OBJECTDIR}/_ext/229759634/DialogBase.o \
	${OBJECTDIR}/_ext/229759634/MainMenu.o \
	${OBJECTDIR}/_ext/229759634/MessageBoxes.o \
	${OBJECTDIR}/_ext/229759634/Program.o \
	${OBJECTDIR}/_ext/229759634/Prompts.o \
	${OBJECTDIR}/_ext/229759634/Utilites.o \
	${OBJECTDIR}/_ext/229759634/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lcryptopp -lncurses -lpanel -lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kryptan

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kryptan: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kryptan ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/1228218613/ModifiedEncryptor.o: ../src/kryptan_core/ModifiedEncryptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/ModifiedEncryptor.o ../src/kryptan_core/ModifiedEncryptor.cpp

${OBJECTDIR}/_ext/1228218613/Pwd.o: ../src/kryptan_core/Pwd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/Pwd.o ../src/kryptan_core/Pwd.cpp

${OBJECTDIR}/_ext/1228218613/PwdFile.o: ../src/kryptan_core/PwdFile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/PwdFile.o ../src/kryptan_core/PwdFile.cpp

${OBJECTDIR}/_ext/1228218613/PwdFileWorker.o: ../src/kryptan_core/PwdFileWorker.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/PwdFileWorker.o ../src/kryptan_core/PwdFileWorker.cpp

${OBJECTDIR}/_ext/1228218613/PwdList.o: ../src/kryptan_core/PwdList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/PwdList.o ../src/kryptan_core/PwdList.cpp

${OBJECTDIR}/_ext/1228218613/SecureString.o: ../src/kryptan_core/SecureString.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/SecureString.o ../src/kryptan_core/SecureString.cpp

${OBJECTDIR}/_ext/229759634/DialogBase.o: ../src/kryptan_program/DialogBase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/DialogBase.o ../src/kryptan_program/DialogBase.cpp

${OBJECTDIR}/_ext/229759634/MainMenu.o: ../src/kryptan_program/MainMenu.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/MainMenu.o ../src/kryptan_program/MainMenu.cpp

${OBJECTDIR}/_ext/229759634/MessageBoxes.o: ../src/kryptan_program/MessageBoxes.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/MessageBoxes.o ../src/kryptan_program/MessageBoxes.cpp

${OBJECTDIR}/_ext/229759634/Program.o: ../src/kryptan_program/Program.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/Program.o ../src/kryptan_program/Program.cpp

${OBJECTDIR}/_ext/229759634/Prompts.o: ../src/kryptan_program/Prompts.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/Prompts.o ../src/kryptan_program/Prompts.cpp

${OBJECTDIR}/_ext/229759634/Utilites.o: ../src/kryptan_program/Utilites.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/Utilites.o ../src/kryptan_program/Utilites.cpp

${OBJECTDIR}/_ext/229759634/main.o: ../src/kryptan_program/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/main.o ../src/kryptan_program/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kryptan

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
