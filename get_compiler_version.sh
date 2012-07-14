#!/bin/bash
# Only supports detecting g++ and clang++ versions at the moment.

function help() {
	echo "Usage: $(basename "$0") {-h | -c | -v } [-s path]"
	echo
	echo " -h	- Display this help."
	echo " -c	- Return the detected compiler name."
	echo " -v	- Return the detected compiler version string. Format: #[.#[.#]]"
	echo " -s	- Set the compiler path."
}

if [ "$1" = "-h" -o "$1" = "--help" ]; then
	help
	exit
fi

COMPILER="g++"
COMPILER_PATH="g++"

if [ "$CXX" ]; then
	COMPILER_PATH="$CXX"
	COMPILER="$(basename $CXX)"
fi

DISPLAY_COMPILER=0
DISPLAY_VERSION=0

OPTIND=1
while getopts ":hcvs:" optname; do
	case "$optname" in
		"h")
			help
			exit
		;;
		"c")
			DISPLAY_COMPILER=1
		;;
		"v")
			DISPLAY_VERSION=1
		;;
		"s")
			COMPILER_PATH="$OPTARG"
			COMPILER="$(basename $OPTARG)"
		;;
		":")
			echo "No argument value for option $OPTARG"
			exit
		;;
		"?")
			echo "Unknown option $OPTARG"
			exit
		;;
	esac
done

if [ $DISPLAY_COMPILER -ne 1 -a $DISPLAY_VERSION -ne 1 ]; then
	help
	exit
fi



if [ "$COMPILER" = "g++" -o "$COMPILER" = "clang++" ]; then
	VERSION="$($COMPILER_PATH -v 2>&1 | grep version | sed -e 's/^.*version //' | cut -d' ' -f1)"
fi



if [ $DISPLAY_COMPILER -eq 1 -a $DISPLAY_VERSION -eq 1 ]; then
	echo "$COMPILER $VERSION"
elif [ $DISPLAY_COMPILER -eq 1 ]; then
	echo "$COMPILER"
else
	echo "$VERSION"
fi
