# Building T1, version 0.1
OBJS = boardwidget.o\
t1.o\
mainwindow.o\
newwindow.o\
optionwindow.o\
showmovewidget.o\
situation.o\
computermove.o\
evaluate2.o\
file.o\
levelwindow.o\
pattern.o

CC = g++
CFLAGS = `fltk-config --cxxflags`
LDFLAGS = `fltk-config --ldflags`
TARGET = T1
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -s -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
