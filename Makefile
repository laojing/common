ifeq ($(shell uname),Linux)
OS                     = linux
else
OS                     = win64
endif

Source                 = String.c \
						 GtkCommon.c \
						 XmlRead.c \
						 RecentFile.c \
						 Common.c

Empty                  =
Space                  = $(Empty) $(Empty)
temproot               = $(subst /Source,$(Space),$(CURDIR))
DEV_ROOT               = $(word 1,$(temproot))

SourceDir              = $(DEV_ROOT)/Source
LibraryDir             = $(DEV_ROOT)/../../Librarys

ifeq ($(OS),win32)
CCompiler              = gcc -std=gnu99 -g
CStaticCompiler        = ar rc
OBJMID                 = win32
LinkerOption           = $(CLIBS) -lgsl -lgslcblas
makelinux:
	@echo "Start Make Window32"
endif

ifeq ($(OS),win64)
CCompiler              = gcc -std=gnu99 -g
CStaticCompiler        = ar rc
OBJMID                 = win64
LinkerOption           = $(CLIBS) -lgsl -lgslcblas
makelinux:
	@echo "Start Make Window64"
endif
ifeq ($(OS),linux)
CCompiler              = gcc -std=gnu99 -g
CStaticCompiler        = ar rc
OBJMID                 = linux
LinkerOption           = `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0` -lm -lgsl -lgslcblas
COPTION                = -rdynamic `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0`
makelinux:
	@cp $(SourceDir)/common.h $(LibraryDir)/common/linux/include/common.h
	@cp $(SourceDir)/common.h $(LibraryDir)/common/win64/include/common.h
	@cp $(SourceDir)/common.h $(LibraryDir)/common/win32/include/common.h
	@echo "Start Make Linux"

endif

TargetDir              = $(DEV_ROOT)/Object/$(OBJMID)
CSourceFiles    	   = $(Source:%.c= $(SourceDir)/%.c)
CObjectFiles           = $(Source:%.c= $(TargetDir)/%.o)

build: makelinux $(CObjectFiles)
	$(CStaticCompiler) $(LibraryDir)/common/$(OBJMID)/lib/libcommon.a $(CObjectFiles)
	@echo "Build Down"

$(TargetDir)/%.o : $(SourceDir)/%.c
	@echo $@
	@$(CCompiler) -c $< -o $@ $(COPTION)

clean :
	@rm $(TargetDir)/* 

