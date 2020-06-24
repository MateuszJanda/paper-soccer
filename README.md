## Building googletest instruction
'''
cd lib/
git clone https://github.com/google/googletest.git
cd googletest/
cmake CMakeLists.txt
make

cd ..
ln -s googletest/lib/\*.a .
'''

## Building
'''
cmake CMakeLists.txt
make
'''

## Code style
'''
clang-format -style='{BasedOnStyle: WebKit, Cpp11BracedListStyle: true, SpaceBeforeCpp11BracedList: false}' -i *.cpp
clang-format -style='{BasedOnStyle: WebKit, Cpp11BracedListStyle: true, SpaceBeforeCpp11BracedList: false}' -i *.hpp
'''