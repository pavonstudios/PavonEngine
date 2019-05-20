
.PHONY: all
es2:
	$(MAKE) -C ./src es2

.PHONY: clean
clean:
	rm -f ./src/*.o
