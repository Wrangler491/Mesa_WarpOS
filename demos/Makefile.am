## Process this file with automake to produce Makefile.in

#AUTOMAKE_OPTIONS = no-dependencies

INCLUDES = -I$(top_srcdir)/include $(GLUT_CFLAGS)

check_PROGRAMS = bounce clearspd drawpix gamma gears glinfo gloss \
	glutfx isosurf morph3d multiarb osdemo paltex pointblast reflect \
	renormal spectex stex3d tessdemo texcyl texobj trispd winpos

CLEANFILES = test.ppm

LDADD = $(GLUT_LIBS) $(top_builddir)/src-glu/libGLU.la \
	$(top_builddir)/src/libGL.la -lm

EXTRA_DIST = isosurf.dat

# use double buffering if possible
BUFFER = -db

# execute all programs
exec: $(check_PROGRAMS)
	@for prog in $(check_PROGRAMS);	\
	do				\
	 echo -n "Running $$prog ..." ;	\
	 case "$$prog" in		\
	 osdemo) ./$$prog test.ppm ;; 	\
	 stex3d) ./$$prog $(BUFFER) ;; 	\
	 *) ./$$prog ;;			\
	 esac ;				\
	 echo ;				\
	done
