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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1228218613/Utilites.o \
	${OBJECTDIR}/_ext/229759634/UI.o \
	${OBJECTDIR}/_ext/229759634/UiElement.o \
	${OBJECTDIR}/_ext/1228218613/PwdList.o \
	${OBJECTDIR}/_ext/1228218613/Pwd.o \
	${OBJECTDIR}/_ext/1228218613/PwdLabel.o \
	${OBJECTDIR}/_ext/1228218613/SecureString.o \
	${OBJECTDIR}/_ext/229759634/main.o \
	${OBJECTDIR}/_ext/229759634/Kryptan.o \
	${OBJECTDIR}/_ext/1228218613/ModifiedEncryptor.o \
	${OBJECTDIR}/_ext/1228218613/PwdFile.o


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
LDLIBSOPTIONS=-lcryptopp -lncurses -lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kryptan

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kryptan: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kryptan ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1228218613/Utilites.o: ../src/kryptan_core/Utilites.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/Utilites.o ../src/kryptan_core/Utilites.cpp

${OBJECTDIR}/_ext/229759634/UI.o: ../src/kryptan_program/UI.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/UI.o ../src/kryptan_program/UI.cpp

${OBJECTDIR}/_ext/229759634/UiElement.o: ../src/kryptan_program/UiElement.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/UiElement.o ../src/kryptan_program/UiElement.cpp

${OBJECTDIR}/_ext/1228218613/PwdList.o: ../src/kryptan_core/PwdList.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/PwdList.o ../src/kryptan_core/PwdList.cpp

${OBJECTDIR}/_ext/1228218613/Pwd.o: ../src/kryptan_core/Pwd.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/Pwd.o ../src/kryptan_core/Pwd.cpp

${OBJECTDIR}/_ext/1228218613/PwdLabel.o: ../src/kryptan_core/PwdLabel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/PwdLabel.o ../src/kryptan_core/PwdLabel.cpp

${OBJECTDIR}/_ext/1228218613/SecureString.o: ../src/kryptan_core/SecureString.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/SecureString.o ../src/kryptan_core/SecureString.cpp

${OBJECTDIR}/_ext/229759634/main.o: ../src/kryptan_program/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/main.o ../src/kryptan_program/main.cpp

${OBJECTDIR}/_ext/229759634/Kryptan.o: ../src/kryptan_program/Kryptan.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/229759634
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/229759634/Kryptan.o ../src/kryptan_program/Kryptan.cpp

${OBJECTDIR}/_ext/1228218613/ModifiedEncryptor.o: ../src/kryptan_core/ModifiedEncryptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/ModifiedEncryptor.o ../src/kryptan_core/ModifiedEncryptor.cpp

${OBJECTDIR}/_ext/1228218613/PwdFile.o: ../src/kryptan_core/PwdFile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1228218613
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1228218613/PwdFile.o ../src/kryptan_core/PwdFile.cpp

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
