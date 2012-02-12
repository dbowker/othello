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
CCC=mpic++
CXX=mpic++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/squareScore.o \
	${OBJECTDIR}/returnResult.o \
	${OBJECTDIR}/buildNextDepthRequest.o \
	${OBJECTDIR}/validMoves.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/Communicator.o \
	${OBJECTDIR}/buildWorkRequest.o \
	${OBJECTDIR}/WorkQueue.o \
	${OBJECTDIR}/parseRequest.o \
	${OBJECTDIR}/displayBoard.o \
	${OBJECTDIR}/utilities.o \
	${OBJECTDIR}/setupBoard.o \
	${OBJECTDIR}/findBestMove.o \
	${OBJECTDIR}/sendRequest.o \
	${OBJECTDIR}/parseNextDepthRequest.o


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
LDLIBSOPTIONS=`pkg-config --libs mpich2-ch3`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/othello

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/othello: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/othello ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/squareScore.o: squareScore.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/squareScore.o squareScore.cpp

${OBJECTDIR}/returnResult.o: returnResult.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/returnResult.o returnResult.cpp

${OBJECTDIR}/buildNextDepthRequest.o: buildNextDepthRequest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/buildNextDepthRequest.o buildNextDepthRequest.cpp

${OBJECTDIR}/validMoves.o: validMoves.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/validMoves.o validMoves.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/Communicator.o: Communicator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/Communicator.o Communicator.cpp

${OBJECTDIR}/buildWorkRequest.o: buildWorkRequest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/buildWorkRequest.o buildWorkRequest.cpp

${OBJECTDIR}/WorkQueue.o: WorkQueue.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/WorkQueue.o WorkQueue.cpp

${OBJECTDIR}/parseRequest.o: parseRequest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/parseRequest.o parseRequest.cpp

${OBJECTDIR}/displayBoard.o: displayBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/displayBoard.o displayBoard.cpp

${OBJECTDIR}/utilities.o: utilities.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/utilities.o utilities.cpp

${OBJECTDIR}/setupBoard.o: setupBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/setupBoard.o setupBoard.cpp

${OBJECTDIR}/findBestMove.o: findBestMove.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/findBestMove.o findBestMove.cpp

${OBJECTDIR}/sendRequest.o: sendRequest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/sendRequest.o sendRequest.cpp

${OBJECTDIR}/parseNextDepthRequest.o: parseNextDepthRequest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/mpi -I/usr/include/mpich2 `pkg-config --cflags mpich2-ch3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/parseNextDepthRequest.o parseNextDepthRequest.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/othello

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
