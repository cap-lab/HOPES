# HOPES(Hope Of Parallel Embedded Software development)
HOPES is a model-based embedded software design framework that supports all stages from application specification including static analysis, design space exploration, to real code generation. HOPES is developed by Codesign and Parallel Processing Laboratory(CAPLab), Seoul National University.

For more detailed information about HOPES, You can refer to the following document.(https://link.springer.com/chapter/10.1007/978-3-319-95246-8_18)

# License

Software license is provided separately per each folder (HOPES_UI, Public/UEMLibraryCode, Public/UEMTranslator).

# Installation

## On Linux
To install HOPES on Linux, do the following steps.

1. Install prerequsite : 
  >Java Development Kit (version 8 and above, 64bit) and ant.
  
  >For example, in Ubuntu, you can install as the below
  ```
  sudo apt install openjdk-8-jdk
  sudo apt install ant
  ```
  
  >You can check the install as the below.
  ```
  java -version
  javac -version
  ant -version
  ```
  
  >**Please be careful of setting environment variables (PATH, JAVA_HOME, ANT_HOME) if you install manually.**

2. Clone git repository and update submodules to your *HOPES_DIR*.
  ```
  git clone https://github.com/cap-lab/HOPES.git
  cd HOPES/
  git submodule update --init -- "Public"
  ```

3. Build "UEMTranslator" project inside "Public".
  ```
  cd Public/UEMTranslator/
  ant
  ```
  
4. To execute HOPES program, execute uem_ui.jar inside HOPES_UI directory.
```
cd (HOPES_DIR)/HOPES_UI
java -jar uem_ui.jar
```

## On Windows
To install HOPES on Windows, do the following steps:

1. Install prerequsite :
  
  >Java Development Kit (version 8 and above, 64bit), ant, and MSYS64.
  
  >You can check the installation the same way as the Linux on the Prompt.
  
  >**Please be careful of setting environment variables (Path, JAVA_HOME, ANT_HOME).**
  
2. Clone git repository and update submodules
  
  >(**Note that the directory should not contain non-english characters**).
    
3. Build source files inside "Public" submodule on the prompt. 
  >Go to "Public\UEMTranslator" directory in the cloned directory and type "ant" as the below.
  ```
  cd Public\UemTranslator
  ant
  ```

  >Or one way to build files is just simply open submodule project with eclipse IDE.

4. To execute HOPES program, execute uem_ui.jar inside HOPES_UI directory.
```
cd (HOPES_DIR)\HOPES_UI
java -jar uem_ui.jar
```
>Or just double click the "uem_ui.jar" to execute.

# Note

Gantt chart viewer of performance estimation is not currently supported in Linux platform.

# Publication


# Contact


# Appendix: Libraries We Use

