

run:
	clear
	gcc simplex1.cpp -o smd -lstdc++
	./smd
	@echo " "
	ctags -R .

clean:
	rm -rf smd
