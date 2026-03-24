CC ?= gcc
CFLAGS ?= -O2
CFLAGS += -Wall -Wextra -std=c99 -DWIN32_LEAN_AND_MEAN -DDIS_R3000_DLL_BUILD
INCLUDES := -I./src
RM ?= rm -f
LDFLAGS ?=
LDFLAGS += -shared -Wl,--out-implib,libDisR3000.a
LIBS ?=
LIBS += -lkernel32

TARGET := DisR3000.dll
SRCS := src/DisR3000.c src/DisR3000Main.c
OBJS := $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET) libDisR3000.a

