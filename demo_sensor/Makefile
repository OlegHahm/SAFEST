####
#### Sample Makefile for building apps with the RIOT OS
####
#### The Sample Filesystem Layout is:
#### /this makefile
#### ../../RIOT 
#### 

# name of your project
export PROJECT =safest_demo_sensor

export PORT =/dev/ttyUSB0

# for easy switching of boards
ifeq ($(strip $(BOARD)),)
	export BOARD =avsextrem
endif

# this has to be the absolute path of the RIOT-base dir
export RIOTBASE =$(CURDIR)/../../RIOT

## Modules to include. 

USEMODULE += auto_init
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += uart0
USEMODULE += posix
USEMODULE += ps 
USEMODULE += vtimer 
USEMODULE += cc110x
USEMODULE += transceiver
USEMODULE += sixlowpan
USEMODULE += rpl
USEMODULE += destiny
USEMODULE += gpioint

export CFLAGAS = -DENABLE_DEBUG=1

export INCLUDES = -I$(RIOTBOARD)/$(BOARD)/include -I$(RIOTBASE)/core/include -I$(RIOTCPU)/$(CPU)/include -I$(RIOTBASE)/sys/lib -I$(RIOTBASE)/sys/include/ -I$(RIOTBASE)/drivers/include/ -I$(RIOTBASE)/sys/net/include -I$(RIOTBASE)/sys/net/rpl -I$(RIOTBASE)/drivers/cc110x/

include $(RIOTBASE)/Makefile.include

$(info *******************************************)
$(info **** Building Project $(PROJECT) ****)
$(info *******************************************)
$(info **** BOARD $(BOARD))
$(info **** BASE:  $(RIOTBASE))
$(info **** CPU:   $(RIOTCPU))
$(info **** BOARD: $(RIOTBOARD))
$(info *************************************)
$(info **** MODULES:)
$(foreach module,$(USEMODULE),$(info ++++           $(module)))
$(info **** INCLUDES:)
$(foreach include,$(INCLUDES),$(info ++++           $(include)))
$(info **** BASELIBS:)
$(foreach lib,$(BASELIBS),$(info ++++           $(lib)))
$(info **** SRC:)
$(foreach srcfile,$(SRC),$(info ++++           $(srcfile)))
$(info **** OBJ:)
$(foreach objfile,$(OBJ),$(info ++++           $(objfile)))
$(info **** CC: $(CC))
$(foreach flag,$(CFLAGS),$(info ++++            $(flag)))
