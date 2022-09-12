

minimax:
	clear
	g++ minimax_driver.cpp -D ALPHA_BETA_PRUNING=0
	./a.out

alpha_beta:
	clear
	g++ minimax_driver.cpp -D ALPHA_BETA_PRUNING=1
	./a.out

mcts:
	clear
	g++ mcts_driver.cpp #-fsanitize=address -g
	./a.out

fclean:
	rm -rf a.out
