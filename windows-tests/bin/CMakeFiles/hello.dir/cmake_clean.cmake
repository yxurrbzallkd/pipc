file(REMOVE_RECURSE
  "../hello.exe"
  "../hello.exe.manifest"
  "../hello.pdb"
  "CMakeFiles/hello.dir/process/hello.cpp.obj"
  "libhello.dll.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/hello.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
