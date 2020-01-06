# HOPES(Hope Of Parallel Embedded Software development)
HOPES is a model-based embedded software design framework that supports all stages from application specification including static analysis, design space exploration, to real code generation. HOPES is developed by Codesign and Parallel Processing Laboratory(CAPLab), Seoul National University.

For more detailed information about HOPES, You can refer to the following document.(https://link.springer.com/chapter/10.1007/978-3-319-95246-8_18)

# License

Software license is provided separately per each folder (HOPES_UI, Public/UEMLibraryCode, Public/UEMTranslator).

# Installation

## On Linux
To install HOPES on Linux, do the following steps.

  1. Install prerequsite :
Java (version 1.8 and above, 64bit), ant

  2. Clone git repository and update submodules to your *HOPES_DIR*.
  ```
  git clone git@github.com:cap-lab/HOPES.git
  cd HOPES/
  git submodule update --init -- "Public"
  ```

3. Build "UEMTranslator" project inside "Public".
  ```
  cd Public/UEMTranslator/
  ant
  ```
  
4. To execute HOPES program, execute uem_ui.jar inside HOPES_UI.
```
  cd (HOPES_DIR)/HOPES_UI
  java -jar uem_ui.jar
```

## On Windows

To install HOPES on Windows, do the following steps:

  1. Install prerequsite :
Java (version 1.8 and above, 64bit, not necessary to install jdk), MSYS2
  
  2. Clone git repository and update submodules (**caution : Directory should not contain non-english characters).
    
  3. Build source files inside "Public" submodule. One way to build files is just simply open submodule project with eclipse IDE.

To execute HOPES program, execute uem_ui.jar inside HOPES_UI folder.

# Note

Gantt chart viewer of performance estimation is not currently supported in Linux platform.

# Publication


# Contact


# Appendix: Libraries We Use

