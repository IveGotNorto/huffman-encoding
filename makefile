
charcount: count_characters.c
	gcc -o charcount count_characters.c

encode: huffman_encode.c huffman_tree_node.o
	gcc -std=gnu99 -o encode huffman_encode.c huffman_tree_node.c 
decode: huffman_decode.c huffman_tree_node.o
	gcc -std=gnu99 -o decode huffman_decode.c huffman_tree_node.c

huffman_tree_node.o: huffman_tree_node.c huffman_tree_node.h
	gcc -c huffman_tree_node.c

clean:
	rm -f *.o *.counts *.huf *.dhuf charcount encode decode
