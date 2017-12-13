# Anrew Candelaresi final project
EXE=FalconSim

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
OGLFalconSim.o: OGLFalconSim.c OGLFalconSim.h Common.h
fatal.o: fatal.c OGLFalconSim.h
loadtexbmp.o: loadtexbmp.c OGLFalconSim.h
print.o: print.c OGLFalconSim.h
project.o: project.c OGLFalconSim.h
errcheck.o: errcheck.c OGLFalconSim.h
falcon.o: falcon.c OGLFalconSim.h Common.h




#  Create archive
OGLFalconSim.a:fatal.o loadtexbmp.o print.o project.o errcheck.o falcon.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) $<

#  Link
FalconSim:OGLFalconSim.o OGLFalconSim.a
	gcc -O3 -o $@ $^   $(LIBS)

#  Clean
clean:
	$(CLEAN)
