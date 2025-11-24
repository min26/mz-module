
# Aliases for common commands
alias ls='ls --color=auto'
alias grep='grep --color=auto'

# Customize the PS1 prompt (what you see in the terminal)
# This example shows username, hostname, and current directory
# Green username, blue hostname, yellow path, reset color
export PS1='\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;33m\]\w\[\033[00m\]\$ '

## Tree aliase
#
mtree() {
	echo "tree $1 -C -aph -L $2"
	tree "$1" -C -aph -L "$2" 
}
alias mtree='mtree'

## Setting Zephyr-rtos environment
# file location: /mnt/c/Home/min/zephyrproject/zephyr
#
alias zephyr-env='source ~/zephyrproject/venv/bin/activate && source ~/zephyrproject/zephyr/zephyr-env.sh'


