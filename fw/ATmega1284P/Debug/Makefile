###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = ATmega1284p
COMPILER = "WinAVR"
TARGET = Debug
SHELL = C:\WINDOWS\system32\cmd.exe

## Tools general options
CCFLAGS=-Wall -gdwarf-2 -fsigned-char -MD -MP -DF_CPU=8000000 -O1 -mmcu=atmega1284p
LDFLAGS=-mmcu=atmega1284p

## Processing Tools
CC=avr-gcc.exe
ASM=avr-gcc.exe
LD=avr-gcc.exe
#  C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/Debug.elf: TOOL=LD
#  Post: avr-objcopy -O ihex -R .eeprom "./Debug.elf" "./Debug.hex"
#  Post: avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex "./Debug.elf" "./Debug.eep" || exit 0 
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/main.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATmega1284P/main.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qatcli.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qatcli.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qbsbuffers.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qbsbuffers.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qclock.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qclock.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qcoroutine.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qcoroutine.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qcritical.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qcritical.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qedgecheck.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qedgecheck.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qfsm.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qfsm.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qioutils.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qioutils.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qkernel.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qkernel.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qlists.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qlists.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qmemmang.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qmemmang.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qqueues.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qqueues.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qresponse.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qresponse.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qstimers.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qstimers.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qtasks.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qtasks.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/qtrace.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/qtrace.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/interrupt.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATmega1284P/interrupt.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/task.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATmega1284P/task.c
#     C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATMEGA1284P/Debug/peripheral.o: TOOL=CC
#        C:/Users/Julian/AppData/Local/Temp/VSM Studio/8cc321c3078d459987fdf1c7707766f5/ATmega1284P/peripheral.c

# Build tree:
all:	Debug

Debug:	Debug.elf

Debug.elf:	main.o qatcli.o qbsbuffers.o qclock.o qcoroutine.o qcritical.o qedgecheck.o qfsm.o qioutils.o qkernel.o qlists.o qmemmang.o qqueues.o qresponse.o qstimers.o qtasks.o qtrace.o interrupt.o task.o peripheral.o 
	$(LD) $(LDFLAGS)  -o "./Debug.elf" "main.o" "qatcli.o" "qbsbuffers.o" "qclock.o" "qcoroutine.o" "qcritical.o" "qedgecheck.o" "qfsm.o" "qioutils.o" "qkernel.o" "qlists.o" "qmemmang.o" "qqueues.o" "qresponse.o" "qstimers.o" "qtasks.o" "qtrace.o" "interrupt.o" "task.o" "peripheral.o"
	avr-objcopy -O ihex -R .eeprom "./Debug.elf" "./Debug.hex"
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex "./Debug.elf" "./Debug.eep" || exit 0 

main.o:	../main.c 
	$(CC) $(CCFLAGS)  -o "main.o" -c "../main.c"

qatcli.o:	../qatcli.c 
	$(CC) $(CCFLAGS)  -o "qatcli.o" -c "../qatcli.c"

qbsbuffers.o:	../qbsbuffers.c 
	$(CC) $(CCFLAGS)  -o "qbsbuffers.o" -c "../qbsbuffers.c"

qclock.o:	../qclock.c 
	$(CC) $(CCFLAGS)  -o "qclock.o" -c "../qclock.c"

qcoroutine.o:	../qcoroutine.c 
	$(CC) $(CCFLAGS)  -o "qcoroutine.o" -c "../qcoroutine.c"

qcritical.o:	../qcritical.c 
	$(CC) $(CCFLAGS)  -o "qcritical.o" -c "../qcritical.c"

qedgecheck.o:	../qedgecheck.c 
	$(CC) $(CCFLAGS)  -o "qedgecheck.o" -c "../qedgecheck.c"

qfsm.o:	../qfsm.c 
	$(CC) $(CCFLAGS)  -o "qfsm.o" -c "../qfsm.c"

qioutils.o:	../qioutils.c 
	$(CC) $(CCFLAGS)  -o "qioutils.o" -c "../qioutils.c"

qkernel.o:	../qkernel.c 
	$(CC) $(CCFLAGS)  -o "qkernel.o" -c "../qkernel.c"

qlists.o:	../qlists.c 
	$(CC) $(CCFLAGS)  -o "qlists.o" -c "../qlists.c"

qmemmang.o:	../qmemmang.c 
	$(CC) $(CCFLAGS)  -o "qmemmang.o" -c "../qmemmang.c"

qqueues.o:	../qqueues.c 
	$(CC) $(CCFLAGS)  -o "qqueues.o" -c "../qqueues.c"

qresponse.o:	../qresponse.c 
	$(CC) $(CCFLAGS)  -o "qresponse.o" -c "../qresponse.c"

qstimers.o:	../qstimers.c 
	$(CC) $(CCFLAGS)  -o "qstimers.o" -c "../qstimers.c"

qtasks.o:	../qtasks.c 
	$(CC) $(CCFLAGS)  -o "qtasks.o" -c "../qtasks.c"

qtrace.o:	../qtrace.c 
	$(CC) $(CCFLAGS)  -o "qtrace.o" -c "../qtrace.c"

interrupt.o:	../interrupt.c 
	$(CC) $(CCFLAGS)  -o "interrupt.o" -c "../interrupt.c"

task.o:	../task.c 
	$(CC) $(CCFLAGS)  -o "task.o" -c "../task.c"

peripheral.o:	../peripheral.c 
	$(CC) $(CCFLAGS)  -o "peripheral.o" -c "../peripheral.c"

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf ./main.d
	rm -rf ./qatcli.d
	rm -rf ./qbsbuffers.d
	rm -rf ./qclock.d
	rm -rf ./qcoroutine.d
	rm -rf ./qcritical.d
	rm -rf ./qedgecheck.d
	rm -rf ./qfsm.d
	rm -rf ./qioutils.d
	rm -rf ./qkernel.d
	rm -rf ./qlists.d
	rm -rf ./qmemmang.d
	rm -rf ./qqueues.d
	rm -rf ./qresponse.d
	rm -rf ./qstimers.d
	rm -rf ./qtasks.d
	rm -rf ./qtrace.d
	rm -rf ./interrupt.d
	rm -rf ./task.d
	rm -rf ./peripheral.d

# cleanup - delete all generated files
clean:	tidy
	rm -rf Debug.elf
	rm -rf main.o
	rm -rf qatcli.o
	rm -rf qbsbuffers.o
	rm -rf qclock.o
	rm -rf qcoroutine.o
	rm -rf qcritical.o
	rm -rf qedgecheck.o
	rm -rf qfsm.o
	rm -rf qioutils.o
	rm -rf qkernel.o
	rm -rf qlists.o
	rm -rf qmemmang.o
	rm -rf qqueues.o
	rm -rf qresponse.o
	rm -rf qstimers.o
	rm -rf qtasks.o
	rm -rf qtrace.o
	rm -rf interrupt.o
	rm -rf task.o
	rm -rf peripheral.o
