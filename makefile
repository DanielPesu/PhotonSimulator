# TODO: Experiment with commands to make the targets/builds more elegant.
BUILDDIR = build
HEADDIR  = Includes
SRCDIR   = src
TESTDIR  = test

FLAGS = -O3 -I./$(HEADDIR)

Photon_Deposition: $(BUILDDIR) $(BUILDDIR)/main.o $(BUILDDIR)/configure.o $(BUILDDIR)/positioning.o $(BUILDDIR)/mathoperations.o
	g++ -o Photon_Deposition $(BUILDDIR)/main.o $(BUILDDIR)/configure.o $(BUILDDIR)/positioning.o $(BUILDDIR)/mathoperations.o $(FLAGS)

Test_Detector:	$(BUILDDIR) $(BUILDDIR)/positioning_test.o $(BUILDDIR)/positioning.o $(BUILDDIR)/mathoperations.o $(BUILDDIR)/configure.o
	g++ -o Test_Detector $(BUILDDIR)/positioning_test.o $(BUILDDIR)/positioning.o $(BUILDDIR)/mathoperations.o $(BUILDDIR)/configure.o $(FLAGS)

$(BUILDDIR)/main.o: $(SRCDIR)/main.cpp $(HEADDIR)/parameters.h
	g++ -c $(SRCDIR)/main.cpp -o $(BUILDDIR)/main.o $(FLAGS)

$(BUILDDIR)/configure.o: $(SRCDIR)/configure.cpp $(HEADDIR)/parameters.h
	g++ -c $(SRCDIR)/configure.cpp -o $(BUILDDIR)/configure.o $(FLAGS)

$(BUILDDIR)/positioning.o: $(SRCDIR)/positioning.cpp $(HEADDIR)/positioning.h $(HEADDIR)/parameters.h $(HEADDIR)/constants.h
	g++ -c $(SRCDIR)/positioning.cpp -o $(BUILDDIR)/positioning.o $(FLAGS)

$(BUILDDIR)/mathoperations.o: $(SRCDIR)/mathoperations.cpp $(HEADDIR)/mathoperations.h $(HEADDIR)/constants.h
	g++ -c $(SRCDIR)/mathoperations.cpp -o $(BUILDDIR)/mathoperations.o $(FLAGS)

$(BUILDDIR)/positioning_test.o: $(TESTDIR)/positioning_test.cpp $(HEADDIR)/positioning.h $(HEADDIR)/constants.h
	g++ -c $(TESTDIR)/positioning_test.cpp -o $(BUILDDIR)/positioning_test.o $(FLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)


clean:
	rm -rf Photon_Deposition
	rm -rf Test_Detector
	rm -rf $(BUILDDIR)
