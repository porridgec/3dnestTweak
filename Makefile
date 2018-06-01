THEOS_DEVICE_IP = 192.168.31.129
THEOS_DEVICE_PORT = 22
ARCHS = armv7 arm64
TARGET = iphone:latest:8.0

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = 3DNestTweak
3DNestTweak_FILES = $(wildcard src/*.m) Tweak.xm

include $(THEOS_MAKE_PATH)/tweak.mk

after-install::
	install.exec "killall -9 NestHunterProgram"
