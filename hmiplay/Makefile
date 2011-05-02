CC =		gcc
LD =		gcc
CFLAGS +=	-Wall `pkg-config --cflags SDL_mixer sdl` -I/usr/local/include
LDFLAGS +=	`pkg-config --libs SDL_mixer sdl`
SRCS =		hmi.cpp hmiplay.cpp
OBJS =		${SRCS:.cpp=.o}
TARGET = 	hmiplay

.SUFFIXES:	.cpp .o

all: ${TARGET}

${TARGET}: ${OBJS}
	${LD} ${LDFLAGS} ${OBJS} -o ${TARGET}

.cpp.o:
	${CC} ${CFLAGS} -c -o $@ ${@:.o=.cpp}

clean:
	rm -f *.o ${TARGET}
