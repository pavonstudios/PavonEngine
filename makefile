
.PHONY: es2
es2:
	$(MAKE) -C ./src es2

.PHONY: win
win:
	$(MAKE) -C ./src win

.PHONY: clean
clean:
	rm -f ./src/*.o
