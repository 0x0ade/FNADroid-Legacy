#mono gives us the boehm GC by default; we want sgen... but mono hates us.
#MONO-2-LIBS=$(shell pkg-config --libs mono-2 | sed -e 's/-lmono-2.0/'$$(ldconfig -p | grep -i monosgen | sed 's/.*=>\ //' | sed -e 's/[\/&]/\\&/g')'/')
#... or just use boehm, as sgen hates us even more. sgen crases on jit_init.
MONO-2-LIBS=$(shell pkg-config --libs mono-2)

#fnadroid-glue-java is currently not required - a fake Java side is a possibility for advanced tests, though.

fnadroid-runner: fnadroid-wrapper fnadroid-runner.o
	gcc -Wall -Wl,-rpath,. -o fnadroid-runner fnadroid-runner.o $(MONO-2-LIBS) -L. -lfnadroid-wrapper

fnadroid-runner.o: fnadroid-wrapper fnadroid-runner.c
	gcc -c -fpic -Wall \
	-DFNADROID_DESKTOP \
	fnadroid-runner.c \
	-I../jni $(shell pkg-config --cflags mono-2)

fnadroid-wrapper: fnadroid-wrapper.o fnadroid-glue-mono.o
	gcc -shared -Wall -Wl,-rpath,. -o libfnadroid-wrapper.so fnadroid-wrapper.o fnadroid-glue-mono.o $(MONO-2-LIBS)

fnadroid-wrapper.o: ../jni/fnadroid-wrapper.c
	gcc -c -fpic -Wall \
	-DFNADROID_DESKTOP \
	../jni/fnadroid-wrapper.c \
	$(shell pkg-config --cflags mono-2)

fnadroid-glue-mono.o: ../jni/fnadroid-glue-mono.c
	gcc -c -fpic -Wall \
	-DFNADROID_DESKTOP \
	../jni/fnadroid-glue-mono.c \
	$(shell pkg-config --cflags mono-2)

#fnadroid-glue-java.o: ../jni/fnadroid-glue-java.c
#	gcc -c -fpic -Wall \
#	-DFNADROID_DESKTOP \
#	../jni/fnadroid-glue-java.c \
#	$(shell pkg-config --cflags mono-2)

clean:
	rm fnadroid-runner fnadroid-runner.o libfnadroid-wrapper.so fnadroid-wrapper.o fnadroid-glue-mono.o
