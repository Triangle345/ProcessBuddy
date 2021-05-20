#./configure --without-database --with-fallbacks=xterm-256color,vt100 --without-xterm-new --disable-home-terminfo --with-termlib --enable-termcap --disable-database --host=arm-linux-gnue
TARGET ?= ProcessBuddy
SRC_DIRS ?= ./
#CXX=/home/peterpan/android-ndk-r21e/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android30-clang++
# CC = /home/peterpan/android-ndk-r21e/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android30-clang++
CC = g++
SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -static 

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS) -std=c++17 -Lncurses/lib -lncurses  -ltinfo -lpanel -lmenu -lform

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
