MKDIR_P := mkdir -p
OUT_DIR := build

PICO_SDK := pico-sdk
PICO_EXTRAS := pico-extras

.PHONY: all clean

all:
	@ make $(OUT_DIR)
	@ make uf2

uf2:
	@ make -C $(OUT_DIR) -j8

$(OUT_DIR): export PICO_SDK_PATH := $(CURDIR)/${PICO_SDK}
$(OUT_DIR): export PICO_EXTRAS_PATH := $(CURDIR)/${PICO_EXTRAS}
$(OUT_DIR):
	cp ${PICO_SDK_PATH}/external/pico_sdk_import.cmake ./
	cp ${PICO_EXTRAS_PATH}/external/pico_extras_import.cmake ./
	${MKDIR_P} $(OUT_DIR)
	@ echo ${PICO_SDK_PATH}
	cd $(OUT_DIR) && PICO_SDK_PATH=${PICO_SDK_PATH} cmake ..

clean:
	rm -rf $(OUT_DIR)
