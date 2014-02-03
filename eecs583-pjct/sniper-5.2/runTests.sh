CODE_DIR=/home/ram/583/eecs583-pjct/sniper-5.2 # Directory where runTest.sh is present
MAX_TESTS=5
if [ ! -z "$1" ]
then
	MAX_TESTS=$1
fi
cd $CODE_DIR;
for i in `seq 1 $MAX_TESTS`
do
	echo "Running test $i of $MAX_TESTS"
	if [ -z "$2" ]
	then
		sh runTest.sh
	else
		if [ -z "$3" ]
		then
			sh runTest.sh $2 0
		else
			if [ $3 -eq 0 ]
			then
				sh runTest.sh $2 0
			else
				sh runTest.sh $2 1
			fi
		fi
	fi
done

