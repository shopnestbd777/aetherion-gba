#---------------------------------------------------------------------------------------------------------------------
# Aetherion: Shards of the Jade Sky - GBA Build Configuration
#---------------------------------------------------------------------------------------------------------------------
TARGET      := aetherion
BUILD       := build
LIBBUTANO   := /opt/butano/butano
PYTHON      := python3
SOURCES     := src
INCLUDES    := include
DATA        :=
GRAPHICS    := graphics
AUDIO       := audio
DMG_AUDIO   := dmg_audio

ROMTITLE    := AETHERION
ROMCODE     := AETJ
MAKERCODE   := 01
VERSION     := 0

# 32KB SRAM Save Support for Open-World Progress
USERFLAGS   := -DBN_CFG_SRAM_WAIT_STATE=BN_SRAM_WAIT_STATE_8
USERCXXFLAGS:= -std=c++20 -O2 -Wall -Wextra
USERASFLAGS :=
USERLDFLAGS :=

export DEPSDIR := $(CURDIR)/$(BUILD)
include $(LIBBUTANO)/butano.mak
