# name of your project
export PROJECT = cebit_sensor

# If no BOARD is found in the environment, use this default:
export BOARD ?= avsextrem

# This has to be the absolute path to the RIOT base directory:
export RIOTBASE ?= $(CURDIR)/../../../riot/RIOT

# Uncomment these lines if you want to use platform support from external
# repositories:
#export RIOTCPU ?= $(CURDIR)/../../RIOT/thirdparty_cpu
#export RIOTBOARD ?= $(CURDIR)/../../RIOT/thirdparty_boards

# Uncomment this to enable scheduler statistics for ps:
#CFLAGS += -DSCHEDSTATISTICS

# If you want to use native with valgrind, you should recompile native
# with the target all-valgrind instead of all:
# make -B clean all-valgrind

# Uncomment this to enable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
#CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
export QUIET ?= 1

# Modules to include:

USEMODULE += auto_init
USEMODULE += posix
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += vtimer
ifneq (,$(filter msba2,$(BOARD)))
	USEMODULE += sht11
	USEMODULE += ltc4150
	USEMODULE += rtc
	USEMODULE += mci
	USEMODULE += cc110x_ng
	USEMODULE += config
	USEMODULE += random
endif
USEMODULE += cc110x_ng


include $(RIOTBASE)/Makefile.include
