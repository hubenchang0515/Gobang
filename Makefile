.PHONY : all mingw clean

all : 
	cd src && $(MAKE)

mingw : 
	cd src && $(MAKE) opt=mingw

clean :
	cd src && $(MAKE) clean