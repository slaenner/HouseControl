CC = gcc
CFLAGSCOM =-Ih -Idht11/h -Ilogger/h -Ibmc/h -Ilcd/h -Wall
DEPS =
OBJ = logger/src/logger.o dht11/src/dht11.o bmc/src/bmc.o lcd/src/lcd.o housecontrol.o

OBJSIM = sim_stubs/src/stubs.o
CFLAGSSIM = -Isim_stubs/h -DSIMULATION

CFLAGSTGT = -lbcm2835 -lm

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGSCOM) $(CFLAGSTGT)

raspberry: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGSCOM) $(CFLAGSTGT)

host: $(OBJ) $(OBJSIM)
	$(CC) -o $@ $^ $(CFLAGSCOM) $(CFLAGSSIM)

clean:
	rm $(OBJ)
	rm host
	rm raspberry
