# which cmd:  which cmd in PATH is executed, version 1

case $# in
0)	echo 'Usage: which command' 1>&2; exit 2
esac
for i in `echo $PATH | sed 's/^:/.:/
                            s/::/:.:/g
                            s/:$/:./
                            s/:/ /g'`
do
	if test -f $i/$1	# use test -x if you can
	then
		echo $i/$1
		exit 0		# found it
	fi
done
exit 1		# not found
@@@  Fri Oct 14 14:21:11 EDT 1983 original version
