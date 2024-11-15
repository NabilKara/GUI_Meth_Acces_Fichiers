#makesure to chmod a+x build.sh
#global compiling
# Assume by default this is for linux
COMPILER="gcc"
SRC="./src/*.c"
INCLUDE="./include"
BINARY="./bin/prog"
LIBS="-lSDL2 -lSDL2_ttf -lSDL2_mixer -ldl"

# Echo out the platform we are compilng on.
echo "Compiling on ${OSTYPE}"

# Build the compile string
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo ""
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Compilng on Mac"
    INCLUDE="./include `sdl2-config --clfags`"
    LIBS="`sdl2-config --libs` -lSDL2_ttf -lSDL2_mixer"
    echo ""
elif [[ "$OSTYPE" == "mysys" ]] ; then
    echo "Compiling on Windows(mysys)"
    LIBS="-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer"
    echo ""
else
    echo "Not sure what operating system to build for?? Trying defaults"
    echo ""
fi

COMPILE="${COMPILER} ${SRC} -I ${INCLUDE} -o ${BINARY} ${LIBS}"

# Write out the compile string for the user to see
echo ${COMPILE}

# Evaluate the compile string
eval $COMPILE

