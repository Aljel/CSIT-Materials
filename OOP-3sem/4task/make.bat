cd build
rmdir /s /q .
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release --parallel
Release\RailwaySimulation.exe
cd ..
