# Declares the current directory as zephyr library
# if no-name is given, uses directory name
## zephyr_library()
# List source code files 
## zephyr_library_sources(m_usbd.c)
# Add header file directories
## zephyr_include_directories(.)

# 
target_include_directories(app PRIVATE .)
target_sources(app PRIVATE m_usbd.c)

