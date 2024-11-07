# Project.sh
# Brief: Contains clangd config and project build commands

cmake --fresh -S . -B Build && cmake --build Build;
cp ./Build/compile_commands.json .
