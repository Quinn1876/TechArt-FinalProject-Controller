BUILD_PATH := ./build
CACHE_PATH := ${BUILD_PATH}/cache
BOARD_PLATFORM := arduino:avr
BOARD_NAME := ${BOARD_PLATFORM}:uno
DEBUG_PORT := /dev/ttyACM0
CONTROLLER_SKETCH := Controller.ino
ARD := /home/quinn/arduino-cli

LIBS := /home/quinn/.arduino15/libraries/Servo
COMMON_LIBS := ../Common

controller-compile:
	${ARD} compile -b ${BOARD_NAME} --libraries ${LIBS} ${CONTROLLER_SKETCH}

upload:
	${ARD} upload -b ${BOARD_NAME} -p ${DEBUG_PORT} ${SKETCH}

install-board:
	${ARD} core install ${BOARD_PLATFORM}

help:
	${ARD} --help
