# Declares the current directory as zephyr library
# if no-name is given, uses directory name
zephyr_library()

# List source code files 
zephyr_library_sources(m_led.c)

# Add header file directories
zephyr_include_directories(.)


