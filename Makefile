.PHONY: all, install, clean

TARGET = led_app

CXX=C:\msys64\mingw64\bin\gcc
INCLUDE= -IApplication/include

led_app:
	$(CXX) \
	$(INCLUDE) \
	Application/src/mjson.c \
	Application/src/led_app.c \
	-o led_app