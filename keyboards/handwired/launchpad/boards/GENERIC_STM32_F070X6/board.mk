# List of all the board related files.
BOARDSRC = $(BOARD_PATH)/boards/GENERIC_STM32_F070X6/board.c

# Required include directories
BOARDINC = $(BOARD_PATH)/boards/GENERIC_STM32_F070X6

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
