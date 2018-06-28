# Project Setup
**Languages**: [Korean](ko_kr/setup_projects.md)

CRSF uses CMake to configure projects.

An empty project is at [crsf-empty github](https://github.com/chic-yukim/crsf-empty).
You can download and use it, or make same file hierarchy like the empty project.



## Getting Started with Empty Project
1. Download CRSF from [http://crseed.org](http://crseed.org).
2. Extract the archive to "crsf" directory.

### Directory Hierarchy
```
─ PROJECT_DIR
  ├ build                   # CMake build directory
  ├ crsf
  │ ├ ...
  │ └ .crsf-root
  ├ empty-module
  ├ empty-project
  ├ vcpkg                   # Only Visual Studio 2017
  └ CMakeLists.txt
```

### Visual Studio 2013
3. Setup project using CMake
   - Where is the source code: `PROJECT_DIR` path
   - Where to build the binaries: `PROJECT_DIR/build` path
   - Select *Configure* button to configure cmake project.
     - Select "Use default native compilers" and finish it.
   - Select *Generate* button to generate Visual Studio solution.

4. Build and run the project
   - You may choose Project configuration as `RelWithDebInfo` or `Release`.
   - You should set the project using `Set as StartUp Project` to run.

### Visual Studio 2017
3. Run `bootstrap.bat` file (**This takes over 20 minutes depending on your system**)
   - This batch file will clone [vcpkg](https://github.com/Microsoft/vcpkg) project.
   - And then, vcpkg will build and install third party libraries.

4. Setup project using CMake **with vcpkg toolchain file**
   1. Where is the source code: `PROJECT_DIR` path
   2. Where to build the binaries: `PROJECT_DIR/build` path
   3. select **Specify toolchain file for cross-compiling**
   4. Specify vcpkg toolchain file as `vcpkg/scripts/buildsystems/vcpkg.cmake`
   5. Finish

5. Build and run the project
   - You should set the project using `Set as StartUp Project`.
   - You may choose Project configuration as `Debug`, `RelWithDebInfo` or `Release`.



## Using CRSF examples
In CMakeLists.txt, uncomment `add_subdirectory("crsf/examples")` line.



## Creating New Project
You can create a new project by copying from an empty project or examples.
And then, you should edit some files according to the following.

### Directory
You should add `add_subdirectory` command in `CMakeLists.txt` if you create a new project.

### Project Name (Module ID)
1. Change a value of `CRMODULE_ID` in `your-project/CMakeLists.txt` file.
2. Change contents in `your-project/config.json`.
3. Change `<id>` tag value in `your-project/config/SystemConfiguration.xml` file.
   - Without this, the application will print error message.
   - Example:
   ```xml
   <dynamic_modules>
     <module>
         ...
     </module>
     <application>
         <id>your-project</id>
     </application>
   </dynamic_modules>
   ```

4. Change tag name in `your-project/config/DynamicModuleConfiguration.xml` file if the tag exists.
