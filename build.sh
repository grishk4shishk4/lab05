if [ ${B_dir}='' ]
then
export B_dir=_build
fi
echo ${B_dir} 
cmake -H. -B${B_dir} -DBUILD_TESTS=ON -DTESTS_NAME=tests -DENABLE_COVERAGE=OFF\
&&\
cmake --build ${B_dir} \
&& \
cmake --build ${B_dir} --target test -- ARGS=--verbose
