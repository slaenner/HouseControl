CC = g++
CFLAGSCOM =-Ih -Idht11/h -Ilogger/h -Ibcm/h -Ilcd/h -Isocketserver/h -Wall
DEPS =
OBJ = logger/src/logger.o dht11/src/dht11.o bcm/src/bcm.o lcd/src/lcd.o socketserver/src/socketserver.o housecontrol.o

OBJSIM = sim_stubs/src/stubs.o
CFLAGS = -Isim_stubs/h -DSIMULATION
#CFLAGS = -lbcm2835 -lm

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGSCOM) $(CFLAGS)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGSCOM) $(CFLAGS)

raspberry: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGSCOM) $(CFLAGS)

host: $(OBJ) $(OBJSIM)
	$(CC) -o $@ $^ $(CFLAGSCOM) $(CFLAGS)

clean:
	rm $(OBJ)
	rm $(OBJSIM)
	rm host
	rm raspberry
