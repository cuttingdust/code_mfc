@else off
chcp 65001 > nul
::conan search opencv --remote=conancenter

conan install . -s build_type=Debug --build=missing
::conan install . -s build_type=Release --build=missing
::conan install . -s build_type=RelWithDebInfo --build=missing
::conan install . -s build_type=MinSizeRel --build=missing

timeout 5