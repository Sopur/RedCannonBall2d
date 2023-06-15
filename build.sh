g++ tree.cpp -g3
g++ tree.cpp -s -Ofast -flto -pthread -march=native -mtune=native -fdiagnostics-color=always
g++ game.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -s -Ofast -flto -pthread -march=native -mtune=native -fdiagnostics-color=always && ./a.out