# HOPES(Hope Of Parallel Embedded Software development)
HOPES is a model-based embedded software design framework that supports all stages from application specification including static analysis, design space exploration, to real code generation. HOPES is developed by Codesign and Parallel Processing Laboratory(CAPLab), Seoul National University.

For more detailed information about HOPES, You can refer to the following document.(https://link.springer.com/chapter/10.1007/978-3-319-95246-8_18)

# License

Software license is provided separately per each folder (HOPES_UI, Public/UEMLibraryCode, Public/UEMTranslator).

# Installation

## On Linux
To install HOPES on Linux, do the following steps.

  1. Install prerequsite :
Java (version 1.7.x or 1.8.x. not necessary to install jdk), ant

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
  
4. Before Execution, change translator configuration path. You can use vim or any other text editors instead of nano.

```
  nano (HOPES_DIR)/Public/UEMTranslator/config/translator.properties
```

Replace two sentences at the top of the text, from ...
```
  template_code_path = ..\\UEMLibraryCode\\templates
  translated_code_template_path = ..\\UEMLibraryCode
```

to ...

```  
  template_code_path = ../UEMLibraryCode/templates
  translated_code_template_path = ../UEMLibraryCode
```

To execute HOPES program, execute uem_ui.jar inside HOPES_UI.
```
  cd (HOPES_DIR)/HOPES_UI
  java -jar uem_ui.jar
```

## On Windows

To install HOPES on Windows, do the following steps:

  1. Install prerequsite :
Java (version 1.7.x or 1.8.x. not necessary to install jdk), MSYS2
  
  2. Clone git repository and update submodules (**caution : Directory should not contain non-english characters).
    
  3. Build source files inside "Public" submodule. One way to build files is just simply open submodule project with eclipse IDE.

To execute HOPES program, execute uem_ui.jar inside HOPES_UI folder.

# Note


# Publication


# Contact


# Appendix: Libraries We Use

