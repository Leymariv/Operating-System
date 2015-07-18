.PHONY: clean all

all:
	$(MAKE) -C user/ all VERBOSE=$(VERBOSE)
	$(MAKE) -C kernel/ kernel.bin VERBOSE=$(VERBOSE)

clean:
	$(MAKE) clean -C kernel/
	$(MAKE) clean -C user/

disk:
	mkdir -p $@

.PHONY: bochs
bochs: all disk
	@echo "### This target will require root access to mont disk image ! ###"
	sudo mount -t ext2 -o loop,offset=1048576 disk.img disk/
	sudo cp kernel/kernel.bin disk/kernel.bin
	sync
	sudo umount disk/
	bochs