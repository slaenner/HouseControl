CC = g++
CFLAGSCOM =-Ih -Idht11/h -Ilogger/h -Ibcm/h -Ilcd/h -Isocketserver/h -Wall
DEPS =
OBJ = logger/src/logger.o dht11/src/dht11.o bcm/src/bcm.o lcd/src/lcd.o socketserver/src/socketserver.o housecontrol.o

OBJSIM = sim_stubs/src/stubs.o
CFLAGSSIM = -Isim_stubs/h -DSIMULATION

CFLAGSTGT = -lbcm2835 -lm

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGSCOM) $(CFLAGSSIM)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGSCOM) $(CFLAGSSIM)

raspberry: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGSCOM) $(CFLAGSTGT)

host: $(OBJ) $(OBJSIM)
	$(CC) -o $@ $^ $(CFLAGSCOM) $(CFLAGSSIM)

clean:
	rm $(OBJ)
	rm $(OBJSIM)
	rm host
	rm raspberry
