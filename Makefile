CC = gcc
CFLAGS = -Wall -Wextra -MMD -MP
LDFLAGS = -lcrypto
SRC = $(wildcard src/*.c)
OBJDIR = obj
OBJ = $(SRC:src/%.c=$(OBJDIR)/%.o)
DEP = $(OBJ:.o=.d)

changed := $(wildcard include/*.h)

app: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	rm -fr $(OBJDIR)

$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(OBJDIR)   # Ensure the obj directory exists
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
	rm -rf $(OBJDIR) app $(DEP)


-include $(DEP)
