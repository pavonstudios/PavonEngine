
.PHONY: all
all:
	$(MAKE) -C ./src es2

.PHONY: clean
clean:
	rm -f ./src/*.o
