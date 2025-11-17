# Project.sh
# Brief: Contains clangd config and project build commands

rm -f ./compile_commands.json
cmake -S . -B Build && cmake --build Build;
cp ./Build/compile_commands.json .
