SRC_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
OUT_DIR ?= $(SRC_DIR)/output
ASM_DIR := $(SRC_DIR)/tools/acme091
ASM ?= acme
CC_DB ?= gdb
CXX = /usr/local/Cellar/gcc/9.2.0/bin/c++-9

.PHONY: build_asm binary run

$(OUT_DIR):
	mkdir -p $@

build_asm:
	cd $(ASM_DIR)/src && $(MAKE)
	cd $(ASM_DIR)/src && $(MAKE) install

binary: $(OUT_DIR)
	cd $(OUT_DIR) && cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-D2A03_INCLUDE_TESTS=TRUE \
		$(SRC_DIR)
	$(MAKE) 2a03 -C $(OUT_DIR)

run: binary
	$(OUT_DIR)/2a03

debug: binary
	$(CC_DB) $(OUT_DIR)/2a03

clean:
	rm -rf $(OUT_DIR)