#!/bin/bash

	echo "Start Generate Flash Code"
	rm -f ${FLASHNAME}
	$(pwd)/imagealign ${BIN_DIR}/${UBOOT_NAME}-uboot.bin ${BIN_DIR}/uboot_file.tmp 0x40000
	chmod 755 ${BIN_DIR}/uboot_file.tmp
	cat ${BIN_DIR}/uboot_file.tmp > ${FLASHNAME}
	rm -f ${BIN_DIR}/uboot_file.tmp
	cat $(pwd)/${PRODUCT_NAME}_uboot_config.bin >> ${FLASHNAME}
	$(pwd)/imagealign ${IMGNAME}-uImage-squashfs-lzma.bix ${BIN_DIR}/img.tmp 0xf80000
	chmod 755 ${BIN_DIR}/img.tmp
	cat ${BIN_DIR}/img.tmp >> ${FLASHNAME}
	rm -f ${BIN_DIR}/img.tmp
	cat $(pwd)/${PRODUCT_NAME}_nvram.bin >> ${FLASHNAME}
	cat $(pwd)/${PRODUCT_NAME}_board.bin >> ${FLASHNAME}
	cat $(pwd)/${PRODUCT_NAME}_factory.bin >> ${FLASHNAME}
